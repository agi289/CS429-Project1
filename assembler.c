#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include "assembler.h"
#include <errno.h>

#include "limits.h"

/*
 * Constants
 * Assuming the max number to be 100
 */

#define maxNumSymbols 100

/*
 * Global variables
 */

// Array of symbol entries
Symbols tableofSymbols[maxNumSymbols];

// Size of table as it grows (being filled in)
int sizeOfTable = 0;
int flag = 2; // Flag for data vs code processing
int moveFlag = 2;

/*Error line counter*/
int lineCounter2 = 0;

/*Checkers for testing*/
int count1 = 0;
int count2 = 0;
int count3 = 0;
int count4 = 0;
int count5 = 0;
int count6 = 0;
int count7 = 0;
int count8 = 0;
int count9 = 0;
int count10 = 0;
int count11 = 0;
int count12 = 0;
int countcode = 0;
int countData = 0;

/*
 * Function to process labels
 */
void labelProcessor(char s[], FILE *output, uint64_t addressIntake, char *outputName)
{
    /*
     * Steps:
     * 1) Remove :
     * 2) Store the code label as the name
     * 3) Store the address as the address
     * 4) Make an entry of this in the table
     */

    // Substring from index 1
    char check[strlen(s)];
    strcpy(check, s + 1);

    /* Error of label coming twice*/
    if (sizeOfTable > 1)
    {
        for (int i = 0; sizeOfTable; i++)
        {
            if (strcmp(tableofSymbols[i].label, check) == 0)
            {
                fprintf(stderr, "Error on line %d.\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }
        }
    }

    int fine = 0;
    int onecharac = 0;
    
    /* check characters in the label*/
    for (int j = 0; j < strlen(check); j++)
    {
        if (isalpha(check[j]) || isdigit(check[j]) || check[j] == '-' || check[j] == '_')
        {
            fine = 0;
            if (isalpha(check[j]))
            {
                onecharac++;
            }
        }
        else
        {
            fprintf(stderr, "Error on line %d.\n", lineCounter2);
            remove(outputName);
            exit(EXIT_FAILURE);
        }
    }

    if (onecharac < 1)
    {
        fprintf(stderr, "Error on line %d.\n", lineCounter2);
        remove(outputName);
        exit(EXIT_FAILURE);
    }

    Symbols entry;
    strcpy(entry.label, check);
    // Make address u_int64_t
    entry.address = addressIntake;
    tableofSymbols[sizeOfTable] = entry;
    sizeOfTable++;

    // printf("Label: %s\n", check);
    // printf("Address %ld\n", addressIntake);
    // printf("Sucess table!\n");
}

/*
 * Function to process Instructions
 * Utilizes reg & binary function
 */
void instructionsProcessors(char *s, FILE *ptr, char *outputName)
{
    /*Check there is no ;*/
    if (strstr(s, ";") != NULL)
    {
        fprintf(stderr, "Error on line %d.\n", lineCounter2);
        remove(outputName);
        exit(EXIT_FAILURE);
    }
    // printf("%s", s);
    /*Syntax error check, starts with tab*/
    if (s[0] != '\t')
    {
        // printf("HI\n");
        fprintf(stderr, "Error on line %d.\n", lineCounter2);
        remove(outputName);
        exit(EXIT_FAILURE);
    }

    /*
     * Remove tab
     */
    int i = 0;
    char line[256];
    while (s[i] == '\t')
    {
        i++;
    }
    strcpy(line, s + i);

    /*Syntax error check of space or semicolon*/
    if (line[0] == '\t' || line[0] == ' ' || line[0] == ';' || line[0] == ':')
    {
        fprintf(stderr, "Error on line %d.\n", lineCounter2);
        remove(outputName);
        exit(EXIT_FAILURE);
    }

    int go = 0;

    /*check that first character in instruction is letter*/
    if (isalpha(line[0]))
    {
        // Just a filler for error check
        go = 1;
    }
    else
    {
        fprintf(stderr, "Error on line %d.\n", lineCounter2);
        remove(outputName);
        exit(EXIT_FAILURE);
    }

    /*
     * Convert mnemonic to binary opcode
     */
    char *mneumonic = strtok(line, " ");

    /*Check mneumonic is all letters*/
    for (int y = 0; y++; y < strlen(mneumonic))
    {
        if (!isalpha(mneumonic[i]))
        {
            fprintf(stderr, "Error on line %d.\n", lineCounter2);
            remove(outputName);
            exit(EXIT_FAILURE);
        }
    }

    char *rest = strtok(NULL, "\0");

    if ((strcmp(mneumonic, "add") == 0) || (strcmp(mneumonic, "sub") == 0) ||
        (strcmp(mneumonic, "mul") == 0) || (strcmp(mneumonic, "div") == 0) ||
        (strcmp(mneumonic, "and") == 0) || (strcmp(mneumonic, "or") == 0) ||
        (strcmp(mneumonic, "xor") == 0) || (strcmp(mneumonic, "shftr") == 0) ||
        (strcmp(mneumonic, "shftl") == 0) || (strcmp(mneumonic, "brgt") == 0) || (strcmp(mneumonic, "addf") == 0) ||
        (strcmp(mneumonic, "subf") == 0) || (strcmp(mneumonic, "mulf") == 0) || (strcmp(mneumonic, "divf") == 0))
    {
        /*Check # correct arguments*/
        // printf("%s\n", rest);
        int spaces = 0;
        for (int i = 0; i < strlen(rest); i++)
        {
            if (rest[i] == ',')
            {
                spaces++;
            }
        }

        if (spaces != 2)
        {
            fprintf(stderr, "Error on line %d.\n", lineCounter2);
            remove(outputName);
            exit(EXIT_FAILURE);
        }

        /* Tokenize the rest as registers*/
        char *r0 = strtok(rest, ", ");
        char *r1 = strtok(NULL, ", ");
        char *r3 = strtok(NULL, ", ");

        /* Process registers */
        char r0Input[strlen(r0)];
        char r1Input[strlen(r1)];
        char r3Input[strlen(r3)];

        registerProcessor(r0, r0Input, outputName);
        registerProcessor(r1, r1Input, outputName);
        registerProcessor(r3, r3Input, outputName);

        count1++;

        // printf("Add parts: %s, %s, %s, %s\n", mneumonic, r0Input, r1Input, r3Input);

        // /* write binary code */
        writeBinary(mneumonic, r0Input, r1Input, r3Input, "00000", ptr, outputName);
    }
    else if ((strcmp(mneumonic, "addi") == 0) || (strcmp(mneumonic, "subi") == 0) || (strcmp(mneumonic, "not") == 0) || (strcmp(mneumonic, "shftri") == 0) || (strcmp(mneumonic, "shftli") == 0) || (strcmp(mneumonic, "brnz") == 0) ||
             (strcmp(mneumonic, "in") == 0) || (strcmp(mneumonic, "out") == 0))
    {
        // register and literal in rest
        if (((strcmp(mneumonic, "addi")) == 0) || ((strcmp(mneumonic, "subi")) == 0) || ((strcmp(mneumonic, "shftri")) == 0) || ((strcmp(mneumonic, "shftli")) == 0))
        {
            /*Check # correct arguments*/
            // printf("%s\n", rest);
            int spaces = 0;
            for (int i = 0; i < strlen(rest); i++)
            {
                if (rest[i] == ',')
                {
                    spaces++;
                }
            }

            if (spaces != 1)
            {
                fprintf(stderr, "Error on line %d.\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /* Tokenize the rest as registers*/
            char *r0 = strtok(rest, ", ");
            char *L = strtok(NULL, ", ");

            /* Process registers */
            char r0Input[strlen(r0)];
            char LInput[strlen(L) + 1];

            registerProcessor(r0, r0Input, outputName);

            /*Literal isn't a reg*/
            if ((L[0] == 'r'))
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /*literal isn't neg*/
            if (L[0] == '-')
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /*Literal 1st digit*/
            if (!isdigit(L[0]))
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /*Literal has num */
            for (int i = 1; i < strlen(L); i++)
            {
                if (!isdigit(L[i]))
                {
                    fprintf(stderr, "Error on line %d\n", lineCounter2);
                    remove(outputName);
                    exit(EXIT_FAILURE);
                }
            }
            strcpy(LInput, L);

            /* write binary code ~ input 12 bit 0's since literal isn't important*/
            writeBinary(mneumonic, r0Input, "00000", "00000", LInput, ptr, outputName);
            count2++;
            // printf("Add parts: %s, %s, %s", mneumonic, r0Input, LInput);
        }
        // // 2 registers
        else if (((strcmp(mneumonic, "not")) == 0) || ((strcmp(mneumonic, "brnz")) == 0) ||
                 ((strcmp(mneumonic, "in")) == 0) || ((strcmp(mneumonic, "out")) == 0))
        {
            /*Check # correct arguments*/
            // printf("%s\n", rest);
            int spaces = 0;
            for (int i = 0; i < strlen(rest); i++)
            {
                if (rest[i] == ',')
                {
                    spaces++;
                }
            }

            if (spaces != 1)
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /* Tokenize the rest as registers*/
            char *r0 = strtok(rest, ", ");
            char *r1 = strtok(NULL, ", ");

            /* Process registers */
            char r0Input[strlen(r0)];
            char r1Input[strlen(r1)];

            registerProcessor(r0, r0Input, outputName);
            registerProcessor(r1, r1Input, outputName);

            /* write binary code ~ input 12 bit 0's since literal isn't important*/
            writeBinary(mneumonic, r0Input, r1Input, "00000", "00000", ptr, outputName);
            // printf("Out parts: %s, %s, %s\n", mneumonic, r0Input, r1Input);
            count3++;
        }
    }
    else if (((strcmp(mneumonic, "br")) == 0) || (strcmp(mneumonic, "call") == 0))
    // 1 register
    {
        /*Check # correct arguments*/
        // printf("%s\n", rest);
        int spaces = 0;
        for (int i = 0; i < strlen(rest); i++)
        {
            if (rest[i] == ',')
            {
                spaces++;
            }
        }

        if (spaces != 0)
        {
            fprintf(stderr, "Error on line %d\n", lineCounter2);
            remove(outputName);
            exit(EXIT_FAILURE);
        }

        /* Tokenize the rest as registers*/
        char *r0 = strtok(rest, ", ");

        /* Process registers */
        char r0Input[strlen(r0)];

        registerProcessor(r0, r0Input, outputName);
        count4++;
        //   printf("BR parts: %s, %s\n", mneumonic, r0Input);

        /* write binary code ~ input 12 bit 0's since literal isn't important*/
        writeBinary(mneumonic, r0Input, "00000", "00000", "00000", ptr, outputName);
    }
    else if (((strcmp(mneumonic, "brr")) == 0))
    {
        /*Check # correct arguments*/
        // printf("%s\n", rest);
        int spaces = 0;
        for (int i = 0; i < strlen(rest); i++)
        {
            if (rest[i] == ',')
            {
                spaces++;
            }
        }

        if (spaces != 0)
        {
            fprintf(stderr, "Error on line %d\n", lineCounter2);
            remove(outputName);
            exit(EXIT_FAILURE);
        }

        // 2 versions: Literal and reg
        /* Tokenize the rest as registers*/
        char *r0 = strtok(rest, ", ");

        if (strstr(r0, "r") != NULL)
        {
            // register version

            /* Process registers */
            char r0Input[strlen(r0)];

            registerProcessor(r0, r0Input, outputName);
            count5++;
            //    printf("Add parts: %s, %s", mneumonic, r0Input);

            /* write binary code ~ input 12 bit 0's since literal isn't important*/
            writeBinary(mneumonic, r0Input, "00000", "00000", "00000", ptr, outputName);
        }
        else
        {
            // literal version

            /* Process registers */
            char LInput[strlen(r0) + 1];

            /*Literal isn't a reg*/
            if ((r0[0] == 'r'))
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /*Literal 1st digit is num or -*/
            if (!isdigit(r0[0]) && r0[0] != '-')
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /*Literal has num */
            for (int i = 1; i < strlen(r0); i++)
            {
                if (!isdigit(r0[i]))
                {
                    fprintf(stderr, "Error on line %d\n", lineCounter2);
                    remove(outputName);
                    exit(EXIT_FAILURE);
                }
            }

            strcpy(LInput, r0);
            count6++;
            //    printf("Add parts: %s, %s", mneumonic, LInput);

            /* write binary code ~ input 12 bit 0's since literal isn't important*/
            writeBinary(mneumonic, "00000", "00000", "00000", LInput, ptr, outputName);
        }
    }
    else if (((strcmp(mneumonic, "mov")) == 0))
    {
        /*Check # correct arguments*/
        // printf("%s\n", rest);
        int spaces = 0;
        for (int i = 0; i < strlen(rest); i++)
        {
            if (rest[i] == ',')
            {
                spaces++;
            }
        }

        if (spaces != 1)
        {
            fprintf(stderr, "Error on line %d\n", lineCounter2);
            remove(outputName);
            exit(EXIT_FAILURE);
        }

        // 4 versions -- all 2 parts but sometimes on part combined
        /* Tokenize the rest as registers*/
        // printf("rest:%s\n", rest);
        char *r0 = strtok(rest, ", ");
        char *r1 = strtok(NULL, ", ");

        if (strstr(r0, "(") != NULL)
        {
            // mov (rd)(L), rs
            
            moveFlag = 1;
            //printf("%s\n", r0);
            if (r0[0] != '(' || r0[3] != ')' || r0[4] != '(' || r0[6] != ')')
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            // r0 part tokenization
            char *r3 = strtok(r0 + 1, ")");
           // printf("%s\n", r3);
            char *L = strtok(NULL, ")");
            //printf("%s\n", L);

            char Lfinal[strlen(L)];
            char rdInput[strlen(r3) + 1];
            char rsInput[strlen(r1) + 1];

            //printf("h\n");
            strcpy(Lfinal, L + 1);
            //printf("%s\n", Lfinal);

            //printf("%d", lineCounter2);
            /*Literal isn't a reg*/
            if ((Lfinal[0] == 'r'))
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /*Literal 1st digit is num*/
            if (!isdigit(Lfinal[0]) && Lfinal[0] != '-')
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }


            /*Literal has num */
            for (int i = 1; i < strlen(Lfinal); i++)
            {
                if (!isdigit(Lfinal[i]))
                {
                    fprintf(stderr, "Error on line %d\n", lineCounter2);
                    remove(outputName);
                    exit(EXIT_FAILURE);
                }
            }
            registerProcessor(r3, rdInput, outputName);
            registerProcessor(r1, rsInput, outputName);

            count7++;
            //  printf("Add parts: %s, %s, %s, %s", mneumonic, rdInput, rsInput, Lfinal);

            /* write binary code ~ input 12 bit 0's since literal isn't important*/
            writeBinary(mneumonic, rdInput, rsInput, "00000", Lfinal, ptr, outputName);
        }
        else if (strstr(r1, "r") == NULL)
        {
            // mov rd, L
            /* Process registers */
            char r0Input[strlen(r0)];
            char LInput[strlen(r1) + 1];

            /*Literal isn't a reg*/
            if ((r1[0] == 'r'))
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /*Literal 1st digit is num or -*/
            if (!isdigit(r1[0]))
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            if (r1[0] == '-')
             {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /*Literal has num */
            for (int i = 1; i < strlen(r1); i++)
            {
                if (!isdigit(r1[i]))
                {
                    fprintf(stderr, "Error on line %d\n", lineCounter2);
                    remove(outputName);
                    exit(EXIT_FAILURE);
                }
            }

            registerProcessor(r0, r0Input, outputName);
            strcpy(LInput, r1);
            count8++;
            //    printf("Add parts: %s, %s, %s", mneumonic, r0Input, LInput);

            /* write binary code ~ input 12 bit 0's since literal isn't important*/
            writeBinary(mneumonic, r0Input, "00000", "00000", LInput, ptr, outputName);
        }
        else if (strstr(r1, "(") != NULL)
        {
            // mov rd, (rs)(L)
            moveFlag = 0;
            // printf("move Flag: %d\n", moveFlag);

            // r0 part tokenization
            char *r3 = strtok(r1 + 1, ")");
            char *L = strtok(NULL, ")");

            char Lfinal[strlen(L)];
            char rsInput[strlen(r3) + 1];
            char rdInput[strlen(r0) + 1];

            strcpy(Lfinal, L + 1);

            /*Literal isn't a reg*/
            if ((L[0] == 'r'))
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /*Literal 1st digit is num*/
            if (!isdigit(Lfinal[0]) && Lfinal[0] != '-')
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /*Literal has num */
            for (int i = 1; i < strlen(Lfinal); i++)
            {
                if (!isdigit(Lfinal[i]))
                {
                    fprintf(stderr, "Error on line %d\n", lineCounter2);
                    remove(outputName);
                    exit(EXIT_FAILURE);
                }
            }
            registerProcessor(r3, rsInput, outputName);
            registerProcessor(r0, rdInput, outputName);
            count9++;
            //    printf("Move parts: %s, %s, %s, %s\n", mneumonic, rdInput, rsInput, Lfinal);

            /* write binary code ~ input 12 bit 0's since literal isn't important*/
            writeBinary(mneumonic, rdInput, rsInput, "00000", Lfinal, ptr, outputName);
        }
        else
        {
            // mov rd, rs

            /* Process registers */
            char r0Input[strlen(r0)];
            char r1Input[strlen(r1)];

            registerProcessor(r0, r0Input, outputName);
            registerProcessor(r1, r1Input, outputName);
            count10++;
            //  printf("Add parts: %s, %s, %s", mneumonic, r0Input, r1Input);

            /* write binary code ~ input 12 bit 0's since literal isn't important*/
            writeBinary(mneumonic, r0Input, r1Input, "00000", "00000", ptr, outputName);
        }
    }
    else if (((strcmp(mneumonic, "return")) == 0) || ((strcmp(mneumonic, "halt")) == 0))
    {
        if (rest != NULL)
        {
            fprintf(stderr, "Error on line %d\n", lineCounter2);
            remove(outputName);
            exit(EXIT_FAILURE);
        }
        count11++;
        // printf("Halt\n");
        writeBinary(mneumonic, "00000", "00000", "00000", "00000", ptr, outputName);
    }
    else if (((strcmp(mneumonic, "clr")) == 0) || ((strcmp(mneumonic, "ld")) == 0) || ((strcmp(mneumonic, "push")) == 0) || ((strcmp(mneumonic, "pop")) == 0))
    {

        // printf("Entered here\n");
        count12++;

        // Break macro into it's components for ease
        /*clr rd -> xor rd, rd, rd*/
        if (strcmp(mneumonic, "clr") == 0)
        {
            /*Check # correct arguments*/
            // printf("%s\n", rest);
            int spaces = 0;
            for (int i = 0; i < strlen(rest); i++)
            {
                if (rest[i] == ',')
                {
                    spaces++;
                }
            }

            if (spaces != 0)
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }
            /* Tokenize the rest as registers*/
            char *r0 = strtok(rest, ", ");

            /* Process registers */
            char r0Input[strlen(r0)];

            registerProcessor(r0, r0Input, outputName);

            writeBinary("xor", r0Input, r0Input, r0Input, "00000", ptr, outputName);
        }
        /*ld rd, L*/
        else if (strcmp(mneumonic, "ld") == 0)
        {
            /*Check # correct arguments*/
            // printf("%s\n", rest);
            int spaces = 0;
            for (int i = 0; i < strlen(rest); i++)
            {
                if (rest[i] == ',')
                {
                    spaces++;
                }
            }

            if (spaces != 1)
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /* Tokenize the rest as registers and literals*/
            char *r0 = strtok(rest, ", ");
            char *L = strtok(NULL, ", ");

            /* Process registers */
            char r0Input[strlen(r0)];
            char LInput[strlen(L) + 1];

            registerProcessor(r0, r0Input, outputName);
            strcpy(LInput, L);
            // printf("%s", LInput);

            convertToBinaryLD("ld", r0Input, LInput, ptr, outputName);
        }
        /* push rd -> mov(r31)(-8), rd & subi r31, 8 */
        else if (strcmp(mneumonic, "push") == 0)
        {
            /*Check # correct arguments*/
            // printf("%s\n", rest);
            int spaces = 0;
            for (int i = 0; i < strlen(rest); i++)
            {
                if (rest[i] == ',')
                {
                    spaces++;
                }
            }

            if (spaces != 0)
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /* Tokenize the rest as registers*/
            char *r0 = strtok(rest, ", ");
            // printf("%s\n", r0);

            /* Process registers */
            char r0Input[strlen(r0)];

            registerProcessor(r0, r0Input, outputName);

            // First is form mov (rd)(L), rs(rd input)
            moveFlag = 1;
            writeBinary("mov", "31", r0Input, "00000", "-8", ptr, outputName);
            writeBinary("subi", "31", "00000", "00000", "8", ptr, outputName);
        }
        /*pop rd -> mov rd, (r31)(0) & addi r31, 8*/
        else if (strcmp(mneumonic, "pop") == 0)
        {
            /*Check # correct arguments*/
            // printf("%s\n", rest);
            int spaces = 0;
            for (int i = 0; i < strlen(rest); i++)
            {
                if (rest[i] == ',')
                {
                    spaces++;
                }
            }

            if (spaces != 0)
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }

            /* Tokenize the rest as registers*/
            char *r0 = strtok(rest, ", ");

            /* Process registers */
            char r0Input[strlen(r0)];

            registerProcessor(r0, r0Input, outputName);

            /* check the first part, since this mov command dne*/
            moveFlag = 0;
            writeBinary("mov", r0Input, "31", "00000", "00000", ptr, outputName);
            writeBinary("addi", "31", "00000", "00000", "8", ptr, outputName);
        }
    }
    else
    {
        fprintf(stderr, "Error on line %d\n", lineCounter2);
        remove(outputName);
        exit(EXIT_FAILURE);
    }
}

/*
 * Function to process labels when register token
 * Simply removes the r if needed (not all at onces)
 * char * registers is the token passed in w no space in front
 * Need to remove the , or spaces should just be r0
 * char numberValue[] should be array of size 1
 */
void registerProcessor(char *registers, char numberValue[], char *outputName1)
{
    if (registers[0] == 'r')
    {
        strcpy(numberValue, registers + 1);
    }
    else
    {
        fprintf(stderr, "Error on line %d\n", lineCounter2);
        remove(outputName1);
        exit(EXIT_FAILURE);
    }
}

/*
 * Function that literally converts the data number into binary
 */
void dataProcessor(char *s, FILE *dataptr, char *outputName)
{
    /*Ouputting into file, 64 bits*/
    /*Check there is no ;*/
    if (strstr(s, ";") != NULL || s[0] != '\t')
    {
        fprintf(stderr, "Error on line %d\n", lineCounter2);
        remove(outputName);
        exit(EXIT_FAILURE);
    }

    /*
     * Remove tab
     */
    int i = 0;
    char line[256];
    while (s[i] == '\t')
    {
        i++;
    }
    strcpy(line, s + i);

    // Data is a number
    for (int i = 0; i < strlen(line); i++)
    {
        if (isdigit(line[i]))
        {
            continue;
        }
        else
        {
            fprintf(stderr, "Error on line %d\n", lineCounter2);
            remove(outputName);
            exit(EXIT_FAILURE);
        }
    }

    // 64 bit int
    char *endptr;
    uint64_t data = strtoul(line, &endptr, 10);
    if (errno == ERANGE || errno == EINVAL)
    {
        fprintf(stderr, "Error on line %d\n", lineCounter2);
        remove(outputName);
        exit(EXIT_FAILURE);
    }
    // printf("DataP: %ld\n", data);

    fwrite(&data, sizeof(data), 1, dataptr);
}

void convertToBinaryLD(char *opcode, char rd[], char L[], FILE *outptr, char *outputName1)
{
    /*
     * 1) Convert L to binary ~ can assume positive because memory address
     * 2) Parse the given components
     * 3) Do the function passes into writeBinary
     * 4) writeBinary will execute necessary commands
     */
    char Literal[strlen(L)];
    uint64_t LDLiteral = 0;
    if (L[0] == ':')
    {
        strcpy(Literal, L + 1);

        // printf("LabelC: %s\n", Literal);

        /*Look through symbol array*/
        int errorFlag = 0;
        for (int i = 0; sizeOfTable; i++)
        {
            if (strcmp(tableofSymbols[i].label, Literal) == 0)
            {
                LDLiteral = tableofSymbols[i].address;
                errorFlag = 1;
                // printf("Point in table: %s\n", tableofSymbols[i].label);
                // printf("Address in table: %ld\n", tableofSymbols[i].address);
                // printf("Literal in table: %ld\n", LDLiteral);
                break;
            }
        }
        if (errorFlag == 0)
        {
            fprintf(stderr, "Error on line %d\n", lineCounter2);
            remove(outputName1);
            exit(EXIT_FAILURE);
        }
        // printf("LDLiteral: %ld\n", LDLiteral);
    }
    else
    {
        for (int i = 0; i < strlen(L); i++)
        {
            if (!isdigit(L[i]))
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName1);
                exit(EXIT_FAILURE);
            }
        }

        // 64 bit int
        char *endptr;
        LDLiteral = strtoul(L, &endptr, 10);
        if (errno == ERANGE || errno == EINVAL)
        {
            fprintf(stderr, "Error on line %d\n", lineCounter2);
            remove(outputName1);
            exit(EXIT_FAILURE);
        }
    }

    uint64_t map12 = 0b1111111111110000000000000000000000000000000000000000000000000000;
    uint64_t map24 = 0b0000000000001111111111110000000000000000000000000000000000000000;
    uint64_t map36 = 0b0000000000000000000000001111111111110000000000000000000000000000;
    uint64_t map48 = 0b0000000000000000000000000000000000001111111111110000000000000000;
    uint64_t map60 = 0b0000000000000000000000000000000000000000000000001111111111110000;
    uint64_t map64 = 0b0000000000000000000000000000000000000000000000000000000000001111;

    int literalFinalLD12 = (LDLiteral & map12) >> 52;
    // printf("Map: %ld\n", map12);
    // printf("12 bits: %d\n", literalFinalLD12);

    int literalFinalLD24 = (LDLiteral & map24) >> 40;
    // printf("Map: %ld\n", map24);
    // printf("12 bits: %d\n", literalFinalLD24);

    int literalFinalLD36 = (LDLiteral & map36) >> 28;
    // printf("Map: %ld\n", map36);
    // printf("12 bits: %d\n", literalFinalLD36);

    int literalFinalLD48 = (LDLiteral & map48) >> 16;
    // printf("Map: %ld\n", map48);
    // printf("12 bits: %d\n", literalFinalLD48);

    int literalFinalLD60 = (LDLiteral & map60) >> 4;
    // printf("Map: %ld\n", map60);
    // printf("12 bits: %d\n", literalFinalLD60);

    int literalFinalLD64 = LDLiteral & map64;
    // printf("Map: %ld\n", map64);
    // printf("12 bits: %d\n", literalFinalLD64);

    /*clr rd ->*/

    writeBinary("xor", rd, rd, rd, "00000", outptr, outputName1);

    /*addi rd, L[0:11]*/

    writeBinaryLDAddi("addi", rd, "00000", "00000", literalFinalLD12, outptr, outputName1);

    /*shftli rd, 12*/

    writeBinary("shftli", rd, "00000", "00000", "12", outptr, outputName1);

    /*addi rd, L[12:23]*/

    writeBinaryLDAddi("addi", rd, "00000", "00000", literalFinalLD24, outptr, outputName1);

    /*shftli rd, 12*/

    writeBinary("shftli", rd, "00000", "00000", "12", outptr, outputName1);

    /*addi rd, L[24, 35]*/

    writeBinaryLDAddi("addi", rd, "00000", "00000", literalFinalLD36, outptr, outputName1);

    /*shftli rd, 12*/

    writeBinary("shftli", rd, "00000", "00000", "12", outptr, outputName1);

    /*addi rd, L[36, 47]*/

    writeBinaryLDAddi("addi", rd, "00000", "00000", literalFinalLD48, outptr, outputName1);

    /*shftli rd, 12*/

    writeBinary("shftli", rd, "00000", "00000", "12", outptr, outputName1);

    /*addi rd, L[48, 59]*/

    writeBinaryLDAddi("addi", rd, "00000", "00000", literalFinalLD60, outptr, outputName1);

    /*shftli rd, 4*/

    writeBinary("shftli", rd, "00000", "00000", "4", outptr, outputName1);

    /*addi rd, L[60, 63]*/

    writeBinaryLDAddi("addi", rd, "00000", "00000", literalFinalLD64, outptr, outputName1);
}

