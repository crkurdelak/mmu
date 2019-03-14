/**
 * @file mmu.h
 * @brief Function prototypes and type definitions for the MMU.
 *
 * The MMU translates 20-bit virtual addresses to 16-bit physical addresses,
 * and maps 8-bit virtual page numbers to 4-bit physical page frame numbers.
 * This yields a page table with 2^8 page table entries, one per virtual page,
 * mapping to 2^4 page frames in physical memory.  Each page is 2^12 bytes,
 * with each byte within a page addressable via the offset specified in the
 * virtual or physical address.
 *
 * @note MMU functions are loosely based on page table management functions
 * in the Linux kernel, described here:
 * https://www.kernel.org/doc/gorman/html/understand/understand006.html
 *
 * TODO fully document each method
 *
 * @author Brian R. Snider (bsnider@georgefox.edu)
 */

#ifndef MMU_H
#define MMU_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define PAGETABLE_SIZE  (1UL << 8)
#define PAGE_SHIFT      12
#define PAGE_SIZE       (1UL << PAGE_SHIFT)

/**
 * @struct vaddr_t
 * @brief A 20-bit virtual address type.
 */
typedef struct {
    uint32_t            : 12;   /* unused */
    uint32_t pagenum    : 8;    /**< virtual page number */
    uint32_t offset     : 12;   /**< offset within the page */
} vaddr_t;

/**
 * @struct addr_t
 * @brief A 16-bit physical address type.
 */
typedef struct {
    uint32_t            : 16;   /* unused */
    uint32_t framenum   : 4;    /**< physical page frame number */
    uint32_t offset     : 12;   /**< offset within the frame */
} addr_t;

/**
 * @struct pte_t
 * @brief An 8-bit page table entry type.
 */
typedef struct {
    uint8_t framenum    : 4;    /**< physical page frame number */
    uint8_t present     : 1;    /**< present/absent bit */
    uint8_t             : 1;    /* unused */
    uint8_t M           : 1;    /**< modified bit */
    uint8_t R           : 1;    /**< referenced bit */
} pte_t;

/**
 * @struct pagetable_t
 * @brief A page table type, consisting of many page table entries.
 * @see pagetable_alloc(), pagetable_free().
 */
typedef struct {
    pte_t* entries;             /**< page table entries */
    size_t size;                /**< number of page table entries */
} pagetable_t;

typedef uint8_t pagenum_t;
typedef uint8_t framenum_t;


/**
 * @brief Dynamically allocates a new page table.
 */
pagetable_t* pagetable_alloc();

/**
 * @brief Frees the specified page table from memory.
 */
void pagetable_free(pagetable_t* tbl);


/**
 * @brief Makes a new page table entry.
 */
pte_t mk_pte(framenum_t framenum);

/**
 * @brief Sets the page table entry for the given virtual page number.
 */
void set_pte(pagetable_t* tbl, pagenum_t pagenum, pte_t pte);

/**
 * @brief Clears the page table entry for the given virtual page number.
 */
pte_t pte_clear(pagetable_t* tbl, pagenum_t pagenum);

/**
 * @brief Returns 1 if the given page table entry is not set.
 */
int pte_none(const pagetable_t* tbl, pagenum_t pagenum);

/**
 * @brief Returns 1 if the given page is present.
 */
int pte_present(const pagetable_t* tbl, pagenum_t pagenum);


/**
 * @brief Returns 1 if the given page is modified.
 */
int pte_dirty(const pagetable_t* tbl, pagenum_t pagenum);

/**
 * @brief Sets the modified bit for the given page.
 */
void pte_mkdirty(pagetable_t* tbl, pagenum_t pagenum);

/**
 * @brief Clears the modified bit for the given page.
 */
void pte_mkclean(pagetable_t* tbl, pagenum_t pagenum);


/**
 * @brief Returns 1 if the given page has been recently referenced.
 */
int pte_young(const pagetable_t* tbl, pagenum_t pagenum);

/**
 * @brief Sets the referenced bit for the given page.
 */
void pte_mkyoung(pagetable_t* tbl, pagenum_t pagenum);

/**
 * @brief Clears the referenced bit for the given page.
 */
void pte_old(pagetable_t* tbl, pagenum_t pagenum);


/**
 * @brief Returns the specified page table entry.
 */
pte_t pte_val(pagetable_t* tbl, pagenum_t pagenum);


/**
 * @brief Translates a virtual address to a physical address.
 */
addr_t pagetable_translate(const pagetable_t* tbl, const vaddr_t vaddr);

#endif /* MMU_H */
