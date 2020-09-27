/* 
 * INF559 Data Lab 
 * 
 * Ali MAMMADOV, ali.mammadov
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Run ./driver.pl on the lab machines before submitting.  
 *      If you don't, you are on your own.
 */

#endif

/* bit manipulation */

/*
 * func1 - returns 1 if x == 0, and 0 otherwise 
 *   Examples: func1(5) = 0, func1(0) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int func1(int x) {
// No comment :)
  return !x;
}

/* 
 * func2 - ~(x|y) using only ~ and & 
 *   Example: func2(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int func2(int x, int y) {
// De Morgan's laws
    return (~x)&(~y);
}

/* 
 * func3 - pads n upper bits with 1's
 *  You may assume 0 <= n <= 32
 *  Example: func3(4) = 0xF0000000
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 1
 */
int func3(int n) {
  // if n is 0, answer will be 0, shifts can't change the 0 
  // if not 0, (1 << 31) by setting 1 the most significant bit 
  // and doing n - 1 times arithmetic right shitf, (+~0 = -1)
  return (!!n << 31) >> (n+~0) ;
}

/* 
 * func4 - set all bits of result to least significant bit of x
 *   Example: func4(5) = 0xFFFFFFFF, func4(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int func4(int x) {
  /*
    10101010....11  
                 ^   least significant bit
    10000000.....0   left shift 31 bits  
    ^ making it most significant bit 
    11111111.....1   arithmetic right shift 31 bits  
  */
  return x << 31 >> 31;

}

/* 
 * func5 - return 1 if all even-numbered bits in word set to 1
 *   Examples func5(0xFFFFFFFE) = 0, func5(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int func5(int x) {
  /*
    0xaa = 10101010
    01010101 01010101 01010101 01010101  by doing OR between 0xaa and
                               10101010  even-numbered bits we will get
                      10101010           only 1's and his complement will
             10101010                    give us 0. So !0 = 1.
    10101010  
    ------------------------------------
    11111111 11111111  11111111 11111111 
  */
  return  !(~( x  | 0xaa | (0xaa<<8)  | (0xaa<<16) | (0xaa<<24) ));
}

/* 
 * func6 - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: func6(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int func6(int x, int n) {
  /*
  right_part = ~(!!n << 31) >> (n+~0) this part explained in func3
  
  Ex:  
   x = 1010 1100 1101 1001, n = 3, 
   right_part = 0001 1111 1111 1111    AND operation
       x >> 3 = 1111 0101 1001 1011 
                -------------------
                0001 0101 1001 1011 
  */
  return (x >> n) & ( ~(!!n << 31) >> (n+~0) );

}

/*
 * func7 - returns 1 if x contains an odd number of 0's
 *   Examples: func7(5) = 0, func7(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int func7(int x) { 
 /* Idea of my solution is that: 
        1100 0011 1001 1110     We are dividing number 2 parts,
                 ^              XOR between them, cause if in  
                  1100 0011     initial number 0's are odd in
                  ---------     the half number also will be odd              
                  0101 1101     and if we will repeat it till 
                      ^         the last bit and we will obtain
                       0101     1 - if x is odd
                       ----     0 - if x is even
                       1000     after we set all bits of result      
	                  		^       to least significant bit     
			                   10     !!(1111111111111111) -> 1
                         --     !!(0000000000000000) -> 0
                         10     
                         ^
                          1                         
                          -
                          1           */         
  x = x^(x>>16);
  x = x^(x>>8);
  x = x^(x>>4);  
  x = x^(x>>2);
  x = x^(x>>1);
  x = !!(x << 31 >> 31);
  return x;
}


/* 2's complement */

/* 
 * func8 - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int func8(void) {
  //minimum two's complement integer is 0x80000000 
  return 1<<31;
}

/*
 * func9 - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int func9(int x) {
  // we know that maximum integer has this quality :
  // ~(Xmax+1) = Xmax, but -1 also has a same quality,   
  // but -1 complement is equal to 0, Xmax's not.
  return !( (!(~x)) | (~(x+1)^x) );
}

/* 
 * func10 - return -x 
 *   Example: func10(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int func10(int x) {
  // 2's complement formula
  return ~x+1;

}

/* 
 * func11 - Determine if can compute x+y without overflow
 *   Example: func11(0x80000000,0x80000000) = 0,
 *            func11(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int func11(int x, int y) {

  
  int z = x+y; 
  x = x >> 31; // right shift helps to define the sign 
  y = y >> 31; // all 0's means positive, all 1's negative
  z = z >> 31; // now x,y,z are signs of themselves 
  // x < 0 && y < 0 && z > 0 --> OVERFLOW
  // x > 0 && y > 0 && z < 0 --> OVERFLOW
  // if signs are same, XOR returns 0 
  // and we need that x and y have same sign, z different
  return !!((x^y) | !(y^z))  ;

}

/* 
 * func12 - return 1 if x > 0, return 0 otherwise 
 *   Example: func12(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int func12(int x) {
  // most significant bit defines the sign of the number
  // by doing 31 times right shift we will obtain either 
  // only 0's (if x > 0), or only 1's (if x < 0)
  // for the case x == 0, we add just (!x) 
  return !( (!x) | ( x >> 31 ));
}

/*
 * func13 - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value, and when negative overflow occurs,
 *          it returns minimum positive value.
 *   Examples: func13(0x40000000,0x40000000) = 0x7fffffff
 *             func13(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int func13(int x, int y) {
  
  int b = (1<<31) ; // 2's complement minimum integer
  int a = ~b ; // 2's complement maximum integer
  int z = x+y;
  int x_s = x >> 31;
  int y_s = y >> 31;
  int z_s = z >> 31;
  int over = ~((x_s^y_s) | ~(x_s^z_s));  
  // till this line was explained in func 11 for finding overflow
  int over_s = over & z_s ; // positive overflow if z < 0, vice versa.  

  // x > 0 && y > 0 && z < 0 --> return a
  // x < 0 && y < 0 && z > 0 --> return b
  // else --> return z

  return (over_s & a & over) | ((~over_s) & b & over ) | ( z & (~over) );

}