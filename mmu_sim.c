/**
 * @file mmu_sim.c
 * @brief A memory management unit (MMU) simulation.
 *
 * @author ckurdelak20@georgefox.edu
 */

#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include "mmu.h"
#include "mmu_sim.h"

int main() {
    // Initialize 64KB pseudo-physical memory buffer
    mm_mem_init();

    char* pagefile = "pagefile.sys";
    mm_vmem_init(pagefile);

    // Allocate page table
    pagetable_t* pagetable = pagetable_alloc();

    int last_exit_status = 0;

    // input buffer
    char cmd[255];
    // buffer for current working directory
    char current_dir[255];
    bool quit = false;

    // while not exit:
    while (!quit) {
        // clear out input buffer
        memset(cmd, '\0', 255);

        // output prompt to stdout
        printf("mmu sim$ ");
        fflush(stdout);

        // read user command from stdin
        read(STDIN_FILENO, cmd, 255);

        // the args from the user's command
        char* args[255];
        memset(args, '\0', 255);
        get_args(cmd, args);

        // branching logic to handle specific, well-defined commands
        /*
         *
    READ <VADDR> — read one byte at the specified virtual address
    READN <VADDR> <NBYTES> — read the specified number of bytes starting at the specified virtual address
    WRITE <VADDR> <VALUE> — write the specified byte value at the specified virtual address
    WRITEW <VADDR> <VALUE> <VALUE> — write the specified word (2 byte) value starting at the specified virtual address
    WRITEDW <VADDR> <VALUE> <VALUE> <VALUE> <VALUE> — write the specified double word (4 byte) value starting at the specified virtual address
    WRITEZ <VADDR> <NBYTES> — write a zero value for the specified number of bytes starting at the specified virtual address
    HALT — halt execution; exit the instruction processing loop, evict all present pages to the backing page file, then exit the program

         */

        // if the command is empty
        if (strcmp(cmd, "\n") == 0) {
            // this does not print a newline
        }
            // if command is "HALT", exit = true
        else if (strcmp(args[0], "HALT") == 0) {
            quit = true;
        }
        // else if READ
        else if (strcmp(args[0], "READ") == 0) {
            // call fn
        }
        // else if READN
        else if (strcmp(args[0], "READN") == 0) {
            //
        }
        // else if WRITE
        else if (strcmp(args[0], "WRITE") == 0) {
            //
        }
        // else if WRITEW
        else if (strcmp(args[0], "WRITEW") == 0) {
            //
        }
        // else if WRITEDW
        else if (strcmp(args[0], "WRITEDW") == 0) {
            //
        }
        // else if WRITEZ
        else if (strcmp(args[0], "WRITEZ") == 0) {
            //
        }
        // else error
        else {
            //
        }
    }

    exit(0);
}


int get_args(char* cmd, char* args_array[]) {
    int is_bg = 0;     // 1 if the command is a background job, else 0
    // check if last char is &
    int cmd_len = strlen(cmd);
    char *last_char = &cmd[cmd_len - 2];
    if (strcmp(last_char, "&\n") == 0) {
        is_bg = 1;
    }
    char *current_token = strtok(cmd, " \n");
    int i = 0;
    while (current_token) {
        // if token is &, do not put in array
        if (strcmp(current_token, "&") != 0) {
            // put current token in array
            args_array[i] = current_token;
        }
        i++;
        // get new token
        current_token = strtok(NULL, " \n");
    }
    return is_bg;
}


    /*
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
    int byte_read = current_frame->bytes[p_start_addr.offset];

    // Read 4096 bytes sequentially starting at virtual address 0110 0100 0100 0000 0000
    v_start_addr.value = 0b01100100010000000000;
    int bytes_read[4096] = {};
    // initialize it to be all 1s so we can see if the bytes were read from the pagefile
    memset(bytes_read, 1, 4096);
    for (int i = 0; i < 4096; i++) {
        p_start_addr = pagetable_translate(pagetable, v_start_addr);
        current_entry = pagetable->entries[p_start_addr.framenum];
        // get frame number
        current_pgnum = current_entry.framenum;
        // load page
        current_frame = pte_page(pagefile, pagetable, current_pgnum);
        // go to pg offset to get correct bytes, and put them into a variable
        bytes_read[i] = current_frame->bytes[p_start_addr.offset];
        v_start_addr.value += 0b1;
    }

    // Write 4 bytes sequentially starting at virtual address 0010 1010 0000 0010 1010
    v_start_addr.value = 0b00101010000000101010;
    int new_bytes[4] = {0b1111, 0b1111, 0b1111, 0b1111};
    for (int i = 0; i < 4; i++) {
        p_start_addr = pagetable_translate(pagetable, v_start_addr);
        current_entry = pagetable->entries[p_start_addr.framenum];
        // get pg number
        current_pgnum = current_entry.framenum;
        // load pg
        current_frame = pte_page(pagefile, pagetable, current_pgnum);
        // write to page at offset
        current_frame->bytes[p_start_addr.offset] = new_bytes[i];
        pte_mkdirty(pagetable, current_entry.framenum);
        v_start_addr.value += 0b1;
        pte_mkdirty(pagetable, current_pgnum);
    }

    // Evict virtual page 102
    mm_page_evict(pagefile, pagetable, 102);

    // Free page table
    pagetable_free(pagetable);
    // Destroy pseudo-physical memory frames
    mm_mem_destroy();

    exit(EXIT_SUCCESS);
     */
