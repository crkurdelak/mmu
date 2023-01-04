/**
 * @file mmu.c
 * @brief MMU API implementation.
 *
 * TODO implement all MMU API methods
 *
 * @author Brian R. Snider (bsnider@georgefox.edu)
 */

#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"

/* the pseudo-physical memory frames; globally accessible to the MMU */
frame_t* mem_frames;

frame_t* mm_mem_init() {
    if (mem_frames != NULL) {
        fprintf(stderr, "simulation aborted\n");
        abort();
    }
    mem_frames = calloc(PAGE_FRAMES, PAGE_SIZE);
    return mem_frames;
}

void mm_mem_destroy() {
    if (mem_frames != NULL) {
        free(mem_frames);
    }
}

pagetable_t* pagetable_alloc() {
    pagetable_t* tbl = malloc(sizeof(pagetable_t));
    if (tbl != NULL) {
        tbl->entries = calloc(PAGETABLE_SIZE, sizeof(pte_t));
        if (tbl->entries != NULL) {
            tbl->size = PAGETABLE_SIZE;
        }
        else {
            free(tbl);
            tbl = NULL;
        }
    }
    return tbl;
}

void pagetable_free(pagetable_t* tbl) {
    if (tbl != NULL) {
        free(tbl->entries);
        free(tbl);
    }
}
