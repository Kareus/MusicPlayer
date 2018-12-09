#include "PlayList.h"

PlayList::PlayList()
{
	list.MakeEmpty();
	id = -1;
}

PlayList::~PlayList()
{
	list.MakeEmpty();
}

PlayList::PlayList(const PlayList& data)
{
	list = data.list;
}

int PlayList::AddMusic(const SimpleMusicType& data)
{
	return list.Add(data);
}

int PlayList::DeleteMusic(const SimpleMusicType& data)
{
	return list.Delete(data);
}

int PlayList::ReplaceMusic(const SimpleMusicType& data)
{
	return list.Replace(data);
}

int PlayList::GetMusic(SimpleMusicType& data)
{
	return list.Get(data);
}

int PlayList::GetMusicNum() const
{
	return list.GetLength();
}

DoublyIterator<SimpleMusicType> PlayList::GetIterator()
{
	return DoublyIterator<SimpleMusicType>(list);
}

PlayList& PlayList::operator=(const PlayList& data)
{
	list = data.list;
	return *this;
}

bool PlayList::operator<(const PlayList& data) const
{
	return id < data.GetID();
}

bool PlayList::operator>(const PlayList& data) const
{
	return id > data.GetID();
}

bool PlayList::operator==(const PlayList& data) const
{
	return id == data.GetID();
}

void PlayList::SetID(int id)
{
	this->id = id;
}

int PlayList::GetID() const
{
	return id;
}