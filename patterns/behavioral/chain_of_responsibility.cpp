#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

// Цепочка обязанностей — это поведенческий паттерн проектирования, 
// который позволяет передавать запросы последовательно по цепочке 
// обработчиков. Каждый последующий обработчик решает, может ли он 
// обработать запрос сам и стоит ли передавать запрос дальше по цепи.

// Применимость

// 1. Когда программа должна обрабатывать разнообразные запросы несколькими 
// способами, но заранее неизвестно, какие конкретно запросы будут 
// приходить и какие обработчики для них понадобятся.
//
// С помощью Цепочки обязанностей вы можете связать потенциальных 
// обработчиков в одну цепь и при получении запроса поочерёдно спрашивать 
// каждого из них, не хочет ли он обработать запрос.

// 2. Когда важно, чтобы обработчики выполнялись один за другим в строгом порядке.
//
// Цепочка обязанностей позволяет запускать обработчиков последовательно один 
// за другим в том порядке, в котором они находятся в цепочке.

// 3. Когда набор объектов, способных обработать запрос, должен 
// задаваться динамически.
//
// В любой момент вы можете вмешаться в существующую цепочку и переназначить
// связи так, чтобы убрать или добавить новое звено.

struct IHandler
{
	virtual ~IHandler() = default;

	virtual bool handle(const std::string& request) = 0;
	virtual const std::shared_ptr<IHandler>& SetNext(const std::shared_ptr<IHandler>&) = 0;
};

class BaseHandler : public IHandler
{
public:
	bool handle(const std::string& request) override
	{
		if (m_next)
		{
			return m_next->handle(request);
		}

		return true;
	}

	const std::shared_ptr<IHandler>& SetNext(const std::shared_ptr<IHandler>& next) override
	{
		m_next = next;
		return next;
	}

private:
	std::shared_ptr<IHandler> m_next;
};

class BananaHandler : public BaseHandler
{
public:
	BananaHandler(int id): m_id(id) {}

	bool handle(const std::string& request) override
	{
		if (request.find(std::to_string(m_id) + "banana") == std::string::npos)
		{
			// Если первая же проверку неудачна - прерыванием цепочку
			return false;
		}

		return BaseHandler::handle(request);
	}

private:
	int m_id = 0;
};

class AppleHandler : public BaseHandler
{
public:
	bool handle(const std::string& request) override
	{
		if (request.find("apple") == std::string::npos)
		{
			return false;
		}

		return BaseHandler::handle(request);
	}
};

class OrangeHandler : public BaseHandler
{
public:
	bool handle(const std::string& request) override
	{
		if (request.find("orange") == std::string::npos)
		{
			return false;
		}

		return BaseHandler::handle(request);
	}
};


void ClientCode(IHandler& chain_of_responsibility)
{
	std::vector<std::string> requests{	"orange juice",
									  	"apple orange  juice",
									  	"99banana apple orange"
										};
	for (const auto& req : requests)
	{
		if (!chain_of_responsibility.handle(req))
		{
			cout << "request handdle failed: " << req << endl;
		}
		else
		{
			cout << "request handle succeed: " << req << endl;
		}

	}

}

int main()
{
	auto h1 = std::make_shared<BananaHandler>(99);
	auto h2 = std::make_shared<AppleHandler>();
	auto h3 = std::make_shared<OrangeHandler>();

	// Chain
	h1->SetNext(h2)->SetNext(h3);

	ClientCode(*h1);
	ClientCode(*h2);	// Can be proceed not from beginning

	return 0;
}