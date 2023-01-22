#include <iostream>
#include <memory>
#include <list>
#include <vector>
#include <string>

using namespace std;

// Наблюдатель — это поведенческий паттерн проектирования, который 
// создаёт механизм подписки, позволяющий одним объектам следить и 
// реагировать на события, происходящие в других объектах.

// Применимость: Наблюдатель можно часто встретить в C++ коде, особенно там, 
// где применяется событийная модель отношений между компонентами. Наблюдатель 
// позволяет отдельным компонентам реагировать на события, происходящие в других 
// компонентах.

// Признаки применения паттерна: Наблюдатель можно определить по механизму 
// подписки и методам оповещения, которые вызывают компоненты программы.


struct ISubscriber
{
	virtual ~ISubscriber() = default;

	virtual void Update(const std::string& data) = 0;
};

struct IPublisher
{
	virtual ~IPublisher() = default;

	virtual void Subscribe(const std::shared_ptr<ISubscriber>& subscriber) = 0;
	virtual void Unsubscribe(const std::shared_ptr<ISubscriber>& subscriber) = 0;
	virtual void NotifyAllSubscribers() = 0;
};


class Publisher : public IPublisher
{
public:
	void Subscribe(const std::shared_ptr<ISubscriber>& subscriber) override
	{
		m_subscribers.push_back(subscriber);
	}

	void Unsubscribe(const std::shared_ptr<ISubscriber>& subscriber) override
	{

		m_subscribers.remove_if([&subscriber](std::weak_ptr<ISubscriber> wp)
		{
			// auto swp = wp.lock();

			// if(subscriber && swp)
			// {
			// 	return subscriber == swp;
			// }

			// return false;

			return !(subscriber.owner_before(wp) || wp.owner_before(subscriber));
		});
	}

	void NotifyAllSubscribers() override
	{
		cout << "Subscribers number:" << m_subscribers.size() << endl;

		auto it = m_subscribers.cbegin();

		while (it != m_subscribers.cend())
		{
			if (it->expired())
			{
				const auto expired = it;
				++it;
				m_subscribers.erase(expired);
			}
			else
			{
				it->lock()->Update(m_state);
				++it;
			}
		}
	}

	void BusinessLogic()
	{
		m_state = "new state";
		NotifyAllSubscribers();
	}

private:
	std::list<std::weak_ptr<ISubscriber>> m_subscribers;
	std::string m_state = "default state";	
};

class Subscriber : public ISubscriber
{
public:
	// Use Factory Method for object creation
	static std::shared_ptr<ISubscriber> Create(IPublisher& publisher)
	{
		// Can't use make_shared because Ctr is private
		auto res = std::shared_ptr<Subscriber>(new Subscriber());
		publisher.Subscribe(res);
		return res;
	}

	virtual ~Subscriber()
	{
		cout << "~Subscriber" << m_counter << endl;
	}

	void Update(const std::string& data) override
	{
		cout << "Subscriber" << m_counter << ": " << data << endl;
	}

private:
	Subscriber()
	{
		static uint Counter = 0;
		m_counter = ++Counter;
	}

	uint m_counter = 0;
};


void ClientCode()
{
	const auto publisher = std::make_unique<Publisher>();

	const auto sub1 = Subscriber::Create(*publisher);
	const auto sub2 = Subscriber::Create(*publisher);
	{
		const auto sub3 = Subscriber::Create(*publisher);
	}
	
	publisher->Subscribe(sub1);
	
	const auto sub4 = Subscriber::Create(*publisher);
	publisher->Unsubscribe(sub4);

	const auto sub5 = Subscriber::Create(*publisher);

	publisher->NotifyAllSubscribers();
	publisher->BusinessLogic();
}

int main()
{
	ClientCode();
	
	auto sp = make_shared<int>(10);
	auto sp2 = sp;

	weak_ptr<int> wp(sp);
	weak_ptr<int> wp2(sp);

	auto sp3 = make_shared<int>(20);

	cout << "wp.owner_before(sp): " << std::boolalpha << wp.owner_before(sp) << endl;
	cout << "wp.owner_before(wp2): " << std::boolalpha << wp.owner_before(wp2) << endl;
	cout << "wp2.owner_before(wp): " << std::boolalpha << wp2.owner_before(wp) << endl;
	cout << "wp2.owner_before(sp): " << std::boolalpha << wp2.owner_before(sp) << endl;
	cout << "sp.owner_before(wp): " << std::boolalpha << sp.owner_before(wp) << endl;
	cout << "sp.owner_before(wp2): " << std::boolalpha << sp.owner_before(wp2) << endl;
	cout << "sp.owner_before(sp2): " << std::boolalpha << sp.owner_before(sp2) << endl;
	cout << "sp2.owner_before(sp): " << std::boolalpha << sp2.owner_before(sp) << endl;

	cout << "sp3.owner_before(sp): " << std::boolalpha << sp3.owner_before(sp) << endl;

	return 0;
}