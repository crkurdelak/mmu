/**
 * @file mmu_sim_cmd.c
 * @brief Implementation of the commands for mmu_sim.
 *
 * @author ckurdelak20@georgefox.edu
 */

#include "mmu_sim_cmd.h"

uint8_t mmu_sim_read(char *pagefile, pagetable_t *tbl, vaddr_t vaddr) {
    // translate to physical address
    addr_t paddr = pagetable_translate(tbl, vaddr);
    pte_t current_entry = tbl->entries[paddr.framenum];
    // get frame number
    pagenum_t current_pgnum = current_entry.framenum;
    // load page
    frame_t* current_frame = pte_page(pagefile, tbl, current_pgnum);
    // go to pg offset to get correct bytes, and put them into a variable
    int byte_read = current_frame->bytes[paddr.offset];

    return byte_read;
}

void mmu_sim_readn(char *pagefile, pagetable_t *tbl, vaddr_t vaddr, int nbytes) {
    uint8_t bytes_read[] = {};
    for (int i = 0b0; i < nbytes; i++) {
        vaddr.value ++;
        bytes_read[i] = mmu_sim_read(pagefile, tbl, vaddr);
    }
}

void mmu_sim_write(char *pagefile, pagetable_t *tbl, vaddr_t vaddr, uint8_t val) {
    pagenum_t pagenum = vaddr.pagenum;
    frame_t* frame = pte_page(pagefile, tbl, pagenum);
    frame->bytes[vaddr.offset] = val;
    pte_mkdirty(tbl, pagenum);
}

void mmu_sim_writew(char *pagefile, pagetable_t *tbl, vaddr_t vaddr, uint8_t val1, uint8_t val2) {
    // write 2 bytes
    mmu_sim_write(pagefile, tbl, vaddr, val1);
    vaddr.value ++;
    mmu_sim_write(pagefile, tbl, vaddr, val2);
}

void mmu_sim_writedw(char *pagefile, pagetable_t *tbl, vaddr_t vaddr, uint8_t val1, uint8_t val2,
                     uint8_t val3, uint8_t val4) {
    // write 4 bytes
    mmu_sim_write(pagefile, tbl, vaddr, val1);
    vaddr.value ++;
    mmu_sim_write(pagefile, tbl, vaddr, val2);
    vaddr.value ++;
    mmu_sim_write(pagefile, tbl, vaddr, val3);
    vaddr.value ++;
    mmu_sim_write(pagefile, tbl, vaddr, val4);
}

void mmu_sim_writez(char *pagefile, pagetable_t *tbl, vaddr_t vaddr, int nbytes) {
    // write nbytes zeros
    for (int i = 0; i < nbytes; i++) {
        vaddr.value ++;
        mmu_sim_write(pagefile, tbl, vaddr, 0);
    }
}
