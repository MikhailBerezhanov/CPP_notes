
#include <iostream>

// Константные выражения constexpr: возвращаемый тип и тип каждого параметра должны быть литералами 
// и return может быть только один
/*constexpr*/ int const_init() { return 10; }

constexpr size_t scale(size_t cnt) { return 50 * cnt;}	// константное выражение, если cnt - константное выражение

void chapter3 (void);
void chapter4 (void);
void chapter5 (void);
void chapter6 (void);
void chapter7 (void);
void chapter8 (void);
void chapter9 (void);
void chapter10 (void);
void chapter11 (void);
void chapter12 (void);
void chapter13 (void);

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

	chapter7();

	//chapter8();

	chapter9();

	return 0;
}


#include <string>
#include <cctype>
#include <vector>
void binary_search(std::vector<int> &vec, int sought);

void chapter3 (void)
{
	std::string str;
	// Блокирует поток до введения строки или конца файла
	if(std::cin >> str)
	{
		decltype(str.size()) size = 0;	// имеет тип  std::string::size_type
		size = str.size();

		for(auto c : str) {
			std::cout << ' ' << c << std::endl; 
			c = 'a';	// изменяет значение управляющей переменной, но не строки. для изменения
						// элемента строки нужно использовать ссылку &c в for
		}

		for(char &c : str) c = toupper(c);

		std::cout << "string '" << str << "' size:" << size << std::endl;
	} 

	const std::string s;
	for(auto &c : s){ //c = 0;	ОШИБКА: нельзя изменять строку (auto - const char)
	}


	using std::vector;
	vector<std::string> svec(10, "hello");	// содержит 10 элементов "hello"
	vector<int> ivec{1, 2, 3};				// содержит 3 элемента
	vector<std::string> svec2{10};			// содержит 10 элементов пустых строк, т.к. int не преобразуется в string
	svec2.push_back("ika");
	vector<int>::size_type svec2_size = svec2.size();
	std::cout << "svec2_size : " << svec2_size << std::endl;

	// Итераторы (end - указывает на эл-нт после конца контейнера (несуществующий))
	auto b = svec2.begin(), e = svec2.end();
	if(b != e) {
		*b = 'e';	// ПРИМ. *b возвращает ссылку на эл-т, обозначенный итератором
		std::cout << "svec2 begin iter: '" << *b << "'" << std::endl;
	}
	// ПРИМ. большинство итераторов не имеют оператора < поэтому используется == или != для сравнения
	for(auto iter = svec.begin(); iter != svec.end(); iter++){
		std::cout << *iter << std::endl;
		iter->empty();
		*iter++;
		//(*iter)++;		ОШИБКА
		//++*iter;			ОШИБКА
		//*iter.empty(); 	ОШИБКА
		//iter++->size(); 	Допустимо, но итератор будет указывать за пределы контейнера в данном случае
	}

	vector<int> __vec{1, 2, 3, 4, 5};
	binary_search(__vec, 1);
}

void binary_search(std::vector<int> &vec, int sought)
{
	auto left = vec.begin();
	auto right = vec.end() - 1;
	auto mid = left + (right - left) / 2;

	bool found = false;

	while(mid != right)
	{
		if(*mid == sought){
			found = true;
			break;
		} 

		if(sought < *mid) right = mid;
		else left = mid + 1;

		mid = left + (right - left) / 2;
	}

	std::cout << "Found: " << found << std::endl;
}


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



#include <stdexcept>

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






#include "Sales_data.h"


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


#include <fstream>
#include <sstream>

std::istream& my_print (std::istream& is)
{
	while(!is.eof())
	{
		std::string buf;
		is >> buf;
		std::cout << buf << std::endl;
	}
	 
	std::cout << "end of file" << std::endl;
	is.clear();

	return is;
}

int my_fread(const std::string& fname)
{
	std::string str;
	std::vector<std::string> vec;

	std::ifstream file(fname);	// Открыть файл в режиме in

	if(!file.is_open()) std::cerr << "file '" << fname << "' open failed" << std::endl;

	while(getline(file, str)) vec.push_back(str);	//Помещает каждую строку в вектор
	//while(file >> str) vec.push_back(str);	//Помещает каждое слово в вектор

	for(auto &it : vec) std::cout << it << std::endl;

	file.close(); // Закрывается неявно когда удаляется объект fstream
}

void chapter8 (void)
{
	int ival;
	using std::cin;
	using std::cout;
	using std::endl;

	//my_print(cin);

	//while(cin >> ival);

	cout << "fail: " << cin.fail() << " badbit: " << cin.bad() <<
	" eof:" << cin.eof() << endl;

	//my_fread("makefile");

	// ПРИМ, по умолчанию файл открывается в режиме out | trunc (усекается)
	// Открыть файл в режиме дозаписи (не стирая содержимое).
	std::ofstream out("file", std::ofstream::app); // out установлен неявно

	std::string my_str("first_word 2nd_word 3d_word 4th_word");
	std::string word;
	std::istringstream istr(my_str);// консруктор - поток содержит копию строки
	istr.str(my_str);				// копировать строку в поток 
	istr.str();						// вернуть копию строки, которую хранит поток
	// ПРИМ. можно читать из потоко строк istr по одному слову за раз
	istr >> word;;
	cout << word << endl;
	//my_print(istr);

	std::string my_str2;
	std::istringstream istr2(my_str2);
	my_str2 = my_str;
	//istr2.str(my_str2);
	cout << "istr2 contains: " << istr2.str() << endl;
}




