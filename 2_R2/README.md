## Basic Radare2 Operations: Hello World

Radare 2 is a set of libraries and tools for reverse engineering / static analysis of binaries, available on windows, linux and osx.

You can find it on GitHub [here](https://github.com/radare/radare2).

To follow along with this basic guide, download the hello world binary provided in this repo.

Note: R2 comes with a dope in terminal graph view for easily seeing the structure of a program and different routes through it. The version in the kali repo's is pretty old I think, as activating this view (at least for me, across multiple verisons of kali) has resulted in loading up a gross barely usable web UI. If using Kali, I'd probably reccomend you remove this versio nand instead install from source.

### Basic Usage and Analysis

To load up a binary for analysis in R2, simple type `r2 [binary name]` like so:
```
r2 helloworld
```
The first thing you should do is call up the help menu to see the list of available commands, by simply inputting `?`.
```
[0x08048300]> ?
Usage: [.][times][cmd][~grep][@[@iter]addr!size][|>pipe] ; ...
Append '?' to any char command to get detailed help
Prefix with number to repeat command N times (f.ex: 3x)
|%var =valueAlias for 'env' command
| *off[=[0x]value]     Pointer read/write data/values (see ?v, wx, wv)
| (macro arg0 arg1)    Manage scripting macros
| .[-|(m)|f|!sh|cmd]   Define macro or load r2, cparse or rlang file
| = [cmd]              Run this command via rap://
| /                    Search for bytes, regexps, patterns, ..
| ! [cmd]              Run given command as in system(3)
| # [algo] [len]       Calculate hash checksum of current block
| #!lang [..]          Hashbang to run an rlang script
| a                    Perform analysis of code
| b                    Get or change block size
| c [arg]              Compare block with given data
| C                    Code metadata management
| d                    Debugger commands
| e [a[=b]]            List/get/set config evaluable vars
| f [name][sz][at]     Set flag at current address
| g [arg]              Go compile shellcodes with r_egg
| i [file]             Get info about opened file
| k [sdb-query]        Run sdb-query. see k? for help, 'k *', 'k **' ...
| m                    Mountpoints commands
| o [file] ([offset])  Open file at optional address
| p [len]              Print current block with format and length
| P                    Project management utilities
| q [ret]              Quit program with a return value
| r [len]              Resize file
| s [addr]             Seek to address (also for '0x', '0x1' == 's 0x1')
| S                    Io section manipulation information
| t                    Cparse types management
| T [-] [num|msg]      Text log utility
| u                    uname/undo seek/write
| V                    Enter visual mode (vcmds=visualvisual  keystrokes)
| w [str]              Multiple write operations
| x [len]              Alias for 'px' (print hexadecimal)
| y [len] [[[@]addr    Yank/paste bytes from/to memory
| z                    Zignatures management
| ?[??][expr]          Help or evaluate math expression
| ?$?                  Show available '$' variables and aliases
| ?@?                  Misc help for '@' (seek), '~' (grep) (see ~??)
| ?:?                  List and manage core plugins
[0x08048300]> 
```
As you can see, there are lots of commands for us to choose from. You can find further info on any of these commands by typing in the command followed by a `?` again. Usually we like to start by analysing the binary, so lets looks at the help menu for `a`.
```
[0x08048300]> a?
|Usage: a[abdefFghoprxstc] [...]
| ab [hexpairs]     analyze bytes
| aa                analyze all (fcns + bbs) (aa0 to avoid sub renaming)
| ac [cycles]       analyze which op could be executed in [cycles]
| ad                analyze data trampoline (wip)
| ad [from] [to]    analyze data pointers to (from-to)
| ae [expr]         analyze opcode eval expression (see ao)
| af[rnbcsl?+-*]    analyze Functions
| aF                same as above, but using anal.depth=1
| ag[?acgdlf]       output Graphviz code
| ah[?lba-]         analysis hints (force opcode size, ...)
| ai [addr]         address information (show perms, stack, heap, ...)
| ao[e?] [len]      analyze Opcodes (or emulate it)
| an[an-] [...]     manage no-return addresses/symbols/functions
| ar                like 'dr' but for the esil vm. (registers)
| ap                find prelude for current offset
| ax[?ld-*]         manage refs/xrefs (see also afx?)
| as [num]          analyze syscall using dbg.reg
| at[trd+-%*?] [.]  analyze execution traces
Examples:
 f ts @ `S*~text:0[3]`; f t @ section..text
 f ds @ `S*~data:0[3]`; f d @ section..data
 .ad t t+ts @ d:ds
[0x08048300]> 
```
The first thing you want to do is 'Analyse All' with `aa`. I'd then reccomend 'A'nalyzing the 'F'unctions and 'L'isting them with `afl`.

```
[0x08048300]> aa
[0x08048300]> afl
0x08048300  34  1  entry0
0x080482f0  16  2  sym.imp.__libc_start_main
0x08048413  53  3  sym.main
0x080483fb  24  1  sym.hello
0x080482d0  16  2  sym.imp.puts
0x08048340  43  4  sym.deregister_tm_clones
0x08048370  53  4  sym.register_tm_clones
0x080483b0  30  3  sym.__do_global_dtors_aux
0x080483d0  43  4  sym.frame_dummy
0x080484c0  2  1  sym.__libc_csu_fini
0x08048330  4  1  sym.__x86.get_pc_thunk.bx
0x080484c4  20  1  sym._fini
0x08048450  97  4  sym.__libc_csu_init
0x08048290  35  3  sym._init
0x080482e0  16  2  loc.imp.__gmon_start__
[0x08048300]> 
```
Now we can start to get a sense of the structure of our binary and begin to snoop around.

### Seeking and Viewing Different Parts of the Binary

In the previous list of functions, one jumps out that would be a great starting point... main!

To seek to a different point of the binary, use the command `s` followed by the address or symbol.

```
[0x08048300]> s sym.main
[0x08048413]>
```
Notice how the pointer now changes to show we are at main!

That's cool and all, but it would be nice to view the contents of main... So let's 'P'rint the 'D'isassembly of the current 'F'unction using `pdf`.

```
[0x08048413]> pdf
            ;-- main:
/ (fcn) sym.main 53
|           ; var int local_ch     @ ebp-0xc
|           ; var int local_10h    @ ebp-0x10
|           ; DATA XREF from 0x08048317 (sym.main)
|           0x08048413      8d4c2404       lea ecx, [esp + 4]          ; 0x4 
|           0x08048417      83e4f0         and esp, 0xfffffff0
|           0x0804841a      ff71fc         push dword [ecx - 4]
|           0x0804841d      55             push ebp
|           0x0804841e      89e5           mov ebp, esp
|           0x08048420      51             push ecx
|           0x08048421      83ec14         sub esp, 0x14
|           0x08048424      c745f4010000.  mov dword [ebp - local_ch], 1
|           0x0804842b      c745f0020000.  mov dword [ebp - local_10h], 2
|           0x08048432      8b45f4         mov eax, dword [ebp - local_ch]
|           0x08048435      3b45f0         cmp eax, dword [ebp - local_10h]
|       ,=< 0x08048438      7d05           jge 0x804843f              
|       |   0x0804843a      e8bcffffff     call sym.hello
|       `-> 0x0804843f      83c414         add esp, 0x14
|           0x08048442      59             pop ecx
|           0x08048443      5d             pop ebp
|           0x08048444      8d61fc         lea esp, [ecx - 4]
\           0x08048447      c3             ret
[0x08048413]> 
```
Finally we can start to analyse the assembly and work out what's going on (life protip™: not much in this binary).

### Graphy View

In the assembly of main, we can see there is a jump condition in the middle (also shown by the arrow to the left of the hex values). In particular, a jump if greater than equal on the comparrison of eax and the integer variable in the memory location stored at ebp - 10h. This might be a simple example, but sometimes the flow of control can get really messy and convoluted within binaries.

One easier way to visualise this is to enter visual graph mode with `VV`.

```
[0x08048413]> VV @ sym.main (nodes 3 edges 3 zoom 100%) BB-NORM mouse:canvas-y movements-speed:5                                                              
                                                            =------------------------------------=
                                                            | [0x8048413]                        |
                                                            |   ;-- main:                        |
                                                            | (fcn) sym.main 53                  |
                                                            | ; var int local_ch     @ ebp-0xc   |
                                                            | ; var int local_10h    @ ebp-0x10  |
                                                            | lea ecx, [esp + 4]                 |
                                                            | and esp, 0xfffffff0                |
                                                            | push dword [ecx - 4]               |
                                                            | push ebp                           |
                                                            | mov ebp, esp                       |
                                                            | push ecx                           |
                                                            | sub esp, 0x14                      |
                                                            | mov dword [ebp - local_ch], 1      |
                                                            | mov dword [ebp - local_10h], 2     |
                                                            | mov eax, dword [ebp - local_ch]    |
                                                            | cmp eax, dword [ebp - local_10h]   |
                                                            | jge 0x804843f ;[a]                 |
                                                            =------------------------------------=
                                                                    f t
                                                                    | '-----------------.
                                                                    |                   |
                                                                    |                   |
                                                            =---------------------=     |
                                                            |  0x804843a          |     |
                                                            | call sym.hello ;[b] |     |
                                                            =---------------------=     |
                                                                v                       |
                                                                '-------.     .---------'
                                                                        |     |
                                                                        |     |
                                                                    =---------------------=
                                                                    |  0x804843f          |
                                                                    | add esp, 0x14       |
                                                                    | pop ecx             |
                                                                    | pop ebp             |
                                                                    | lea esp, [ecx - 4]  |
                                                                    | ret                 |
                                                                    =---------------------=

```
The 't' and the 'f' on each branch show us which way the program will diverge wether ther condition is true or false. As should be farely obvious now, we want the condition to fail to gain access to the hello function!

### Printing Data

There really isn't much else to show right now, but here's an example of printing data stored at a certain locations. For this, we will use the r2 debugger, so relaunch with the `-d` flag this time.
```
root@kali:~/Documents/retalk# r2 -d helloworld
Process with PID 3838 started...
attach 3838 3838
bin.baddr 0x08048000
Assuming filepath ./helloworld
Version symbols section '.gnu.version' contains 5 entries:
 Addr: 0x08048246  Offset: 0x00000246  Link: 5 (.dynsym)
  000:   0 (*local*)       0 (*local*)       0 (*local*)       0 (*local*)    
  004:   0 (*local*)    
Version needs section '.gnu.version_r' contains 1 entries:
 Addr: 0x08048250 Offset: 0x00000250  Link to section: 6 (.gnu.version_r)
asm.bits 32
 -- Excellent; we can attack in any direction!
[0xf77b0000]>
```
Again, follow the same steps as above to get to the disassembled main. We will set a breakpoint at the jump so we can inspect the registers and print out data.

To set a breakpoint, use `db` followed by the address to break on, then `dc` to run and continue to that breakpoint.

```
[0x08048413]> db 0x08048438
[0x08048413]> dc
hit breakpoint at: 8048438
attach 3838 1
[0x08048413]> 
```
Now we can inspect the value of the registers and see what exactly is being compared! To see the registers, simple enter `dr`.

```
[0x08048413]> dr
eip = 0x08048438
oeax = 0xffffffff
eax = 0x00000001
ebx = 0xf7784000
ecx = 0xff894be0
edx = 0xff894c04
esp = 0xff894bb0
ebp = 0xff894bc8
esi = 0x00000000
edi = 0x00000000
eflags = 0x00000297
[0x08048413]> 
```

We now know that the value of eax was 0x00000001 or 1. If the hex was a little more complicated you can convert it with `?`. For example, with `0x08008135`:

```
[0x08048413]> ? 0x08008135
134250805 0x8008135 01000100465 128M 800000:0135 134250805 00110101 134250805.0 0.000000f 0.000000
[0x08048413]> 
```
... easy!

Next, we want to find the second operand to the `cmp` instruction. We know that it is stored at `0x10` less than the address in ebp, and we know what ebp is thanks to running `dr`... so lets print that exact value!

To do so, we want to 'P'rint the he'x' 32bit 'w' value at the specific address using `pxw @ [address]`.

```
[0x08048413]> pxw @ 0xff894bc8 - 0x10
0xff894bb8  0x00000002 0x00000001 0xf77843c4 0xff894be0  .........Cx..K..
0xff894bc8  0x00000000 0xf75f7a63 0x08048450 0x00000000  ....cz_.P.......
0xff894bd8  0x00000000 0xf75f7a63 0x00000001 0xff894c74  ....cz_.....tL..
0xff894be8  0xff894c7c 0xf77bdc7a 0x00000001 0xff894c74  |L..z.{.....tL..
0xff894bf8  0xff894c14 0x0804970c 0x080481fc 0xf7784000  .L...........@x.
0xff894c08  0x00000000 0x00000000 0x00000000 0x5a81631b  .............c.Z
0xff894c18  0xf6e2e70a 0x00000000 0x00000000 0x00000000  ................
0xff894c28  0x00000001 0x08048300 0x00000000 0xf77c34b0  .............4|.
0xff894c38  0xf75f7979 0xf77d0000 0x00000001 0x08048300  yy_...}.........
0xff894c48  0x00000000 0x08048321 0x08048413 0x00000001  ....!...........
0xff894c58  0xff894c74 0x08048450 0x080484c0 0xf77be130  tL..P.......0.{.
0xff894c68  0xff894c6c 0x0000001c 0x00000001 0xff896682  lL...........f..
0xff894c78  0x00000000 0xff89668f 0xff89669a 0xff8966ad  .....f...f...f..
0xff894c88  0xff8966be 0xff8966e9 0xff8966f4 0xff896704  .f...f...f...g..
0xff894c98  0xff89671b 0xff89672c 0xff89673e 0xff896748  .g..,g..>g..Hg..
0xff894ca8  0xff896ce1 0xff896d07 0xff896d55 0xff896d63  .l...m..Um..cm..
[0x08048413]> 
```

The value is 0x00000002 or 2. This will obviously always fail, and will therefore always go on to print 'hello world!'

### Finishing
Well done on reversing your first binary (ish)!

To see more capability of r2, check out this cheatsheet [here](https://github.com/pwntester/cheatsheets/blob/master/radare2.md) for a great list of commands with descriptions.
