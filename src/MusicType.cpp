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
	length = 0;
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
	length = data.GetLength();
	time = data.GetPlayedTime();
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

unsigned int MusicType::GetLength() const {
	return length;
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

void MusicType::SetGenre(const string& gerne) {
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

void MusicType::SetLength(unsigned int length) {
	this->length = length;
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
	getline(fin, lyrics);
	getline(fin, album);
	getline(fin, genre);

	fin >> date;
	fin >> length;
	fin >> time;
	fin.ignore();

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
	fout << lyrics << endl;
	fout << album << endl;
	fout << genre << endl;
	fout << date << " ";
	fout << length << " ";
	fout << time << endl;
	fout << endl;

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
	length = data.GetLength();
	time = data.GetPlayedTime();
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
	if (!reader.read(path)) return 0;
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
		length = 0;
		wstring len = reader.getFrame("TLEN");
		for (int i = 0; i < len.size(); i++) length = length * 10 + len.at(i) - 48;
	}

	return 1;
}