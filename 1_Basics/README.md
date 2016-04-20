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

EIP also exists: The instruction pointer. This will lways hold the next instruction to be executed by the CPU (useful for exploitation purposes!).

Not: NOT case sensitive. EAX and eax are the same.

### Instructions
Assembly instructions usually consist of a short code / keyword to represent the instruction followed by 0 to many operands or arguments.

The first instruction we weill look at is the MOV instruction.

#### MOV

The MOV instruction simply copies the data referred to by the second operand into the location referred to by the first operand. An example would be:

```assembly
mov eax, ecx
```
Move the value stored in register ECX into EAX.

If memory addresses are stored at a location, it is possible to reference the data stored within said memory address directly using []. For example:

```assembly
mov eax, [ebx]
```
Move the value stored in memory at the address contained in EBX into EAX.

Simple arithmetic may also be used upon operands. For example:

```assembly
mov eax, [ebx+4]
```
Move the value stored in memory at the address contained in (ebx + 4).

####PUSH & POP

Push and pop are pretty simple... They push items onto the stack, and pop items off of the stack.

PUSH will take a single operand and push the value onto the stack after decrementing ESP by 4, because the stack grows DOWNWARDS to a lower address space on x86.

POP will remove the value at the top of the stack and place it into the single operand provided, before incrementing ESP by 4 again to show that the stack has shrunk.

```assembly
push eax
```
Push eax onto the stack.

```assembly
pop [ebx]
```
Pop the top of the stack into memory at the location stored within ebx.

#### ADD

Add the two operands together and store the result in the first operand.

```assembly
add eax, 10
```
Add 10 to EAX and store the result in EAX (think EAX += 10)

#### JMP

Jump to the memory location provided. Used for changing control flow. Essentially, the new memory location provided by the operand is loaded into EIP so that it is the next instruction execruted.

```assembly
jmp 0x456789AB
```
Load the hardcoded address into EIP and continue.

####CMP & J conditions

First, CMP. CMP compares the two operands (essentially performing a SUB or subtraction) and saves the results in the mahicne status word (magic lol). Then we can use J conditions to act accordingly... This is how C if statements are handled basically.

An example:

```assembly
cmp eax, ebx
jle 0x456789AB
```
If the value of eax is less than ebx, jump to the provided memory address.

#### Other Instructions
These are just the tip of the iceberg, and there are many more instructions available, such as xor, and, imul (integer multiplication) etc. See [Guide to x86 assembly](http://www.cs.virginia.edu/~evans/cs216/guides/x86.html) for a great short guide or [x86 Assembly Langyage reference (PDF)](https://docs.oracle.com/cd/E19641-01/802-1948/802-1948.pdf) for a complete reference.

### Calling functions / finding arguments

This won't go too deep into the official C language calling conventions. You can and should find them in the Guide to x86 assembly linked above.

The basics you need to know for the intro to RE is how arguments are passed to a function.

Arguments are passed by placing on them stack before calling the function using the `call` instruction. The function will then pop them off the stack to use. So that the fucntion pops them off in the correct order, you should remember that all arguments are actually put onto the stack in reverse order! For example:

```assembly
push eax
push ebx
push ecx
call _main
```
Where eax, ebx and ecx are the 3rd, 2nd and 1st arguments respectively.
