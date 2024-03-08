#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define addOA 0b00000
#define addiOA 0b00001
#define subOA 0b00010
#define subiOA 0b00011
#define mulOA 0b00100
#define divOA 0b00101
#define andLogicOA 0b00110
#define orLogicOA 0b00111
#define xorOA 0b01000
#define notOA 0b01001
#define shftrOA 0b01010
#define shftriOA 0b01011
#define shftlOA 0b01100
#define shftliOA 0b01101
#define brOA 0b01110
#define brrOA 0b01111
#define brrOAL 0b10000
#define brnzOA 0b10001
#define callOA 0b10010
#define returnOA 0b10011
#define brgtOA 0b10100
#define haltOA 0b11111
#define mov_rd_rd_L 0b10101
#define mov_rd_rs 0b10110
#define mov_rd_L 0b10111
#define mov_rd_L_rs 0b11000
#define addfOA 0b11001
#define subfOA 0b11010
#define mulfOA 0b11011
#define divfOA 0b11100
#define inOA 0b11101
#define outOA 0b11110

/*
 * Mneumonic struct
 */
struct MneuomonicOpcode
{
    const char *mneumonic;
    int opcode;
};

/*
 * Opcode struct
 * Contains each opcodes binary
 * 2 ands, 4 mov, 2 brrs
 */
struct MneuomonicOpcode binaryOpcode [] = {
    {"add", addOA}, {"addi", addiOA}, {"sub", subOA}, {"subi", subiOA}, {"mul", mulOA}, 
    {"div", divOA}, {"and",andLogicOA}, {"or", orLogicOA},
    {"xor", xorOA}, {"not", notOA}, {"shftr", shftrOA}, {"shftri", shftriOA},
    {"shftl", shftlOA}, {"shftli", shftliOA}, {"br", brOA}, {"brr", brrOA}, {"brr", brrOAL},
    {"brnz", brnzOA}, {"call", callOA}, {"return", returnOA}, {"brgt", brgtOA},
    {"halt", haltOA}, {"mov", mov_rd_rd_L}, {"mov", mov_rd_rs}, {"mov", mov_rd_L}, 
    {"mov", mov_rd_L_rs}, {"addf",addfOA}, {"subf", subfOA},
    {"mulf", mulfOA}, {"divf", divfOA}, {"in", inOA}, {"out", outOA}
};

/*
 * Symbol table struct
 * Has a label and an address
 * Used for the instructions and register addresses
 */

typedef struct
{
    char label[64]; // 64 bit
    uint64_t address;
} Symbols;

/*
 * Oncode mapping
 * First are all opcode
 * Last four are the macro labels
 * brr has 2 types (diff reg arguments)
 * mov has 5 (diff reg arguements)
 */
const char *opcodeMapping[] = {
    "add", "addi", "sub", "subi", "mul", "div", "and", "or",
    "xor", "not", "shftr", "shftri", "shftl", "shftli", "br", "brr",
    "brnz", "call", "return", "brgt", "halt", "mov",
    "addf", "subf", "mulf", "divf", "in", "out"};

const char *macroMapping[] = {
    "clr", "ld", "push", "pop"};

/* Function declarations*/

/*
 * Function to process labels
 * Stores instruction (need to figure out)
 */
void labelProcessor(char *s, FILE *output, uint64_t addressValue, char * outputName);

/*
 * Function to process Instructions
 * Utilizes reg & binary function
 */
void instructionsProcessors(char *s, FILE *ptr, char * outputName);

/*
 * Function to process labels when 3 registers
 */
void registerProcessor(char *registers, char numberValue[], char * outputName1);

/*
* Function that literally converts the data number into binary
*/
void dataProcessor(char *s, FILE *dataptr, char * outputName);

/*
* Function that converts the LD code specifically to binary
* Takes care of the cases where literals are each separate bits
*/
void convertToBinaryLD(char *opcode, char *rd, char *L, FILE * outptr, char * outputName1);


/*
* Function to process only the addi in the LD macro
* Here the third register is passed as an int
*/
void writeBinaryLDAddi(char *opcode, char register1[], char register2[], char register[], int literal, FILE *ptr, char * outputName);

/*
 * Function to process labels
 */
void writeBinary(char *opcode, char register1 [], char register2 [], char register3 [], char literal[], FILE *ptr, char * outputName);
