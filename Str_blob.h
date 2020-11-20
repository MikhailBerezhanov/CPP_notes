
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