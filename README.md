# Instructions for Compilation and Execution:

This guide explains how to compile and run code using the g++ compiler in a Ubuntu 20.04 terminal. The versions of gcc and g++ used here are 9.30.

## Compilation:

Organize your code: Place all your source code files in a single directory.
Navigate to the directory: Open a terminal and use the cd command to change the current directory to the folder containing your source code.
Compile the code: Run the following command in the terminal to compile your code:
g++ –o executable_name ./*

## Execution:

Run the program: After successful compilation, use the following command to execute your program:
./executable_name input_file -o output_file
Replace executable_name with the actual name of your compiled program (the one you specified in step 3 of compilation).
input_file.s represents the name of your input assembly file (.s extension).
output_file is the name of the output file that will be generated after running the program.

input file example: <br/>
![image](https://github.com/ankica11/single-pass-assembler/assets/76231958/9ba2d05b-a515-4a97-a48d-e85575d47695)

output file example: <br/>
![image](https://github.com/ankica11/single-pass-assembler/assets/76231958/424048a3-9d8f-4f35-9363-1e8a106affba)

