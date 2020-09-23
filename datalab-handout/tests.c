/* Testing Code */

#include <limits.h>
#include <math.h>

/* Routines used by floation point test code */

/* Convert from bit level representation to floating point number */
float u2f(unsigned u) {
  union {
    unsigned u;
    float f;
  } a;
  a.u = u;
  return a.f;
}

/* Convert from floating point number to bit-level representation */
unsigned f2u(float f) {
  union {
    unsigned u;
    float f;
  } a;
  a.f = f;
  return a.u;
}

/* bit manipulation */

int test_func1(int x) {
  return x == 0;
}

int test_func2(int x, int y)
{
  return ~(x|y);
}

int test_func3(int x) {
  int result = 0;
  int i;
  for (i = 0; i < x; i++)
    result |= (1 << (31-i));
  return result;
}

int test_func4(int x)
{
  return (x & 0x1) ? -1 : 0;
}

int test_func5(int x) {
  int i;
  for (i = 0; i < 32; i+=2)
      if ((x & (1<<i)) == 0)
	  return 0;
  return 1;
}

int test_func6(int x, int n) {
  unsigned u = (unsigned) x;
  unsigned shifted = u >> n;
  return (int) shifted;
}

int test_func7(int x) {
  int result = 0;
  int i;
  for (i = 0; i < 32; i++)
    result ^=  (x >> i) & 0x1;
  return result;
}


/* 2's complement */

int test_func8(void) {
  return 0x80000000;
}

int test_func9(int x) {
    return x == 0x7FFFFFFF;
}

int test_func10(int x) {
  return -x;
}

int test_func11(int x, int y)
{
    long long lsum = (long long) x + y;
    return lsum == (int) lsum;
}

int test_func12(int x) {
  return x > 0;
}

int test_func13(int x, int y)
{
  if (x > 0 && y > 0 && x+y < 0)
    return (0x7FFFFFFF);
  if (x < 0 && y < 0 && x+y >= 0)
    return (0x80000000);
  return x + y;
}


