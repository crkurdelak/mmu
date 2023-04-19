/**
 * @file mmu_sim.c
 * @brief Declarations for helper functions for mmu_sim.
 *
 * @author ckurdelak20@georgefox.edu
 */

#ifndef MMU_NEW_MMU_SIM_H
#define MMU_NEW_MMU_SIM_H

/**
 * Extracts an array of args from the given command string.
 * @param cmd the command entered by the user
 * @param args_array the array in which to store the args
 */
void get_args(char* cmd, char* args_array[]);

#endif //MMU_NEW_MMU_SIM_H