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
public:
	// стандартный конструктор
	MyString(const char* s = "");

	// конструктор копирования
	MyString(const MyString&);

	// конструктор перемещения
	MyString(MyString&&);

	// деструктор
	~MyString();

	// присвоение копирования
	MyString& operator= (const MyString&);

	// присвоение перемещения
	MyString& operator= (MyString&&);

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



#endif