## Project #2: MIPS Emulator

### ***Due on November 17 (Friday), 24:00 ***

### Goal

Implement a MIPS emulator that executes MIPS instructions loaded on the memory.


### Problem Specification

- In this programming assignment, we implement a **MIPS emulator** which processes MIPS instructions as the name implies.

- Like previous PAs, the framework gets a line of command, parses it, and calls appropriate procedures to process the request. Your task is to complete three procedures; `process_instruction()`, `load_program()`,  and `run_program()`. The framework invokes appropriate procedures according to the input.

- When a user inputs a hexadecimal number on the CLI prompt, `process_instruction()` is called with the inputted number as the argument. The function is supposed to interprete the argument as an MIPS instruction, and **execute** it. For example, when the user inputs 0x014b4820, the emulator should add the values in `$t2` and `$t3` and stores the result into `$t1`. If the instruction is 0x8e2a0010, the a word at memory `$s1+16` is loaded into register `$t2`, so forth.

- The emulator should support following instructions. Note that some conditional branch (`slt`, `slti`), jump (`jr`, `j`, `jal`), and a special (`halt`) instructions are newly added to the previous PA1 instruction set. All instructions except `halt` is as-is of the MIPS ISA we covered in the classes. `halt` is not an actual MIPS instruction but a hypothetical instruction for testing purpose. It's machine code is 0xffffffff, and we will discuss this instruction later.

  | Name   | Format |Opcode / opcode + funct |
  | ------ | ------ | ---------------------- |
  | `add`  |   R    | 0 + 0x20               |
  | `addi` |   I    | 0x08                   |
  | `sub`  |   R    | 0 + 0x22               |
  | `and`  |   R    | 0 + 0x24               |
  | `andi` |   I    | 0x0c                   |
  | `or`   |   R    | 0 + 0x25               |
  | `ori`  |   I    | 0x0d                   |
  | `nor`  |   R    | 0 + 0x27               |
  | `sll`  |   R    | 0 + 0x00               |
  | `srl`  |   R    | 0 + 0x02               |
  | `sra`  |   R    | 0 + 0x03               |
  | `lw`   |   I    | 0x23                   |
  | `sw`   |   I    | 0x2b                   |
  | `beq`  |   I    | 0x04                   |
  | `bne`  |   I    | 0x05                   |
  | **`slt`**  |   R    | 0 + 0x2a               |
  | **`slti`** |   I    | 0x0a                   |
  | **`jr`**   |   R    | 0 + 0x08               |
  | **`j`**    |   J    | 0x02                   |
  | **`jal`**  |   J    | 0x03                   |
  | **`halt`** |   -    | -                      |


- The machine to emulate has 32 + 1 registers which correspond to `unsigned int registers[32]` and `unsigned int pc`. Their name is specified in `const char *register_names[32]`. The framework provides a helper CLI command `show` to examine the registers. When you enter `show` to the prompt, it shows all register values. You may specify register name to see the value of the register only.

  ```
  >> show
  [00:zr] 0x00000000    0
  [01:at] 0x00000000    0
  [02:v0] 0x00000000    0
  [03:v1] 0x00000000    0
  [04:a0] 0x00000000    0
  [05:a1] 0x00000000    0
  [06:a2] 0x00000000    0
  [07:a3] 0x00000000    0
  [08:t0] 0x00000000    0
  [09:t1] 0x00000000    0
  [10:t2] 0x00000000    0
  [11:t3] 0x00000000    0
  [12:t4] 0x00000000    0
  [13:t5] 0x00000000    0
  [14:t6] 0x00000000    0
  [15:t7] 0x00000000    0
  [16:s0] 0x00000010    16
  [17:s1] 0x00000001    1
  [18:s2] 0x00000002    2
  [19:s3] 0x00000003    3
  [20:s4] 0xbadacafe    3134900990
  [21:s5] 0x00000005    5
  [22:s6] 0x00000006    6
  [23:s7] 0x00000007    7
  [24:t8] 0x00000000    0
  [25:t9] 0x00000000    0
  [26:k0] 0x00000000    0
  [27:k1] 0x00000000    0
  [28:gp] 0x00000000    0
  [29:sp] 0x00008000    32768
  [30:fp] 0x00000000    0
  [31:ra] 0x00000000    0
  [  pc ] 0x00001000
  >> show s4
  [20:s4] 0xbadacafe    3134900990
  >> show pc
  [  pc ] 0x00001000
  >> 0x02324020    // add t0 s1 s2. Should be processed in `process_instruction()`
  >> show t0
  [08:t0] 0x00000003    3
  ```

