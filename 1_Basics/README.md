## Basic Assembly / Theory For Static Reverse Engineering

###The Compilation Process

When a C program is compiled, it goes through a number of stages to get from your plain unobfuscated code (.c file) to a working distributable binary.

![diagram](https://upload.wikimedia.org/wikipedia/commons/0/0f/Avr-gcc.png)

1. The compiler combines each .c file with its corresponding header (.h file) and 'converts' it in to assembly, a set of instructions to present to the CPU (.s files).
2. The assembler assembles each .s file into its 'object code', essentially converting each piece of human readable assembly into alpha-numeric machine code, represented in hex.
3. The linker combines the objects with additional boilerplate code and any required libraries into the file ELF binary.

The object (or 'machine') code is basically a one to one convertion from each instruction i nthe assembly into a hex representation.

When we reverse engineer a binary such as this, we are trying to work out the inner workings, or get as close to the original source as possible... Since the binary file is essentially all of the machine code clumped together, we can use a dissassembler to fairly easily convert it back into a human readable assembly (although sometimes the disassembler may be confused as to which numbers represent code, which represent data etc.).

Although tools do exist, it is pretty difficult to take it any further than this. Different compilers on different operating systems (or different versions of the same operating system) on machines using different architectures are all likely to produce wildly different assembly to get the job done. This makes it extremely difficult to then easily convert the assembly back to an accurate representation of the original code in C. One notable tool is IDA Pro with its decompiler plugin. It definitely helps with the reverse engineering process but a) It is extremely expensive and b) it is still not a perfect decompilation. The output is definitely more readable than common assembly but it is still unlikely to compile and should instead be labeled closer to pseudocode than pure C.

For the reasons stated above, during the reverse engineering process you usually only take it as far as dissasembly... from there it is up to you to work the rest out. You should therefore get to learning assembly!

##Intel x86 

Assembly comes in many shapes and sizes for different CPU architectures. In this guide we'll be looking at Intels assembly, predominantly the x86 (32bit) instruction set. You are most likely (at least in earlier levels) to encounter this in CTF challenges. However saying that, x64 is increasingly being used in such challenges. I'll do a seperate writeup on that sometime later.

### Registers
Registers are small 'places' within a CPU that hold data. They are used to speedily pass data to be used by the CPU. There are 8 general purpose registers in x86 assembly: EAX, EBX, ECX, EDX, ESI, EDI, ESP and EBP. The first 6 are used for general storing / passing of data (CPU instructions, memory addresses, individual characters etc.). The last 2 are reserved for special purposes. The stack pointer (ESP) points to the top of the stack. The base pointer (EBP) points to the top of the current frame, or, generally holds the return address.

### Instructions
