
//--- Выражения ---

#include <iostream>
#include <string>
#include <vector>

#include "exercises.h"

void chapter4 (void)
{
	// l-value и r-value
	// l-value представляет собой объект, который занимает идентифицируемое место в памяти (например, имеет адрес)
	// rvalue — это выражение, которое не представляет собой объект, который занимает идентифицируемое место в памяти
	int var;			//
	var = 4;			// var - lvalue, 4 - rvalue
	int& ref = var;		// ref - lvalue (Обычно функции возвращают временное значение rvalue, но ссылки исключение)
	//4 = var;       	// ERROR!
	//(var + 1) = 4; 	// ERROR! (var + 1) являeтся временным результатом выражений, которые не имеют определённого места в памяти
	// ПРИМ, Все lvalue, которые не являются массивом, функцией и не имеют неполный тип, могут быть преобразованы в rvalue.
	int a = 1;        	// a - lvalue
	int b = 2;        	// b - lvalue
	int c = a + b;   	// '+' требует rvalue, поэтому a и b конвертируются в rvalue 
                  		// и rvalue возвращается в качестве результата
	// ПРИМ, однако неявное преобразование rvalue в lvalue отсутсвует!
	// Тем не менее lvalue могут быть получены из rvalue явным способом - унарным оператором разыменовывания
	int arr[] = {1, 2};
	int* pp = &arr[0];
	*(pp + 1) = 10;   // OK: p + 1 rvalue, однако *(p + 1) уже lvalue
	// Обратно, унарный оператор '&' (адрес) принимает lvalue как аргумент и производит rvalue:
	//&a = 40;  ERROR!
	//int* bad_addr = &(var + 1);  // ОШИБКА: требуется lvalue для унарного оператора '&'
	int* _addr = &var;	// ОК: var - lvalue
	// Константным ссылкам на lvalue можно присвоить rvalue. Так как они константы, значение не может быть изменено по ссылке 
	// и поэтому проблема модификации rvalue просто отсутствует. Неконстантной ссылке на lvalue не может быть присвоено rvalue, 
	// так как это потребовало бы неверное rvalue-в-lvalue преобразование
	//std::string& sref = std::string();  // ОШИБКА: неверная инициализация             
                                    // неконстантной ссылки типа 'std::string&'
                                    // rvalue типа 'std::string'
	const std::string& sref = "ref";

	std::vector<int> __vec{5, 4, 3, 2, 1};
	int *ptr = nullptr;
	int _ival = 0;
	// ПРИМ. логические && и || гарантируют вызов правой части только при истинности (ложности для || ) левой
	// Только операторы &&, || , ? : и , определяют порядок выполнения своих операндов
	if( ptr != 0 && *ptr++){}
	if( _ival++ || _ival){}
	//ОШИБКА: недерменирован порядок выполнения инкремента
	if( __vec[_ival++] <= __vec[_ival] ) std::cout << "ОШИБКА " << std::endl;


	// ПРИМ, sizeof - константное выражение, выполняется во время компиляции
	std::cout << "sizeof char: " << sizeof(char) << std::endl;		// 1
	std::cout << "sizeof char*: " << sizeof(char*) << std::endl;	// 4 or 8 (x32 x64)
	std::cout << "sizeof int: " << sizeof(int) << std::endl;		// 4 (2)
	std::cout << "sizeof int*: " << sizeof(int*) << std::endl;		// 4 or 8 (x32 x64)
	std::cout << "sizeof long: " << sizeof(long) << std::endl;		// 4
	std::cout << "sizeof long*: " << sizeof(long*) << std::endl;	// 4 or 8 (x32 x64)
	std::cout << "sizeof short: " << sizeof(short) << std::endl;	// 2
	std::cout << "sizeof short*: " << sizeof(short*) << std::endl;	// 4 or 8 (x32 x64)
	std::cout << "sizeof bool: " << sizeof(bool) << std::endl;		// 1
	std::cout << "sizeof bool*: " << sizeof(bool*) << std::endl;	// 4 or 8 (x32 x64)
	std::cout << "sizeof float: " << sizeof(float) << std::endl;	// 4
	std::cout << "sizeof float*: " << sizeof(float*) << std::endl;	// 4 or 8 (x32 x64)
	std::cout << "sizeof double: " << sizeof(double) << std::endl;	// 8
	std::cout << "sizeof double*: " << sizeof(double*) << std::endl;// 4 or 8 (x32 x64)

	int x[10];
	int *p = x;
	std::cout << sizeof(x)/sizeof(*x) << std::endl; // 10
	std::cout << sizeof(p)/sizeof(*p) << std::endl; // 2

	int a_ = 0, b_ = 0;

	true ? ++a_, b_+=2 : --a_, --b_ ;	// (true ? ++a, b+=2 : --a), --b ;
	std::cout << "a: " << a_ << " b: " << b_ << std::endl;


	bool flag; 
	char cval;
	short sval; 
	unsigned short usval;
	int ival; 
	unsigned int uival;
	long lval; 
	unsigned long ulval;
	float fval; 
	double  dval;

	3.14159L + 'a'; // ' а ' преобразуется в int , а затем int в long dоublе
	dval + ival; 	// ival преобразуется в dоuble
	dval + fval; 	// fval преобразуется в dоuble
	ival = dval; 	// dval преобразуется в int (с усечением)
	flag = dval; 	// если dval - О , flag - false , в противном случае - true
	cval + fval; 	// cval преoбразуется в int , затем int во float
	sval + cval; 	// sval и cval преобразуется в int
	cval + lval; 	// cval преобразуется в long
	ival + ulval; 	// ival преобразуется в unsigned long
	// Если все значения беззнакового типа соответствуют большему типу, то операнд беззнакового типа
	// преобразуется в знаковый. Если значения не соответствуют, то знаковый операнд преобразуется в беззнаковый.
	usval + ival; 	// преобразование зависит от соотношения размеров типов unsigned short и int
	uival + lval; 	// преобразование зависит от соотношения размеров типов unsigned int и long

	int i = 233, j = 234;
	// приведя тип одного из операндов к типу double,
	// можно заставить выражение использовать деление с плавающей точкой
	double slope = static_cast<double>(j) / i;	
	std::cout << "slope: " << slope << std::endl;

	const char* pc = &cval;
	char* p2 = const_cast<char*>(pc);	// ok : однако запись при помощи р указателя непредсказуема
										// т.к. изначальный объект мог быть константой

	// ПРИМ. reinterpret_cast также используется для преобразования указателя в целочисленный тип и наоборот
	int* ip;
	char* pc2 = reinterpret_cast<char*>(ip); 
	char* pc3 = (char*)ip;
	unsigned long addr = reinterpret_cast<unsigned long>(pc3);	// long для x64, int для x32
	std::cout << "addr = " << addr << std::endl;

	int i5 = 2;
	double d5 = 2.5;
	i5 *= static_cast<int>(d5);	//  4
	//i5 *= d5;						5
	std::cout << "i5 * d5 = " << i5 << std::endl;


	int i6;
	double d6;
	std::string str = "test";
	const std::string* ps6 = &str;
	char* pc6;
	void* pv6;
	int* pi6;
	std::string* tmp = const_cast<std::string*>(ps6);
	pv6 = reinterpret_cast<void*>(tmp);	// ПРИМ. для void* указателя можно использовать static_cast
	i6 = static_cast<int>(*pc6);
	pv6 = &d6;
	pc6 = static_cast<char*>(pv6);		// ПРИМ. для void* указателя можно использовать static_cast
	pc6 = reinterpret_cast<char*>(pi6);	// Но для остальных типов указателей используется reinterpret
}
