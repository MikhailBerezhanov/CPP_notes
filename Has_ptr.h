#ifndef _HAS_PTR_H
#define _HAS_PTR_H

#include <string>
#include <iostream>
#include <vector>
#include <memory>

#define MODULE_NAME	 	"[ Has_ptr ] "
#define _MODULE_NAME	"[ Has_ptr_ ] "

// --- Класс действуют как значение (у каждого объекта класса есть собственный экземпляр строки, на которую указывает ps)
class Has_ptr
{

friend void swap(Has_ptr&, Has_ptr&);

public:
	Has_ptr(const std::string& s = std::string()): 
		ps(new std::string(s)), i(0) { std::cout << MODULE_NAME << "Standart Constructor called" << std::endl; }

	Has_ptr(const char* s, int ii):
		ps(new std::string(s)), i(ii) { std::cout << MODULE_NAME << "Overloaded Constructor called" << std::endl; }

	// Конструктор копирования. Если бы передача была без ссылки - то для передачи параметра в сам конструктор , 
	// должен был бы использоваться этот конструктор копии, и вызов зациклился бы до бесконечности
	// ПРИМ: вызывается при
	// - инициализации объекта копией
	// - передачи параметра в ф-ию (или конструктор) присваиванием копирования  
	// - возвращения значения из функции не по ссылке
	// - инициализация эл-тов массива списком инициализации {..}
	Has_ptr(const Has_ptr& h);
	// ПРИМ: для классов, которые должны предотвравить копирование, следует определять собственный конструктор копий и оператор
	// присвоения копии, используя часть =delete вмето объявления их закрытыми (private)


	// без деструктора не будут освобождены объекты, на которые указывает ps объектов
	// Деструктор вызывает свое тело, после чего запускает деструкторы переменных-членов в порядке обратном инициализации 
	// ПРИМ: Когда из области видимости выходит ссылка или встроенный указателей на объект, деструктор не вызывается
	~Has_ptr();

	// Операторы присвоения обычно должны возвращать ссылку на свой левый операнд
	Has_ptr& operator= (const Has_ptr& );

	bool operator< (const Has_ptr&);

	void show_ps() { std::cout << *ps << std::endl; }

private:
	// переменные-члены инициализируется в порядке объявления
	std::string* ps;
	int i;
};

// --- Класс действуют как указатель (копируется указатель на строку)
class Has_ptr_
{


// Если класс и определяет собственную ф-ию swap, алгоритмы используют именно ее для перемещения эл-тов
// в противном случае используется библиотечная ф-ия, осуществляющая копирование переменных-членов, которое
// зачастую бывает затратным. В данном случае можно было бы обменять лишь указатели на строки, вместо создания
// временного объекта Has_ptr_ и копирований
friend void swap(Has_ptr_&, Has_ptr_&);	// дружественная ф-ия для доступа к закрытым членам


public:
	Has_ptr_(const std::string s = std::string()):
		ps(new std::string(s)), i(0), use(new std::size_t(1)) 
			{ std::cout << _MODULE_NAME << "Standart Constructor called" << std::endl; }

	// Конструктор копий копирует все трри переменные-члена и увеличивает счетчик ссылок на ps
	Has_ptr_(const Has_ptr_& p):
		ps(p.ps), i(p.i), use(p.use) { ++use; }

	Has_ptr_& operator=(const Has_ptr_&);

	~Has_ptr_();
private:
	std::string* ps;
	int i;
	std::size_t* use;	// счетчик ссылок, расположенный в дин.памяти для общего доступа из разных объектов
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