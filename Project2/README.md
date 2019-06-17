**LC2k Assembler & Simulator**

*Computer Architecture Project 2: Pipelining*

Files in repository  include:
- Assembler2.c: improved version of Assembler1
- Simulator2.c: Simulator Program

Run the following code to execute:
```
gcc -o simulator Simulator2.c
gcc -o asol Assembler2.c
./asol <file_name>.as test.mc
./simulator test.mc // Will print output to output.txt

```