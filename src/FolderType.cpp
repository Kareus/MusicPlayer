#include "FolderType.h"

FolderType::FolderType()
{
	dirPath = L"";
	list.MakeEmpty();
}

FolderType::FolderType(const FolderType& data)
{
	dirPath = data.GetPath();
	list = data.list;
}

FolderType::~FolderType()
{
	list.MakeEmpty();
}

std::wstring FolderType::GetPath() const
{
	return dirPath;
}

void FolderType::SetPath(const std::wstring& path)
{
	dirPath = path;
}

int FolderType::AddMusic(const SimpleMusicType& data)
{
	return list.Add(data);
}

int FolderType::DeleteMusic(const SimpleMusicType& data)
{
	return list.Delete(data);
}

int FolderType::ReplaceMusic(const SimpleMusicType& data)
{
	return list.Replace(data);
}

int FolderType::GetMusic(SimpleMusicType& data)
{
	return list.Get(data);
}

int FolderType::GetLength() const
{
	return list.GetLength();
}

FolderType& FolderType::operator=(const FolderType& data)
{
	dirPath = data.GetPath();
	list = data.list;
	return *this;
}

bool FolderType::operator<(const FolderType& data) const
{
	return wcscmp(dirPath.c_str(), data.GetPath().c_str()) < 0;
}

bool FolderType::operator>(const FolderType& data) const
{
	return wcscmp(dirPath.c_str(), data.GetPath().c_str()) > 0;
}

bool FolderType::operator==(const FolderType& data) const
{
	return wcscmp(dirPath.c_str(), data.GetPath().c_str()) == 0;
}

DoublyIterator<SimpleMusicType> FolderType::GetIterator()
{
	return DoublyIterator<SimpleMusicType>(list);
}