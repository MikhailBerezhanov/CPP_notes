#ifndef _MY_STRING_H
#define _MY_STRING_H

#include <string>
#include <iostream>
#include <memory>	// allocator
#include <utility>	// pair, move
#include <memory>	// unitialized_copy
#include <algorithm>
#include <cstring>

#define MY_STRING_NAME	"[ MyString ] "

class MyString
{

friend std::ostream& operator<< (std::ostream& os, const MyString& s);
friend std::istream& operator>> (std::istream& is, MyString& s);

public:
	// стандартный конструктор
	MyString(const char* s = "");

	// конструктор копирования
	MyString(const MyString&);

	// конструктор перемещения
	MyString(MyString&&) noexcept;

	// деструктор
	~MyString();

	// присвоение копирования
	MyString& operator= (const MyString&);

	// присвоение перемещения
	MyString& operator= (MyString&&);

	
	// ПРИМ: Перегрузка операторов - если ф-ия обяъевлена как член класса, то левым операндом становится this. Таким образом,
	// 1. Операторы = , [], (), *, -> следует определять как члены класса, т.к. они могут непосредственно изменять объект
	// 2. Составные операторы присвоения +=, -= обычно тоже должны быть членами класса. т.к. они изменяют объект
	// 3. Инкремент и декремент обычно тоже должны быть членами класса.
	// 4. Симметричные операторы, такие как арифметические, равнества, сравнения и побитовые, лучше опеределять как обычные ф-ии.
	// string u = "hi" + s; - если + был бы членом класса string, была бы ошибка, т.к. const char* не имеет оператора +
	// 5. Операторы ввода-вывода <<, >> должны быть обычными ф-иями, а не членами класса
	// ПРИМ: обычно оператора запятая (,), обращение к адресу (&), логические AND и OR (&& и ||) не должны быть перегружены

	// Составной оператор приставивания. (Изменяет ЭТОТ объект, поэтому является членом класса)
	MyString& operator+= (const MyString& rhs);

	// Оператор индексировани (возвращающий ссылку оператор применим к обеих сторон присвоения, поэтому имеет смысл
	// определить и константную, и неконстантную версии оператора индексирования)
	char& operator[] (size_t n) const { return beg_ptr[n]; }
	char& operator[] (size_t n) { return beg_ptr[n]; }

	// Операторы инкремента и декремента ( не определены, только для демонстрации объявления)
	// Префиксные
	MyString& operator++ ();
	MyString& operator-- ();
	// Постфиксные (для отличения от перфиксных используется неиспользуемый доп. параметр int, кудл компилятор передает 0)
	MyString operator++ (int);	// возвращают копию сохраненного предыдущего состояния объекта. после чего делают операции
	MyString operator-- (int);


	// Перегрузка преобразования из тип класса (пользовательские преобразования)
	// ф-ия преобразования должна быть ф-ией членом и у нее не определен тип возвращаемого значения и пустой список
	// параметорв. Обычно ф-ия константная
	// ПРИМ: Компилятор применяет только одно пользовательское преобразование за раз, неявное пользовательское
	// преобразование можно предварить встроенным преобразованием
	// ПРИМ: Если оператор преобразования является явным (explicit), приведение следует осуществлять явно, за исключениеми:
	// ( компилятор приметнит неявное преобразование для следующих выражений )
	// 1. условные операторы if, while, do
	// 2. выражение условия в заголовке for
	// 3. логические операнды !, ||, &&
	// 4. выражение условия в лог.оператора ?:
	explicit operator int() const { return static_cast<int>(size()); }
	// преобразование в bool обычно должно быть явным
	explicit operator bool() const { return static_cast<bool>(size()); }

	operator const int()  { return static_cast<const int>(size()); }
	// ПРИМ: обычно не стоит делать больше одного преобразования в арифметические типы из-за возникающей неопределенности
	// Также не стоит создавать взаимных преобразований типов внутри классов ( если класс Foo имеет конструктор, 
	// получающий объект класса Bar, не надо создавать в классе Bar оператор преобразования в тип Foo )


	std::ostream& show(std::ostream& os) const;
	size_t size() const { return end_ptr - beg_ptr; }
	char* begin() const { return beg_ptr; }
	char* end() const { return end_ptr; }

private:
	std::allocator<char> alloc;		// алокатор символов
	char* beg_ptr = nullptr;		// указатель на начало строки
	char* end_ptr = nullptr;		// указатель на эл-т после последнего 


	std::pair<char*, char*> alloc_n_copy(char* b, char* e);
	void free();
};


MyString operator+ (const MyString& lhs, const MyString& rhs);
bool operator== (const MyString& lhs, const MyString& rhs);
bool operator!= (const MyString& lhs, const MyString& rhs);



class PrintString
{
public:
	PrintString(std::ostream& o = std::cout, char c = ' '):
		os(o), sep(c) {}

	// Оператор вызова объекта класса как ф-ии (объекта ф-ии)
	// ПРИМ: может быть сколько угодно перегруженный операторов вызова ф-ии, главное с разным числом и типом параметров
	void operator() (const std::string& s) const { os << s << sep; }

private:
	std::ostream& os;
	char sep;
};

// Пример использования в стандартных алгоритмах:
// for_each(vs.begin(), vs.end(), PrintString(cerr, '\n'));


class ReadString
{
public:
	ReadString(std::istream& i = std::cin): is(i) {}

	bool operator() (std::string& s) const 
	{ 
		if(!(is >> s)) {
			s = "";
			return false;
		}
		return true; 
	}

private:
	std::istream& is;
};



class MyStringNew
{

//friend std::ostream& operator<< (std::ostream& os, const MyStringNew& s);
//friend std::istream& operator>> (std::istream& is, MyStringNew& s);

public:
	// стандартный конструктор
	MyStringNew(const char* s = "");

	// конструктор копирования
	MyStringNew(const MyStringNew&);

	// конструктор перемещения
	MyStringNew(MyStringNew&&) noexcept;

	// деструктор
	~MyStringNew();

	// присвоение копирования
	MyStringNew& operator= (const MyStringNew&);

	// присвоение перемещения
	MyStringNew& operator= (MyStringNew&&);


	// Оператор индексировани (возвращающий ссылку оператор применим к обеих сторон присвоения, поэтому имеет смысл
	// определить и константную, и неконстантную версии оператора индексирования)
	char& operator[] (size_t n) const { return beg_ptr[n]; }
	char& operator[] (size_t n) { return beg_ptr[n]; }

	std::ostream& show(std::ostream& os) const;
	size_t size() const { return end_ptr - beg_ptr; }
	char* begin() const { return beg_ptr; }
	char* end() const { return end_ptr; }

private:
	char* beg_ptr = nullptr;		// указатель на начало строки
	char* end_ptr = nullptr;		// указатель на эл-т после последнего 


	std::pair<char*, char*> alloc_n_copy(char* b, char* e);
	void free();
};







#endif