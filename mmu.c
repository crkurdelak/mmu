/**
 * @file mmu.c
 * @brief MMU API implementation.
 *
 * @author ckurdelak20@georgefox.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"

/**
 * @struct fte_t
 * @brief A 16-bit frame table entry type.
 */
typedef struct {
    uint16_t occupied    : 1;  /**< occupied/unoccupied bit (1 if frame is occupied) */
    uint16_t pagenum     : 4;  /**< virtual page number */
} fte_t;

/**
 * @struct frametable_t
 * @brief A frame table type, consisting of many frame table entries.
 * @see frametable_alloc(), frametable_free().
 */
typedef struct {
    fte_t* entries;    /**< frame table entries */
    size_t size;       /**< number of frame table entries */
} frametable_t;

/* the pseudo-physical memory frames; globally accessible to the MMU */
frame_t* mem_frames;

/* the frame table */
frametable_t* frametable;

/**
 * @brief Dynamically allocates a new frame table.
 * @return a pointer to the new frame table
 */
frametable_t* frametable_alloc(){
    frametable_t* tbl = malloc(sizeof(frametable_t));
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


frame_t* mm_mem_init() {
    if (mem_frames != NULL) {
        fprintf(stderr, "simulation aborted\n");
        abort();
    }
    mem_frames = calloc(PAGE_FRAMES, PAGE_SIZE);

    // initialize frame table
    frametable = frametable_alloc();

    return mem_frames;
}


void mm_mem_destroy() {
    if (mem_frames != NULL) {
        free(mem_frames);
    }
}

bool mm_vmem_init(char* pagefile) {
    /*
     * This function will be used to initialize a 256 page × 4KB/page = 1024KB = 1MB page file.
     * Note: these pages should not actually appear in memory (yet), only on disk.  The contents
     * of each page should initially be all zeroes.  Each time the simulation program is run, the
     * page file should be overwritten, to simulate a cold start of actual hardware without
     * hibernation.
     */

    bool success = false;
    int filesize = PAGETABLE_SIZE * PAGE_SIZE;
    // open new pagefile in binary mode
    FILE *pg_file = fopen(pagefile, "wb");
    if (pg_file != NULL) {
        // make contents of pages all zeros
        for (int i = 0; i < filesize; i++) {
            // write to the file
            fputc(0, pg_file);
        }
    }

    return success;
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


/**
 * @brief Frees the specified frame table from memory.
 * @param tbl the frame table to be freed from memory
 */
void frametable_free(frametable_t* tbl){
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
    pte.set = 1;            // entry has been set
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
    tbl->entries[pagenum].M = 0;
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

/**
 * A helper function that returns the frame corresponding to the specified page number
 * @param tbl the page table
 * @param pagenum the specified page number
 * @return the frame corresponding to the specified page number
 */
frame_t* get_frame(pagetable_t* tbl, pagenum_t pagenum) {
    int current_framenum = (tbl->entries[pagenum].framenum);
    return &(mem_frames[current_framenum]);
}

void mm_page_evict(char* pagefile, pagetable_t* tbl, pagenum_t pagenum) {
    // TODO implement mm_page_evict
    pte_t* current_pte = &tbl->entries[pagenum];
    // if modified, write back to disk
    if (current_pte->M == 1) {
        frame_t* current_frame = get_frame(tbl, pagenum);

        // open file
        FILE *pg_file = fopen(pagefile, "wb");
        // seek to correct pg num
        fseek(pg_file, PAGE_SIZE * pagenum, SEEK_SET);
        // write page from frame starting at that spot
        fwrite(current_frame, 1, PAGE_SIZE, pg_file);
    }

    // update pte for this page (present = 0)
    // remove from memory
}

void mm_page_load(char* pagefile, pagetable_t* tbl, pagenum_t pagenum) {
    // look at pte for this pgnum and figure out which pg frame
    pte_t* current_pte = &(tbl->entries[pagenum]);

    if(current_pte->set == 0) {
        // make and set entry for next available frame number
        // TODO get next available frame number
        pte_t new_pte = mk_pte(framenum);
        set_pte(tbl, pagenum, new_pte);
    }
    else {
        fte_t* requested_fte = &(frametable->entries[pagenum]);
        if (requested_fte->occupied == 1) {
            // evict existing frame
            mm_page_evict(pagefile, tbl, pagenum);
        }
    }

    frame_t* current_frame = get_frame(tbl, pagenum);

    // open file
    FILE *pg_file = fopen(pagefile, "wb");
    // seek to correct pg num
    fseek(pg_file, PAGE_SIZE * pagenum, SEEK_SET);
    // read 4k bytes for page
    // put bytes into page frame
    fread(current_frame, 1, PAGE_SIZE, pg_file);
    // mark as present
    current_pte->present = 1;
    // reset necessary bits
    current_pte->M = 0;
    current_pte->R = 0;
}

frame_t* pte_page(char* pagefile, pagetable_t* tbl, pagenum_t pagenum) {
    pte_t* current_pte = &(tbl->entries[pagenum]);
    if (current_pte->present == 0) {
        // simulate pg fault by evicting current pg (if any) and loading requested pg
        // call evict
        mm_page_evict(pagefile, tbl, pagenum);
        // call load
        mm_page_load(pagefile, tbl, pagenum);
    }
    // return ptr to corresponding pg frame in pseudo-physical mem buffer
    return get_frame(tbl, pagenum);
}
