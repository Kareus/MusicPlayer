#include "Album.h"

Album::Album()
{
	album = "";
	artist = "";
	record = "";
	date = 0;
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
	record = data.GetRecord();
	date = data.GetDate();
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

string Album::GetRecord() const
{
	return record;
}

unsigned int Album::GetMusicNum() const
{
	return list.GetLength();
}

string Album::GetID() const
{
	return ID;
}

unsigned int Album::GetDate() const
{
	return date;
}

void Album::SetAlbumName(const string& name)
{
	album = name;
}

void Album::SetArtist(const string& artist)
{
	this->artist = artist;
}

void Album::SetRecord(const string& record)
{
	this->record = record;
}

void Album::SetID(const string& id)
{
	ID = id;
}

void Album::SetDate(unsigned int date)
{
	this->date = date;
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
	record = data.GetRecord();
	list = data.list;
	ID = data.GetID();
	date = data.GetDate();

	return *this;
}

void Album::SetIDFromKB()
{
	cout << "\tAlbum ID : ";
	string id;
	getline(cin, id);
	SetID(id);
}

void Album::SetAlbumNameFromKB()
{
	cout << "\tAlbum Name : ";
	string name;
	getline(cin, name);
	SetAlbumName(name);
}

void Album::SetArtistFromKB()
{
	cout << "\tAlbum Artist : ";
	string artist;
	getline(cin, artist);
	SetArtist(artist);
}

void Album::SetRecordFromKB()
{
	cout << "\tAlbum Record : ";
	string record;
	getline(cin, record);
	SetRecord(record);
}

void Album::SetDateFromKB()
{
	cout << "\tAlbum Release Date (YYYYMMDD) : ";
	cin >> date;

	while (cin.fail())
	{
		cout << "\tYou should input integer value" << endl;
		cin.clear();
		cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
		cout << "\tAlbum Release Date (YYYYMMDD) : ";
		cin >> date;
	}
	cin.ignore();
}

void Album::DisplayIDOnScreen()
{
	cout << "\tAlbum ID : " << ID << endl;
}

void Album::DisplayAlbumNameOnScreen()
{
	cout << "\tAlbum Name : " << album << endl;
}

void Album::DisplayArtistOnScreen()
{
	cout << "\tAlbum Artist : " << artist << endl;
}

void Album::DisplayRecordOnScreen()
{
	cout << "\tAlbum Record : " << record << endl;
}

void Album::DisplayDateOnScreen()
{
	unsigned int m, d;

	m = date % 10000 / 100;
	d = date % 100;
	cout << "\tRelease Date : " << date / 10000 << '.';

	if (m == 0)
	{
		cout << endl;
		return; //월 정보가 없으면 끝냄
	}

	if (m < 10) cout << 0;
	cout << m << '.';

	if (d == 0)
	{
		cout << endl;
		return; //일 정보가 없으면 끝냄
	}
	if (d < 10) cout << 0;
	cout << d << endl;
}

void Album::DisplayAllOnScreen()
{
	DisplayIDOnScreen();
	DisplayAlbumNameOnScreen();
	DisplayArtistOnScreen();
	DisplayRecordOnScreen();
	DisplayDateOnScreen();

	cout << "\tThe number of musics: " << list.GetLength() << endl;

	SimpleMusicType temp;
	list.ResetList();

	cout << endl << "\t---------------" << endl;

	int count = 0;
	while (list.GetNextItem(temp))
	{
		count++;
		cout << "\tMusic No. " << count << endl;
		temp.DisplayIDOnScreen();
		temp.DisplayNameOnScreen();
		temp.DisplayLengthOnScreen();
		cout << "\t---------------" << endl;
	}
}

int Album::ReadDataFromFile(ifstream& fin)
{
	if (!fin) return 0;
	getline(fin, album);
	getline(fin, artist);
	getline(fin, record);
	fin >> date;
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
	fout << record << endl;
	fout << date << endl;
	fout << list.GetLength() << endl;

	list.ResetList();
	SimpleMusicType music;

	while (list.GetNextItem(music))
	{
		fout << music.GetID() << endl;
		fout << music.GetName() << endl;
		fout << music.GetLength() << endl;
	}
	return 1;
}