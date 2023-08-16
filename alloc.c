// A segregated storage memory allocator.  Complete the code required in the functions below
// to implement a segregated memory allocator.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "alloc.h"

int max_heap_size = 1024;  // default is a 1K heap (pretty small)

void *heap[4] = {NULL,NULL,NULL,NULL}; // the heap bins for each segregated size 8,16,32,64

unsigned int heap_size = 0; // A variable that keeps track of the how much memory we allocated

// allocate_slab uses the default malloc library to allocate memory
// input: size in bytes of slab to allocate
// return: pointer to slab allocated, NULL if allocation would exceed max_heap_size
void *allocate_slab(unsigned int size) {
    // return NULL;
    // print_heap();
    if (heap_size + size  > max_heap_size) {
        return NULL;
    }
    heap_size += size ;
    void *slab = (void *) malloc(size);
    // if (slab == NULL) {
    //     return NULL;
    // }

    
    return slab;
}

// Returns a pointer to a heap block of the appropriate
// size.
// If no such block exists in the free list, this function calls
// allocate_slab to get space NUM_HEAP_BLOCKS blocks of the
// appropriate size and adds them to the free list, then returns
// one of them.
void *my_alloc(unsigned int size) {
    // Calculate the bin number for the requested size
    int bin = -1;
    int nsize = -1;
    switch(size) {
        case 0 ... 8:
            bin = 0;
            nsize = 8;
            break;
        case 9 ... 16:
            bin = 1;
            nsize = 16;
            break;
        case 17 ... 32:
            bin = 2;
            nsize = 32;
            break;
        case 33 ... 64:
            bin = 3;
            nsize = 64;
            break;
        default:
            exit(0);
    }

    // Check if there are blocks in the free list for the corresponding bin
    // if (heap[bin] != NULL) {
    //     // Remove the first block from the free list and return it
    //     void *header = heap[bin] + sizeof(blk_header_t);
    //     heap[bin] = (void *) (((blk_header_t *) heap[bin])->next);
    //     return  header ;
    // }
    blk_header_t *header = (blk_header_t *) heap[bin];

    if (heap[bin] == NULL){
           // Call allocate_slab() 
        unsigned int alloc_size = ((nsize + sizeof(blk_header_t)) * NUM_HEAP_BLOCKS);
        
        if ( alloc_size+ heap_size > max_heap_size) {
            printf("Malloc failed, out of memory!\n");
            exit(0);
        }
        else{
            header = (blk_header_t *) allocate_slab(alloc_size);
            // header = (blk_header_t *) slab;
            
            heap[bin] = (void *) header;

            // Add the new blocks to the free list for the corresponding bin
            for (int i = 0; i < NUM_HEAP_BLOCKS-1; i++) {
                // int block_offset = ((bin + 1) * 8 + sizeof(blk_header_t));
                header->size = nsize;
                // header -> size =(8 << bin);
                header->next = (void *) header + sizeof(blk_header_t) + nsize;
                // heap[bin] = header;
                header = header ->next;
            }
            header->size = nsize;
            header -> next = NULL;
        }
    }

 
    void *header_ret = heap[bin] + sizeof(blk_header_t);
    heap[bin] = (void *) (((blk_header_t *) heap[bin])->next);
    return header_ret;
    // heap[bin] = (blk_header_t *) header->next;
    // return (void *)((char *)header + sizeof(blk_header_t));
}

void my_free(void *ptr) {
    // if (ptr == NULL){
    //     return;
    // }
    blk_header_t *header = (blk_header_t *)(ptr - sizeof(blk_header_t));

    // Determine which bin owns the block
    int bin = -1;
    switch(header->size) {
        case 8:
            bin = 0;
            break;
        case 16:
            bin = 1;
            break;
        case 32:
            bin = 2;
            break;
        default:
            bin = 3;
            break;
    }

    // Insert the block into the front of the appropriate bin
    header->next = heap[bin];
    heap[bin] = (void *) header;
    return;
}






//     int bin = 0;
    // for (int i = 0; i < 3; i++) {
    //     if ((char *)header >= (char *)heap[i] && (char *)header < (char *)heap[i+1]) {
    //         bin = i;
    //         break;
    //     }
    // }