#include "Artist.h"
#include "DoublyIterator.h"

Artist::Artist()
{
	name = "";
	musicList.MakeEmpty();
}

Artist::~Artist()
{
	musicList.MakeEmpty();
}

Artist::Artist(const Artist& data)
{
	name = data.GetName();
	musicList = data.musicList;
}

string Artist::GetName() const
{
	return name;
}

unsigned int Artist::GetMusicNum() const
{
	return musicList.GetLength();
}

string Artist::GetID() const
{
	return ID;
}

void Artist::SetName(const string& name)
{
	this->name = name;
}

void Artist::SetID(const string& id)
{
	ID = id;
}

int Artist::AddMusic(const SimpleMusicType& data)
{
	return musicList.Add(data);
}

int Artist::DeleteMusic(const SimpleMusicType& data)
{
	return musicList.Delete(data);
}

int Artist::ReplaceMusic(const SimpleMusicType& data)
{
	return musicList.Replace(data);
}

bool Artist::operator<(const Artist& data) const
{
	return strcmp(ID.c_str(), data.GetID().c_str()) < 0;
}

bool Artist::operator>(const Artist& data) const
{
	return strcmp(ID.c_str(), data.GetID().c_str()) > 0;
}

bool Artist::operator==(const Artist& data) const
{
	return strcmp(ID.c_str(), data.GetID().c_str()) == 0;
}

Artist& Artist::operator=(const Artist& data)
{
	name = data.GetName();
	musicList = data.musicList;
	ID = data.GetID();

	return *this;
}

DoublyIterator<SimpleMusicType>& Artist::GetIterator() const
{
	DoublyIterator<SimpleMusicType> iter(musicList);
	return iter;
}