void writeBinaryLDAddi(char *opcode, char register1[], char register2[], char register3[], int literal, FILE *ptr, char *outputName)
{
    // printf("Parts to be processed: %s, %s, %s, %s, %d\n", opcode, register1, register2, register3, literal);

    /* Total bits is 32 bits*/
    int opcodeBinary = 0;               // 5 bits
    int r1Binary = 0;                   // 5 bits
    int r2Binary = 0;                   // 5 bits
    int r3Binary = 0;                   // 5 bits
    signed int literalBinary = literal; // 12 bits, don't check neg cause pass literal
    uint32_t finalBinary = 0;           // Combined at end

    // Keep if nothing in the label
    if (strcmp(register1, "00000") == 0)
    {
        r1Binary = 0;
    }
    else
    {
        r1Binary = atoi(register1);
    }

    if ((strcmp(register2, "00000") == 0))
    {
        r2Binary = 0;
    }
    else
    {
        r2Binary = atoi(register2);
    }

    if ((strcmp(register3, "00000") == 0))
    {
        r3Binary = 0;
    }
    else
    {
        r3Binary = atoi(register3);
    }
    // printf("%d", r1Binary);

    // printf("r1: %d\n", r1Binary);
    // printf("r2: %d\n", r2Binary);
    // printf("r3: %d\n", r3Binary);
    // printf("Lit: %d\n", literalBinary);

    /*Error statements*/
    if ((r1Binary < 0) || (r1Binary >= 32) || (r2Binary < 0) || (r2Binary >= 32) || (r3Binary < 0) || (r3Binary >= 32))
    {
        fprintf(stderr, "Error on line %d\n", lineCounter2);
        remove(outputName);
        exit(EXIT_FAILURE);
    }

    if (strcmp(opcode, "addi") == 0)
    {
        /*find the opcode binary*/
        for (int i = 0; i < 32; i++)
        {
            if (strcmp(binaryOpcode[i].mneumonic, opcode) == 0)
            {
                opcodeBinary = binaryOpcode[i].opcode;
                // printf("Size: %s\n", binaryOpcode[i].mneumonic);
                break;
            }
        }

        // printf("Mneumonic value stored for bits: %d\n", opcodeBinary);
        // printf("r1 %d\n", r1Binary);
        // printf("r2 %d\n", r2Binary);
        // printf("r3 %d\n", r3Binary);
        // printf("li %d\n", literalBinary);

        finalBinary = (opcodeBinary << 27) | (r1Binary << 22) | (r2Binary << 17) | (r3Binary << 12) | (literalBinary);

        // printf("Final Binary: %x\n", finalBinary);

        fwrite(&finalBinary, sizeof(finalBinary), 1, ptr);
    }
}

