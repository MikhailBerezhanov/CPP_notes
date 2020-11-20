
#include <iostream>

#include "exercises.h"

// Константные выражения constexpr: возвращаемый тип и тип каждого параметра должны быть литералами 
// и return может быть только один
/*constexpr*/ int const_init() { return 10; }

constexpr size_t scale(size_t cnt) { return 50 * cnt;}	// константное выражение, если cnt - константное выражение

int a;	// в глобальной области видимости объект встроенного типа инициализируется 0, но не в локальной
//const int c ;	// ОШИБКА: Необходима инициализация константы
extern const int c = 1; // для совместного использования констант в файлах программы необходим extern в объявлении

int main(int argc, char* argv[]) 
{ 

	unsigned u = 10 , u2 = 42 ;
	std::cout << u2 - u << std::endl;	// 32
	std::cout << u - u2 << std::endl;	// 32 mod 4 294 967 296 = 4 294 967 264
	int i = 10 , i2 = 42;
	std::cout << i2 - i << std::endl;	// 32
	std::cout << i - i2 << std::endl;	// -32
	std::cout << i - u << std::endl;	// 0
	std::cout << u - i << std::endl;	// 0
	std::cout << "balbalbalabla "  
	"babaopdofpso" << std::endl;
	std::cout << 0x000C << std::endl;


	int i3 = 1.111;	// 1
	int i4 = {i3};	// Инициализация с проверкой допустимости значения
	//double *dp = &i3, dp2 = {"avc"};	ОШИБКА инициализации


	int var = 5;
	int* ip = &var, *&r = ip;	// ссылка на указатель
	std::cout << "ip: "<<  *ip << " r: "<< r << " *r: "<< *r << std::endl;
	// ПРИМ. Указатель на ссылку не сущесствует т.к. ссылка - не объект в отличие от указателя


	double dval = 3.14;
	// создается временная переменная и ссылка связывается с temp (3)( const int temp = dval; )
	// без const была бы ошибка т.к. тогда был бы доступ на изменение временной переменной
	const int &ri = dval;
	std::cout << "ri: " << ri << std::endl;


	const int ic=0, &rc = 0, *pc;
	//int *const ptr;  		ОШИБКА: Необходима инициализация константного указателя
	//const int &const r2;	ОШИБКА: Необходима инициализация ссылки
	int a = -1;
	const int a2 = a, &ra = a;
	// ПРИМ. Константная ссылка (указатель) определяет только набор доступных операция по ссылку (указателю)
	// Сам объект на который ссылаются (указывают) может быть неконстантным и изменяться напрямую. 


	// ПРИМ. Преобразование не константы в константу возможно, но не наоборот
	const int *p1, b = 1;
	int *const p2 = nullptr;	// nullptr - литеральная константа, означающая нулевой указатель
	p1 = p2;
	//int &rb = b;		 	ОШИБКА


	constexpr int ce = 20;		// константное выражение (инициализируется во время компиляции и просто подставляется значение)
	const int c = const_init();	// неконстантное выражение т.к. инициализация происходит во время выполнения, а не компиляции.
								// такие функции должны быть простыми чтобы компилятор мог выполнять их во время компиляции.
	//constexpr int ce = const_init(); // ОШИБКА: функция объявлена без модификатора constexpr
	constexpr int *q = nullptr; // Константный указатель ( int *const q )
	// ПРИМ. constexpr можно определить только для литеральных типов, не для собственных вроде классов,
	// Для указателей и ссылок constexpr используется ВНЕ любых функций, т.к.определенные в функции переменные 
	// не хранятся по определенному адресу. Адрес переменной должен быть фиксированным (если внутри ф-ии то static)
	//constexpr int *q1 = &a;	ОШИБКА: a определена внутри функции ( на стеке ) 
	constexpr int *q2 = &::a;	// OK: ::а - обращение к глобально объявленной переменной 


	auto ai = 0, *pa = &ai;		// ai - int, pa - int* (auto - int)
	//auto aj = 0, pj = 3.14;	// ОШИБКА: несовместимые типы (auto - становится сначала int , затем double)
	auto ac = ce;				// auto - int , const верхнего уровня отбрасывается
	ac = 10;
	auto acp = p1, acpe = &ce;	// auto - const int* , const нижнего уровня сохраняется
	//*acp =10;	 ОШИБКА: нельзя изменять константу
	const auto accp = a;		// auto - const int
	const auto &ar = 42;


	// decltype определяет типо , но не инициализирует переменную значением.
	// decltype не отбрасывает const верхнего уровня
	decltype(const_init()) x1 = 33.3;	// x - int , т.к. функция имеет возвращаемое значение этого типа.
	std::cout << "x1: " << x1 << std::endl;	// 33
	//decltype(ce) x2;		ОШИБКА: константа должна быть инициализирована
	//decltype(ra) x3;		ОШИБКА: ссылка должна быть инициализирована
	// decltype возвращает ссылочный тип для выражений, результатомкоторых являются объекты, способные стоять 
	// лева от оператора присвоения. Некоторые выражения заставят возвращать ссылочный тип. 
	decltype(ra + 0) x4;	// ra - int& , но сложение возвращает тип int 
	//decltype(*q) x5; 		ОШИБКА: ссылка должна быть инициализирована ( получился ссылочный тип int& )
	decltype((a)) da = a;	// () - выражение, возвращающее ссылочный тип, da - ссылка на a


	struct Sales_data{
		std::string book_no;		// используется инициализатор по-умолчанию - ""
		unsigned units_sold = 0;	// внутриклассовый инициализатор
		double revenue {0.1};		// внутриклассовый инициализатор
	};
	using Sd = Sales_data;
	const Sd obj;	// ОК т.к. Sales_data имеет инициализаторы
	//constexpr Sales_data obj;	ОШИБКА: Sales_data не литерального типа (ни арифметический, ни указатель, ни ссылка)
	std::cout << "Sales_data obj: '" << obj.book_no << "' " << obj.units_sold << " " << obj.revenue << std::endl;	


	using std::endl; using std::cout;
	// ПРИМ. Не рекомендуется включать using в хедеры, т.к. текст хедера вставляется в исходник 
	// и может привести к нежелательному включению пространства имен
	cout << "No std needed, Hooray!" << endl;


	//chapter3();

	//chapter4();

	//chapter5();

	//chapter6();

	//chapter7();

	//chapter8();

	//chapter9();

	//chapter10();

	//chapter11();

	chapter12();

	return 0;
}

