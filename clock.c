#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

// gives the index of frame that may be evicted if it hasn't been referenced
int indexFrame;
// gives a boolean array of frames that have been referenced
// ie, given that index of the array, true if referenced and false if not
int *refBits;

/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int clock_evict() {
	int frameEvicted;
	// go to next unreferenced frame in circular clock
	while (refBits[indexFrame] == 1) {
		refBits[indexFrame] = 0;
		indexFrame = (indexFrame + 1) % memsize;
	}
	// evict that frame
	frameEvicted = indexFrame;
	// Go to next frame (with mod for memsize) on the next evict
	indexFrame = (indexFrame + 1) % memsize;
	return frameEvicted;
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {
	// get the referenced frame from the PTE (mod for memsize)
	int frameRef = p->frame % memsize;
	// set its referenced bit to true
	refBits[frameRef] = 1;
}

/* Initialize any data structures needed for this replacement
 * algorithm. 
 */
void clock_init() {
	// start on first frame
	indexFrame = 0;
	// create list of reference bits by allocating space then initialize those bits to 0
	refBits = calloc(memsize, sizeof(int));
}
