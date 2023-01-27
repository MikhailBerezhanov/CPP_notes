#include <iostream>
#include <string>
#include <memory>

using namespace std;

// Посредник — это поведенческий паттерн проектирования, который позволяет 
// уменьшить связанность множества классов между собой, благодаря перемещению 
// этих связей в один класс-посредник.

// Применимость

// 1. Когда вам сложно менять некоторые классы из-за того, что они имеют множество 
// хаотичных связей с другими классами.
//
//  Посредник позволяет поместить все эти связи в один класс, после чего вам будет
//  легче их отрефакторить, сделать более понятными и гибкими.

// 2. Когда вы не можете повторно использовать класс, поскольку он зависит от уймы 
// других классов.
//
//  После применения паттерна компоненты теряют прежние связи с другими компонентами, 
//  а всё их общение происходит косвенно, через объект-посредник.

// 3. Когда вам приходится создавать множество подклассов компонентов, чтобы 
// использовать одни и те же компоненты в разных контекстах.
//
//  Если раньше изменение отношений в одном компоненте могли повлечь за собой лавину 
//  изменений во всех остальных компонентах, то теперь вам достаточно создать подкласс 
//  посредника и поменять в нём связи между компонентами.

struct IMediator;

class Component
{
public:
	Component(IMediator* mediator): m_mediator(mediator) {}
	virtual ~Component() = default;

	virtual void on_click();
	virtual void keypress();

protected:
	IMediator* m_mediator;
};

// Общий интерфейс посредников.
struct IMediator
{
	virtual ~IMediator() = default;

	virtual void notify(const Component* component, const std::string& event) = 0;
};

void Component::on_click()
{
	m_mediator->notify(this, "on_click");
}

void Component::keypress()
{
	m_mediator->notify(this, "keypress");
}

struct Button : public Component 
{
	using Component::Component;
};

struct Textbox : public Component 
{
	using Component::Component;
};

struct Checkbox : public Component 
{
	using Component::Component;

	void check()
	{
		m_mediator->notify(this, "check");
	}

	bool checked()
	{
		return false;
	}
};

// Конкретный посредник. Все связи между конкретными
// компонентами переехали в код посредника. Он получает
// извещения от своих компонентов и знает, как на них
// реагировать.
class DialogMediator : public IMediator
{
public:
	DialogMediator()	
	{
		// construct and setup all components
	}

	// Когда что-то случается с компонентом, он шлёт посреднику
    // оповещение. После получения извещения посредник может
    // либо сделать что-то самостоятельно, либо перенаправить
    // запрос другому компоненту.
	void notify(const Component* component, const std::string& event) override
	{
		if (component == &loginOrRegisterCheckBox && event == "check")
		{
			if (loginOrRegisterCheckBox.checked())
			{
				title = "Log in";
			}
			else
			{
				title = "Register";
			}
		}

		if (component == &okBtn && event == "click")
		{
			if (loginOrRegisterCheckBox.checked())
			{
				//...
			}
			else
			{
				//...
			}
		}
	}

private:
	std::string title = "Dialog";

	Checkbox loginOrRegisterCheckBox{this};

	Textbox loginUserName{this};
	Textbox loginPassword{this};
	Textbox registrationUserName{this};
	Textbox registrationPassword{this};
	Textbox registrationEmail{this};

	Button okBtn{this};
	Button cancelBtn{this};
};



void ClientCode()
{
	DialogMediator dialog;
}

int main()
{
	ClientCode();
	
	return 0;
}