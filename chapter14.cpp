
//--- Перегрузка операторов и преобразований ---

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
#include <list>
#include <algorithm>			// sort, unique, partition
#include <map>
#include <set>
#include <utility>				// pair, move
#include <memory>				// shared_ptr, unique_ptr, weak_ptr, allocator
#include <new>					// bad_alloc, nothrow

#include "Str_blob.h"
#include "Text_query.h"
#include "Has_ptr.h"
#include "Message.h"
#include "StrVec.h"
#include "MyString.h"
#include "exercises.h"


void chapter14(void)
{


	MyString s = "hello Dasha";
	MyString s2 = s;	// Инициализация копированием (вызывается конструктор копирования)

	MyString s3 = "BADABASH";	
	//s = s3;			// Присвоение копирования (переменная s3 - l-value)
	//s = "KAKULYA";	// Присвоение перемещения (литерал - r-value)
	// ИЛИ
	s = std::move(s3);	// Присвоение перемещения (move делает из l-value - r-value), НО s3 уже пользоваться нельзя

	std::cout << "My string (size= "<< s.size() <<") is: ";
	s.show(std::cout);
	std::cout << std::endl;
	s2.show(std::cout);
	std::cout << std::endl;
}