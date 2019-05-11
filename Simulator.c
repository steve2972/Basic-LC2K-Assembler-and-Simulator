#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536
#define NUMREGS 8
#define MAXLINELENGTH 1000

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY], reg[NUMREGS];
    int numMemory;
} stateType;

void fprintState(FILE *outFile, stateType *statePtr) {
    int i;
    fprintf(outFile, "\n@@@\nstate:\n");
    fprintf(outFile, "\tpc %d\n", statePtr->pc);
    fprintf(outFile, "\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        fprintf(outFile, "\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }

    fprintf(outFile, "\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        fprintf(outFile, "\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    fprintf(outFile, "end state\n");
}

void printState(stateType *);
void decompose(int, int *, int *, int *, int *);
int convertNum(int);

int main(int argc, char *argv[]) {
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr, *outFile;

    if (argc < 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");

    char *outFileString;
    if (argc == 3) {
        outFileString = argv[2];
        outFile = fopen(outFileString, "w");
    }

    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }
    //initialize all registers and program counters
    state.pc = 0;
    for (int i = 0; i < NUMREGS; i++) {
        state.reg[i] = 0;
    }

    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
        if (sscanf(line, "%d", state.mem + state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);

    }

    int opcode, regA, regB, offset;
    int counter = 0;
    putchar('\n');
    while (1) { //simulation continues until "halt"
        //printState(&state);
        //fprintState(outFile, &state);
        decompose(state.mem[state.pc], &opcode, &regA, &regB, &offset);
        switch(opcode) {
            case 0://add
                state.reg[offset] = state.reg[regA] + state.reg[regB];
                break;
            case 1://nor
                state.reg[offset] = ~(state.reg[regA] | state.reg[regB]);
                break;
            case 2://lw
                state.reg[regB] = state.mem[state.reg[regA] + offset];
                break;
            case 3://sw
                state.mem[state.reg[regA] + offset] = state.reg[regB];
                break;
            case 4://beq
                if (state.reg[regA] == state.reg[regB])
                    state.pc += offset;
                break;
            case 5://jalr
                state.reg[regB] = state.pc + 1;
                state.pc = state.reg[regA] - 1;
                break;
            case 6://halt
                printf("Machine halted.\ntotal of %d instructions"
                " executed.\nFinal state of machine: \n", counter + 1);
                state.pc++;
                printState(&state);
                printf("\n\nRegister 1 holds: %d\n", state.reg[1]);
                return 0;
            case 7://noop
                break;
            default:
                break;
        }
        state.pc++;
        counter++;
    }

    return 0;
}

void printState(stateType *statePtr) {
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }

    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

void decompose(int mc, int *opcode, int *regA, int *regB, int *Offset) {
  *opcode = (mc >> 22) & 7;
  *regA = (mc >> 19) & 7;
  *regB = (mc >> 16) & 7;
  *Offset = convertNum(mc & 65535);

}

int convertNum(int num) {
    //convert 16 bit integer to 32 bit Linux integer

    if (num & (1 << 15)) {
        num -= (1 << 16);
    }

    return num;
}