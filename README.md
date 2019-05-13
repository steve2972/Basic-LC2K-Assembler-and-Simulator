**LC2k Assembler & Simulator**

*Computer Architecture Project 1*

Files in repository  include:
- Project1.c: Assembler Program
- Simulator.c: Simulator Program
- test[i].as: Test programs - multiplying, adding, control flow, etc.

Run the following code to execute:
```
gcc Project1.c -o assembler
./assembler test[i].as test.mc
gcc Simulator.c -o simulator
./simulator test.mc output[i].txt(optional)

```

A simple overview of the assembly and simulator programs:

*Assembler*

Takes in an assembly code file as an input, parses through the instructions,
creates respective hexadecimal instructions, and writes the instructions onto 
a machine code file.

*Simulator*

Using the machine code file genereated by the assembler program, the simulator prints out
a step by step process of the computer carrying out the instructions written in the file. This includes
a real time process of the instruction memory, the Program Counter, and any changes to the registry files.

Test Cases Include the following:

1. test1 = 27252 * 67852

2. test2 = count down to 0 from 5

3. test3 = 2^9 * 111111

4. test4 = 32766 * 10838

5. test5= 4 + summation(1...100) = 5054

6. test6 = error check: undefined label
