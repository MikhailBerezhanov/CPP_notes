
#include "Str_blob.h"

// Конструкторы

Str_blob::Str_blob(): data(std::make_shared<std::vector<std::string>>()) {}

Str_blob::Str_blob(std::initializer_list<std::string> il): data(std::make_shared<std::vector<std::string>>(il)) {}


void Str_blob::check(size_type i, const std::string &msg) const
{
	if(i >= data->size()) throw std::out_of_range(msg);
}


std::string& Str_blob::front() const
{
	check(0, "front on empty Str_blob");

	return data->front();
}

std::string& Str_blob::back() const
{
	check(0, "back on empty Str_blob");

	return data->back();
}

void Str_blob::pop_back() const
{
	check(0, "pop_back on empty Str_blob");

	return data->pop_back();
}



// --- Шаблонная версия ---
template<typename T>
int Blob<T>::example = 1;

// Исключение может происзойти во время инициализации. Единственный способ его обработать, добавить try
// в блок инициализации, а обработчик поместить после тела конструктора
template<typename T>
Blob<T>::Blob() try: data(std::make_shared<std::vector<T>>()) {}
catch(const std::bad_alloc& e){}	// обработчик связан с блоком try списка инициализации

template<typename T>
Blob<T>::Blob(std::initializer_list<T> il) try: data(std::make_shared<std::vector<T>>(il)) {}
catch(const std::bad_alloc& e){}

template<typename T>
void Blob<T>::check(size_type i, const std::string &msg) const
{
	if(i >= data->size()) throw std::out_of_range(msg);
}

template<typename T>
T& Blob<T>::front() const
{
	check(0, "front on empty Blob");

	return data->front();
}

template<typename T>
T& Blob<T>::back() const
{
	check(0, "back on empty Blob");

	return data->back();
}

template<typename T>
void Blob<T>::pop_back() const
{
	check(0, "pop_back on empty Blob");

	return data->pop_back();
}

// ПРИМ: при определении члена -шаблона-ф-ии вне тела шаблона класса следует предоставить список параметров 
// шаблона для шаблона класса и для шаблона функции
template<typename T>
template<typename It>
Blob<T>::Blob(It b, It e): data(std::make_shared<std::vector<T>>(b, e))
{

}

template class Blob<std::string>;	// Определение (создает ВСЕ члены шаблоны класса)