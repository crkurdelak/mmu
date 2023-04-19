/**
 * @file mmu_sim_cmd.h
 * @brief Function declarations for the commands for mmu_sim.
 *
 * @author ckurdelak20@georgefox.edu
 */

#ifndef MMU_NEW_MMU_SIM_CMD_H
#define MMU_NEW_MMU_SIM_CMD_H
#include "mmu.h"

/**
 * Reads one byte at the specified virtual address.
 * @param pagefile the filename of the pagefile
 * @param tbl a pointer to the page table
 * @param vaddr the virtual address of the byte to be read
 * @return the byte stored at the specified virtual address
 */
uint8_t mmu_sim_read(char *pagefile, pagetable_t *tbl, vaddr_t vaddr);

/**
 * Reads the specified number of bytes starting at the specified virtual address.
 * @param pagefile the filename of the pagefile
 * @param tbl a pointer to the page table
 * @param vaddr the virtual address of the first byte to be read
 * @param nbytes the number of bytes to be read
 * @return the bytes stored starting at the specified starting address
 */
void mmu_sim_readn(char *pagefile, pagetable_t *tbl, vaddr_t vaddr, int nbytes);

/**
 * Writes the specified byte value at the specified virtual address.
 * @param pagefile the filename of the pagefile
 * @param tbl a pointer to the page table
 * @param vaddr the virtual address to write to
 * @param val the byte to be written
 */
void mmu_sim_write(char *pagefile, pagetable_t *tbl, vaddr_t vaddr, uint8_t val);

/**
 * Writes the specified word (2 byte) value starting at the specified virtual address.
 * @param vaddr the starting virtual address to write to
 * @param val1 the first byte of the word to be written
 * @param val2 the second byte of the word to be written
 */
void mmu_sim_writew(char *pagefile, pagetable_t *tbl, vaddr_t vaddr, uint8_t val1, uint8_t val2);

/**
 * Writes the specified double word (4 byte) value starting at the specified virtual address.
 * @param vaddr the starting virtual address to write to
 * @param val1 the first byte of the double word to be written
 * @param val2 the second byte of the double word to be written
 * @param val3 the third byte of the double word to be written
 * @param val4 the fourth byte of the double word to be written
 */
void mmu_sim_writedw(char *pagefile, pagetable_t *tbl, vaddr_t vaddr, uint8_t val1, uint8_t val2,
                     uint8_t val3, uint8_t val4);

/**
 * Writes a zero value for the specified number of bytes starting at the specified virtual address.
 * @param vaddr the virtual address to write to
 * @param nbytes the number of bytes of zeros to be written
 */
void mmu_sim_writez(char *pagefile, pagetable_t *tbl, vaddr_t vaddr, int nbytes);

#endif //MMU_NEW_MMU_SIM_CMD_H