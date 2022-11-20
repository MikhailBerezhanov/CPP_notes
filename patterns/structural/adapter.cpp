#include <iostream>
#include <memory>
#include <string>
#include <algorithm>

using namespace std;

// Адаптер — это структурный паттерн проектирования, который 
// позволяет объектам с несовместимыми интерфейсами работать вместе.

// Применимость

// 1. Когда вы хотите использовать сторонний класс, но его интерфейс 
// не соответствует остальному коду приложения.
//
// Адаптер позволяет создать объект-прокладку, который будет превращать 
// вызовы приложения в формат, понятный стороннему классу.

// 2. Когда вам нужно использовать несколько существующих подклассов, 
// но в них не хватает какой-то общей функциональности, причём расширить 
// суперкласс вы не можете.
//
// Вы могли бы создать ещё один уровень подклассов и добавить в них недостающую 
// функциональность. Но при этом придётся дублировать один и тот же код в обеих 
// ветках подклассов.


struct Target
{
	// Supported interface
	virtual string Request(int value)
	{
		return "NormalMethod";
	}
};

// Adaptee
struct ServiceWithUnsupportedInterface
{
	string SpecialRequest()
	{
		return "dohteMlaicepS";
	}
};

// Var.1 - member object
class Adapter1 : public Target
{
public:
	Adapter1(ServiceWithUnsupportedInterface *service): m_service(service) {}

	string Request(int value) override
	{
		return ToTargetRequest();
	}

private:
	ServiceWithUnsupportedInterface *m_service;

	string ToTargetRequest()
	{
		string s = m_service->SpecialRequest();

		std::reverse(s.begin(), s.end());

		return s;
	}
};

// Var.2 - multi inheritance
class Adapter2 : public Target, public ServiceWithUnsupportedInterface
{
public:

	string Request(int value) override
	{
		string s = ServiceWithUnsupportedInterface::SpecialRequest();

		std::reverse(s.begin(), s.end());

		return s;
	}
};




void ClientCode(Target &target)
{
	cout << target.Request(50);
}

int main()
{
	std::cout << "Client: I can work just fine with the Target objects:\n";
	auto target = make_unique<Target>();
	ClientCode(*target);
	std::cout << "\n\n";
	auto adaptee = make_unique<ServiceWithUnsupportedInterface>();
	std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
	std::cout << "Adaptee: " << adaptee->SpecialRequest();
	std::cout << "\n\n";

	std::cout << "Client: But I can work with it via the Adapter1:\n";
	auto adapter = make_unique<Adapter1>(adaptee.get());
	ClientCode(*adapter);
	std::cout << "\n";

	std::cout << "Client: But I can work with it via the Adapter2:\n";
	auto adapter2 = make_unique<Adapter2>();
	ClientCode(*adapter2);
	std::cout << "\n";

	return 0;
}