#include <list>
#include <forward_list>
#include <deque>
#include <array>

static void read_str_to_deque()
{
	using std::string; using std::deque; using std::cin; using std::cout; using std::endl; using std::list;

	string word;
#ifdef DEQUE
	cout << "[ " << __func__ << "() ] --- Using deque as container" << endl;
	deque<string> c;
#else
	cout << "[ " << __func__ << "() ] --- Using list as container" << endl;
	list<string> c;
#endif

	auto iter = c.begin();
	// ПРИМ insert, emplace выполняют операции с эт-ом ПЕРЕД, обозначенным итератором
	// ПРИМ при изменении контейнера (добавление \ удаление эл-тов) его итераторы становятся недействительными
	// (инвалидация итераторов) поэтому в теле цикла необходимо обновлять интератор, а не кэшировать их заранее
	// данный цикл аналогичен вызовам push_front()
	while(cin >> word) iter = c.insert(iter, word);	// insert возвращает итератор на добавленный эл-т

	cin.clear();
	cout << "-------------------------------------------" << endl;
	cout << "container size: " << c.size() << endl;


	auto tmp = c.end();
	auto t = c.erase(tmp, tmp);	// удалить диапазон между двумя итераторами (в данном случае ничего не удаляется)
						
	for(auto &it : c) cout << it << endl;
}

// параметр - ссылка на массив из 12 эл-тов ттипа int
static void arr_remove_elems(int(&arr)[12])
{
	std::vector<int> vi(std::begin(arr), std::end(arr));
	std::list<int> li(std::begin(arr), std::end(arr));

	auto v_it = vi.begin();
	auto l_it = li.begin();

	// удалить из вектора четные эл-ты
	while(v_it != vi.end()){
		if(*v_it % 2) ++v_it;		// переход на следующий эл-т
		else v_it = vi.erase(v_it);	// возвращает итератор на эл-т сразу после последнего удаленного
	}
	std::cout << "Vector without even elements: " << std::endl;
	for(auto &it : vi) std::cout << it << std::endl;

	// удалить из списка нечетные эл-ты
	while(l_it != li.end()){
		if(*l_it % 2) l_it = li.erase(l_it);
		else ++l_it;
	}
	std::cout << "List without odd elements: " << std::endl;
	for(auto &it : li) std::cout << it << std::endl;
}

void chapter9 (void)
{
	std::list<std::list<int>> example;

	std::list<int> l;
	l.emplace(l.begin(), 12);
	std::cout << "list size: " << l.size() << std::endl;

	std::array<char, 10> carr = {'a', 0x31, 'b', 0x43};
	std::array<char, 10>::size_type idx = 0;
	carr[idx] = 'v';

	std::vector<int> vec1;			// Инициализация стандартным конструкторм (пустой вектор)
	std::vector<int> vec2(10);		// Создание 10 элементов, инициализация по умолчанию (0)
	std::vector<int> vec3(10, 43);	// Создание 10 эл-в и их инициализация значением 43
	std::vector<int> vec4(vec3);	// Конструктор копирования (типы должны совпадать)
	std::vector<int> vec5 = {1, 2, 3, 4, 5};	// Инициализация списком инициализации
	std::vector<int> vec6(vec5.begin(), vec5.end());	// Конструктор получающий два интератора для копирования (диапазон)


	
	std::list<int> la{1, 2, 3, 4, 5};
	std::vector<double> va(la.begin(), la.end());

	// ОШИБКА сравнения котнейнеров разных типов
	//if(la == va) std::cout << "Container copy succeed" << std::endl;
	auto it_vec = va.begin();
	for(auto it_list = la.begin(); it_list != la.end() && it_vec != va.end(); ++it_list, ++it_vec)
	{
		if(*it_list != *it_vec) std::cout << "Container copy failed" << std::endl;
	}

	std::vector<char> vt;
	//vt[0] = 'a';		ОШИБКА. Вектор пуст, нет элементов
	try{
		vt.at(0) = 'a';	// возбудит исключение, но не крах программы
	}
	catch(std::out_of_range err){
		std::cout << "[ TEST ] Exception occured: " << err.what() << std::endl;
	}

	if(!vt.empty()){
		vt.front() = 'a';	// требуют проверки наличия эл-тов в контейнере
		*vt.begin() = 'a';
	}
	

	vt.insert(vt.begin(), 10, 'a');	// вставить 10 элементов 'a' перед итератором begin()
	std::vector<Sales_data> vsales;
	vsales.emplace(vsales.begin(), "666");	// вызывает конструктор класса объекта, создает объект в контейнере
	vsales.emplace_back();	// используется конструктор Sales_data по умолчанию
	vsales.emplace(vsales.end(), "999", 25, 15.99);	// аргументы должны соответствовать конструктору типа эл-та


	//read_str_to_deque();


	int ai[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89};
	arr_remove_elems(ai);
}