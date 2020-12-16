
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

template<typename T>
Blob<T>::Blob(): data(std::make_shared<std::vector<T>>()) {}

template<typename T>
Blob<T>::Blob(std::initializer_list<T> il): data(std::make_shared<std::vector<T>>(il)) {}

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