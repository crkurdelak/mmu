/**
 * @file mmu_sim_cmd.c
 * @brief Implementation of the commands for mmu_sim.
 *
 * @author ckurdelak20@georgefox.edu
 */

#include "mmu_sim_cmd.h"

uint8_t mmu_sim_read(vaddr_t vaddr) {
    return 0;
}

uint8_t *mmu_sim_readn(vaddr_t vaddr, int nbytes) {
    return NULL;
}

void mmu_sim_write(char *pagefile, pagetable_t *tbl, vaddr_t vaddr, uint8_t val) {
    pagenum_t pagenum = vaddr.pagenum;
    frame_t* frame = pte_page(pagefile, tbl, pagenum);
    frame->bytes[vaddr.offset] = val;
    pte_mkdirty(tbl, pagenum);
}

void mmu_sim_writew(vaddr_t vaddr, uint8_t val1, uint8_t val2) {

}

void mmu_sim_writedw(vaddr_t vaddr, uint8_t val1, uint8_t val2, uint8_t val3, uint8_t val4) {

}

void mmu_sim_writez(vaddr_t vaddr, int nbytes) {

}