/*
 * Function to process labels
 * First 4 components are 5 bits long, last is 12 bits
 */
void writeBinary(char *opcode, char register1[], char register2[], char register3[], char literal[], FILE *ptr, char *outputName)
{
    // printf("Parts to be processed: %s, %s, %s, %s, %s\n", opcode, register1, register2, register3, literal);
    /* Total bits is 32 bits*/
    int opcodeBinary = 0;         // 5 bits
    int r1Binary = 0;             // 5 bits
    int r2Binary = 0;             // 5 bits
    int r3Binary = 0;             // 5 bits
    signed int literalBinary = 0; // 12 bits
    uint32_t finalBinary = 0;     // Combined at end
    // printf("Before: %d\n", r1Binary);
    // printf("Before: %d\n", r2Binary);
    // printf("Before: %d\n", r3Binary);

    // Keep if nothing in the label
    if (strcmp(register1, "00000") == 0)
    {
        r1Binary = 0;
    }
    else
    {
        r1Binary = atoi(register1);
        // printf("Before: %d\n", r1Binary);
    }

    if ((strcmp(register2, "00000") == 0))
    {
        r2Binary = 0;
    }
    else
    {
        r2Binary = atoi(register2);
    }

    if ((strcmp(register3, "00000") == 0))
    {
        r3Binary = 0;
    }
    else
    {
        r3Binary = atoi(register3);
    }

    /*Convert to numbers, account neg numbers*/
    if ((strcmp(literal, "00000") == 0))
    {
        literalBinary = 0;
    }
    else
    {
        // printf("literalchecknow: %s\n", literal);
        if (strstr(literal, "-") != NULL)
        {
            //  neg num b/c - exists or != NULL
            /*Error statement*/
            if (atoi(literal) < -2048 || atoi(literal) > 2047)
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }
            else
            {
                // Two's complement
                signed int literalBinaryTemp = atoi(literal);
                // printf("Temp: %d\n", literalBinaryTemp);
                uint32_t mapLiteral = 0b00000000000000000000111111111111;
                //  printf("Map: %d\n", mapLiteral);
                // Two's complement version of the number w leading 1's
                literalBinary = literalBinaryTemp & mapLiteral;
                // printf("LB: %d\n", literalBinary);
            }
        }
        else
        {
            // pos num
            if (atoi(literal) < 0 || atoi(literal) > 4095)
            {
                fprintf(stderr, "Error on line %d\n", lineCounter2);
                remove(outputName);
                exit(EXIT_FAILURE);
            }
            literalBinary = atoi(literal);
        }
    }
    // printf("r1: %d\n", r1Binary);
    // printf("r2: %d\n", r2Binary);
    // printf("r3: %d\n", r3Binary);
    // printf("Lit: %d\n", literalBinary);

    /*Error statements*/
    if ((r1Binary < 0) || (r1Binary >= 32) || (r2Binary < 0) || (r2Binary >= 32) || (r3Binary < 0) || (r3Binary >= 32))
    {
        fprintf(stderr, "Error on line %d\n", lineCounter2);
        remove(outputName);
        exit(EXIT_FAILURE);
    }

    /*Switch case per command*/
    if ((strcmp(opcode, "add") == 0) || (strcmp(opcode, "sub") == 0) ||
        (strcmp(opcode, "mul") == 0) || (strcmp(opcode, "div") == 0) ||
        (strcmp(opcode, "and") == 0) || (strcmp(opcode, "or") == 0) ||
        (strcmp(opcode, "xor") == 0) || (strcmp(opcode, "shftr") == 0) ||
        (strcmp(opcode, "shftl") == 0) || (strcmp(opcode, "brgt") == 0) || (strcmp(opcode, "addf") == 0) ||
        (strcmp(opcode, "subf") == 0) || (strcmp(opcode, "mulf") == 0) || (strcmp(opcode, "divf") == 0))
    {
        // instr rd, rs, rt
        /*find the opcode binary*/
        for (int i = 0; i < 32; i++)
        {
            if (strcmp(binaryOpcode[i].mneumonic, opcode) == 0)
            {
                opcodeBinary = binaryOpcode[i].opcode;
                // printf("Size: %s\n", binaryOpcode[i].mneumonic);
                break;
            }
        }
        // printf("Mneumonic value stored for bits: %d\n", opcodeBinary);
        // printf("r1 %d\n", r1Binary);
        // printf("r2 %d\n", r2Binary);
        // printf("r3 %d\n", r3Binary);
        // printf("li %d\n", literalBinary);

        finalBinary = (opcodeBinary << 27) | (r1Binary << 22) | (r2Binary << 17) | (r3Binary << 12) | (literalBinary);

        // printf("Final Binary: %x\n", finalBinary);

        fwrite(&finalBinary, sizeof(finalBinary), 1, ptr);
    }
    else if ((strcmp(opcode, "addi") == 0) || (strcmp(opcode, "subi") == 0) || (strcmp(opcode, "not") == 0) || (strcmp(opcode, "shftri") == 0) || (strcmp(opcode, "shftli") == 0) || (strcmp(opcode, "brnz") == 0) ||
             (strcmp(opcode, "in") == 0) || (strcmp(opcode, "out") == 0))
    {
        // register and literal in rest
        if (((strcmp(opcode, "addi")) == 0) || ((strcmp(opcode, "subi")) == 0) || ((strcmp(opcode, "shftri")) == 0) || ((strcmp(opcode, "shftli")) == 0))
        {
            /*find the opcode binary*/
            for (int i = 0; i < 32; i++)
            {
                if (strcmp(binaryOpcode[i].mneumonic, opcode) == 0)
                {
                    opcodeBinary = binaryOpcode[i].opcode;
                    //    printf("Size: %s\n", binaryOpcode[i].mneumonic);
                    break;
                }
            }

            // printf("Mneumonic value stored for bits: %d\n", opcodeBinary);
            // printf("r1 %d\n", r1Binary);
            // printf("r2 %d\n", r2Binary);
            // printf("r3 %d\n", r3Binary);
            // printf("li %d\n", literalBinary);

            finalBinary = (opcodeBinary << 27) | (r1Binary << 22) | (r2Binary << 17) | (r3Binary << 12) | (literalBinary);

            //  printf("Final Binary: %x\n", finalBinary);

            fwrite(&finalBinary, sizeof(finalBinary), 1, ptr);
        }
        // // 2 registers
        else if (((strcmp(opcode, "not")) == 0) || ((strcmp(opcode, "brnz")) == 0) ||
                 ((strcmp(opcode, "in")) == 0) || ((strcmp(opcode, "out")) == 0))
        {
            /*find the opcode binary*/
            for (int i = 0; i < 32; i++)
            {
                if (strcmp(binaryOpcode[i].mneumonic, opcode) == 0)
                {
                    opcodeBinary = binaryOpcode[i].opcode;
                    //  printf("Size: %s\n", binaryOpcode[i].mneumonic);
                    break;
                }
            }
            // printf("Mneumonic value stored for bits: %d\n", opcodeBinary);
            // printf("r1 %d\n", r1Binary);
            // printf("r2 %d\n", r2Binary);
            // printf("r3 %d\n", r3Binary);
            // printf("li %d\n", literalBinary);

            finalBinary = (opcodeBinary << 27) | (r1Binary << 22) | (r2Binary << 17) | (r3Binary << 12) | (literalBinary);

            //  printf("Final Binary:  %x\n", finalBinary);

            fwrite(&finalBinary, sizeof(finalBinary), 1, ptr);
        }
    }
    else if (((strcmp(opcode, "br")) == 0) || (strcmp(opcode, "call") == 0))
    // 1 register
    {
        /*find the opcode binary*/
        for (int i = 0; i < 32; i++)
        {
            if (strcmp(binaryOpcode[i].mneumonic, opcode) == 0)
            {
                opcodeBinary = binaryOpcode[i].opcode;
                //  printf("Size: %s\n", binaryOpcode[i].mneumonic);
                break;
            }
        }

        // printf("Mneumonic value stored for bits: %d\n", opcodeBinary);
        // printf("r1 %d\n", r1Binary);
        // printf("r2 %d\n", r2Binary);
        // printf("r3 %d\n", r3Binary);
        // printf("li %d\n", literalBinary);

        finalBinary = (opcodeBinary << 27) | (r1Binary << 22) | (r2Binary << 17) | (r3Binary << 12) | (literalBinary);

        //  printf("Final Binary:  %x\n", finalBinary);

        fwrite(&finalBinary, sizeof(finalBinary), 1, ptr);
    }
    else if (((strcmp(opcode, "brr")) == 0))
    {
        // 2 versions
        if (literalBinary == 0)
        {
            // brr rd
            opcodeBinary = 0b01111;

            // printf("Mneumonic value stored for bits: %d\n", opcodeBinary);
            // printf("r1 %d\n", r1Binary);
            // printf("r2 %d\n", r2Binary);
            // printf("r3 %d\n", r3Binary);
            // printf("li %d\n", literalBinary);

            finalBinary = (opcodeBinary << 27) | (r1Binary << 22) | (r2Binary << 17) | (r3Binary << 12) | (literalBinary);

            //  printf("Final Binary: %x\n", finalBinary);

            fwrite(&finalBinary, sizeof(finalBinary), 1, ptr);
        }
        else
        {
            // brr L
            opcodeBinary = 0b10000;

            if (literalBinary < 0)
            {
                short mask = 0xFFF;
                literalBinary = literalBinary & mask;
            }

            // printf("Mneumonic value stored for bits: %d\n", opcodeBinary);
            // printf("r1 %d\n", r1Binary);
            // printf("r2 %d\n", r2Binary);
            // printf("r3 %d\n", r3Binary);
            // printf("li %d\n", literalBinary);

            finalBinary = (opcodeBinary << 27) | (r1Binary << 22) | (r2Binary << 17) | (r3Binary << 12) | (literalBinary);

            //  printf("Final Binary: %x\n", finalBinary);

            fwrite(&finalBinary, sizeof(finalBinary), 1, ptr);
        }
    }
    else if (((strcmp(opcode, "mov")) == 0))
    {
        // printf("Move flag Start: %d\n", moveFlag);
        // 4 versions
        // mov rd, (rs)(L)
        // printf("Literal Binary: %d\n", literalBinary);
        if (moveFlag == 0)
        {
            opcodeBinary = 0b10101;
            if (literalBinary < 0)
            {
                short mask = 0xFFF;
                literalBinary = literalBinary & mask;
            }
            moveFlag = 2;
        }
        // mov (rd)(L), rs
        else if (moveFlag == 1)
        {
            opcodeBinary = 0b11000;
            if (literalBinary < 0)
            {
                short mask = 0xFFF;
                literalBinary = literalBinary & mask;
            }
            // printf("Opcode here: %d\n", opcodeBinary);
            moveFlag = 2;
        }
        // mov rd, rs
        else if (literalBinary == 0)
        {
            // printf("Here");
            opcodeBinary = 0b10110;
        }
        // mov rd, L
        else if (r2Binary == 0)
        {
            opcodeBinary = 0b10111;
        }

        // printf("Move flag: %d\n", moveFlag);
        // printf("Mneumonic value stored for bits: %d\n", opcodeBinary);
        // printf("r1 %d\n", r1Binary);
        // printf("r2 %d\n", r2Binary);
        // printf("r3 %d\n", r3Binary);
        // printf("li %d\n", literalBinary);

        finalBinary = (opcodeBinary << 27) | (r1Binary << 22) | (r2Binary << 17) | (r3Binary << 12) | (literalBinary);

        //  printf("Final Binary: %d\n", finalBinary);

        fwrite(&finalBinary, sizeof(finalBinary), 1, ptr);
    }
    else if (((strcmp(opcode, "return")) == 0) || ((strcmp(opcode, "halt")) == 0))
    {
        /*find the opcode binary*/
        for (int i = 0; i < 32; i++)
        {
            if (strcmp(binaryOpcode[i].mneumonic, opcode) == 0)
            {
                opcodeBinary = binaryOpcode[i].opcode;
                // printf("Size: %s\n", binaryOpcode[i].mneumonic);
                break;
            }
        }

        // printf("Mneumonic value stored for bits: %d\n", opcodeBinary);
        // printf("r1 %d\n", r1Binary);
        // printf("r2 %d\n", r2Binary);
        // printf("r3 %d\n", r3Binary);
        // printf("li %d\n", literalBinary);

        finalBinary = (opcodeBinary << 27) | (r1Binary << 22) | (r2Binary << 17) | (r3Binary << 12) | (literalBinary);

        //  printf("Final Binary: %x\n", finalBinary);

        fwrite(&finalBinary, sizeof(finalBinary), 1, ptr);
    }
}

