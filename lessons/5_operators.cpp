
//--- Операторы ---

#include <iostream>
#include <stdexcept>

#include "exercises.h"


static int delim(int& a, int& b)
{
	if (!b) throw std::runtime_error("division by 0");

	return (a / b) ;
}

void chapter5 (void)
{
	int a = 0, b = 0, res = 0;

	std::cout << "Enter two integers: " << std::endl;
again:
	std::cin.clear();
	std::cin >> a >> b;

	try{
		res = delim(a, b);
		std::cout << "res: " << res << std::endl;
	}
	catch(std::runtime_error err){
		std::cin.clear();
		std::cout << "exception occured: " << err.what() << " . try again" << std::endl;
		goto again;
	}
}