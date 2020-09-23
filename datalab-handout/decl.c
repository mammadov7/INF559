#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TMin INT_MIN
#define TMax INT_MAX

#include "btest.h"
#include "bits.h"

test_rec test_set[] = {
/* bit manipulation */






 {"func1", (funct_t) func1, (funct_t) test_func1, 1, "! ~ & ^ | + << >>", 2, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},






 {"func2", (funct_t) func2, (funct_t) test_func2, 2, "& ~", 8, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},






 {"func3", (funct_t) func3, (funct_t) test_func3, 1, "! ~ & ^ | + << >>", 10, 1,
  {{0, 32},{TMin,TMax},{TMin,TMax}}},






 {"func4", (funct_t) func4, (funct_t) test_func4, 1,
    "! ~ & ^ | + << >>", 5, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},






 {"func5", (funct_t) func5, (funct_t) test_func5, 1,
    "! ~ & ^ | + << >>", 12, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},






 {"func6", (funct_t) func6, (funct_t) test_func6,
   2, "! ~ & ^ | + << >>", 20, 3,
  {{TMin, TMax},{0,31},{TMin,TMax}}},






 {"func7", (funct_t) func7, (funct_t) test_func7, 1, "! ~ & ^ | + << >>", 20, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},


/* 2's complement */






 {"func8", (funct_t) func8, (funct_t) test_func8, 0, "! ~ & ^ | + << >>", 4, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},






 {"func9", (funct_t) func9, (funct_t) test_func9, 1, "! ~ & ^ | +", 10, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},






 {"func10", (funct_t) func10, (funct_t) test_func10, 1,
    "! ~ & ^ | + << >>", 5, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},






 {"func11", (funct_t) func11, (funct_t) test_func11, 2,
    "! ~ & ^ | + << >>", 20, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},






 {"func12", (funct_t) func12, (funct_t) test_func12, 1,
    "! ~ & ^ | + << >>", 8, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},






 {"func13", (funct_t) func13, (funct_t) test_func13, 2,
    "! ~ & ^ | + << >>", 30, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},


  {"", NULL, NULL, 0, "", 0, 0,
   {{0, 0},{0,0},{0,0}}}
};
