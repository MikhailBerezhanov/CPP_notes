
#include "StrVec.h"


void StrVec::check_n_alloc() 
{ 
	//std::cout << "StrVec::check_n_alloc called. Size: " << size() << " Cap: " << capacity() << std::endl;

	if(size() == capacity()) {
		std::cout << "reallocating.." << std::endl;
		reallocate(); 
	}
}



// Резервирует достаточно места для содержания заданного диапазона элементов, а затем копирует эти эл-ты в новое пространство
// Возвращает пару, с указателями на начало нового пространства и на позицию после конца
std::pair<std::string*, std::string*> StrVec::alloc_n_copy (const std::string* b, const std::string* e)
{
	auto data = alloc.allocate(e - b);

	// unitialized_copy - лагоритм связанный с классом allocator: копирует эл-ты из исходного диапазона в незаполненную
	// память, обозначенную последним итератором. Память должна быть достаточно велика для содержания копии исходных эл-тов.
	// возвращает итератор на эл-т за последним скопированным эл-том в новой области.
	return {data, std::uninitialized_copy(b, e, data)};
}

// выделение новой памяти и ПЕРЕМЕЩЕНИЕ всех эл-тов в новое пространство
// 1. зарезервировать память для нового, большего массива строк
// 2. заполнить первую часть этого пространтсва существующими эл-тами
// 3. удалить эл-ты в существующей памяти и освободить ее
// ПРИМ: т.к. эл-ты будут почти сразу удалены, нет смысла их копировать. Лучше использовать ПЕРЕМЕЩЕНИЕ
void StrVec::reallocate()
{
	//std::cout << "StrVec::reallocate called" << std::endl;
	// при каждом пересоздании вектора будем удваивать его емкость
	auto new_capacity = size() ? 2*size() : 1;

	auto newdata = alloc.allocate(new_capacity);

	auto dest = newdata;	// указывает на следующую свободную позицию в новом массиве
	auto elem = elements;	// указывает на следующий эл-т в старом массиве

	for(size_t i = 0; i != size(); ++i){
		// ПРИМ: std::move() вызывает конструктор ПЕРЕМЕЩЕНИЯ (вместо копирования) объекта (строки в данном случае)
		std::cout << " moving: " << *elem << " ...";
		alloc.construct(dest++, std::move(*elem++));

		//std::cout << " after moving: " << *(elem-1) << std::endl; // ОШИБКА: после перемещения данные недоступны
		std::cout << " after moving: " << *(dest-1) << std::endl;
	}

	std::cout << " calling free from reallocate" << std::endl;
	free(); // освободить старый массив эл-тов

	// обновить переменные-члены
	elements = newdata;
	first_free = dest;
	cap = elements + new_capacity;
}

void StrVec::free()
{
	if(elements == nullptr) return;

	std::cout << __func__ <<  " called" << std::endl;

	//for(auto p = first_free; p != elements; ) alloc.destroy(--p);	// вызывает деструктор для каждой строки
	// эквивалентно
	for_each(begin(), end(), [this](std::string& s)
		{ std::cout << "Destroying: " << s << std::endl; alloc.destroy(&s); });

	alloc.deallocate(elements, capacity());
}



StrVec::StrVec(std::initializer_list<std::string> il)
	//elements(nullptr), first_free(nullptr), cap(nullptr)
{
	for(const auto el : il) push_back(el);
}


StrVec::StrVec(const StrVec& s)
{
	auto ret = alloc_n_copy(s.begin(), s.end());

	elements = ret.first;
	first_free = ret.second;
	cap = ret.second;
}

StrVec& StrVec::operator= (const StrVec& s)
{
	// ПРИМ: в случае присвоения самого себя, копирование эл-тов в новую область перед освобождением
	auto ret = alloc_n_copy(s.begin(), s.end()); 

	free();	// освободить старые эл-ты

	elements = ret.first;
	first_free = ret.second;
	cap = ret.second;

	return *this;
}

StrVec::~StrVec()
{
	free();
}


void StrVec::push_back(const std::string& s)
{
	std::cout << "StrVec::push_back called" << std::endl;

	check_n_alloc();
	alloc.construct(first_free++, s);	// создать эл-т в выделенной памяти, сместить указатель за вновь созданный эл-т
}







