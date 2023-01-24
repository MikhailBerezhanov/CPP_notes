#include <iostream>
#include <string>
#include <memory>

using namespace std;

// Итератор — это поведенческий паттерн проектирования, который даёт возможность 
// последовательно обходить элементы составных объектов, не раскрывая их 
// внутреннего представления.

// Применимость

// 1. Когда у вас есть сложная структура данных, и вы хотите скрыть от клиента
// детали её реализации (из-за сложности или вопросов безопасности).
// 
//  Итератор предоставляет клиенту всего несколько простых методов перебора 
//  элементов коллекции. Это не только упрощает доступ к коллекции, но и защищает 
//  её данные от неосторожных или злоумышленных действий.

// 2. Когда вам нужно иметь несколько вариантов обхода одной и той же структуры данных.
//
//  Нетривиальные алгоритмы обхода структуры данных могут иметь довольно объёмный код. 
//  Этот код будет захламлять всё вокруг — будь то сам класс коллекции или часть
//  бизнес-логики программы. Применив итератор, вы можете выделить код обхода структуры 
//  данных в собственный класс, упростив поддержку остального кода.

// 3. Когда вам хочется иметь единый интерфейс обхода различных структур данных.
//
//  Итератор позволяет вынести реализации различных вариантов обхода в подклассы. 
//  Это позволит легко взаимозаменять объекты итераторов, в зависимости от того, 
//  с какой структурой данных приходится работать.


struct Profile
{
	int id = 0;
	std::string email;
};

// Iterator Interface
struct IProfileIterator
{
	virtual ~IProfileIterator() = default;

	virtual Profile& GetNext() = 0;
	virtual bool HasMore() = 0;
};

// Container Interface
struct ISocialNetwork
{
	virtual ~ISocialNetwork() = default;

	// Provide supported iterators types
	IProfileIterator& CreateFriendsIterator() = 0;
	IProfileIterator& CreateCoworkersIterator() = 0;
};


class FriendsIterator : public IProfileIterator
{
public:
	FriendsIterator(const std::vector<Profile>* profiles): m_profiles(profiles) {}

	virtual Profile& GetNext() override
	{

	}

	virtual bool HasMore() override
	{
		
	}

private:

	std::vector<Profile>* m_profiles;
	size_t m_pos = 0;
};

// Custom container
class Facebook : public ISocialNetwork
{

	IProfileIterator& CreateFriendsIterator()
	{

	}

	IProfileIterator& CreateCoworkersIterator()
	{

	}
};








void ClientCode()
{
	
}

int main()
{
	ClientCode();

	return 0;
}