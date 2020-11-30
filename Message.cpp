
#include "Message.h"


void Folder::add_msg(Message* m)
{
	messages.insert(m);
}


void Folder::remove_msg(Message* m)
{
	messages.erase(m);
}



void Message::save(Folder& f)
{
	folders.insert(&f);
	f.add_msg(this);
}


void Message::remove(Folder& f)
{
	folders.erase(&f);
	f.remove_msg(this);
}

// Добавить это сообщение в папки, на которые указывает m
void Message::add_to_folders(const Message& m)
{
	for(auto f : m.folders) f->add_msg(this);
}

Message::Message(const Message& m):
	content(m.content), folders(m.folders)
{
	add_to_folders(m);
}

// Удалить это сообщение из всех его папок
void Message::remove_from_folders()
{
	for(auto f : folders) f->remove_msg(this);
}

Message::~Message()
{
	remove_from_folders();
}



Message& Message::operator= (const Message& m)
{
	remove_from_folders();

	content = m.content;
	folders = m.folders;

	add_to_folders(m);

	return *this;
}