- Similar to the registers, the machine is equipped with 1 MiB memory which is accessible through `unsigned char memory[]`. The memory contents can be examined with `dump` CLI command, which accepts the starting address and length to dump. Non-printable characters are printed in '.'.
  ```
  >> dump 0 32
  0x00000000:  00 11 22 33    . . " 3
  0x00000004:  44 55 66 77    D U f w
  0x00000008:  de ad be ef    . . . .
  0x0000000c:  00 00 00 00    . . . .
  0x00000010:  68 65 6c 6c    h e l l
  0x00000014:  6f 20 77 6f    o   w o
  0x00000018:  72 6c 64 21    r l d !
  0x0000001c:  21 00 00 00    ! . . .
  >> dump 0x1000 0x20
  0x00001000:  00 00 00 00    . . . .
  0x00001004:  00 00 00 00    . . . .
  0x00001008:  00 00 00 00    . . . .
  0x0000100c:  00 00 00 00    . . . .
  0x00001010:  00 00 00 00    . . . .
  0x00001014:  00 00 00 00    . . . .
  0x00001018:  00 00 00 00    . . . .
  0x0000101c:  00 00 00 00    . . . .
  ```

-  Let's extend this emulator to understand and process a **program**. We can define a program as *a list of machine instructions*. The program file contains MIPS instructions, one instruction per line. So, the program file looks like;

  ```
  0x8c080000
  0x8c090008  lw t1, zero + 8
  0xac090020
  0x20080030  // addi t0 zero 0x30
  0xffffffff  this is the halt instruction
  ```
  Each line in a program file starts with a hexadecimal number, which is a MIPS machine code. The machine code could be followed by some comments (like the 2nd and 4th instructions above). The emulator is supposed to load the program into memory and starts executing the instructions until it meets the `halt` instruction. See test case files for examples of programs.

- `load_program()` is to load a program into the memory. The function is invoked when you enter the `load` command on the prompt with the name of the program file to load. The program described in the specified file should be loaded from `INITIAL_PC`, and ends with the `halt` instruction. Suppose a file "myprogram" contains following instruction;

  ```
  0x02324020  >> add t0 s1 s2
  0x8e090000  >> lw t1 s0 0
  0x8e0a0004  >> lw t2 s0 4
  0xae0a0008  >> sw t2 s0 8
  ```

  Then the memory should look like;

  ```
  >> dump 0x1000 32
  0x00001000:  00 00 00 00    . . . .
  0x00001004:  00 00 00 00    . . . .
  0x00001008:  00 00 00 00    . . . .
  0x0000100c:  00 00 00 00    . . . .
  0x00001010:  00 00 00 00    . . . .
  0x00001014:  00 00 00 00    . . . .
  0x00001018:  00 00 00 00    . . . .
  0x0000101c:  00 00 00 00    . . . .
  >> load myprogram
  >> dump 0x1000 32
  0x00001000:  02 32 40 20    . 2 @
  0x00001004:  8e 09 00 00    . . . .
  0x00001008:  8e 0a 00 04    . . . .
  0x0000100c:  ae 0a 00 08    . . . .
  0x00001010:  ff ff ff ff    . . . .
  0x00001014:  00 00 00 00    . . . .
  0x00001018:  00 00 00 00    . . . .
  0x0000101c:  00 00 00 00    . . . .
  ```

  Note that the halt instruction `0xffffffff` is automatically inserted at 0x00001010 after the program.

- `run_program()` is to execute the loaded program. This function is invoked when you enter `run` on the prompt, and it is supposed to start **executing the program** from `INITIAL_PC` until it meets the halt instruction. You will actually use `process_instruction()` in this function to execute each instruction in the program. Have a look at the comment in the template code for more explanation.


### Hints and Comments

- Use the program for PA1 to generate the machine codes for the testing input. Further, you can make the emulator accept assembly code instead of machine code by
copy-pasting your `translate()` function from PA1, and compiling the code with `INPUT_ASSEMBLY` defined. This feature is not evaluated in the submission, but it is really awesome!!!
- You may use `strtoimax/strtol` to implement `load_program()`.
- Be careful to handle [endianness](https://en.wikipedia.org/wiki/Endianness). Sometimes, values seem to be flipped because MIPS is big endian whereas x86/ARM in your desktop/laptop is little endian. Thus, if you `printf()` an integer value, it will be interpreted as little endian!!!
- Some testcases will show only the decision (i.e., pass or fail) not the details. Their results will be used for grading.
- Do not modify codes in restriction zones. However, you can do whatever you want outside the zones.

### Submission / Grading

- Total 520 pts

- Source: pa2.c (500 pts)
  - You can submit the code up to 30 times.
  - Basic execution: 200 pts (10 pts x 20 instructions)
  - Load program: 40 pts
  - Run program: 260 pts
    - Basic: 60 pts
    - Advanced with jumps and loops: 100 pts
    - Hidden with complicated sequences: 100 pts

- Document: one PDF document (20 pts) including **all** the following items;
  - Outline how you process the MIPS instructions
  - Outline how you load the program into the memory
  - How you run the program
  - What `program-hidden` does
  - And lessons learned
  - No more than three pages

- WILL NOT ANSWER THE QUESTIONS ABOUT THOSE ALREADY SPECIFIED ON THE HANDOUT.
