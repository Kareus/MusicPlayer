#include "PlayList.h"
#include "GlobalFunctions.h"

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
	id = data.GetID();
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
	id = data.GetID();
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

int PlayList::ReadDataFromFile(ifstream& fin)
{
	if (!fin) return 0;

	int len = 0;
	fin >> id;
	fin >> len;
	fin.ignore();

	for (int i = 0; i < len; i++)
	{
		SimpleMusicType simple;
		string data;
		getline(fin, data);
		simple.SetID(data);
		getline(fin, data);
		simple.SetPath(String::StrToWstr(data));
		list.Add(simple);
	}

	return 1;
};

int PlayList::WriteDataToFile(ofstream& fout)
{
	if (!fout) return 0;

	fout << id << ' ';
	fout << list.GetLength() << endl;

	DoublyIterator<SimpleMusicType> iter(list);
	while (iter.NotNull())
	{
		fout << iter.CurrentPtr()->GetID() << endl;
		fout << String::WstrToStr(iter.CurrentPtr()->GetPath()) << endl;
		iter.Next();
	}

	return 1;
}