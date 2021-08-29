
//--- Классы ---

#include <iostream>
#include <string>

#include "Sales_data.h"
#include "exercises.h"

struct test_s
{
	int a = 2;
	int b = 5;

	int summ() { return (a+b); }
}test;

void chapter7 (void)
{
	std::cout << test.summ() << std::endl;


	Sales_data s;
	std::string str;

	// Требуется явное преобразование т.к. конструтор(string) explicit
	s.combine(static_cast<Sales_data>(str));	// ПРИМ: создается временный объект Sales_data из строки
}