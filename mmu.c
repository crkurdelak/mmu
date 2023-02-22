/**
 * @file mmu.c
 * @brief MMU API implementation.
 *
 * @author ckurdelak20@georgefox.edu
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

pte_t mk_pte(framenum_t framenum) {
    pte_t result;
    result.age = 0;
    result.R = 0;       // has not been referenced yet
    result.M = 0;
    result.set = 0;
    // the page is not yet in a frame
    result.present = 0;
    result.framenum = framenum;
    return result;
}

void set_pte(pagetable_t *tbl, pagenum_t pagenum, pte_t pte) {
    pte.framenum = pagenum;
    pte.set = 1;            // entry has been set
    pte.present = 0;        // the page is not yet in a frame
    pte.M = 0;              // not yet modified
    tbl->entries[pagenum] = pte;
}

pte_t pte_clear(pagetable_t *tbl, pagenum_t pagenum) {
    pte_t old_page = tbl->entries[pagenum]; // copy of page
    pte_t* result;
    result = &tbl->entries[pagenum];     // get pointer to the entry
    // reset everything
    result->age = 0;
    result->R = 0;
    result->M = 0;
    result->set = 0;
    result->present = 0;
    return old_page;
}

int pte_none(const pagetable_t *tbl, pagenum_t pagenum) {
    int result = 1;
    if (tbl->entries[pagenum].set) {
        result = 0;
    }
    return result;
}

int pte_present(const pagetable_t *tbl, pagenum_t pagenum) {
    // look up the page's entry in the pagetable
    pte_t entry = tbl->entries[pagenum];
    // look at the present/absent bit in the entry
    return entry.present;
}

int pte_dirty(const pagetable_t *tbl, pagenum_t pagenum) {
    return tbl->entries[pagenum].M;
}

void pte_mkdirty(pagetable_t *tbl, pagenum_t pagenum) {
    tbl->entries[pagenum].M = 1;
}

void pte_mkclean(pagetable_t *tbl, pagenum_t pagenum) {
    tbl->entries[pagenum].M = 1;
}

int pte_young(const pagetable_t *tbl, pagenum_t pagenum) {
    return tbl->entries[pagenum].R;
}

void pte_mkyoung(pagetable_t *tbl, pagenum_t pagenum) {
    tbl->entries[pagenum].R = 1;
}

void pte_mkold(pagetable_t *tbl, pagenum_t pagenum) {
    tbl->entries[pagenum].R = 0;
}

pte_t pte_val(pagetable_t *tbl, pagenum_t pagenum) {
    pte_t result;
    result = tbl->entries[pagenum];
    return result;
}

addr_t pagetable_translate(const pagetable_t *tbl, const vaddr_t vaddr) {
    addr_t result;

    // offset stays the same
    // translate 8 bit virtual pagenum to 4 bit physical framenum

    // get page table entry of pagenum
    pte_t* pt_entry = &tbl->entries[vaddr.pagenum];
    // ask its framenum and return it
    // result.framenum = entry.framenum
    result.framenum = pt_entry->framenum;

    // offset = other offset
    result.offset = vaddr.offset;

    return result;
}
