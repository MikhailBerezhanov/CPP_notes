#include <iostream>
#include <stack>
#include <memory>
#include <string>

using namespace std;

// Команда — это поведенческий паттерн проектирования, который 
// превращает запросы в объекты, позволяя передавать их как аргументы 
// при вызове методов, ставить запросы в очередь, логировать их, 
// а также поддерживать отмену операций.

// Применимость

// 1. Когда вы хотите параметризовать объекты выполняемым действием.
//
//  Команда превращает операции в объекты. А объекты можно передавать, 
//  хранить и взаимозаменять внутри других объектов.

// 2. Когда вы хотите ставить операции в очередь, 
// выполнять их по расписанию или передавать по сети.
//
//  Как и любые другие объекты, команды можно сериализовать, то есть 
//  превратить в строку, чтобы потом сохранить в файл или базу данных. 
//  Затем в любой удобный момент её можно достать обратно, снова превратить 
//  в объект команды и выполнить. Таким же образом команды можно передавать 
//  по сети, логировать или выполнять на удалённом сервере.

// 3. Когда вам нужна операция отмены.
//
//  Главная вещь, которая вам нужна, чтобы иметь возможность отмены операций, 
//  — это хранение истории.


struct Application;

struct CopyCommand;
struct CutCommand;
struct PasteCommand;
struct UndoCommand;

// Класс редактора содержит непосредственные операции над
// текстом. Он отыгрывает роль получателя — команды делегируют
// ему свои действия.
struct Editor
{
	// Вернуть выбранный текст.
	std::string GetSelection()
	{
		return "abcd";
	}

	// Удалить выбранный текст.
	void DeleteSelection(){}

	// Вставить текст из буфера обмена в текущей позиции.
	void ReplaceSelection(const std::string& text){}

	std::string text;
};

// Абстрактная команда задаёт общий интерфейс для конкретных
// классов команд и содержит базовое поведение отмены операции.
class Command
{
public:
	Command(Application* app, const std::shared_ptr<Editor>& editor)
		: m_app(app), m_editor(editor) {}

	virtual ~Command() = default;

	// Главный метод команды остаётся абстрактным, чтобы каждая
    // конкретная команда определила его по-своему. Метод должен
    // возвратить true или false в зависимости о того, изменила
    // ли команда состояние редактора, а значит, нужно ли её
    // сохранить в истории.
	virtual bool Execute() = 0;

	void SaveBackup()
	{
		m_backupText = m_editor->text;
	}

	void Undo()
	{
		m_editor->text = m_backupText;
	}

protected:
	std::string m_backupText;
	std::shared_ptr<Editor> m_editor;
	Application* const m_app = nullptr;
};


class CommandHistory
{
public:
	void push(const std::shared_ptr<Command>& command)
	{
		m_stack.push(command);
	}

	std::shared_ptr<Command> pop()
	{
		if (m_stack.empty())
		{
			return nullptr;
		}

		auto ret = m_stack.top();
		m_stack.pop();

		return ret;
	}

private:
	std::stack<std::shared_ptr<Command>> m_stack;
};



// Класс приложения настраивает объекты для совместной работы.
// Он выступает в роли отправителя — создаёт команды, чтобы
// выполнить какие-то действия.
struct Application
{
	std::string clipboard;
	std::shared_ptr<Editor> editor;
	CommandHistory history;

	// Запускаем команду и проверяем, надо ли добавить её в историю.
	void ExecuteCommand(const std::shared_ptr<Command>& command)
	{
		if (command->Execute())
		{
			history.push(command);
		}
	}

	// Берём последнюю команду из истории и заставляем её все отменить.
	void Undo()
	{
		if (auto command = history.pop(); command)
		{
			command->Undo();
		}
	}

	void CreateUI()
	{
		editor = std::make_shared<Editor>();


		shared_ptr<Command> copyCmd = std::make_shared<CopyCommand>(this, editor);

		// auto copyCmd = shared_ptr<Command>(new CopyCommand(this, editor));
		// auto cutCmd = std::make_shared<CutCommand>(this, editor);
		// auto pasteCmd = std::make_shared<PasteCommand>(this, editor);
		// auto undoCmd = std::make_shared<UndoCommand>(this, editor);

		// auto copy = [this, cmd = copyCmd](){
		// 	ExecuteCommand(cmd);
		// };
	}
};


struct CopyCommand : public Command
{
	using Command::Command;

	bool Execute() override
	{
		m_app->clipboard = m_editor->GetSelection();
		return false;	// No changes
	}
};

struct CutCommand : public Command
{
	using Command::Command;

	bool Execute() override
	{
		SaveBackup();
		m_app->clipboard = m_editor->GetSelection();
		m_editor->DeleteSelection();
		return true;	// Changes took place
	}
};

struct PasteCommand : public Command
{
	using Command::Command;

	bool Execute() override
	{
		SaveBackup();
		m_editor->ReplaceSelection(m_app->clipboard);
		return true;
	}
};

struct UndoCommand : public Command
{
	using Command::Command;

	bool Execute() override
	{
		m_app->Undo();
		return false;
	}
};


int main()
{
	return 0;
}