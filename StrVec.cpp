
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

#if 0
	auto dest = newdata;	// указывает на следующую свободную позицию в новом массиве
	auto elem = elements;	// указывает на следующий эл-т в старом массиве

	for(size_t i = 0; i != size(); ++i){
		// ПРИМ: std::move() вызывает конструктор ПЕРЕМЕЩЕНИЯ (вместо копирования) объекта (строки в данном случае)
		std::cout << " moving: " << *elem << " ...";
		alloc.construct(dest++, std::move(*elem++));

		//std::cout << " after moving: " << *(elem-1) << std::endl; // ОШИБКА: после перемещения данные недоступны
		std::cout << " after moving: " << *(dest-1) << std::endl;
	}
#else
	// ЭКВИВАЛЕНТНО
	// использование итератора перемещения для копирования диапазона эл-тов в новое пространство
	// ПРИМ: алгоритм uninitialized_copy() вызывает ф-ию construct для каждого эл-та исходной последовательности
	// чтобы скопировать э-лты по назначению. Поскольку был передан итератора премещения make_move_iterator(),
	// оператор обращения к значению возвращает ссылку на r-value. Это означает, что construct() будет использовать
	// конструктор перемещения для создания эл-тов.
	auto dest = uninitialized_copy(make_move_iterator(begin()), make_move_iterator(end()), newdata);
#endif

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

// noexcept используется чтобы показать, что использовать конструктор перемещения безопасно и можно 
// вызывать его вместо конструктора копирования при необходимости реалокации данных вектора 
// (ведь вектор должен гарантировать сохранность исходных данных)
StrVec::StrVec(StrVec&& s) noexcept:
	elements(s.elements), first_free(s.first_free), cap(s.cap)
{
	// оставить s в состоянии, при котором вызов деструктора безопасен
	s.elements = s.first_free = s.cap = nullptr;

	// ПРИМ: после перемещения оригинальный объект (s) должен остаться корректным,  
	// допускающим удаление объектом, но для пользователей его значение непредсказуемо
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

StrVec& StrVec::operator= (StrVec&& rhs) noexcept
{
	// прямая проверка присвоения себя себе
	if(this != &rhs){
		// освободить существующие эл-ты
		free();
		// переместить rhs
		elements = rhs.elements;
		first_free = rhs.first_free;
		cap = rhs.cap;
		// оставить s в состоянии, при котором вызов деструктора безопасен
		rhs.elements = rhs.first_free = rhs.cap = nullptr;
	}

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

void StrVec::push_back(std::string&& s)
{
	std::cout << "StrVec::move push_back called" << std::endl;

	check_n_alloc();
	alloc.construct(first_free++, std::move(s));	// будет использован конструктор перемещения строки
}



// ПРИМ: Если у ф-ии-члена есть квалификатор ссылки, то у всех версий этой ф-ии с тем же список параметров
// должны быть квалификаторы ссылки.

// этот оюъект - r-value, поэтому его можно сортировать на месте
// Вполне безопасно сортировать вектор-член data непосредственно, т.к. объект является r-value, означая, 
// что у него нет никаких других пользователей, поэтому данный объект можно изменить непосредственно.
Foo Foo::sorted() &&
{
	std::cout << "sorted && called" << std::endl;
	std::sort(data.begin(), data.end());
	return *this;
}

// этот объект либо константа, либо l-value. его нельзя сортировать на месте
Foo Foo::sorted() const &
{

#if 1
	std::cout << "sorted const & called" << std::endl;

	Foo ret(*this);
	//return ret.sorted();	// вызывает рекусивно эту же ф-ию и зацикливается т.к. ret - l-value

	return Foo(*this).sorted();	// вызывает версию && для r-value, т.к Foo(*this) - r-value
#else
	Foo ret(*this);	// создать копию
	std::sort(ret.data.begin(), ret.data.end()); // отсортировать копию
	return ret;	// вернуть копию
#endif
}