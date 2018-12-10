#include "MusicType.h"
#include "ID3Reader.h"

MusicType::MusicType()
{
	name = "";
	artist = "";
	album = "";
	genre = "";
	lyrics = "";
	composer = "";
	writer = "";
	path = L"";
	date = 0;
	time = 0;
	ID = "";
}

MusicType::MusicType(const MusicType &data)
{
	ID = data.GetID();
	name = data.GetName();
	artist = data.GetArtist();
	album = data.GetAlbum();
	genre = data.GetGenre();
	lyrics = data.GetLyrics();
	composer = data.GetComposer();
	writer = data.GetWriter();
	date = data.GetDate();
	time = data.GetPlayedTime();
	path = data.GetPath();
}

MusicType::~MusicType() {

}

string MusicType::GetName() const {
	return name;
}

string MusicType::GetArtist() const {
	return artist;
}

string MusicType::GetAlbum() const {
	return album;
}

string MusicType::GetGenre() const {
	return genre;
}

string MusicType::GetLyrics() const {
	return lyrics;
}

string MusicType::GetID() const {
	return ID;
}

string MusicType::GetComposer() const {
	return composer;
}

string MusicType::GetWriter() const {
	return writer;
}

unsigned int MusicType::GetDate() const {
	return date;
}

unsigned int MusicType::GetPlayedTime() const {
	return time;
}

wstring MusicType::GetPath() const {
	return path;
}

void MusicType::SetName(const string& musicName) {
	name = musicName;
}

void MusicType::SetArtist(const string& artistName) {
	artist = artistName;
}

void MusicType::SetAlbum(const string& album) {
	this->album = album;
}

void MusicType::SetGenre(const string& genre) {
	this->genre = genre;
}

void MusicType::SetLyrics(const string& lyrics) {
	this->lyrics = lyrics;
}

void MusicType::SetID(const string& id) {
	ID = id;
}

void MusicType::SetComposer(const string& composer) {
	this->composer = composer;
}

void MusicType::SetWriter(const string& writer) {
	this->writer = writer;
}

void MusicType::SetDate(unsigned int date) {
	this->date = date;
}

void MusicType::SetPlayedTime(unsigned int time) {
	this->time = time;
}

void MusicType::SetPath(const wstring& path) {
	this->path = path;
}

int MusicType::ReadDataFromFile(ifstream& fin)
{
	if (!fin) return 0;
	
	getline(fin, ID);
	getline(fin, name);
	getline(fin, artist);
	getline(fin, composer);
	getline(fin, writer);
	int count;
	fin >> count;
	fin.ignore();

	string line;
	for (int i = 0; i < count; i++)
	{
		getline(fin, line);
		lyrics += line + '\n';
	}

	getline(fin, album);
	getline(fin, genre);

	fin >> date;
	fin >> time;
	fin.ignore();
	string data;
	getline(fin, data);
	path = String::StrToWstr(data);

	return 1;
};

int MusicType::WriteDataToFile(ofstream& fout)
{
	if (!fout) return 0;

	fout << ID << endl;
	fout << name << endl;
	fout << artist << endl;
	fout << composer << endl;
	fout << writer << endl;

	int count = 1;

	for (int i = 0; i < lyrics.size(); i++)
	{
		if (lyrics.at(i) == '\n') count++;
	}

	fout << count << endl;
	fout << lyrics << endl;
	fout << album << endl;
	fout << genre << endl;
	fout << date << " ";
	fout << time << endl;
	fout << String::WstrToStr(path) << endl;

	return 1;
}

RelationType MusicType::Compare(const MusicType &data) const
{
	int compare = strcmp(ID.c_str(), data.GetID().c_str());

	if (compare) return compare < 0 ? LESS : GREATER;
	else return EQUAL;
}

MusicType& MusicType::operator=(const MusicType &data)
{
	ID = data.GetID();
	name = data.GetName();
	artist = data.GetArtist();
	album = data.GetAlbum();
	genre = data.GetGenre();
	lyrics = data.GetLyrics();
	composer = data.GetComposer();
	writer = data.GetWriter();
	date = data.GetDate();
	time = data.GetPlayedTime();
	path = data.GetPath();
	return *this;
}

bool MusicType::operator<(const MusicType &data) const
{
	return Compare(data) == LESS;
}

bool MusicType::operator>(const MusicType &data) const
{
	return Compare(data) == GREATER;
}

bool MusicType::operator==(const MusicType &data) const
{
	return Compare(data) == EQUAL;
}

bool MusicType::operator!=(const MusicType &data) const
{
	return Compare(data) != EQUAL;
}

int MusicType::ReadDataFromID3()
{
	ID3Reader reader;
	if (!reader.read(path))
	{
		reader.close();
		return 0;
	}
	name = String::WstrToStr(reader.getTitle());
	artist = String::WstrToStr(reader.getArtist());
	album = String::WstrToStr(reader.getAlbum());
	genre = String::WstrToStr(reader.getGenre());
	date = reader.getYearAsInteger();
	while (date > 0 && date < 10000000) date *= 10;

	if (reader.getMajorVersion() == 2) //id3v2
	{
		composer = String::WstrToStr(reader.getFrame("TCOM")); //composer
		writer = String::WstrToStr(reader.getFrame("TOLY")); //writer
		if (writer.empty()) writer = String::WstrToStr(reader.getFrame("TEXT")); //writer
		lyrics = String::WstrToStr(reader.getFrame("SYLT")); //lyrics
		if (lyrics.empty()) lyrics = String::WstrToStr(reader.getFrame("USLT")); //lyrics
	}

	return 1;
}