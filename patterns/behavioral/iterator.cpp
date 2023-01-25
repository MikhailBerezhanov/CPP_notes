#include <iostream>
#include <string>
#include <memory>
#include <vector>

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
	virtual std::unique_ptr<IProfileIterator> CreateFriendsIterator() = 0;
	virtual std::unique_ptr<IProfileIterator> CreateCoworkersIterator() = 0;
};

// Concrete iteratos for custom container 
class FriendsIterator : public IProfileIterator
{
public:
	FriendsIterator(std::vector<Profile>& profiles): m_profiles(profiles) {}

	// Each iterator implemets traversal methods (based on container specification)
	virtual Profile& GetNext() override
	{
		return m_profiles[m_pos++];
	}

	virtual bool HasMore() override
	{
		return m_pos < m_profiles.size();
	}

private:
	std::vector<Profile>& m_profiles;
	size_t m_pos = 0;
};

class CoworkersIterator : public IProfileIterator
{
public:
	CoworkersIterator(std::vector<Profile>& profiles): m_profiles(profiles) {}

	virtual Profile& GetNext() override
	{
		Profile& res = m_profiles[m_pos];
		m_pos += 2;

		return res;
	}

	virtual bool HasMore() override
	{
		return m_pos < m_profiles.size();
	}

private:
	std::vector<Profile>& m_profiles;
	size_t m_pos = 0;
};

// Custom container
class Facebook : public ISocialNetwork
{
public:

	Facebook(std::initializer_list<Profile> ilist): m_profilesData(ilist) {}
	Facebook(std::vector<Profile> vec): m_profilesData(std::move(vec)) {}

	std::unique_ptr<IProfileIterator> CreateFriendsIterator() override
	{
		return std::make_unique<FriendsIterator>(m_profilesData);
	}

	std::unique_ptr<IProfileIterator> CreateCoworkersIterator() override
	{
		return std::make_unique<CoworkersIterator>(m_profilesData);
	}

private:
	std::vector<Profile> m_profilesData;
};



class Spammer
{
public:
	void Send(const std::unique_ptr<IProfileIterator>& iter, const std::string& msg)
	{
		while (iter->HasMore())
		{
			Profile& profile = iter->GetNext();
			SendMail(profile.email, msg);
		}
	}

private:
	void SendMail(const std::string& addr, const std::string& msg)
	{
		cout << "sending email to '" << addr << "'" << " : " << msg << endl;
	}
};

// Client knows nothing about container and iterator impelemtation, 
// but can traverse it
void ClientCode(ISocialNetwork& network)
{
	Spammer spam;

	cout << "---- Spamming to all friends -----" << endl;
	spam.Send(network.CreateFriendsIterator(), "hello friend");

	cout << "---- Spamming to all coworkers -----" << endl;
	spam.Send(network.CreateCoworkersIterator(), "hello coworker");
}

int main()
{
	std::vector<Profile> vec;
	for (int i = 0; i < 52; ++i)
	{
		vec.push_back({.id = i, .email = std::to_string(i) + "@mail.abc"});
	}

	Facebook facebook(std::move(vec));

	ClientCode(facebook);

	return 0;
}