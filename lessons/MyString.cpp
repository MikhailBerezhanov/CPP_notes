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
MyString::MyString(MyString&& s) noexcept:
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








std::ostream& operator<< (std::ostream& os, const MyString& s)
{
	//std::cout << MY_STRING_NAME << "Custom operator<< called" << std::endl;

	return s.show(os);
}

std::istream& operator>> (std::istream& is, MyString& s)
{
	char symb = 0;
	char buf[512];
	std::memset(buf, 0, sizeof(buf));

	size_t len = 0;

	std::cout << MY_STRING_NAME << "Custom operator>> called" << std::endl;

	while(is >> symb){

		if(len >= sizeof(buf)) {
			// TODO
			s.free();
			s = MyString();
			return is;	
		}

		buf[len++] = symb;	// помещение прочитанного символа в буфер

	}

	if(is.bad()) {	// ввод неудачен, вернуть объект в стандартное состояние
		s.free();
		s = MyString();
		std::cout << MY_STRING_NAME << "Custom operator>> badbit detected" << std::endl;
		return is;
	}

	s.free();	// освободить что было в объекте ранее
	auto ret = s.alloc_n_copy(&buf[0], &buf[len]);
	s.beg_ptr = ret.first;
	s.end_ptr = ret.second;

	return is;
}

// Составной оператор приставивания. (Изменяет ЭТОТ объект, поэтому является членом класса)
MyString& MyString::operator+= (const MyString& rhs)
{
	std::cout << MY_STRING_NAME << "Custom operator+= called" << std::endl;

	size_t newlen = size() + rhs.size();

	// создать копию текущей строки
	MyString tmp(*this);
	// освободить текущие эл-ты
	free();	
	// создать новое пространство, вмещающее обе строки
	beg_ptr = alloc.allocate(newlen);
	// скопировать левую строку в новый объект
	end_ptr = std::uninitialized_copy(tmp.begin(), tmp.end(), beg_ptr);
	// добавить правую строку к новому объекту
	end_ptr = std::uninitialized_copy(rhs.begin(), rhs.end(), end_ptr);

	return *this;
}


// Конкатенация двух строк
MyString operator+ (const MyString& lhs, const MyString& rhs)
{
	MyString sum = lhs;	// копирование переменных-членов lhs 
	sum += rhs;			// добавить rhs уже поределенным составным оператором +=

	return sum;
}


bool operator== (const MyString& lhs, const MyString& rhs)
{
	size_t len = lhs.size();

	if(len != rhs.size()) return false;

	for(size_t i = 0; i != len; ++i){
		if(lhs[i] != rhs[i]) return false;
	}

	return true;
}

bool operator!= (const MyString& lhs, const MyString& rhs)
{
	return !(lhs == rhs);
}






// Реализация класса MyStringNew  без использования библиотечного класса аллокатора
#define MY_STRING_NEW_NAME "[MyStringNew] "
// Создает пространство для нового объекта, копирует диапазон эл-тов в выделенное пр-во
// возвращает пару указателей на начало нового пр-ва и эл-т за последним
std::pair<char*, char*> MyStringNew::alloc_n_copy(char* b, char* e)
{
	char* new_beg = new char[e - b];
	// копировать эл-ты в новую область
	//auto new_end = std::uninitialized_copy(b, e, new_beg); 
	char* new_end = new_beg;

	for (char* data_ptr = b; data_ptr != e; ++data_ptr) *new_end++ = *data_ptr;

	return {new_beg, new_end};
}

// Освобождает выделенную аллокатором память в куче
void MyStringNew::free()
{
	if(beg_ptr == nullptr) return;

	delete[] beg_ptr;
}

// Стандартный конструктор
MyStringNew::MyStringNew(const char* s):
	beg_ptr(nullptr), end_ptr(nullptr)
{
	std::cout << MY_STRING_NEW_NAME << "Standart constructor called" << std::endl;

	size_t len = std::strlen(s);	// символа конца строки не будет в хранимом диапазоне, только символы
	beg_ptr = new char[len]; 
	end_ptr = beg_ptr;

	// заполенние выделенной памяти значениями символов
	for(size_t i = 0; i != len; ++i) *end_ptr++ = s[i];

}

// Конструктор копировани (создает новую область в дин.памяти и копирует туда существующие эл-ты правого операнда)
// таким образом копия (левый операнд) остается доступным даже при удалении правого (исходного) объекта
MyStringNew::MyStringNew(const MyStringNew& s)
{
	std::cout << MY_STRING_NEW_NAME << "Copy constructor called" << std::endl;

	auto ret = alloc_n_copy(s.begin(), s.end());

	beg_ptr = ret.first;
	end_ptr = ret.second;
}

// Конструктор перемещения (не копирует эл-ты, а перемещает указатели правого операнда в левый и подготавливает правый для удаления)
MyStringNew::MyStringNew(MyStringNew&& s) noexcept:
	beg_ptr(s.beg_ptr), end_ptr(s.end_ptr)
{
	std::cout << MY_STRING_NEW_NAME << "Move constructor called" << std::endl;

	// оставить перемещаемый объект в приемлимом состоянии для вызова деструктора
	// (нам не надо освобождать перемещенное пространство, т.к. на него смотрит ЭТОТ объект)  
	s.beg_ptr = s.end_ptr = nullptr;
}

MyStringNew::~MyStringNew()
{
	std::cout << MY_STRING_NEW_NAME << "Destructor called for '";
	this->show(std::cout);
	std::cout <<"'"<< std::endl;

	free();
}

// Оператор присвоения копирования (выполняет ф-ии конструктор копии и деструктора)
MyStringNew& MyStringNew::operator= (const MyStringNew& rhs)
{
	std::cout << MY_STRING_NEW_NAME << "Custom copy operator= called" << std::endl;
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
MyStringNew& MyStringNew::operator= (MyStringNew&& rhs)
{
	std::cout << MY_STRING_NEW_NAME << "Custom move operator= called" << std::endl;

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


std::ostream& MyStringNew::show(std::ostream& os) const
{
	// толерантно к begin() и end() возвращающим nullptr
	for(auto it = begin(); it != end(); ++it) os << *it;

	return os;
}