int main(int argc, char *argv[])
{
    /*
     * Step 1: Scan the file
     */
    // Scan with .tk extension
    FILE *fptr;

    if ((argc != 2) || (argv[1] == NULL) || (!strstr(argv[1], ".tk")))
    {
        fprintf(stderr, "Invalid tinker filepath\n");
        exit(EXIT_FAILURE);
    }

    // Open a file in read mode
    fptr = fopen(argv[1], "r");

    // Open the binary file for writing mode
    FILE *binaryOutput;

    char *inputName = argv[1];
    // printf("%s", inputName);
    char *name = strtok(inputName, ".");
    // printf("%s\n", name);
    char *tkoEnd = ".tko";
    char *fileNameOutput = strcat(name, tkoEnd);
    // printf("%s\n", fileNameOutput);

    // printf("%s", fileNameOutput);

    binaryOutput = fopen(fileNameOutput, "wb");

    if (fptr == NULL)
    {
        fprintf(stderr, "Invalid tinker filepath.\n");
        remove(fileNameOutput);
        exit(EXIT_FAILURE);
    }
    if (binaryOutput == NULL)
    {
        fprintf(stderr, "Invalid tinker filepath\n");
        remove(fileNameOutput);
        exit(EXIT_FAILURE);
    }

    char buffer[256];

    int lines;
    int codeCheck = 0;

    /*First loop to count number of lines & check at least 1 .code*/
    // Read the content and finds number of lines
    while (fgets(buffer, 256, fptr))
    {
        lines++;

        // Step 1: Replace new line character with null terminator
        if (buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
        }

        if (strcmp(buffer, ".code") == 0)
        {
            codeCheck++;
        }
    }
    // printf("Hi\n");
    // printf("%d\n", codeCheck);
    if (codeCheck < 1)
    {
        fprintf(stderr, "Error on line 0.\n");
        remove(fileNameOutput);
        exit(EXIT_FAILURE);
    }

    //  printf("Lines: %d\n", lines);

    /* Start over file by moving pointer to end*/
    fseek(fptr, 0, SEEK_SET);

    /*Initialize address array for label*/
    uint64_t addresses[lines];
    // printf("Check value: %ld\n", addresses[0]);
    int index = 0; // index in array
    int lineHere = 0;
    int value = 0; // incrementing value overtime

    /*Loop through to make the bytes*/
    while (fgets(buffer, 256, fptr))
    {
        lineHere++;
        // Step 1: Replace new line character with null terminator
        if (buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
        }

        // Step 2: empty lines and ; (meaning comments)
        if (buffer[0] == '\0' || buffer[0] == ';')
        {
            value += 0;
            addresses[index] = value;
            index++;
        }
        // Step 3: Process instructions after the : character
        else if (buffer[0] == ':')
        {
            value += 0;
            addresses[index] = value;
            index++;
        }
        // Step 4: .code use instructions processor function
        else if (strcmp(buffer, ".code") == 0) // Since string has to use strcmp
        {
            // Indicates gonna have instructions (assembly) after and do related querries
            flag = 1;
            value += 0;
            addresses[index] = value;
            index++;
        }
        // Step 5: .data need to use data processor function
        else if (strcmp(buffer, ".data") == 0)
        {
            // Indicates gonna have : after and need to do data related querries
            flag = 0;
            value += 0;
            addresses[index] = value;
            index++;
        }
        else
        {
            if (flag == 1) // instructions are 4 bytes each
            {
                /*macro check, diff bytes per command*/
                /*
                 * Remove tab
                 */
                int i = 0;
                char lineCheck[256];
                while (buffer[i] == '\t')
                {
                    i++;
                }
                strcpy(lineCheck, buffer + i);

                /*
                 * Convert mnemonic to binary opcode
                 */
                char *mneumonicForAddress = strtok(lineCheck, " ");

                if (strcmp(mneumonicForAddress, "clr") == 0)
                {
                    value += 4;
                }
                else if (strcmp(mneumonicForAddress, "ld") == 0)
                {
                    value += 48;
                }
                else if (strcmp(mneumonicForAddress, "push") == 0)
                {
                    value += 8;
                }
                else if (strcmp(mneumonicForAddress, "pop") == 0)
                {
                    value += 8;
                }
                else
                {
                    value += 4;
                }

                addresses[index] = value;
                index++;
            }
            else if (flag == 0)
            {
                value += 8;
                addresses[index] = value;
                index++;
            }
            else
            {
                fprintf(stderr, "Error on line %d.\n", (lineHere));
                remove(fileNameOutput);
                exit(EXIT_FAILURE);
            }
        }
        // printf("Flag is: %d\n", flag);
        // printf("Value now is: %d\n", value);
        // printf("Line is: %s\n", buffer);
        // printf("Index is: %d\n", index);
        // printf("Array now:%ld\n", addresses[index - 1]);
    }

    /* Start over file by moving pointer to end*/
    fseek(fptr, 0, SEEK_SET);
    int lineCounter = 0;

    /* Stores the label addresses*/
    while (fgets(buffer, 256, fptr))
    {
        lineCounter2++;
        // Step 1: Replace new line character with null terminator
        if (buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
        }
        if (buffer[0] == ':')
        {
            uint64_t addressValue = addresses[lineCounter];
            labelProcessor(buffer, binaryOutput, addressValue, fileNameOutput);
        }
        lineCounter++;
    }

    lineCounter2 = 0;

    /* Start over file by moving pointer to end*/
    fseek(fptr, 0, SEEK_SET);

    while (fgets(buffer, 256, fptr))
    {
        // Step 1: Replace new line character with null terminator
        if (buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
        }

        // Step 2: Remove empty lines and ; (meaning comments)
        if (buffer[0] == '\0' || buffer[0] == ';')
        {
            lineCounter2++;
            continue;
        }
        // Step 3: Process instructions after the : character
        else if (buffer[0] == ':')
        {
            lineCounter2++;
            continue;
        }
        // Step 4: .code use instructions processor function
        else if (strcmp(buffer, ".code") == 0) // Since string has to use strcmp
        {
            // Indicates gonna have instructions (assembly) after and do related querries
            flag = 1;
            lineCounter2++;
            continue;
        }
        // Step 5: .data need to use data processor function
        else if (strcmp(buffer, ".data") == 0)
        {
            // Indicates gonna have : after and need to do data related querries
            flag = 0;
            lineCounter2++;
            continue;
        }
        else
        {
            if (flag == 1)
            {
                countcode++;
                lineCounter2++;
                //printf("%d\n", lineCounter2);
                instructionsProcessors(buffer, binaryOutput, fileNameOutput);
            }
            else if (flag == 0)
            {
                countData++;
                lineCounter2++;
                dataProcessor(buffer, binaryOutput, fileNameOutput);
            }
        }
    }
    // printf("LineCounter:%d\n", lineCounter);
    //  printf("Code: %d, Data: %d\n", countcode, countData);
    //  printf("1:%d\n2:%d\n3:%d\n4:%d\n5:%d\n6:%d\n7:%d\n8:%d\n9:%d\n10:%d\n11:%d\n12:%d\n", count1, count2, count3, count4, count5, count5, count7, count8, count9, count10, count11, count12);
    //       printf("Sum\n");
    //       printf("Sum%d\n", count1+count2+count3+count4+count5+count6+count7+count8+count9+count10+count11+count12);
    if (binaryOutput == NULL)
    {
        fprintf(stderr, "Invalid tinker filepath\n");
        remove(fileNameOutput);
        exit(EXIT_FAILURE);
    }
    // else
    // {
    //     printf("Sucessful output file!\n");
    // }
    fclose(fptr);
    fclose(binaryOutput);

    return 0;
}
