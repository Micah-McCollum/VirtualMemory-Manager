// Author: Micah McCollum
// FILE: vm_manager.c
// Description: Code for the functions that are used in main.c and referenced
//      in header file.

#include "vm_manager.h"

// Initializing Page Table when no frames have been loaded yet
// variable int i: used for marking Page Table number, -1 shows no frames loaded
void initializePageTable (PageTableEntry pageTable[]) {
  for(int i = 0; i < NUM_PAGES; i++) {
	  pageTable[i].pageNumber = i;
	  pageTable[i].frameNumber = -1;
	  pageTable[i].isValid = false;
	}
}

// Initializing TLB if no entries exist yet in the TLB
// variable int i: indicates the
void initializeTLB (TLBEntry tlb[]) {
  for(int i = 0; i < TLB_SIZE; i++) {
	  tlb[i].pageNumber = -1;	// -1 indicates an empty entry
	  tlb[i].frameNumber = -1;
	  tlb[i].isValid = false;
	}
}

// Function that translates logical -> physical address using the intialized
// Page Table and TLB. Checks TLB first, then Page Table if not found in TLB
// If found, updates the TLB, if not found Page Fault is encountered/handled.
int translateAddress(unsigned int logicalAddress, PageTableEntry pageTable[],
				  TLBEntry tlb[], char physicalMemory[][FRAME_SIZE]) {
  unsigned int page_number = (logicalAddress >> 8) & OFFSET_MASK;
  unsigned int offset = logicalAddress & OFFSET_MASK;

  // TLB Lookup for page first
  for(int i = 0; i < TLB_SIZE; i++) {
	  if(tlb[i].isValid && tlb[i].pageNumber == page_number) {
		  int frame_number = tlb[i].frameNumber;
		  tlbHits++;
		  return (frame_number * FRAME_SIZE) + offset;
		}
	}

  // Page Table lookup if not found in the TLB 
  if(pageTable[page_number].isValid) {
	  int frame_number = pageTable[page_number].frameNumber;
	  updateTLB (page_number, frame_number, tlb);	// Update TLB
	  return (frame_number * FRAME_SIZE) + offset;
	}

  // If neither holds address, Page Fault encountered, handled by
  // function below (void handlePageFault)
  handlePageFault(page_number, physicalMemory, pageTable);
  int frame_number = pageTable[page_number].frameNumber;
  updateTLB(page_number, frame_number, tlb);	// Update TLB after handling the fault
  return(frame_number * FRAME_SIZE) + offset;
}

// Function used to handle encountered Page Faults, does so by manually loading
// the missing page into physical memory
void handlePageFault (int pageNumber, char physicalMemory[][FRAME_SIZE],
				 PageTableEntry pageTable[]) {
  static int next_free_frame = 0;	// Simulate finding the next free frame
  FILE *backing_store = fopen("BACKING_STORE.bin", "rb");
    if(!backing_store) {
        perror("Failed to open BACKING_STORE.bin");
        exit(EXIT_FAILURE);
    }

    // Seek to the correct page in the backing store
    fseek(backing_store, pageNumber * PAGE_SIZE, SEEK_SET);
    
    // Read the page into the correct frame in the physical memory
    fread(physicalMemory[next_free_frame], sizeof(char), FRAME_SIZE, backing_store);
    fclose(backing_store);

    // Update the Page Table
    pageTable[pageNumber].frameNumber = next_free_frame;
    pageTable[pageNumber].isValid = true;

    // Update statistics in main
    pageFaults++;
    next_free_frame++;  // Increment to the next free frame (assuming unlimited frames)
}

// Function used to update the TLB using LRU (Least Recently Used) logic for address replacement.
// LRU strategy prioritizes replacing the least recently accessed entry when a new page needs to be loaded.
// `lastUsed` array tracks the last access time of each TLB entry to determine the least recently used one.
// `timeCounter` is a global counter that increments on each TLB access to mark the time of use.
// This function updates existing TLB entries if the page is already present or replaces the least recently used entry.
void updateTLB(int pageNumber, int frameNumber, TLBEntry tlb[]) {
    int lruIndex = 0;
    int oldestTime = lastUsed[0];
    timeCounter++;  // Increment the time counter at each TLB access

    // Search for the page number in the TLB and identify the least recently used entry
    for(int i = 0; i < TLB_SIZE; i++) {
        if(tlb[i].pageNumber == pageNumber && tlb[i].isValid) {
            tlb[i].frameNumber = frameNumber;
            lastUsed[i] = timeCounter;  // Update the last used time to current time
            return;  // Exit as the TLB entry is updated
        }
        // Find the least recently used entry by comparing last used times
        if(lastUsed[i] < oldestTime) {
            oldestTime = lastUsed[i];
            lruIndex = i;
        }
    }

    // Replace the LRU entry with the new page
    tlb[lruIndex].pageNumber = pageNumber;
    tlb[lruIndex].frameNumber = frameNumber;
    tlb[lruIndex].isValid = true;
    lastUsed[lruIndex] = timeCounter;  // Update the last used time for the new entry
}