#include <MyString.h>

// Создает пространство для нового объекта, копирует диапазон эл-тов в выделенное пр-во
// возвращает пару указателей на начало нового пр-ва и эл-т за последним
std::pair<char*, char*> MyString::alloc_n_copy(char* b, char* e)
{
	char* new_beg = alloc.allocate(e - b);
	auto new_end = std::uninitialized_copy(b, e, new_beg); // копировать эл-ты в новую область

	return {new_beg, new_end};
}

// Освобождает выделенную аллокатором память в куче
void MyString::free()
{
	if(beg_ptr == nullptr || end_ptr == nullptr) return;

	for(auto it = begin(); it != end(); ++it) alloc.destroy(it);

	alloc.deallocate(begin(), size());
}

// Стандартный конструктор
MyString::MyString(const char* s):
	beg_ptr(nullptr), end_ptr(nullptr)
{
	std::cout << MY_STRING_NAME << "Standart constructor called" << std::endl;

	size_t len = std::strlen(s);	// символа конца строки не будет в хранимом диапазоне, только символы
	beg_ptr = alloc.allocate(len);
	end_ptr = beg_ptr;

	// заполенние выделенной памяти значениями символов
	for(size_t i = 0; i != len; ++i) alloc.construct(end_ptr++, s[i]);

}

// Конструктор копировани (создает новую область в дин.памяти и копирует туда существующие эл-ты правого операнда)
// таким образом копия (левый операнд) остается доступным даже при удалении правого (исходного) объекта
MyString::MyString(const MyString& s)
{
	std::cout << MY_STRING_NAME << "Copy constructor called" << std::endl;

	auto ret = alloc_n_copy(s.begin(), s.end());

	beg_ptr = ret.first;
	end_ptr = ret.second;
}

// Конструктор перемещения (не копирует эл-ты, а перемещает указатели правого операнда в левый и подготавливает правый для удаления)
MyString::MyString(MyString&& s):
	beg_ptr(s.beg_ptr), end_ptr(s.end_ptr)
{
	std::cout << MY_STRING_NAME << "Move constructor called" << std::endl;

	// оставить перемещаемый объект в приемлимом состоянии для вызова деструктора
	// (нам не надо освобождать перемещенное пространство, т.к. на него смотрит ЭТОТ объект)  
	s.beg_ptr = s.end_ptr = nullptr;
}

MyString::~MyString()
{
	std::cout << MY_STRING_NAME << "Destructor called for '";
	this->show(std::cout);
	std::cout <<"'"<< std::endl;

	free();
}

// Оператор присвоения копирования (выполняет ф-ии конструктор копии и деструктора)
MyString& MyString::operator= (const MyString& rhs)
{
	std::cout << MY_STRING_NAME << "Custom copy operator= called" << std::endl;
	// копировать эл-ты правого операнда в новую область (защита от присваивания себе себя)
	auto ret = alloc_n_copy(rhs.begin(), rhs.end());	
	// освободить текущую память выделенную аллокатором
	free(); 
	// обновить указатели на новую область
	beg_ptr = ret.first;
	end_ptr = ret.second;

	return *this;
}

// Оператор присвоения перемещения (выполняет ф-ии конструктор копии и деструктора)
MyString& MyString::operator= (MyString&& rhs)
{
	std::cout << MY_STRING_NAME << "Custom move operator= called" << std::endl;

	if(this != &rhs){
		// освободить текущую память выделенную аллокатором
		free(); 
		// переместить указатели на эл-ты
		beg_ptr = rhs.beg_ptr;
		end_ptr = rhs.end_ptr;
		// подготовить правый операнд к вызову деструктора (чтобы он не освобождал данную память)
		rhs.end_ptr = rhs.beg_ptr = nullptr;
	}

	return *this;
	// вызывается деструктор для rhs
}


std::ostream& MyString::show(std::ostream& os) const
{
	// толерантно к begin() и end() возвращающим nullptr
	for(auto it = begin(); it != end(); ++it) os << *it;

	return os;
}