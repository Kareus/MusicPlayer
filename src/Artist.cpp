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

int Artist::ReadDataFromFile(ifstream& fin)
{
	if (!fin) return 0;

	getline(fin, name);

	int musicLen;
	fin >> musicLen;
	fin.ignore();

	musicList.MakeEmpty();

	for (int i = 0; i < musicLen; i++)
	{
		string id, name;

		getline(fin, id);
		getline(fin, name);

		SimpleMusicType music;
		music.SetID(id);
		music.SetName(name);

		musicList.Add(music);
	}

	SetID(name);

	return 1;
}

int Artist::WriteDataToFile(ofstream& fout)
{
	if (!fout) return 0;

	fout << name << endl;
	fout << musicList.GetLength() << endl;

	SimpleMusicType* music;
	DoublyIterator<SimpleMusicType> iter(musicList);

	while (iter.NotNull())
	{
		music = iter.CurrentPtr();
		fout << music->GetID() << endl;
		fout << music->GetName() << endl;
		iter.Next();
	}

	return 1;
}

DoublyIterator<SimpleMusicType>& Artist::GetIterator() const
{
	DoublyIterator<SimpleMusicType> iter(musicList);
	return iter;
}