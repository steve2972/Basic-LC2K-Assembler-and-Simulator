#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int returnIndex(FILE *, char *);
int Char2Num(char *);
void check(int);

int main(int argc, char *argv[]) {
    char *inFileString, *outFileString;
    FILE *inFilePtr, *inFilePtr2, *outFilePtr;

    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH];
    char arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc!= 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n"
        , argv[0]);
        exit(1);
    }
    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    inFilePtr2 = fopen(inFileString, "r");

    //check if files exist and have values
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }

    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
    }

/*
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        //reached end of file
        if (strcmp(label, "")) {
            int same = 0;
            char currentLabel[MAXLINELENGTH];
            strcpy(currentLabel, label);
            while (readAndParse(inFilePtr2, label, opcode, arg0, arg1, arg2)) {
                if (!strcmp(currentLabel, label)) {
                    same++;
                }
            }
            if (same >= 2) {
                printf("Duplicated!\n");
                exit(1);
            }
            rewind(inFilePtr2);
        }
    }*/

    rewind(inFilePtr);
    rewind(inFilePtr2);

    int PC = 0;

    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        int op;
        int arg0num, arg1num, arg2num, offset;
        int mc;

        if (!strcmp(opcode, "add")) {
            op = 0;
            arg0num = Char2Num(arg0);
            arg1num = Char2Num(arg1);
            arg2num = Char2Num(arg2);
            offset = arg2num;
        }
        else if (!strcmp(opcode, "nor")) {
            op = 1;
            arg0num = Char2Num(arg0);
            arg1num = Char2Num(arg1);
            arg2num = Char2Num(arg2);
            offset = arg2num;
        }
        else if (!strcmp(opcode, "lw")) {
            op = 2;
            arg0num = Char2Num(arg0);
            arg1num = Char2Num(arg1);
            if (isNumber(arg2)) {
                arg2num = Char2Num(arg2);
                offset = arg2num;
            }
            else {
                arg2num = returnIndex(inFilePtr2, arg2);
                offset = arg2num;
            }
        }
        else if (!strcmp(opcode, "sw")) {
            op = 3;
            arg0num = Char2Num(arg0);
            arg1num = Char2Num(arg1);
            if (isNumber(arg2)) {
                arg2num = Char2Num(arg2);
                offset = arg2num;
            }
            else {
                arg2num = returnIndex(inFilePtr2, arg2);
                offset = arg2num;
            }
        }
        else if (!strcmp(opcode, "beq")) {
            op = 4;
            arg0num = Char2Num(arg0);
            arg1num = Char2Num(arg1);
            if (isNumber(arg2)) {
                arg2num = Char2Num(arg2);
                offset = arg2num;
            }
            else {
                arg2num = returnIndex(inFilePtr2, arg2);
                offset = arg2num - PC - 1;
            }
        }
        else if (!strcmp(opcode, "jalr")) {
            op = 5;
            arg0num = Char2Num(arg0);
            arg1num = Char2Num(arg1);
            offset = 0;
        }
        else if (!strcmp(opcode, ".fill")) {
            if (isNumber(arg0)) {
                arg0num = Char2Num(arg0);
                mc = arg0num;
                fprintf(outFilePtr, "%d\n", mc);
                PC++;
                continue;
            }
            else {
                mc = returnIndex(inFilePtr2, arg0);
                fprintf(outFilePtr, "%d\n", mc);
                PC++;
                continue;
            }
        }
        else if (!strcmp(opcode, "halt")) {
            op = 6;
            arg0num = arg1num = arg2num = offset = 0;
        }
        else if (!strcmp(opcode, "noop")) {
            op = 7;
            arg0num = arg1num = arg2num = offset = 0;
        }
        else {
            printf("undefined op: %s\n", opcode);
            exit(1);
        }
        check(offset);
        if (offset < 0) {
            offset += 65536;
        }
        mc = op * 4194304 + arg0num * 524288 + arg1num * 65536 + offset;
        fprintf(outFilePtr, "%d\n", mc);
        printf("\t0x%X\n", mc);

        PC++;
    }
    printf("Created Machine Code File\n");

    fclose(inFilePtr2);
    fclose(inFilePtr);
    fclose(outFilePtr);
    exit(0);


    return 0;
}

int readAndParse(FILE *inFilePtr, char *label,
                 char *opcode, char *arg0, char *arg1, char *arg2) {
    char line[MAXLINELENGTH];
    char *ptr = line;

    //delete prior values
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        return 0;
    }

    if (strchr(line, '\n') == NULL) {
        printf("error: line too long\n");
        exit(1);
    }

    ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label)) {
        ptr += strlen(label);
    }

    //Parse the rest of the line

    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r "
                  "]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
    
    return 1;
}

int isNumber(char *string) {
    int i;
    return ((sscanf(string, "%d", &i)) == 1);
}

int returnIndex(FILE *inFilePtr2, char *inputlabel) {
  int address;
  int success = 0;
  int counter = 0;
  char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
      arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
  rewind(inFilePtr2);

  while (readAndParse(inFilePtr2, label, opcode, arg0, arg1, arg2)) {
    if (!strcmp(label, inputlabel)) {
      address = counter;
      success = 1;
    }
    counter++;
  }
  if (success) {
    return address;
  } else {
    printf("Label undefined!\n");
    exit(1);
  }

}

int Char2Num(char *string) {
  int i;
  sscanf(string, "%d", &i);
  return i;
}

void check(int offset) {
    if (offset >= -32768 && offset <= 32767) {

    }
    else {
        printf("Overflow offset\n");
        exit(1);
    }
}
