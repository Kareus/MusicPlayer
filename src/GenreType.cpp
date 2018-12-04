#include "GenreType.h"

GenreType::GenreType()
{
	genreName = "";
	list.MakeEmpty();
}

GenreType::GenreType(const GenreType& data)
{
	genreName = data.GetGenre();
	list = data.list;
}

GenreType::~GenreType()
{
	list.MakeEmpty();
}

string GenreType::GetGenre() const
{
	return genreName;
}

void GenreType::SetGenre(const string& name)
{
	genreName = name;
}

int GenreType::AddMusic(const SimpleMusicType& music)
{
	return list.Add(music);
}

int GenreType::DeleteMusic(const SimpleMusicType& music)
{
	return list.Delete(music);
}

int GenreType::ReplaceMusic(const SimpleMusicType& music)
{
	return list.Replace(music);
}

int GenreType::GetMusic(SimpleMusicType& music)
{
	return list.Get(music);
}

DoublyIterator<SimpleMusicType>& GenreType::GetIterator() const
{
	DoublyIterator<SimpleMusicType> iter(list);
	return iter;
}

bool GenreType::operator<(const GenreType& data) const
{
	return strcmp(genreName.c_str(), data.GetGenre().c_str()) < 0;
}

bool GenreType::operator>(const GenreType& data) const
{
	return strcmp(genreName.c_str(), data.GetGenre().c_str()) > 0;
}

bool GenreType::operator==(const GenreType& data) const
{
	return strcmp(genreName.c_str(), data.GetGenre().c_str()) == 0;
}

GenreType& GenreType::operator=(const GenreType& data)
{
	genreName = data.GetGenre();
	list = data.list;
	return *this;
}

int GenreType::GetMusicNum() const
{
	return list.GetLength();
}