#include <iostream>
#include <memory>
#include <string>
#include <algorithm>

using namespace std;

// Адаптер — это структурный паттерн проектирования, который 
// позволяет объектам с несовместимыми интерфейсами работать вместе.


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
	std::cout << "Client: But I can work with it via the Adapter:\n";
	auto adapter = make_unique<Adapter1>(adaptee.get());
	ClientCode(*adapter);
	std::cout << "\n";

	return 0;
}