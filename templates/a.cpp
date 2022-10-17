#include <iostream>

#include "spec_tmpl.hpp"

using namespace std;

// header provides template function definition.
// to make one_definition_rule (ODR) works, 
// specializations of such template MUST be inline. 




int main()
{
	cout << compare(10, 10) << endl;

	cout << compare("1", "2") << endl;

	return 0;
}