#ifndef _HAS_PTR_H
#define _HAS_PTR_H

#include <string>
#include <iostream>
#include <vector>
#include <memory>

#define MODULE_NAME	 "[ Has_ptr ] "

class Has_ptr
{
public:
	Has_ptr(const std::string& s = std::string()): 
		ps(new std::string(s)), i(0) { std::cout << MODULE_NAME << "Standart Constructor called" << std::endl; }

	// Конструктор копирования. Если бы передача была без ссылки - то для передачи параметра в сам конструктор , 
	// должен был бы использоваться этот конструктор копии, и вызов зациклился бы до бесконечности
	// ПРИМ: вызывается при
	// - инициализации объекта копией
	// - передачи параметра в ф-ию (или конструктор) присваиванием копирования  
	// - возвращения значения из функции не по ссылке
	// - инициализация эл-тов массива списком инициализации {..}
	Has_ptr(const Has_ptr& h);

	// без деструктора не будут освобождены объекты, на которые указывает ps объектов
	// Деструктор вызывает свое тело, после чего запускает деструкторы переменных-членов в порядке обратном инициализации 
	// ПРИМ: Когда из области видимости выходит ссылка или встроенный указателей на объект, деструктор не вызывается
	~Has_ptr();

	// Операторы присвоения обычно должны возвращать ссылку на свой левый операнд
	Has_ptr& operator= (const Has_ptr& );

	void show_ps() { std::cout << *ps << std::endl; }

private:
	// переменные-члены инициализируется в порядке объявления
	std::string* ps;
	int i;
};

// debug
struct numbered
{
	numbered(){ mysn = cnt++; }

	numbered(const numbered&){ mysn = cnt++; } 

	size_t mysn;
	static size_t cnt;
};



#endif // _HAS_PTR_H