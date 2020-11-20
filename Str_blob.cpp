
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