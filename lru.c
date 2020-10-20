#include "pagetable.h"

extern int memsize;

extern int debug;

// extern struct frame *coremap;

// gives the time by number of references
static int time;

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int lru_evict(){
    // Start with the first frame and assume the current time is the oldest
    int frameEvicted = 0;
    int lastRef = time;
    // Go thorugh each page and get the oldest frame
    int i=0;
    while ( i< memsize) {
        if (coremap[i].pte->timestamp < lastRef) {
            // If older than any timestamp seen, set as oldest frame to evict
            lastRef = coremap[i].pte->timestamp;
            frameEvicted = i;
        }
        i = i+1;
    }
    return frameEvicted;
}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {
    // Increment the time each access and set that PTE's newest timestamp to that access time
    p->timestamp = ++time;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
    time = 0;
}