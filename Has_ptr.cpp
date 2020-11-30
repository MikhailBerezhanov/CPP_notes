#include "Has_ptr.h"

// Инициализация статического члена класса
size_t numbered::cnt = 0;

// ПРИМ: Синтезируемый конструктор копирования поэлементно копирует НЕСТАТИЧЕСКИЕ переменные-члены объкетов,
// вызывая оператор = для соотвествующих типов (встроенных или классов)
Has_ptr::Has_ptr(const Has_ptr& h)
{
	// копирование объекта в НОВУЮ выделенную память, а не копирование указателя (как в поведении синтезируемой версии)
	std::string str = *(h.ps);
	ps = new std::string(str);
	i = h.i;
	std::cout << MODULE_NAME << "Copy Constructor called" << std::endl;
}


// Обычно операторы присвоения объединяют действия деструктора и конструктора копий
// ПРИМ: Синтезируемый оператор присвоения присваивает значение каждой НЕСТАТИЧЕСКОЙ переменной-члена правого объекта
// соответсвующей переменной-члену левого объекта с использованием оператора присвоения копии = типа этой переменной
Has_ptr& Has_ptr::operator= (const Has_ptr& rhs) 
{
	std::string str = *(rhs.ps);		// Отработка случая присвоения самому себе (rhs == *this):
	auto newp = new std::string(str);	// 1.Сначала осуществляется копирование правого операнда
	delete(ps);							// 2.Освободить прежнюю память
	ps = newp;							// 3.Копировать данные из rhs в текущий объект

	i = rhs.i;

	std::cout << MODULE_NAME << "Custom operator= called" << std::endl;

	return *this;
}

// ПРИМ: Правило три\пять: если классу нужен деструктор, то нужны и конструктор-копирования и оператор присвоения
// Если нужен конструктор-копирования, то понадобится и  оператор присвоения и наоборот. Однако деструктор тогда необязателен
Has_ptr::~Has_ptr() 
{ 
	delete(ps); // если бы использовилсь синтезируемые конструктор копирования или оператор присвоения, происходило бы
				// многократное освобождения памяти по одному и тому же указтелю (ОШИБКА)
	std::cout << MODULE_NAME << "Custom Destructor called" << std::endl; 
}

void swap(Has_ptr& lhs, Has_ptr& rhs)
{
	using std::swap;

	swap(lhs.ps, rhs.ps);	// обмен указателями а не строковыми данными - преимущество над 

	swap(lhs.i, rhs.i);

	std::cout << MODULE_NAME << "Custom Swap called" << std::endl;
}

bool Has_ptr::operator< (const Has_ptr& rhs)
{

	std::cout << MODULE_NAME << "Custom operator< called" << std::endl;
	return *ps < *rhs.ps;
}


// --- Версия класса, дейстивующего как указатель

Has_ptr_& Has_ptr_::operator=(const Has_ptr_& rhs)
{
	std::cout << _MODULE_NAME << "Custom operator= called" << std::endl;

	++*rhs.use;

	if (--*use == 0){
		delete ps;		// удалить строку
		delete use;		// и счетчик
		 
		std::cout << _MODULE_NAME << "deleting()" << std::endl;
	}

	ps = rhs.ps;
	i = rhs.i;
	use = rhs.use;

	return *this;
}

Has_ptr_::~Has_ptr_()
{
	std::cout << _MODULE_NAME << "Custom Destructor called" << std::endl;

	if( --*use == 0 )	// если счетчик ссылок достигает нуля
	{
		delete ps;		// удалить строку
		delete use;		// и счетчик
		std::cout << _MODULE_NAME << "deleting()" << std::endl;
	}
}

inline void swap(Has_ptr_& lhs, Has_ptr_& rhs)
{
	//

	using std::swap;

	// Собственные ф-ии должны вызывать swap() а не std::swap, чтобы срабатывала именно версия класса,а не библиотечная
	swap(lhs.ps, rhs.ps);	// обмен указателями а не строковыми данными
	swap(lhs.i, rhs.i);
}