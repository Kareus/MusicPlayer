#include "SimpleMusicType.h"

SimpleMusicType::SimpleMusicType()
{
	id = "";
	length = 0;
	name = "";
	time = 0;
	path = L"";
}

SimpleMusicType::SimpleMusicType(const SimpleMusicType& data)
{
	name = data.GetName();
	id = data.GetID();
	length = data.GetLength();
	time = data.GetPlayedTime();
	path = data.GetPath();
}

SimpleMusicType::~SimpleMusicType()
{
}

void SimpleMusicType::SetName(const string& name)
{
	this->name = name;
}

void SimpleMusicType::SetID(const string& id)
{
	this->id = id;
}

void SimpleMusicType::SetLength(unsigned int length)
{
	this->length = length;
}

void SimpleMusicType::SetPlayedTime(unsigned int time)
{
	this->time = time;
}

void SimpleMusicType::SetPath(const wstring& path)
{
	this->path = path;
}

string SimpleMusicType::GetName() const
{
	return name;
}

string SimpleMusicType::GetID() const
{
	return id;
}

unsigned int SimpleMusicType::GetLength() const
{
	return length;
}

unsigned int SimpleMusicType::GetPlayedTime() const
{
	return time;
}

wstring SimpleMusicType::GetPath() const
{
	return path;
}

void SimpleMusicType::DisplayIDOnScreen()
{
	cout << "\tID: " << id << endl;
}

void SimpleMusicType::DisplayNameOnScreen()
{
	cout << "\tName: " << name << endl;
}

void SimpleMusicType::DisplayLengthOnScreen()
{
	cout << "\tLength: ";

	unsigned int m, s;
	m = length / 60;
	s = length % 60;

	if (m < 10) cout << 0;
	cout << m << ':';
	if (s < 10) cout << 0;
	cout << s << endl;
}

void SimpleMusicType::DisplayPlayedTimeOnScreen()
{
	cout << "\tPlayed Time: " << time << endl;
}

void SimpleMusicType::DisplayAllOnScreen()
{
	DisplayIDOnScreen();
	DisplayNameOnScreen();
	DisplayLengthOnScreen();
	DisplayPlayedTimeOnScreen();
}

void SimpleMusicType::SetIDFromKB()
{
	cout << "\tID : ";
	getline(cin, id);
}

bool SimpleMusicType::operator>(const SimpleMusicType& data) const
{
	return id > data.GetID();
}

bool SimpleMusicType::operator<(const SimpleMusicType& data) const
{
	return id < data.GetID();
}

bool SimpleMusicType::operator==(const SimpleMusicType& data) const
{
	return id == data.GetID();
}

SimpleMusicType& SimpleMusicType::operator=(const SimpleMusicType& data)
{
	name = data.GetName();
	id = data.GetID();
	length = data.GetLength();
	time = data.GetPlayedTime();
	path = data.GetPath();

	return *this;
}

int SimpleMusicType::ReadDataFromFile(ifstream& fin)
{
	if (!fin) return 0;

	getline(fin, id);
	getline(fin, name);
	fin >> length;
	fin.ignore();
	fin >> time;
	fin.ignore();

	return 1;
}

int SimpleMusicType::WriteDataToFile(ofstream& fout)
{
	if (!fout) return 0;

	fout << id << endl;
	fout << name << endl;
	fout << length << endl;
	fout << time << endl;

	return 1;
}