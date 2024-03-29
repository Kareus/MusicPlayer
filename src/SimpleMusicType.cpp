#include "SimpleMusicType.h"

SimpleMusicType::SimpleMusicType()
{
	id = "";
	name = "";
	time = 0;
	path = L"";
}

SimpleMusicType::SimpleMusicType(const SimpleMusicType& data)
{
	name = data.GetName();
	id = data.GetID();
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

unsigned int SimpleMusicType::GetPlayedTime() const
{
	return time;
}

wstring SimpleMusicType::GetPath() const
{
	return path;
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
	time = data.GetPlayedTime();
	path = data.GetPath();

	return *this;
}

int SimpleMusicType::ReadDataFromFile(ifstream& fin)
{
	if (!fin) return 0;

	getline(fin, id);
	getline(fin, name);
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
	fout << time << endl;

	return 1;
}