// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.

//Load the register R1 
@R1
D=M
//initialise R2;
@R2
M=0
//end program if R1 == 0
@R1
D;JEQ 

//Load the register R0, end program if R0 == 0
@R0
D=M
D;JEQ 

//Copies the value of R0 to a new register
@COUNTER
M=D
(COUNTER)
M=M-1

@R1
D=M

@R2
M=D+M

@COUNTER
D=M
D;JGT

@END
(END)
0;JMP

