#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MEMORY_SIZE 512 * 1024 // 512 KB, or just do 512?
#define REGISTERNUM 32

/*Global variables*/
uint64_t registers[REGISTERNUM];
uint64_t PC = 0;
uint8_t memory[MEMORY_SIZE];

/*
 * Read memory
 * Returns the memory stored at the given address
 */
uint64_t readMemory(uint64_t address, int nbytes)
{
    // printf("Address: %ld\n", address);
    if (address + nbytes < 0)
    {
        fprintf(stderr, "Simulation error\n");
        exit(EXIT_FAILURE);
    }
    if (address + nbytes > MEMORY_SIZE)
    {
        fprintf(stderr, "Simulation error\n");
        exit(EXIT_FAILURE);
    }

    uint64_t final = 0b0ull;

    for (int i = 0; i < nbytes; i++)
    {
        final |= (memory[address + i] << (8 * i));
    }

    return final;
}

/*
 * Write Memory
 * Inputs a value in the memory stored at a given address
 */
void writeMemory(uint64_t address, uint64_t data, int nbytes)
{
    if (address + nbytes > MEMORY_SIZE)
    {
        fprintf(stderr, "Simulation error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nbytes; i++)
    {
        memory[address + i] = (data >> (8 * i));
    }
}

/* Functions for each command*/
/*
 * Add execution
 * Use for add and addf
 */
void add(uint16_t instruc[])
{
    // Tested
    // printf("Inside add:\n");
    // printf("%d\n", instruc[0]);
    // printf("%d\n", instruc[1]);
    // printf("%d\n", instruc[2]);
    // printf("%d\n", instruc[3]);

    registers[instruc[0]] = ((int64_t)registers[instruc[1]]) + ((int64_t)registers[instruc[2]]);
    PC += 4;

    // printf("Rd: %ld\n", registers[instruc[0]]);
    // printf("Rs: %ld\n", registers[instruc[1]]);
    // printf("Rt: %ld\n", registers[instruc[2]]);
}

/*Addi */
void addi(uint16_t instruc[])
{
    // Tested
    //  printf("Inside addi:\n");
    //  printf("%d\n", instruc[0]);
    //  printf("%d\n", instruc[1]);
    //  printf("%d\n", instruc[2]);
    //  printf("%d\n", instruc[3]);

    registers[instruc[0]] = registers[instruc[0]] + instruc[3];
    PC += 4;

    // printf("Rd: %ld\n", registers[instruc[0]]);
    // printf("L: %d\n", instruc[3]);
}

/*
 * Sub execution
 * Use for sub and suf
 */
void sub(uint16_t instruc[])
{
    // Tested
    //  printf("Inside sub:\n");
    //  printf("%d\n", instruc[0]);
    //  printf("%d\n", instruc[1]);
    //  printf("%d\n", instruc[2]);
    //  printf("%d\n", instruc[3]);

    registers[instruc[0]] = ((int64_t)registers[instruc[1]]) - ((int64_t)registers[instruc[2]]);
    PC += 4;

    // printf("Rd: %ld\n", registers[instruc[0]]);
    // printf("Rs: %ld\n", registers[instruc[1]]);
    // printf("Rt: %ld\n", registers[instruc[2]]);
}

/*Subi execution */
void subi(uint16_t instruc[])
{
    // tested
    // printf("Subi\n");
    registers[instruc[0]] = registers[instruc[0]] - instruc[3];
    PC += 4;
}

/*
 * Mul execution
 * Use for mul
 */
void mul(uint16_t instruc[])
{
    // tested
    // printf("MUL\n");
    // printf("MUl\n");
    // printf("rd: %ld\n", registers[instruc[0]]);
    // printf("rs: %ld\n", registers[instruc[1]]);
    // printf("rt: %ld\n", registers[instruc[2]]);
    // printf("L: %d\n", instruc[3]);
    registers[instruc[0]] = ((int64_t)registers[instruc[1]]) * ((int64_t)registers[instruc[2]]);
    // printf("MUl rd: %ld\n", registers[instruc[0]]);
    PC += 4;
}

/*
 * Div execution
 * Use for div and divf
 */
void divInstruc(uint16_t instruc[])
{
    // tested
    if (registers[instruc[2]] == 0)
    {
        fprintf(stderr, "Simulation error\n");
        exit(EXIT_FAILURE);
    }
    if ((registers[instruc[2]] != 0))
    {
        registers[instruc[0]] = ((int64_t)registers[instruc[1]]) / ((int64_t)registers[instruc[2]]);
        PC += 4;
    }
    else
    {
        fprintf(stderr, "Simulation error\n");
        exit(EXIT_FAILURE);
    }
}

void and (uint16_t instruc[])
{
    // tested
    registers[instruc[0]] = registers[instruc[1]] & registers[instruc[2]];
    PC += 4;
}

void or (uint16_t instruc[])
{
    // tested
    registers[instruc[0]] = registers[instruc[1]] | registers[instruc[2]];
    PC += 4;
}

void xor (uint16_t instruc[]) {
    // tested
    registers[instruc[0]] = registers[instruc[1]] ^ registers[instruc[2]];
    PC += 4;
}

    void not(uint16_t instruc[])
{
    // tested
    registers[instruc[0]] = ~registers[instruc[1]];
    PC += 4;
}

void shftr(uint16_t instruc[])
{
    // tested
    registers[instruc[0]] = ((registers[instruc[1]]) >> (registers[instruc[2]]));
    PC += 4;
}

void shftri(uint16_t instruc[])
{
    // tested
    // printf("Shftri:\n");
    // printf("rd: %ld\n", (registers[instruc[0]]));
    // printf("rs: %d\n", instruc[3]);
    registers[instruc[0]] = ((registers[instruc[0]]) >> (instruc[3]));
    // printf("rd: %ld\n", (registers[instruc[0]]));
    PC += 4;
}

void shftl(uint16_t instruc[])
{
    // tested
    registers[instruc[0]] = ((registers[instruc[1]]) << (registers[instruc[2]]));
    PC += 4;
}

void shftli(uint16_t instruc[])
{
    // tested
    // printf("Inside shftli:\n");
    // printf("%d\n", instruc[0]);
    // printf("%d\n", instruc[1]);
    // printf("%d\n", instruc[2]);
    // printf("%d\n", instruc[3]);

    registers[instruc[0]] = ((registers[instruc[0]]) << (instruc[3]));
    PC += 4;

    //  printf("Rd: %ld\n", registers[instruc[0]]);
}

void br(uint16_t instruc[])
{
    // tested
    // printf("Inside br:\n");
    // printf("%d\n", instruc[0]);
    // printf("%d\n", instruc[1]);
    // printf("%d\n", instruc[2]);
    // printf("%d\n", instruc[3]);

    PC = registers[instruc[0]];

    // printf("RD: %ld\n", registers[instruc[0]]);
    // printf("PC: %ld\n", PC);
}

void brr_rd(uint16_t instruc[])
{
    // tested
    PC = PC + registers[instruc[0]];
}

void brr_L(uint16_t instruc[])
{
    int16_t x = instruc[3] << 4;
    x = x >> 4;
    PC = PC + x;
}

void brnz(uint16_t instruc[])
{
    if (registers[instruc[1]] == 0)
    {
        PC = PC + 4;
    }
    else
    {
        PC = registers[instruc[0]];
    }
}

// Section

void call(uint16_t instruc[])
{
    // printf("Call\n");
    writeMemory((registers[31] - 8), (PC + 4), 8);
    PC = registers[instruc[0]];
}

void returnCall(uint16_t instruc[])
{
    PC = readMemory((registers[31] - 8), 8);
}

void brgt(uint16_t instruc[])
{
    if (((int64_t)registers[instruc[1]]) <= ((int64_t)registers[instruc[2]]))
    {
        PC = PC + 4;
    }
    else
    {
        PC = registers[instruc[0]];
    }
}

// Section

void mov_rd_rs_L(uint16_t instruc[])
{
    // printf("HI\n");
    //  tested
    //  printf("Inside mov_rd_rs_L:\n");
    //  printf("%d\n", instruc[0]);
    //  printf("%d\n", instruc[1]);
    //  printf("%d\n", instruc[2]);
    //  printf("%d\n", instruc[3]);
    //  printf("Break\n");
    int16_t x = instruc[3] << 4;
    x = x >> 4;
    registers[instruc[0]] = readMemory((registers[instruc[1]] + x), 8);
    PC += 4;

    // printf("rd: %ld\n", registers[instruc[0]]);
    // printf("rs: %ld\n", registers[instruc[1]]);
    // printf("PC: %ld\n", PC);
}

void mov_rd_rs(uint16_t instruc[])
{
    registers[instruc[0]] = registers[instruc[1]];
    PC += 4;
}

/*Check this*/
void mov_rd_L(uint16_t instruc[])
{
    uint64_t rd_Value = registers[instruc[0]];

    // Mask bits 52 to 63 or last 12 bits
    uint64_t bitMask = 0b1111111111111111111111111111111111111111111111111111000000000000;
    uint64_t rd_AfterMask = rd_Value & bitMask;
    registers[instruc[0]] = rd_AfterMask | (((uint64_t)instruc[3]));
    PC += 4;
}

void mov_rd_L_rs(uint16_t instruc[])
{
    // Tested
    // printf("Inside mov_rd_rs:\n");
    // printf("%d\n", instruc[0]);
    // printf("%d\n", instruc[1]);
    // printf("%d\n", instruc[2]);
    // printf("%d\n", instruc[3]);
    // printf("Break\n");
    int16_t x = instruc[3] << 4;
    x = x >> 4;
    writeMemory((registers[instruc[0]] + x), registers[instruc[1]], 8);
    PC += 4;

    // printf("rd: %ld\n", registers[instruc[0]]);
    // printf("rs: %ld\n", registers[instruc[1]]);
    // printf("PC: %ld\n", PC);
}

void addf(uint16_t instruc[])
{

    double rs = *(double *)&registers[instruc[1]];
    double rt = *(double *)&registers[instruc[2]];
    double result = rs + rt;
    // printf("result: %f\n", rs);
    // printf("result: %f\n", rt);
    // printf("result: %f\n", result);
    registers[instruc[0]] = *((uint64_t *)&result);
    PC += 4;
}

void subf(uint16_t instruc[])
{
    // printf("SUB\n");
    double rs = *(double *)&registers[instruc[1]];
    double rt = *(double *)&registers[instruc[2]];
    double result = rs - rt;

    // printf("result: %f\n", rs);
    // printf("result: %f\n", rt);
    // printf("result: %f\n", result);
    registers[instruc[0]] = *((uint64_t *)&result);
    // printf("Subf result: %ld\n", registers[instruc[0]]);
    PC += 4;
}

void mulf(uint16_t instruc[])
{
    // printf("MUlF\n");
    // printf("rs: %ld\n", registers[instruc[1]]);
    // printf("rt: %ld\n", registers[instruc[2]]);
    // printf("why: %ld\n", (registers[instruc[1]]*registers[instruc[2]]));
    double rs = *(double *)&registers[instruc[1]];
    double rt = *(double *)&registers[instruc[2]];

    // uint64_t value = registers[instruc[1]] * registers[instruc[2]];

    // printf("Val: %ld\n", value);

    // double result = *(double *)&value;
    double result = rs * rt;

    // printf("result: %lf\n", rs);
    // printf("result: %lf\n", rt);
    // printf("result: %lf\n", result);
    registers[instruc[0]] = *((uint64_t *)&result);
    // printf("Fox mulf: %ld", registers[instruc[0]]);
    PC += 4;
}

void divf(uint16_t instruc[])
{
    if (registers[instruc[1]] == 0)
    {
        uint64_t value = 0;
        double result = *(double *)&value;
        registers[instruc[0]] = *((uint64_t *)&result);
        PC += 4;
    }
    else if (registers[instruc[2]] != 0)
    {
        double rs = *(double *)&registers[instruc[1]];
        double rt = *(double *)&registers[instruc[2]];
        // uint64_t value = registers[instruc[1]] / registers[instruc[2]];
        // double result = *(double *)&value;
        double result = rs/rt;
        registers[instruc[0]] = *((uint64_t *)&result);
        PC += 4;
    }
    else
    {
        fprintf(stderr, "Simulation error\n");
        exit(EXIT_FAILURE);
    }
}

void input(uint16_t instruc[])
{
    // tested
    if (registers[instruc[1]] == 0)
    {
        uint64_t num = 0;
        // printf("Enter the value you want to be stored in rd:\n");
        scanf("%lu", &num);
        registers[instruc[0]] = num;
        PC += 4;
    }
}

void output(uint16_t instruc[])
{
    // tested
    if (registers[instruc[0]] == 1)
    {
        printf("%lu\n", registers[instruc[1]]);
    }
    PC += 4;
}

// Section

/*Halt */
void halt()
{
    // printf("Halt is reached, exit!\n");
    exit(EXIT_SUCCESS);
}

/*
 * Finds the opcode and references the correct instruction function
 */
void decodeBinary(uint8_t opcode, uint16_t operands[])
{
    // printf("Opcode: %d\n", opcode);
    // printf("rd: %d\n", operands[0]);
    // printf("rs: %d\n", operands[1]);
    // printf("rt: %d\n", operands[2]);
    // printf("L: %d\n", operands[3]);
    /* start w halt, because need way for loop to end, then check others and go*/
    switch (opcode)
    {
    case 0b00000:
        add(operands);
        break;
    case 0b00001:
        addi(operands);
        break;
    case 0b00010:
        sub(operands);
        break;
    case 0b00011:
        subi(operands);
        break;
    case 0b00100:
        mul(operands);
        break;
    case 0b00101:
        divInstruc(operands);
        break;
    case 0b00110:
        and(operands);
        break;
    case 0b00111:
        or (operands);
        break;
    case 0b01000:
        xor(operands);
        break;
    case 0b01001:
        not(operands);
        break;
    case 0b01010:
        shftr(operands);
        break;
    case 0b01011:
        shftri(operands);
        break;
    case 0b01100:
        shftl(operands);
        break;
    case 0b01101:
        shftli(operands);
        break;
    case 0b01110:
        br(operands);
        break;
    case 0b01111:
        brr_rd(operands);
        break;
    case 0b10000:
        brr_L(operands);
        break;
    case 0b10001:
        brnz(operands);
        break;
    case 0b10010:
        call(operands);
        break;
    case 0b10011:
        returnCall(operands);
        break;
    case 0b10100:
        brgt(operands);
        break;
    case 0b10101:
        mov_rd_rs_L(operands);
        break;
    case 0b10110:
        mov_rd_rs(operands);
        break;
    case 0b10111:
        mov_rd_L(operands);
        break;
    case 0b11000:
        mov_rd_L_rs(operands);
        break;
    case 0b11001:
        addf(operands);
        break;
    case 0b11010:
        subf(operands);
        break;
    case 0b11011:
        mulf(operands);
        break;
    case 0b11100:
        divf(operands);
        break;
    case 0b11101:
        input(operands);
        break;
    case 0b11110:
        output(operands);
        break;
    case 0b11111:
        halt();
        break;
    default:
        fprintf(stderr, "Simulation error\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * Do an execute instructions function
 * switch case per opcode and calls above functions
 */
void readAndExecute()
{
    // Intake 32 bit instruction
    uint32_t instruction = readMemory(PC, 4);

    // Bit mask for each part
    uint32_t bitMaskO = 0b11111;
    uint32_t bitMaskrd = 0b1111100000;                         // 0b11111 << 5
    uint32_t bitMaskrs = 0b111110000000000;                    // 0b11111 << 10
    uint32_t bitMaskrL = 0b11111000000000000000;               // 0b11111 << 15
    uint32_t bitLiteral = 0b11111111111100000000000000000000u; // 0b11111 << 20

    // Opcode
    uint8_t opcode = (instruction >> 27) & (0x1f);
    // printf("Opcode: %d\n", opcode);

    // Operands
    uint16_t operands[4];
    operands[0] = ((instruction >> 22) & (0x1f)); // rd
    operands[1] = ((instruction >> 17) & (0x1f)); // rs
    operands[2] = ((instruction >> 12) & (0x1f)); // rt
    operands[3] = ((instruction) & (0xfff));      // L

    /*
    opcode = (instr >> 27) & 0x1f
rd = (instr >> 22) & 0x1f
rs = (instr >> 17) & 0x1f
rt = (instr >> 12) & 0x1f
L = instr & 0xfff*/

    // printf("rd: %d\n", operands[0]);
    // printf("rs: %d\n", operands[1]);
    // printf("rt: %d\n", operands[2]);
    // printf("L: %d\n", operands[3]);

    if (PC > MEMORY_SIZE || PC < 0)
    {
        fprintf(stderr, "Simulation error\n");
        exit(EXIT_FAILURE);
    }

    decodeBinary(opcode, operands);
    // exit(EXIT_SUCCESS);             // remove once done checking,
}

/*
 * Reads and executes the instructions of the binary file
 * Calls the other methods
 * If read halt terminate
 */

int main(int argc, char *argv[])
{

    /*
     * Step 1: Scan the file
     */
    // Scan with .tk extension
    FILE *fp;

    if ((argc != 2) || (argv[1] == NULL) || (!strstr(argv[1], ".tko")))
    {
        fprintf(stderr, "Invalid tinker filepath\n");
        exit(EXIT_FAILURE);
    }

    // Open a file in read mode ~ first compile assembler
    // Then compile this w "t1.tko"
    fp = fopen(argv[1], "rb");

    // Initialize arrays
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i] = 0;
    }

    for (int j = 0; j < REGISTERNUM; j++)
    {
        registers[j] = 0;
    }

    registers[31] = MEMORY_SIZE;

    if (fp == NULL)
    {
        fprintf(stderr, "Invalid tinker filepath\n");
        exit(EXIT_FAILURE);
    }
    // else
    // {
    //     printf("Sucess\n");
    // }

    size_t readFile = fread(memory, sizeof(uint8_t), MEMORY_SIZE, fp);

    // printf("Size: %ld\n", readFile / sizeof(uint8_t));
    // Check if the data was read successfully.
    if (readFile == 0)
    {
        fprintf(stderr, "Invalid tinker filepath\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    // // fread isn't storing into memory properly
    // for (int i = 0; i < 150; i++)
    // {
    //     printf("%02X", memory[i]);
    // }
    // printf("\n");
    // if (readFile == MEMORY_SIZE)
    // {
    //     printf("Size is correct!\n");
    // }

    while (true)
    {
        readAndExecute();
    }

    // Close the file.
    fclose(fp);

    return 0;
}
