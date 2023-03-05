/**
 * @file mmu_sim.c
 * @brief A memory management unit (MMU) simulation.
 *
 * @author ckurdelak20@georgefox.edu
 */

#include <stdlib.h>
#include <stdio.h>
#include "mmu.h"

int main() {
    // TODO implement simulation
    // Initialize 64KB pseudo-physical memory buffer
    mm_mem_init();

    char* pagefile = "pagefile.sys";
    mm_vmem_init(pagefile);

    // Allocate page table
    pagetable_t* pagetable = pagetable_alloc();

    // Map virtual page 42 to physical frame 8
    pte_t new_pte = mk_pte(8);
    set_pte(pagetable, 42, new_pte);

    // Map virtual page 5 to physical frame 0
    new_pte = mk_pte(0);
    set_pte(pagetable, 5, new_pte);

    // Map virtual pages 100–103 to physical frames 2–5
    int framenum = 2;
    int pagenum = 100;
    while(framenum <= 5) {
        new_pte = mk_pte(framenum);
        set_pte(pagetable, pagenum, new_pte);
        framenum++;
        pagenum++;
    }

    // Read 1 byte starting at virtual address 0000 0101 0000 0000 0000
    vaddr_t v_start_addr = {0b00000101000000000000};
    addr_t p_start_addr = pagetable_translate(pagetable, v_start_addr);
    pte_t current_entry = pagetable->entries[p_start_addr.framenum];
    // get frame number
    pagenum_t current_pgnum = current_entry.framenum;
    // load page
    frame_t* current_frame = pte_page(pagefile, pagetable, current_pgnum);
    // go to pg offset to get correct bytes, and put them into a variable
    int var_1 = current_frame->bytes[p_start_addr.offset];

    // Read 4096 bytes sequentially starting at virtual address 0110 0100 0100 0000 0000
    v_start_addr.value = 0b01100100010000000000;

    for (int i = 0; i < 4096; i++) {
        p_start_addr = pagetable_translate(pagetable, v_start_addr);
        current_entry = pagetable->entries[p_start_addr.framenum];
        // get frame number
        current_pgnum = current_entry.framenum;
        // load page
        current_frame = pte_page(pagefile, pagetable, current_pgnum);
        // go to pg offset to get correct bytes, and put them into a variable
        // TODO change this to accumulate the bits
        int var_2 = current_frame->bytes[p_start_addr.offset];
        v_start_addr.value += 0b1;
    }

    // Write 4 bytes sequentially starting at virtual address 0010 1010 0000 0010 1010
    v_start_addr.value = 0b00101010000000101010;
    for (int i = 0; i < 4; i++) {
        p_start_addr = pagetable_translate(pagetable, v_start_addr);
        current_entry = pagetable->entries[p_start_addr.framenum];
        // TODO get pg number
        // TODO call pte_page w/ page, load pg
        // TODO go to pg offset to get correct bytes
        // TODO write to page
        pte_mkdirty(pagetable, current_entry.framenum);
        v_start_addr.value += 0b1;
    }

    // Evict virtual page 102
    mm_page_evict(pagefile, pagetable, 102);

    // Free page table
    pagetable_free(pagetable);
    // Destroy pseudo-physical memory frames
    mm_mem_destroy();

    exit(EXIT_SUCCESS);
}
