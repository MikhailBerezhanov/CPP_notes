#ifndef _STR_VEC_H
#define _STR_VEC_H

#include <string>
#include <iostream>
#include <memory>	// allocator
#include <utility>	// pair, move
#include <memory>	// unitialized_copy
#include <algorithm>

// Самодельный вектор строк

class StrVec
{
public:
	StrVec(): elements(nullptr), first_free(nullptr), cap(nullptr) {} // аллокатор инициализируется по-умолчанию
	StrVec(std::initializer_list<std::string> il);

	StrVec(const StrVec&);

	// оператор присвоения копирования
	StrVec& operator= (const StrVec&);

	// ПРИМ: если класс сам определеяет конструктор копии, оператор присвоения копии или деструктор,
	// конструктор перемещения и присваивания при перемещении не синтезируется компилятором
	// конструктор перемещения

	// ПРИМ: если класс сам определеяет конструктор перемещения и (или) оператора присваивания при перемещении,
	// то синтезируемый конструктор копий и поератор присвоения копии для этого класса будут удаленными (=delete)

	// ПРИМ: если у класса будет пригодный конструктор копий и не будет конструктора перемещения, то объекты
	// будут перемещены ф-ей std::move() конструктором копий. То же справедливо и для оператора присвоения копии
	// и присвоения при перемещении

	StrVec(StrVec&&) noexcept;	// не будет передавать исключений
	// оператор присвоения перемещения
	StrVec& operator= (StrVec&&) noexcept;

	~StrVec();

	void push_back(const std::string&);	// версия использующая копирование
	void push_back(std::string&&);		// версия использующая перемещение

	size_t size() const { return first_free - elements; }
	size_t capacity() const { return cap - elements; }

	//reserve();
	//resize();

	std::string* begin() const { return elements; }
	std::string* end() const { return first_free; }

private:
	std::allocator<std::string> alloc;	// аллокатор для резервирования памяти для вектора
	std::string* elements = nullptr;		// указатель на первый эл-т в зарезервированной памяти
	std::string* first_free = nullptr;	// указатель на следующий эл-т после фактически последнего
	std::string* cap = nullptr;			// указатель на следующий эл-т после конца зарезервированной памяти


	// проверка зарезервированного ранее места
	void check_n_alloc();

	// Резервирует больше места и копирует существующие эл-ты
	void reallocate();

	// Удаляет эл-ты и освобождает пространство
	void free();

	// Вспомогательная для конструкторов ф-ия
	std::pair<std::string*, std::string*> alloc_n_copy (const std::string*, const std::string*);
};







class Foo
{
public:
	Foo sorted() &&;		// спецификатор &&, относящийся к this (ф-ия применима только к изменяемым r-value объектам)
	Foo sorted() const &;	// спецификатор const &, относящийся к this (ф-ия применима к любому объекту класса Foo)
private:
	StrVec data;
};












#endif