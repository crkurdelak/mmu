/**
 * @file mmu_sim.c
 * @brief A memory management unit (MMU) simulation.
 *
 * @author Brian R. Snider (bsnider@georgefox.edu)
 */

#include <stdlib.h>
#include "mmu.h"

#define PAGEFILE "pagefile.sys"

int main() {
    mm_mem_init();
    pagetable_t* pagetable = pagetable_alloc();

    // TODO implement simulation

    pagetable_free(pagetable);
    mm_mem_destroy();

    return EXIT_SUCCESS;
}
