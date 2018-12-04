#include "Album.h"

Album::Album()
{
	album = "";
	artist = "";
	list.MakeEmpty();
}

Album::~Album()
{
	list.MakeEmpty();
}

Album::Album(const Album& data)
{
	album = data.GetAlbumName();
	artist = data.GetArtist();
	list = data.list;
}

string Album::GetAlbumName() const
{
	return album;
}

string Album::GetArtist() const
{
	return artist;
}

unsigned int Album::GetMusicNum() const
{
	return list.GetLength();
}

string Album::GetID() const
{
	return ID;
}

void Album::SetAlbumName(const string& name)
{
	album = name;
}

void Album::SetArtist(const string& artist)
{
	this->artist = artist;
}

void Album::SetID(const string& id)
{
	ID = id;
}

int Album::AddMusic(const SimpleMusicType& data)
{
	return list.Add(data) > 0;
}

int Album::DeleteMusic(const SimpleMusicType& data)
{
	return list.Delete(data) > 0;
}

int Album::DeleteMusicFrom(const string& key)
{
	SimpleMusicType music;
	music.SetID(key);

	return list.Delete(music);
}

int Album::ReplaceMusic(const SimpleMusicType& data)
{
	return list.Replace(data);
}

DoublyIterator<SimpleMusicType>& Album::GetIterator() const
{
	DoublyIterator<SimpleMusicType> iter(list);
	return iter;
}

RelationType Album::Compare(const Album& data) const
{
	int compare = strcmp(ID.c_str(), data.GetID().c_str());

	if (compare) return compare < 0 ? LESS : GREATER;
	else return EQUAL;
}

bool Album::operator<(const Album& data) const
{
	return Compare(data) == LESS;
}

bool Album::operator>(const Album& data) const
{
	return Compare(data) == GREATER;
}

bool Album::operator==(const Album& data) const
{
	return Compare(data) == EQUAL;
}

Album& Album::operator=(const Album& data)
{
	album = data.GetAlbumName();
	artist = data.GetArtist();
	list = data.list;
	ID = data.GetID();

	return *this;
}

int Album::ReadDataFromFile(ifstream& fin)
{
	if (!fin) return 0;
	getline(fin, album);
	getline(fin, artist);
	fin.ignore();
	
	int musicLen;
	fin >> musicLen;
	fin.ignore();

	for (int i = 0; i < musicLen; i++)
	{
		string id, name;
		unsigned int length;
		getline(fin, id);
		getline(fin, name);
		fin >> length;
		fin.ignore();

		SimpleMusicType music;
		music.SetID(id);
		music.SetName(name);
		music.SetLength(length);

		list.Add(music);
	}

	SetID(album + '_' + artist);
	return 1;
}

int Album::WriteDataToFile(ofstream& fout)
{
	if (!fout) return 0;
	fout << album << endl;
	fout << artist << endl;
	fout << list.GetLength() << endl;

	SimpleMusicType* music;
	DoublyIterator<SimpleMusicType> iter(list);

	while (iter.NotNull())
	{
		music = iter.CurrentPtr();
		fout << music->GetID() << endl;
		fout << music->GetName() << endl;
		fout << music->GetLength() << endl;
		iter.Next();
	}
	return 1;
}