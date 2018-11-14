#include "MusicType.h"

MusicType::MusicType() {
	name = "";
	artist = "";
	album = "";
	genre = "";
	lyrics = "";
	composer = "";
	writer = "";
	record = "";
	date = 0;
	time = 0;
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

string MusicType::GetRecord() const {
	return record;
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

string MusicType::GetNote() const {
	return note;
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

void MusicType::SetRecord(const string& record) {
	this->record = record;
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

void MusicType::SetNote(const string& note) {
	this->note = note;
}

void MusicType::DisplayNameOnScreen()
{
	cout << "\tName : " << name << endl;
};

void MusicType::DisplayArtistOnScreen()
{
	cout << "\tArtist : " << artist << endl;
}

void MusicType::DisplayAlbumOnScreen()
{
	cout << "\tAlbum : " << album << endl;
}

void MusicType::DisplayGenreOnScreen()
{
	cout << "\tGenre : " << genre << endl;
}

void MusicType::DisplayLyricsOnScreen()
{
	cout << "\tLyrics : " << lyrics << endl;
}

void MusicType::DisplayIDOnScreen()
{
	cout << "\tID : " << ID << endl;
}

void MusicType::DisplayRecordOnScreen()
{
	cout << "\tRecord : " << record << endl;
}

void MusicType::DisplayComposerOnScreen()
{
	cout << "\tComposer : " << composer << endl;
}

void MusicType::DisplayWriterOnScreen()
{
	cout << "\tWriter : " << writer << endl;
}

void MusicType::DisplayDateOnScreen()
{
	unsigned int m, d;
	m = date % 10000 / 100;
	d = date % 100;
	cout << "\tDate : " << date / 10000 << '.';

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

void MusicType::DisplayLengthOnScreen()
{
	cout << "\tLength : ";

	unsigned int m, s;
	m = length / 60;
	s = length % 60;

	if (m < 10) cout << 0;
	cout << m << ':';
	if (s < 10) cout << 0;
	cout << s << endl;
}

void MusicType::DisplayPlayedTimeOnScreen()
{
	cout << "\tPlayed Time : " << time << endl;
}

void MusicType::DisplayNoteOnScreen()
{
	cout << "\tNote : " << note << endl;
}

void MusicType::DisplayAllOnScreen()
{
	DisplayIDOnScreen();
	DisplayNameOnScreen();
	DisplayArtistOnScreen();
	DisplayComposerOnScreen();
	DisplayWriterOnScreen();
	DisplayAlbumOnScreen();
	DisplayDateOnScreen();
	DisplayGenreOnScreen();
	DisplayRecordOnScreen();
	DisplayLengthOnScreen();
	DisplayPlayedTimeOnScreen();
	DisplayLyricsOnScreen();
	DisplayNoteOnScreen();
}

void MusicType::SetNameFromKB()
{
	cout << "\tName : ";
	getline(cin, name);
}

void MusicType::SetArtistFromKB()
{
	cout << "\tArtist : ";
	getline(cin, artist);
}

void MusicType::SetAlbumFromKB()
{
	cout << "\tAlbum : ";
	getline(cin, album);
}

void MusicType::SetGenreFromKB()
{
	cout << "\tGenre : ";
	getline(cin, genre);
}

void MusicType::SetLyricsFromKB()
{
	cout << "\tLyrics : ";
	getline(cin, lyrics); //띄어쓰기로 구분되는 걸 방지하기 위해 라인으로 읽음
}

void MusicType::SetIDFromKB()
{
	cout << "\tID : ";
	getline(cin, ID);
}

void MusicType::SetRecordFromKB()
{
	cout << "\tRecord : ";
	getline(cin, record);
}

void MusicType::SetComposerFromKB()
{
	cout << "\tComposer : ";
	getline(cin, composer);
}

void MusicType::SetWriterFromKB()
{
	cout << "\tWriter : ";
	getline(cin, writer);
}

void MusicType::SetDateFromKB()
{
	cout << "\tDate (YYYYMMDD) : ";
	cin >> date;

	while (cin.fail())
	{
		cout << "\tYou should input integer value" << endl;
		cin.clear();
		cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
		cout << "\tDate (YYYYMMDD) : ";
		cin >> date;
	}
	cin.ignore(); //불필요한 데이터 flush
}

void MusicType::SetLengthFromKB()
{
	cout << "\tLength (Seconds) : ";
	cin >> length;

	while (cin.fail())
	{
		cout << "\tYou should input integer value" << endl;
		cin.clear();
		cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
		cout << "\tLength (Seconds) : ";
		cin >> length;
	}
	cin.ignore();
}

void MusicType::SetPlayedTimeFromKB()
{
	cout << "\tPlayed Time : ";
	cin >> time;

	while (cin.fail())
	{
		cout << "\tYou should input integer value" << endl;
		cin.clear();
		cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
		cout << "\tPlayed Time : ";
		cin >> time;
	}
	cin.ignore();
}

void MusicType::SetNoteFromKB()
{
	cout << "\tNote  (ex] C6 E5 C5# D5) : ";
	getline(cin, note);
}

void MusicType::SetAllFromKB()
{
	//SetIDFromKB(); //ID 자동 배정을 위해 주석 처리
	SetNameFromKB();
	SetArtistFromKB();
	SetComposerFromKB();
	SetWriterFromKB();
	SetLyricsFromKB();
	SetAlbumFromKB();
	SetGenreFromKB();
	SetRecordFromKB();
	SetDateFromKB();
	SetLengthFromKB();
	SetPlayedTimeFromKB();
	SetNoteFromKB();
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
	getline(fin, record);
	fin >> date;
	fin >> length;
	fin >> time;
	fin.ignore();
	getline(fin, note);

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
	fout << record << endl;
	fout << date << " ";
	fout << length << " ";
	fout << time << endl;
	fout << note;
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
	record = data.GetRecord();
	date = data.GetDate();
	length = data.GetLength();
	time = data.GetPlayedTime();
	note = data.GetNote();
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