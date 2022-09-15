#pragma once

// Any of the following names
// declared at namespace scope
// have internal linkage:
//   non-volatile
//   non-template
//   non-inline
//   const-qualified
// variables (including constexpr)
// that aren't declared 'extern'
// and aren't previously declared
// to have external linkage.
//
// Each .cpp will have it's
// own version of ival
const int ival = 43; // internal linkage

// const char *name = "name"; // external linkage
const char * const name = "abc"; // internal linkage

constexpr double dval = 1.0; // internal linkage

// Each .cpp will use the same variable
inline const int cval = 10; // external (weak) linkage

// A constexpr specified used in a
// function or static member variable
// declaration implies inline.
constexpr int sqr(int x) // external (weak) linkage
{
	return x * x;
}