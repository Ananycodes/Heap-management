#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 1000

char Heap[SIZE];

typedef struct Metadata{
    int size;
    char mark;
    struct Metadata *next;
}Metadata;

//Meta data size is 12      

Metadata *head_ptr = (Metadata *)Heap;

void Initialize()
{
    head_ptr -> size = (SIZE - sizeof(Metadata)); 
    head_ptr -> mark = 'f';
    head_ptr -> next = NULL;
}


int Allocate(int request_size)
{
    Metadata *ptr = head_ptr;
    Metadata *prev = NULL;
    int index = 0;

    while (ptr != NULL && (ptr->mark == 'a' || ptr->size < request_size))
    {
        prev = ptr;
        ptr = ptr->next;
        index++;
    }

    if (ptr == NULL)
    {
        index = -1; // No block available
    }
    else
    {
        if (ptr->size == request_size)
        {
            ptr->mark = 'a';
        }
        else
        {
            // Metadata *new_block = (Metadata *)malloc(sizeof(Metadata));
            Metadata *new_block = (void*)((void *)ptr + sizeof(Metadata) + request_size);
            new_block->size = ptr->size - (request_size + sizeof(Metadata));
            new_block->mark = 'f';
            new_block->next = ptr->next;

            ptr->size = request_size;
            ptr->mark = 'a';
            ptr->next = new_block;
        }
    }
    return index; // Index of allocated block
}


void Merge()
{
    Metadata *ptr = head_ptr;
    Metadata *next_ptr;
    

    while(ptr != NULL && ptr -> next != NULL)
    {
        next_ptr = ptr -> next;
        
        if(ptr -> mark == 'f' && next_ptr -> mark == 'f')
        {
            ptr -> size = (ptr -> size)+(next_ptr -> size)+(sizeof(Metadata));
           
            ptr -> next = next_ptr -> next;
            //merged
        }
        
        ptr = ptr -> next;
        
    }

    

    
    //merging has been completed
}

bool Free(int index)
{
    Metadata *ptr;
    ptr = head_ptr;
    bool ret_val = 0;

    while(index != 0 && ptr != NULL)
    {
        ptr = ptr -> next;
        index--;
    }
    
    
    if(index == 0 && ptr != NULL && ptr -> mark == 'a')   //block to be freed id found
    {
        ptr -> mark = 'f';
        ret_val = 1;
    }

    Merge();

    return ret_val;

}

void Display_Heap()
{
    Metadata *ptr = head_ptr;
    printf("Heap Details : \n");
    printf("\t Block Size \t \t \t Block status \t \t \t \t Block Address \t \t \t Block Index \n");

    int index = 0;

    while(ptr != NULL)
    {
        printf("\t \t %d \t \t \t \t %c \t \t \t \t %p \t \t \t %d\n",ptr -> size,ptr -> mark,(void *)ptr,index);
        ptr = ptr -> next;
        index++;
    }

}

int main()
{
    Initialize();

    
    int flag = 0;
    while(flag == 0)
    {
        
            printf("Choose the function you want to perform \n");
            int n;
            printf(" 1. \t Allocate memory \n 2. \t De-allocate memory \n 3. \t Display heap elements \n 4. \t Quit \n");
            printf("Enter : ");
            scanf("%d",&n);
            if(n == 1)
            {
                int size;
                printf("Enter the size of block you want to allocate : \t");
                scanf("%d",&size);
                int ret_val;
                ret_val = Allocate(size);
                if(ret_val == -1)
                {
                    printf("Failed to assign memory \n");
                }
                else
                {
                    printf("Required memory is successfully assigned at index : %d \n",ret_val);
                }
            }
                
               
                
            
            if(n == 2) 
            {
                printf("Enter the index you wan to free : ");
                int index;
                scanf("%d",&index);
                bool ret_val = Free(index);
                if(ret_val == true)
                {
                    printf("The de-allocation is done successfully \n");
                }
                else
                {
                    printf("De-allocation has failed \n");
                }
                Merge();
            }   
                
                
                

            if(n == 3)
            {
                Display_Heap();
                printf("\n");
            }
                
            
                
                

            if(n == 4)
                {
                flag = 1;
                }
        }
    
    
    return 0;

}