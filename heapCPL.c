#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define HEAP_SIZE 1024

// Structure to represent a block of memory in the heap
typedef struct Block {
    size_t size;
    int free; // 1 if the block is free, 0 if it's allocated
    struct Block *next; // Pointer to the next block in the linked list
} Block;

// The heap itself
static Block *heap_start = NULL;
void printList(Block *heap_start) {
    Block *curr = heap_start;
    while (curr) {
        printf("Block: Size = %zu, %s", curr->size, curr->free ? "Free" : "Allocated");
        if (curr->free == 0 && curr->size==4) {
            int data_value = *((int *)(curr + 1));
            printf(", Data = %d\n", data_value);
            
        } else {
            printf("\n");
        }
        curr = curr->next;
    }
}

// Function to allocate memory from the heap
void *alloc(size_t size) {
    // Traverse the linked list to find a suitable free block
    Block *curr = heap_start;
    while (curr) {
        if (curr->free && curr->size >= size) {
            // If the block is free and large enough, allocate from it
            if (curr->size > size + sizeof(Block)) {
                // Split the block if it's larger than the requested size
                Block *new_block = (Block *)((char *)curr + sizeof(Block) + size);
               //to perform pointer arithmetic in terms of bytes rather than in terms of the size of the structure (Block structure) it points to.
               	//Block *new_block = curr + size;
                new_block->size = curr->size - size - sizeof(Block);
                new_block->free = 1;
                new_block->next = curr->next;
                curr->size = size;
                curr->next = new_block;
            }
            curr->free = 0;
            return (void *)(curr + 1); // Return a pointer to the allocated memory
        }
        curr = curr->next;
    }
    return NULL; // If no suitable block is found, return NULL
}

// Function to free memory allocated from the heap
void free_mem(void *ptr) {
    if (ptr == NULL)
        return;
    Block *block = (Block *)ptr - 1;
    block->free = 1;
    
    // Merge adjacent free blocks
    Block *curr = heap_start;
    while (curr) {
        if (curr->free && curr->next && curr->next->free) {
            curr->size += sizeof(Block) + curr->next->size;
            curr->next = curr->next->next;
        }
        curr = curr->next;
    }
    
}




void merge_freeList()
{
	Block *curr = heap_start;
	 	while (curr) {
			 if (curr->free && curr->next && curr->next->free) {
				// Merge the current block with its next block
				curr->size += sizeof(Block) + curr->next->size;
				curr->next = curr->next->next;
				}
				curr = curr->next;
		    }
}

// Function to initialize the heap
void init_heap() {
    heap_start = (Block *)malloc(HEAP_SIZE);
    if (heap_start == NULL) {
        fprintf(stderr, "Error: Unable to initialize heap\n");
        exit(1);
    }
    heap_start->size = HEAP_SIZE - sizeof(Block);
    heap_start->free = 1;
    heap_start->next = NULL;
}

void dealloc_heap(Block* heap_start) {
    Block *curr = heap_start;
    while (curr != NULL) {
        Block *temp = curr;
        curr->free=1;
        curr = curr->next;
        memset(temp, 0, sizeof(Block));
        free(temp);
    }
    heap_start = NULL; // Reset heap_start to indicate that the heap is deallocated
    printf("Whole memory deallocated successfully!");
}

int main() {
    // Initialize the heap
    init_heap();
   // printf("%d",sizeof(Block));
   //size of this structure Block is 24 due to padding
	//input entered using array and also have maintained a array pointer of same size
	int n,i=0;
	printf("enter the max limit of number of pointers:");
	scanf("%d",&n);
	int * ptr[n],j;
	int flag=0;
    while(flag==0)
    {
        int choice;
        printf("\nOptions:\n");
        printf("1. Allocate integer value\n");
        printf("2. Allocate double value\n");
        printf("3. Allocate string value\n");
        printf("4. Print list\n");
        printf("5. Free memory\n");
        printf("6. Deallocate and terminate\n");
        printf("7. merge free lists\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1:
            	int inputI;
            	printf("enter integer data value:\n");
            	scanf("%d",&inputI);
                ptr[i] = (int *)alloc(sizeof(int));
    			*ptr[i] = inputI;
    			i++;
                break;
        	case 2:
			    double inputD;
			    printf("enter double data value:\n");
			    scanf("%lf", &inputD);
			    double *doublePtr; // Declare a pointer of type double *
			    doublePtr = (double *)alloc(sizeof(double)); // Allocate memory and typecast
			    *doublePtr = inputD; // Assign value to the allocated memory
			    ptr[i] = (int *)doublePtr; // Assign the pointer to ptr[i], casting it to int *
			    i++;
			    break;

			case 3:      
			    char inputS[100];
			    printf("enter string data:\n");
			    //3 litr nako
			    scanf("%s", inputS);
			    char *stringPtr; // Declare a pointer of type char *
			    stringPtr = (char *)alloc(strlen(inputS) + 1); // Allocate memory for the string
			    strcpy(stringPtr, inputS); // Copy the input string into the allocated memory
			    ptr[i] = (int *)stringPtr; // Assign the pointer to ptr[i], casting it to int *
			    i++;
			    break;
            case 4:
                printList(heap_start);
                break;
            case 5:
            	printf("enter the index node to be freed:\n");
            	scanf("%d",&j);
            	if(j>=0 && j<n)
            	{
                free_mem(ptr[j]);
                break;
            	}
                break;
            case 6:
				dealloc_heap(heap_start);
				break;
            case 7:
	            merge_freeList();
            	break;
            default:
            	flag=1;
        }
    }

    return 0;
}
