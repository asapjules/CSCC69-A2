#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

// the list of pages to simulate fifo (first in at head)
int list_head;

/* Page to evict is chosen using the fifo algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int fifo_evict() {
	int victim_frame = list_head;
	//traverse to next head after page evicted
	list_head = (list_head + 1) % memsize;
	return victim_frame;
}

/* This function is called on each access to a page to update any information
 * needed by the fifo algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void fifo_ref(pgtbl_entry_t *p) {
	return;
}

/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void fifo_init() {
	// start on first frame
	list_head = 0;
}