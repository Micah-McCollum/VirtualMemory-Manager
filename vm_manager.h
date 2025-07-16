// Author: Micah McCollum
// FILE: vm_manager.h
// Description: Header file for Project 3 that defines constants, data structures, and
// function prototypes used for managing virtual memory. This includes translating
// logical addresses to physical addresses, and managing TLB and page faults.

#ifndef VM_MANAGER_H
#define VM_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Constants defined as per the project specification to control the virtual memory management
#define ADDRESS_MASK 0xFFFF  // Mask to get the lower 16 bits of an address
#define OFFSET_MASK 0xFF     // Mask to get the offset within a page or frame
#define PAGE_SIZE 256        // Number of bytes in a page
#define FRAME_SIZE 256       // Number of bytes in a frame
#define NUM_FRAMES 256       // Total number of frames in physical memory
#define NUM_PAGES 256        // Total number of pages in logical memory
#define TLB_SIZE 16          // Size of the Translation Lookaside Buffer (TLB)

// Data structures used in virtual memory management

// PageTableEntry: Represents an entry in the page table
typedef struct {
    int pageNumber; // Logical page number
    int frameNumber; // Corresponding frame number in physical memory
    bool isValid;    // Validity flag to check if the page is in the frame
} PageTableEntry;

// TLBEntry: Represents an entry in the TLB
typedef struct {
    int pageNumber; // Logical page number
    int frameNumber; // Frame number in physical memory stored in the TLB
    bool isValid;    // Validity flag to check if the TLB entry is active
} TLBEntry;

// Global variables for tracking statistics across the program
extern int pageFaults; // Number of page faults encountered
extern int tlbHits;    // Number of TLB hits encountered

// Function prototypes for managing virtual memory

// Initializes the page table with all entries set to invalid
void initializePageTable(PageTableEntry pageTable[]);

// Initializes the TLB with all entries set to invalid
void initializeTLB(TLBEntry tlb[]);

// Translates a logical address to a physical address using the page table and TLB
int translateAddress(unsigned int logicalAddress, PageTableEntry pageTable[], TLBEntry tlb[], char physicalMemory[][FRAME_SIZE]);

// Handles a page fault by loading the page into physical memory and updating the page table
void handlePageFault(int pageNumber, char physicalMemory[][FRAME_SIZE], PageTableEntry pageTable[]);

// Updates the TLB using FIFO replacement policy when a new frame is loaded into physical memory
void updateTLB(int pageNumber, int frameNumber, TLBEntry tlb[]);

#endif // VM_MANAGER_H