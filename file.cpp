#include <iostream>
#include <cstring>

#define SIZE 1000

char Heap[SIZE];

class Metadata {
public:
    int size;
    char mark;
    Metadata *next;

    Metadata(int s = 0, char m = 'f', Metadata *n = nullptr)
        : size(s), mark(m), next(n) {}
};

// Metadata size is 12

Metadata *head_ptr = (Metadata *)Heap;

void Initialize() {
    head_ptr->size = (SIZE - sizeof(Metadata));
    head_ptr->mark = 'f';
    head_ptr->next = nullptr;
}

int Allocate(int request_size) {
    Metadata *ptr = head_ptr;
    Metadata *prev = nullptr;
    int index = 0;

    while (ptr != nullptr && (ptr->mark == 'a' || ptr->size < request_size)) {
        prev = ptr;
        ptr = ptr->next;
        index++;
    }

    if (ptr == nullptr) {
        index = -1; // No block available
    } else {
        if (ptr->size == request_size) {
            ptr->mark = 'a';
        } else {
            Metadata *new_block = (Metadata *)((char *)ptr + sizeof(Metadata) + request_size);
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

void Merge() {
    Metadata *ptr = head_ptr;
    Metadata *next_ptr;

    while (ptr != nullptr && ptr->next != nullptr) {
        next_ptr = ptr->next;

        if (ptr->mark == 'f' && next_ptr->mark == 'f') {
            ptr->size = ptr->size + next_ptr->size + sizeof(Metadata);
            ptr->next = next_ptr->next;
        }

        ptr = ptr->next;
    }
}

bool Free(int index) {
    Metadata *ptr = head_ptr;
    bool ret_val = false;

    while (index != 0 && ptr != nullptr) {
        ptr = ptr->next;
        index--;
    }

    if (index == 0 && ptr != nullptr && ptr->mark == 'a') { // Block to be freed is found
        ptr->mark = 'f';
        ret_val = true;
    }

    Merge();

    return ret_val;
}

void Display_Heap() {
    Metadata *ptr = head_ptr;
    std::cout << "Heap Details:\n";
    std::cout << "\tBlock Size\t\tBlock Status\t\tBlock Address\t\tBlock Index\n";

    int index = 0;

    while (ptr != nullptr) {
        std::cout << "\t\t" << ptr->size << "\t\t\t" << ptr->mark << "\t\t\t" << (void *)ptr << "\t\t" << index << "\n";
        ptr = ptr->next;
        index++;
    }
}

int main() {
    Initialize();

    bool flag = false;
    while (!flag) {
        std::cout << "Choose the function you want to perform\n";
        int n;
        std::cout << "1. Allocate memory\n2. De-allocate memory\n3. Display heap elements\n4. Quit\n";
        std::cout << "Enter: ";
        std::cin >> n;

        if (n == 1) {
            int size;
            std::cout << "Enter the size of block you want to allocate: ";
            std::cin >> size;
            int ret_val = Allocate(size);
            if (ret_val == -1) {
                std::cout << "Failed to assign memory\n";
            } else {
                std::cout << "Required memory is successfully assigned at index: " << ret_val << "\n";
            }
        } else if (n == 2) {
            int index;
            std::cout << "Enter the index you want to free: ";
            std::cin >> index;
            bool ret_val = Free(index);
            if (ret_val) {
                std::cout << "The de-allocation is done successfully\n";
            } else {
                std::cout << "De-allocation has failed\n";
            }
        } else if (n == 3) {
            Display_Heap();
            std::cout << "\n";
        } else if (n == 4) {
            flag = true;
        }
    }

    return 0;
}
