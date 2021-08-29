
#include <string>
#include <iostream>
#include <vector>
#include <memory>

// Объекты класса совместно используют вектор в динамической памяти, 
// вместо предоставления собственного вектора каждому объекту
class Str_blob
{
public:
	typedef std::vector<std::string>::size_type size_type;

	// Конструкторы
	Str_blob();
	Str_blob(std::initializer_list<std::string> il);

	// информация о состоянии общего контейнера
	size_type size() const { return data->size(); }
	bool empty() const { return data->empty(); }

	// добавление и удаление эл-тов
	void push_back(const std::string &s) const { data->push_back(s); }
	void pop_back() const;

	// доступ к эл-там
	std::string& front() const;
	std::string& back() const;

	int use_count() const { return data.use_count(); }
	bool unique() const { return data.unique(); }

private:
	std::shared_ptr<std::vector<std::string>> data;
	// проверка допустимости индекса в векторе (передает сообщение о недопустимости)
	void check(size_type i, const std::string &msg) const;
};

// Константа будет добавлена во все файлы, включающая данный хедер как static
const int str_blob_test = 10;




// --- Шаблонная версия ---
// При каждом создании экземпляра шаблона класса получается независимый класс.
// Blob<int> bi; создаст класс:
// template<> class Blob<int> {...};
//
// ПРИМ: по умолчанию экземпляр ф-ии-члена шаблона класса создается ТОЛЬКО если программа использует эту ф-ию.
// ПРИМ: в области видимости шаблона класса компилятор рассматривает ссылки на сам шаблон так, как буедто были 
// подставлены аргументы шаблона (Blob& и Blob<T>& эквивалентны).В области видимости шаблона класса можно 
// обращаться к шаблону, не определяя его аргументы
//
// Дружественные отношения:
// 1. Шаблон класса, у которого есть друг, не являющийся шаблоном, предоставляет дружественный доступ ко всем 
// 	  экземплярам шаблона.
// 2. Когда друг сам является шаблоном, предоставляющий дружественные отношения класс контролирует на какие 
//	  экземпляры распросраняется дружба.

// ПРИМ: При объявлении шаблона класса создаются экземпляры всех его членов!
template<typename T> class BlobPtr;	// объявление шаблона класса, НЕ определение


template<typename T> class Blob
{
public:
	typedef T value_type;
	// ПРИМ: по умолчанию доступ к члену через область видимости :: осуществляет доступ к переменной, 
	// а не к типу. Чтобы обратиться к ТИПУ нужно явно писать typename
	typedef typename std::vector<T>::size_type size_type;

	// Дружба "один к одному"
	friend class BlobPtr<T>;	// дружественный доступ предоставляется экзаемплярам с тем же типом шаблона

	// Конструкторы
	Blob();
	Blob(std::initializer_list<T> il);

	// информация о состоянии общего контейнера
	size_type size() const { return data->size(); }
	bool empty() const { return data->empty(); }

	// добавление и удаление эл-тов
	void push_back(const T& s) const { data->push_back(s); }
	// версия перемещения
	void push_back(T&& s) { data->push_back(std::move(s)); }
	void pop_back() const;

	// доступ к эл-там
	T& front() const;
	T& back() const;
	T& operator[] (size_type i) { check(i, "out of Blob index"); return (*data)[i]; }

	int use_count() const { return data.use_count(); }
	bool unique() const { return data.unique(); }

	// член-шаблон (конструктор из эл-тов различных контейнеров)
	template<typename It> Blob(It b, It e);	

private:
	std::shared_ptr<std::vector<T>> data;
	// проверка допустимости индекса в векторе (передает сообщение о недопустимости)
	void check(size_type i, const std::string &msg) const;

	// Для любого конкретного типа Т будет по одной переменной example
	// как и любой другой член шаблона создается только при применении в программе
	static int example;
};



template <typename T> class Pal;

class C // обычный нешаблонный класс
{
	friend class Pal<C>; // Экземпляр Pal создается с классом С как дружественным

	// Все экземпляры Pal2 дружественны С. При предоставлении дружественных отношений всем экземплярам
	// предварительное объявление не обязательно
	template <typename T> friend class Pal2;


	// внутри класса также может быть член-шаблон
	template <typename T> void operator() (T* p) const { delete p; }
	// C tmp;
	// doudle* p = new double;
	// unique_ptr<int, C> p(new int, C());	-  создает экземпляр C::operator()<int> (int* p)
	// tmp(p); - создает экземпляр оператора вызова ф-ии C::operator()<double> (dpuble* p) и освобождает память
};


template<typename T = int> class C2	// шаблонный класс  int - параметр по умолчанию
{
	// у каждого экземпляра С2 есть тот же экземпляр Pal, что и у друга
	friend class Pal<T>;

	// все экземпляры Pal2 - друзья каждого экземпляра С2
	template<typename X> friend class Pal2;

	// Pal3 - не щаблонный класс, являющийся другом каждого экземпляра С2
	friend class Pal3;
};