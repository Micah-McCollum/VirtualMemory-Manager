// Author: Micah McCollum  
// FILE: main.c
// Description: Main file for driving the virtual memory management project. This program uses functions defined in vm_manager.h to translate logical addresses into physical addresses and fetch byte values from a simulated physical memory.

#include "vm_manager.h"

// Global counters for page faults and TLB hits
int pageFaults = 0;
int tlbHits = 0;

int main() {
    // Open the file containing the logical addresses
    FILE *address_file = fopen("addresses.txt", "r");
    if (!address_file) {
        perror("Error opening addresses.txt");
        return EXIT_FAILURE;
    }

    // Initialize the main memory structures needed for the virtual memory manager
    PageTableEntry pageTable[NUM_PAGES];
    TLBEntry tlb[TLB_SIZE];
    char physicalMemory[NUM_FRAMES][FRAME_SIZE];

    initializePageTable(pageTable); // Set up the page table
    initializeTLB(tlb);             // Set up the TLB

    // Variables to hold read addresses, and count them
    int totalAddresses = 0;
    int translatedAddress;
    unsigned int logicalAddress;

    // Read and translate addresses until the end of the file
    while (fscanf(address_file, "%u", &logicalAddress) != EOF) {
        totalAddresses++;
        translatedAddress = translateAddress(logicalAddress, pageTable, tlb, physicalMemory);

        // Calculate frame number and offset within frame
        int frameNumber = translatedAddress / FRAME_SIZE;
        int offset = translatedAddress % FRAME_SIZE;
        signed char byteValue = physicalMemory[frameNumber][offset];

        // Printing the logical and physical address, along with the fetched byte value
        printf("Logical Address: %u, Physical Address: %d, Value: %d\n",
               logicalAddress, translatedAddress, byteValue);
    }

    // Close the file stream as it is no longer needed
    fclose(address_file);

    // Output final statistics for the session
    printf("\n");
    printf("Micah McCollum - Project Number: 3\n");
    printf("TLB Hit Rate: %.2f%%\n", (tlbHits / (double) totalAddresses) * 100.0);
    printf("Page Fault Rate: %.2f%%\n", (pageFaults / (double) totalAddresses) * 100.0);

    return EXIT_SUCCESS;
}