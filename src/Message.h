#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
#include <iostream>
#include <set>

#define MSG_NAME	 	"[ Message ] "

class Folder;
// Приложение обработки почты

// Сообщение . Каждый объект хранит набор папок, в которых оно присутствует
class Message
{
friend class Folder;

public:
	Message(const std::string& s = std::string()):
		content(s) {}	// folders инициализируется неявно пустым 

	// копирование скопирует содержимое и набор указателеей на папку
	Message(const Message&);

	// Перемещение
	Message(Message&&);

	// деструктор должен удалять заданное сообщение из папок
	~Message();

	// присвоение присваивает другому объекту содержимое сообщения и модифицирует набор папок, удаляя левый операнд и добавляя правый
	Message& operator= (const Message&);

	// присвоение пермещения
	Message& operator= (Message&&);

	// Добавить \ удалить это сообщение из набор папок
	void save(Folder&);
	void remove(Folder&);

private:
	std::string content;		// текст сообщения
	std::set<Folder*> folders;	// папки, содержащие это сообщение

	// Вспомогательные для конструкторов ф-ии

	// Добавить это сообщение в папки, на которые указывает m
	void add_to_folders(const Message&);

	//
	void remove_from_folders();

	void move_folders(Message* m);
};



// Папка. Каждое сообщение может находиться в нескольких папках. Каждый объект хранит набор указателей на хранимые сообщения
class Folder
{
public:

	Folder(){}

	~Folder(){ messages.clear(); }

	void add_msg(Message*);
	void remove_msg(Message*);

private:

	std::set<Message*> messages;
};



#endif