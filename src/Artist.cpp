#include "Artist.h"

Artist::Artist()
{
	name = "";
	birthDate = 0;
	albumList.MakeEmpty();
}

Artist::~Artist()
{
	albumList.MakeEmpty();
}

Artist::Artist(const Artist& data)
{
	name = data.GetName();
	birthDate = data.GetBirthDate();
	albumList = data.albumList;
}

string Artist::GetName() const
{
	return name;
}

unsigned int Artist::GetBirthDate() const
{
	return birthDate;
}

unsigned int Artist::GetAlbumNum() const
{
	return albumList.GetLength();
}

string Artist::GetID() const
{
	return ID;
}

void Artist::SetName(const string& name)
{
	this->name = name;
}

void Artist::SetBirthDate(unsigned int date)
{
	this->birthDate = date;
}

void Artist::SetID(const string& id)
{
	ID = id;
}

int Artist::AddAlbum(const Album& data)
{
	return albumList.Add(data);
}

int Artist::DeleteAlbum(const Album& data)
{
	return albumList.Delete(data);
}

int Artist::DeleteAlbumFrom(const string& albumName)
{
	Album album;
	album.SetAlbumName(albumName);
	album.SetArtist(name);

	return albumList.Delete(album);
}

int Artist::ReplaceAlbum(const Album& data)
{
	return albumList.Replace(data);
}

RelationType Artist::Compare(const Artist& data) const
{
	int compare = strcmp(ID.c_str(), data.GetID().c_str());

	if (compare) return compare < 0 ? LESS : GREATER;
	
	return EQUAL;
}

bool Artist::operator<(const Artist& data) const
{
	return Compare(data) == LESS;
}

bool Artist::operator>(const Artist& data) const
{
	return Compare(data) == GREATER;
}

bool Artist::operator==(const Artist& data) const
{
	return Compare(data) == EQUAL;
}

Artist& Artist::operator=(const Artist& data)
{
	name = data.GetName();
	birthDate = data.GetBirthDate();
	albumList = data.albumList;
	ID = data.GetID();

	return *this;
}

void Artist::DisplayIDOnScreen()
{
	cout << "\tArtist ID : ";
	cout << ID << endl;
}

void Artist::DisplayNameOnScreen()
{
	cout << "\tArtist Name : ";
	cout << name << endl;
}

void Artist::DisplayBirthDateOnScreen()
{
	cout << "\tArtist Birth Date : ";
	unsigned int m, d;
	m = birthDate % 10000 / 100;
	d = birthDate % 100;
	cout << birthDate / 10000 << '.';

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

void Artist::DisplayAllOnScreen()
{
	DisplayIDOnScreen();
	DisplayNameOnScreen();
	DisplayBirthDateOnScreen();

	cout << "\tThe number of albums: " << albumList.GetLength() << endl;
	cout << "\t---------------" << endl;
	Album album;

	albumList.ResetList();

	for (int i = 0; i < albumList.GetLength(); i++)
	{
		albumList.GetNextItem(album);
		cout << "\tAlbum Num: " << i + 1 << endl;
		album.DisplayIDOnScreen();
		album.DisplayAlbumNameOnScreen();
		album.DisplayDateOnScreen();
		cout << endl;
	}
}

void Artist::SetIDFromKB()
{
	cout << "\tArtist ID: ";
	getline(cin, ID);
}

void Artist::SetNameFromKB()
{
	cout << "\tArtist Name : ";
	getline(cin, name);
}

void Artist::SetBirthDateFromKB()
{
	cout << "\tBirth Date (YYYYMMDD) : ";
	cin >> birthDate;

	while (cin.fail())
	{
		cout << "\tYou should input integer value" << endl;
		cin.clear();
		cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
		cout << "\tBirth Date (YYYYMMDD) : ";
		cin >> birthDate;
	}
	cin.ignore();
}

int Artist::ReadDataFromFile(ifstream& fin)
{
	if (!fin) return 0;

	getline(fin, name);
	fin >> birthDate;
	fin.ignore();

	int albumLen;
	fin >> albumLen;
	fin.ignore();

	albumList.MakeEmpty();
	for (int i = 0; i < albumLen; i++)
	{
		string id, name;
		unsigned int date;

		getline(fin, id);
		getline(fin, name);
		fin >> date;
		fin.ignore();

		Album album;
		album.SetID(id);
		album.SetAlbumName(name);
		album.SetDate(date);

		albumList.Add(album);
	}

	SetID(name + '_' + std::to_string(birthDate));

	return 1;
}

int Artist::WriteDataToFile(ofstream& fout)
{
	if (!fout) return 0;

	fout << name << endl;
	fout << birthDate << endl;
	fout << albumList.GetLength() << endl;

	albumList.ResetList();
	Album album;
	while (albumList.GetNextItem(album))
	{
		fout << album.GetID() << endl;
		fout << album.GetAlbumName() << endl;
		fout << album.GetDate() << endl;
	}

	return 1;
}

void Artist::ResetList()
{
	albumList.ResetList();
}

int Artist::GetNextAlbum(Album& data)
{
	return albumList.GetNextItem(data);
}