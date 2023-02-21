/**
 * @file mmu_sim.c
 * @brief A memory management unit (MMU) simulation.
 *
 * @author ckurdelak20@georgefox.edu
 */

#include <stdlib.h>
#include "mmu.h"

#define PAGEFILE "pagefile.sys"

int main() {
    // TODO implement simulation
    // Initialize 64KB pseudo-physical memory buffer
    mm_mem_init();
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
    vaddr_t v_start_addr = 0000 0101 0000 0000 0000;
    addr_t p_start_addr = pagetable_translate(pagetable, v_start_addr);
    pte_t current_frame = pagetable->entries[p_start_addr.framenum];
    pte_mkyoung(pagetable, current_frame.framenum);

    // Read 4096 bytes sequentially starting at virtual address 0110 0100 0100 0000 0000
    // Write 4 bytes sequentially starting at virtual address 0010 1010 0000 0010 1010

    // Evict virtual page 102
    // take it out of its frame
    // set present bit to 0

    // Free page table
    pagetable_free(pagetable);
    // Destroy pseudo-physical memory frames
    mm_mem_destroy();

    exit(EXIT_SUCCESS);
}
