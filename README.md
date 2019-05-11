**LC2k Assembler & Simulator**

*Computer Architecture Project 1*

Files in Project include:
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

Test Cases Include the following:

1. test1 = 27252 * 67852

2. test2 = count down to 0 from 5

3. test3 = 2^9 * 111111

4. test4 = 32766 * 10838

5. test 5= 4 + summation(1...100) = 5054
