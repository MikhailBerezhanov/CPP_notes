#pragma once

#include <cstring>

template<typename T>
bool compare(const T &lhs, const T &rhs)
{
	return std::less<T>()(lhs, rhs);
}


// This header provides template function definition.
// To make one_definition_rule (ODR) works if included in
// more than one file, specializations of such template MUST be inline. 

// Explicit (full) specialization
template<>
inline bool compare<>(const int &lhs, const int &rhs)
{
	return true;
}

// For non fully specialized function templates, i.e. 
// ones that carry at least one unknown type, you can 
// omit inline, and not receive errors, but still they 
// are not inline. 
//
// For full specializations, i.e. ones that use only known types, 
// you cannot omit it
