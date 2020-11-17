
//--- Функции ---

#include <iostream>
#include <string>
#include <vector>

#include "exercises.h"

// Параметры - члены функции инициализирующиеся аргументами. Параметр задается единожды, а аргументов может быть много
// Аргументы - то, что функция принимает при вызове и подставляет в параметры
static std::vector<std::string> unknown_num_of_args(std::initializer_list<int> args)
{
	auto num = args.size(); // std::initializer_list<int>::size_type

	std::cout << "arg num is: " << num << ". args are: ";

	for(const auto param : args){
		std::cout << param << " ";
	}
	std::cout << std::endl;

	return{"hello", "from", "func"}; // С++11 можно возвращать инициализированным списоком
}

// Возвращает ссылку на элемент строки (lvalue). Во всех остальных случая - rvalue
// ПРИМ, встраиваемые функции часто определяют в хедер файлах
static inline char& get_val(std::string& s, std::string::size_type idx) { return s[idx]; }


using arr_t = int[10];	// typedef int arr_t[10]; - Эквиваленто
static arr_t* func(int i);	// Возвращает указатель на массив из 10 элментов типа int 

// Возвращает указатель на массив типа int ( без скобок - массив указателей)
static int (*func2(int i))[];

// C++11 Замыкающий тип возвращаемого значения Возвращает ссылку на массив из 10 элментов типа int
static auto func3(int i) -> int(&)[10];

int odd[] = {1, 3, 5, 7, 9};
int even[] = {2, 4, 6, 8};
// C++11 Возвращает указатель на массив типа определенного decltype
// ПРИМ, decltype не преобразует массив в указатель, поэтому необходимо добавить *
static decltype(odd)* func4(int i);

// Функция может иметь аргументы по-умолчанию (один или несколько), но если оно объявлено, то все параметры 
// за ним тоже должны иметь значение по по-умолчаниюю
char cdef = 'd';	// может быть любым выражение, тип которого приводим к типу параметра
char def() { return 'k'; }
void foo(int len, char c = cdef, char = def(), std::string s = "default value"/*, int a - ошибка*/){ len = 0; };

// Перегрузка функций (выбирается наиболее подходящая по принципу максимального соответсвия кол-ва аргументов и их типов)
// Больший приоритет получаетя функция без неявных преобразований типа аргумента
void f() { std::cout << "f() called" << std::endl; }
void f(int a) { std::cout << "f(int) called" << std::endl; }
void f(int a, int b) { std::cout << "f(int, int) called" << std::endl; }
void f(double a, double b = 3.14) { std::cout << "f(double, double) called" << std::endl; }

int calc(int&, int&);
int calc(const int&, const int&);	// OK по ссылке нельзя будет изменить объект
int calc(char*, char*);
int calc(const char*, const char*);	// OK по указателю нельзя будет изменить объект

int calc2(double*, double*);
int calc2(double* const, double* const);


void chapter6 (void)
{
	int arr[] = {1, 2, 3, 4};
	unknown_num_of_args({arr[0], arr[1], arr[2], arr[3], 5});

	std::cout << "array after func call: ";
	for(auto &elem : arr){
		//elem = 99;
		std::cout << elem << " ";
	}
	std::cout << std::endl;

	std::string str("test string");
	char* p = &str[0];
	get_val(str, 0) = 'T';
	std::cout << str << std::endl;

	
	// Вызов с аргументами по-умолчанию следует делать с осторожностью. Пропускать можно только аргументы крайние справа
	//foo(); 		// ОШИБКА: len не имеет значения по умолчанию
	foo(1);
	foo('a');		// ВНИМАНИЕ: Эквивалентно foo('a', 'd', 'k', "default value") из-за преобразования char в int
	//foo('a', "string"); ОШИБКА: нет преобразования const char* в char (второй параметр)
	int tmp = 99;
	char cdef = 'm';// Скрывает внешнее определение cdef но не изменяет значение по умолчанию
	foo(tmp);		// foo(99, 'd', 'k', std::string s = "default value")


	//f(2.56, 42);	// ERROR: неоднозначность вызова (2.56 можно преобдразовать в int, и 42 можно в double)
	f(42);			// (int)
	f(42, 0);		// (int, int)
	f(2.56, 3.14);	// (double, double)
}