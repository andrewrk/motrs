#ifndef DEBUG_H
#define DEBUG_H

#ifdef RELEASE
#define NDEBUG
#else
#undef NDEBUG
#endif

#include <cassert>

// the following function is defined by this header:
// void assert(expression)
// if expression is false, crashes the program and reports why.
// if in RELEASE mode, the assertion is not compiled into the program

#endif
