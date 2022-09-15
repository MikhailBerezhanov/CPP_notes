#include <iostream>

#include "common.hpp"

void func_with_ival();

int main()
{
	using namespace std;

	cout << &ival << endl;
	cout << &cval << endl;

	func_with_ival();

	cout << sqr(ival) << endl;

	return 0;
}