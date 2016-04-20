##An Example 'Crackme' / CTF RE Challenge using Radare2

Now we'll look at a couple of the earlier levels in the set of crackmes released with RPISEC's Modern Binary Exploitation course released last year (a great course covering not only re but all forms of binary exploitation, would recommend for anyone studying for CTFs).

### crackme0x00a

Following the previous guide, lets get to the point of having main disassembled.

```
[0x08048430]> aa
[0x08048430]> s sym.main
[0x080484e4]> pdf
            ;-- main:
/ (fcn) sym.main 133
|           ; DATA XREF from 0x08048447 (sym.main)
|           0x080484e4      55             push ebp
|           0x080484e5      89e5           mov ebp, esp
|           0x080484e7      83e4f0         and esp, 0xfffffff0
|           0x080484ea      83ec30         sub esp, 0x30
|           0x080484ed      65a114000000   mov eax, dword gs:[0x14]    ; [0x14:4]=1
|           0x080484f3      8944242c       mov dword [esp + 0x2c], eax
|           0x080484f7      31c0           xor eax, eax
|           ; JMP XREF from 0x08048560 (sym.main)
|       .-> 0x080484f9      b840860408     mov eax, str.Enter_password: ; "Enter password: " @ 0x8048640
|       |   0x080484fe      890424         mov dword [esp], eax
|       |   0x08048501      e8cafeffff     call sym.imp.printf
|       |   0x08048506      b851860408     mov eax, 0x8048651          ; "%s" @ 0x8048651
|       |   0x0804850b      8d542413       lea edx, [esp + 0x13]       ; 0x13 
|       |   0x0804850f      89542404       mov dword [esp + 4], edx
|       |   0x08048513      890424         mov dword [esp], eax
|       |   0x08048516      e805ffffff     call sym.imp.__isoc99_scanf
|       |   0x0804851b      8d442413       lea eax, [esp + 0x13]       ; 0x13 
|       |   0x0804851f      89442404       mov dword [esp + 4], eax
|       |   0x08048523      c7042424a004.  mov dword [esp], str.g00dJ0B_ ; [0x804a024:4]=0x64303067  LEA obj.pass.1685 ; "g00dJ0B!" @ 0x804a024
|       |   0x0804852a      e891feffff     call sym.imp.strcmp
|       |   0x0804852f      85c0           test eax, eax
|      ,==< 0x08048531      7521           jne 0x8048554              
|      ||   0x08048533      c70424548604.  mov dword [esp], str.Congrats_ ; [0x8048654:4]=0x676e6f43  LEA str.Congrats_ ; "Congrats!" @ 0x8048654
|      ||   0x0804853a      e8b1feffff     call sym.imp.puts
|      ||   0x0804853f      90             nop
|      ||   0x08048540      b800000000     mov eax, 0
|      ||   0x08048545      8b54242c       mov edx, dword [esp + 0x2c] ; [0x2c:4]=0x280009  ; ','
|      ||   0x08048549      653315140000.  xor edx, dword gs:[0x14]
|     ,===< 0x08048550      7415           je 0x8048567               
|    ,====< 0x08048552      eb0e           jmp 0x8048562              
|    ||`--> 0x08048554      c704245e8604.  mov dword [esp], str.Wrong_ ; [0x804865e:4]=0x6e6f7257  LEA str.Wrong_ ; "Wrong!" @ 0x804865e
|    || |   0x0804855b      e890feffff     call sym.imp.puts
|    || `=< 0x08048560      eb97           jmp 0x80484f9              
|    ||     ; JMP XREF from 0x08048552 (sym.main)
|    `----> 0x08048562      e879feffff     call sym.imp.__stack_chk_fail
|     `---> 0x08048567      c9             leave
\           0x08048568      c3             ret
[0x080484e4]> 
```

And we're done! r2 picked up the password for us. Can you see it?

(Hint: Look at the arguments passed to string compare)

### crackme0x01

This one is slightly (but not much!) more difficult.

Again, lets print the assembly of main:

```
[0x08048330]> aa
[0x08048330]> s sym.main
[0x080483e4]> pdf
            ;-- main:
/ (fcn) sym.main 113
|           ; arg int arg_149ah    @ ebp+0x149a
|           ; var int local_4h     @ ebp-0x4
|           ; DATA XREF from 0x08048347 (sym.main)
|           0x080483e4      55             push ebp
|           0x080483e5      89e5           mov ebp, esp
|           0x080483e7      83ec18         sub esp, 0x18
|           0x080483ea      83e4f0         and esp, 0xfffffff0
|           0x080483ed      b800000000     mov eax, 0
|           0x080483f2      83c00f         add eax, 0xf
|           0x080483f5      83c00f         add eax, 0xf
|           0x080483f8      c1e804         shr eax, 4
|           0x080483fb      c1e004         shl eax, 4
|           0x080483fe      29c4           sub esp, eax
|           0x08048400      c70424288504.  mov dword [esp], str.IOLI_Crackme_Level_0x01_n ; [0x8048528:4]=0x494c4f49  LEA str.IOLI_Crackme_Level_0x01_n ; "IOLI Crackme Level 0x01." @ 0x8048528
|           0x08048407      e810ffffff     call sym.imp.printf
|           0x0804840c      c70424418504.  mov dword [esp], str.Password: ; [0x8048541:4]=0x73736150  LEA str.Password: ; "Password: " @ 0x8048541
|           0x08048413      e804ffffff     call sym.imp.printf
|           0x08048418      8d45fc         lea eax, [ebp - local_4h]
|           0x0804841b      89442404       mov dword [esp + 4], eax
|           0x0804841f      c704244c8504.  mov dword [esp], 0x804854c  ; [0x804854c:4]=0x49006425  ; "%d" @ 0x804854c
|           0x08048426      e8e1feffff     call sym.imp.scanf
|           0x0804842b      817dfc9a1400.  cmp dword [ebp - local_4h], 0x149a ; [0x149a:4]=0x2ec0804 
|       ,=< 0x08048432      740e           je 0x8048442               
|       |   0x08048434      c704244f8504.  mov dword [esp], str.Invalid_Password__n ; [0x804854f:4]=0x61766e49  LEA str.Invalid_Password__n ; "Invalid Password!." @ 0x804854f
|       |   0x0804843b      e8dcfeffff     call sym.imp.printf
|      ,==< 0x08048440      eb0c           jmp 0x804844e              
|      |`-> 0x08048442      c70424628504.  mov dword [esp], str.Password_OK_:__n ; [0x8048562:4]=0x73736150  LEA str.Password_OK_:__n ; "Password OK :)." @ 0x8048562
|      |    0x08048449      e8cefeffff     call sym.imp.printf
|      |    ; JMP XREF from 0x08048440 (sym.main)
|      `--> 0x0804844e      b800000000     mov eax, 0
|           0x08048453      c9             leave
\           0x08048454      c3             ret
[0x080483e4]> 
```
The input provided to scanf this time is a little bit different. It seems they are instead looking for an int. We know this because the string passed (handily provided to us in the comments by r2) is "%d".

It is then probably safe to bet that the very next instruction is the comparison to see if the int is correct!

A quick conversion of `0x149a`:
```
[0x080483e4]> ? 0x149a
5274 0x149a 012232 5.2K 0000:049a 5274 10011010 5274.0 0.000000f 0.000000
```
and we have our password!
```
root@kali:~/Documents/courses/rpisec/modbin/challenges# ./crackme0x01
IOLI Crackme Level 0x01
Password: 5274
Password OK :)
```

