#include "PlayList.h"

PlayList::PlayList()
{
	list.MakeEmpty();
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

PlayList& PlayList::operator=(const PlayList& data)
{
	list = data.list;
	return *this;
}