# TASH: Tutorial Shell

TASH (Tutorial Shell) is a simple Unix shell written in C. It's designed as an educational tool to demonstrate the basics of how a shell works. TASH can execute commands, navigate directories, and includes a handful of built-in commands.

## Features

- Execution of system commands with arguments
- Built-in commands:
  - `cd`: Change the directory
  - `help`: Display information about the built-in commands
  - `exit`: Exit the shell
- Basic command line input

## Compilation

To compile TASH, you will need a C compiler (such as gcc) and standard build tools. Follow these steps:

1. Open a terminal window.
2. Navigate to the directory containing `tash.c`.
3. Compile the shell using the following command:

   ```bash
   gcc -o tash tash.c
