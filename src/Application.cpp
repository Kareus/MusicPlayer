#include "Application.h"
#include <Windows.h>

#undef max //max macro 혼용 방지

Application::Application()
{
	m_Command = 0;
	recentListCount = 0;
	addedCount = 0;
	nameList.SetCompareFunction(compareMusicName);
	recentPlayedList.SetCompareFunction(compareToLast);
	mostPlayedList.SetCompareFunction(comparePlayedTime);
	/*
	ID가 제목_가수이름 이지만, 두 곡이 각각
	1. ABC_ARTIST1
	2. ABCA_ARTIST2
	일때 제목 순으로는 1 -> 2이지만
	ID순으로는 2 -> 1 (A의 ascii code가 65, _의 ascii code가 95)이므로
	이런 상황을 막기 위해 compare function을 새로 설정.
	*/

}

Application::~Application()
{
}

void Application::Run()
{
	while (1)
	{
		m_Command = GetCommand();

		switch (m_Command)
		{
		case 1:	// 음악 메뉴
			ShowMusicMenu();
			break;
		case 2:	// 앨범 메뉴
			ShowAlbumMenu();
			break;
		case 3:	// 아티스트 메뉴
			ShowArtistMenu();
			break;
		case 4:	// txt 파일에서 음악정보를 읽어온다
			ReadDataFromFile();
			break;
		case 5:	// txt 파일에 음악정보를 쓴다
			WriteDataToFile();
			break;
		case 0:
			return;
		default:
			cout << "\tIllegal selection...\n";
			break;
		}
	}
}


int Application::GetCommand()
{
	int command;
	cout << endl << endl;
	cout << "\t---ID -- Command ----- " << endl;
	cout << "\t   1 : Open music menu" << endl;
	cout << "\t   2 : Open album menu" << endl;
	cout << "\t   3 : Open artist menu" << endl;
	cout << "\t   4 : Read data from file" << endl;
	cout << "\t   5 : Write data to file" << endl;
	cout << "\t   0 : Quit" << endl;

	cout << endl << "\t Choose a Command--> ";

	cin >> command;

	while (cin.fail())
	{
		cout << "\t You should input integer value" << endl;
		cin.clear();
		cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
		cout << endl << "\t Choose a Command--> ";
		cin >> command;
	}

	cin.ignore();
	cout << endl;

	return command;
}

int Application::AddMusic()
{
	// 입력받은 레코드를 리스트에 add

	MusicType music;

	music.SetAllFromKB();
	music.SetID(music.GetName() + '_' + music.GetArtist());

	if (musicList.Add(music) == 0)
	{
		cout << "\tMusic that has the same ID already exists" << endl;
		return 0; //이미 존재하는 음악이면 실패 
	}

	cout << endl;
	cout << "\tMusic Added. ID = " << music.GetID() << endl << endl;

	//장르 확인
	bool exist = false;
	genreList.ResetList();

	SimpleMusicType simpleMusic; //간단한 정보만 저장한 음악 데이터
	simpleMusic.SetName(music.GetName());
	simpleMusic.SetID(music.GetID());
	simpleMusic.SetLength(music.GetLength());
	simpleMusic.SetPlayedTime(music.GetPlayedTime());

	for (int i = 0; i < genreList.GetLength(); i++)
	{
		GenreType genre;
		genreList.GetNextItem(genre);
		
		if (music.GetGenre() == genre.GetGenre()) //장르가 이미 있다면
		{
			exist = true;

			genre.Add(simpleMusic);
			
			genreList.Replace(genre);
			break;
		}
	}

	if (!exist) //리스트에 없다면
	{
		GenreType newGenre;
		newGenre.SetGenre(music.GetGenre());
		newGenre.Add(simpleMusic);
		genreList.Add(newGenre);
	}

	if (newAddMusicList.IsFull()) //이미 꽉 차 있다면
	{
		SimpleMusicType temp;
		newAddMusicList.DeQueue(temp);
	}
	else addedCount++;

	newAddMusicList.EnQueue(simpleMusic); //최근 음악 추가
	
	nameList.Add(simpleMusic); //곡명 순으로 삽입

	AddToMostPlayed(music); //가장 많이 재생한 목록에 추가

	cout << "\tDo you want to add the music to album and artist automatically? (y/n): ";

	char answer;

	cin >> answer;

	bool yes = (answer == 'Y' || answer == 'y');
	bool no = (answer == 'N' || answer == 'n');
	while (cin.fail() || !(yes || no))
	{
		cout << "\tPut Y(y) for yes, or N(n) for no:";
		cin.clear();
		cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
		cin >> answer;

		yes = (answer == 'Y' || answer == 'y');
		no = (answer == 'N' || answer == 'n');
	}

	cin.ignore();

	cout << endl;

	if (no) return 1;

	Album album;
	album.SetID(music.GetAlbum() + '_' + music.GetArtist());

	if (albumList.Get(album))
	{
		album.AddMusic(simpleMusic);
		albumList.Replace(album);
	}
	else
	{
		album.SetAlbumName(music.GetAlbum());
		album.SetArtist(music.GetArtist());

		cout << "\tThere is no album with that name and artist. Input the album info" << endl;
		album.SetRecordFromKB();
		album.SetDateFromKB();

		album.AddMusic(simpleMusic);
		albumList.Add(album);
	}

	Artist artist;
	
	cout << endl;
	cout << "\tInput the birth date of artist to identify" << endl;
	artist.SetBirthDateFromKB();
	artist.SetID(music.GetArtist() + '_' + std::to_string(artist.GetBirthDate()));

	cout << endl;
	if (artistList.Get(artist))
	{
		artist.AddAlbum(album);
		artistList.Replace(artist);
	}
	else
	{
		artist.SetName(music.GetArtist());
		artist.AddAlbum(album);
		artistList.Add(artist);
	}

	return 1;
}

void Application::DisplayAllMusic()
{
	MusicType data;

	cout << "\n\tCurrent music list" << endl;

	// list의 모든 데이터를 화면에 출력
	musicList.ResetList();
	int curIndex = 1;
	while (musicList.GetNextItem(data))
	{
		cout << "\tMusic Num: " << curIndex << endl;
		cout << "\t---------------" << endl;
		data.DisplayAllOnScreen();
		cout << endl;
		curIndex++;
	}

	if (musicList.GetLength() == 0) cout << "\tThere is no music added" << endl;
}

int Application::OpenInFile(char *fileName)
{
	m_inputFile.open(fileName);	// file open for reading.

	// 파일 오픈에 성공하면 1, 그렇지 않다면 0을 리턴.
	if (!m_inputFile)	return 0;
	return 1;
}

int Application::OpenOutFile(char *fileName)
{
	m_outFile.open(fileName);	// file open for writing.

	// 파일 오픈에 성공하면 1, 그렇지 않다면 0을 리턴.
	if (!m_outFile)	return 0;
	return 1;
}

int Application::ReadDataFromFile()
{
	int index = 0;
	MusicType data;	// 읽기용 임시 변수
	Album album;
	Artist artist;
	SimpleMusicType music;

	char filename[FILENAMESIZE];
	cout << "\n\tEnter Input file Name : ";
	cin.getline(filename, FILENAMESIZE);

	// file open, open error가 발생하면 0을 리턴
	if (!OpenInFile(filename))
		return 0;

	MakeEmpty();

	int musicLen, albumLen, artistLen;
	m_inputFile >> musicLen;
	m_inputFile.ignore();

	for (int i = 0; i < musicLen; i++)
	{
		data.ReadDataFromFile(m_inputFile);
		AddMusic(data);
	}

	m_inputFile >> albumLen;
	m_inputFile.ignore();

	for (int i = 0; i < albumLen; i++)
	{
		album.ReadDataFromFile(m_inputFile);
		albumList.Add(album);
	}
	
	m_inputFile >> artistLen;
	m_inputFile.ignore();

	for (int i = 0; i < artistLen; i++)
	{
		artist.ReadDataFromFile(m_inputFile);
		artistList.Add(artist);
	}

	m_inputFile >> addedCount;
	m_inputFile.ignore();

	for (int i = 0; i < addedCount; i++)
	{
		music.ReadDataFromFile(m_inputFile);
		newAddMusicList.EnQueue(music);
	}

	m_inputFile >> recentListCount;
	m_inputFile.ignore();

	for (int i = 0; i < recentListCount; i++)
	{
		music.ReadDataFromFile(m_inputFile);
		recentPlayedList.Add(music);
	}

	m_inputFile.close();	// file close

	// 현재 list 출력
	DisplayAllMusic();

	return 1;
}

int Application::WriteDataToFile()
{
	MusicType data;	// 쓰기용 임시 변수
	Album album;
	Artist artist;
	SimpleMusicType music;

	char filename[FILENAMESIZE];
	cout << "\n\tEnter Output file Name : ";
	cin.getline(filename, FILENAMESIZE);

	// file open, open error가 발생하면 0을 리턴
	if (!OpenOutFile(filename))
		return 0;

	m_outFile << musicList.GetLength() << endl; //음악 수 기록

	// list 포인터를 초기화
	musicList.ResetList();
	albumList.ResetList();
	artistList.ResetList();

	// list의 모든 데이터를 파일에 쓰기
	while (musicList.GetNextItem(data)) data.WriteDataToFile(m_outFile);

	m_outFile << albumList.GetLength() << endl;

	while (albumList.GetNextItem(album)) album.WriteDataToFile(m_outFile);

	m_outFile << artistList.GetLength() << endl;

	while (artistList.GetNextItem(artist)) artist.WriteDataToFile(m_outFile);

	m_outFile << addedCount << endl;

	newAddMusicList.ResetQueue();

	for (int i = 0; i < addedCount; i++)
	{
		newAddMusicList.GetNextItem(music);
		music.WriteDataToFile(m_outFile);
	}

	m_outFile << recentListCount << endl;

	recentPlayedList.ResetList();

	for (int i = 0; i < recentListCount; i++)
	{
		recentPlayedList.GetNextItem(music);
		music.WriteDataToFile(m_outFile);
	}

	m_outFile.close();	// file close

	return 1;
}

int Application::DeleteMusic()
{
	MusicType music;

	music.SetIDFromKB();

	if (DeleteMusic(music))
	{
		cout << "\tMusic has been successfully deleted" << endl;
		return 1;
	}

	cout << "\tThere is no music that has such ID" << endl;
	return 0;
}

int Application::ReplaceMusic()
{
	cout << "\tInput the ID of the music you want to edit" << endl;
	MusicType music;
	music.SetIDFromKB();
	
	cout << "\tInput the data of the music" << endl;

	MusicType* data = musicList.GetPoint(music);

	if (data == NULL)
	{
		cout << "\tThere is no music that has such ID" << endl;
		return 0;
	}

	data->SetAllFromKB();

	data->SetID(data->GetName() + '_' + data->GetArtist());

	DeleteMusic(music);

	if (ReplaceMusic(music))
	{
		cout << "\tMusic has been successfully replaced" << endl;
		return 1;
	}

	cout << "\tFailed to replace the music" << endl;
	return 0;
}

int Application::SearchMusicByName()
{
	MusicType music;

	music.SetNameFromKB();

	int success = 0;
	
	nameList.ResetList();

	cout << "\t---------------" << endl;
	for (int i = 0; i < nameList.GetLength(); i++)
	{
		SimpleMusicType data;
		nameList.GetNextItem(data);
		if ((int)data.GetName().find(music.GetName()) >= 0)
		{
			success++;
			cout << "\tMusic num : " << success << endl;
			data.DisplayIDOnScreen();
			data.DisplayNameOnScreen();
			cout << endl;
		}
	}

	if (success > 0) return 1;
	else cout << "\tMusic doesn't exist" << endl;

	return 0;
}

int Application::SearchMusicByArtist()
{
	MusicType music;

	music.SetArtistFromKB();

	int success = 0;

	musicList.ResetList();
	cout << "\t---------------" << endl;

	for (int i = 0; i < musicList.GetLength(); i++)
	{
		MusicType data;
		musicList.GetNextItem(data);
		if ((int)data.GetArtist().find(music.GetArtist()) >= 0)
		{
			success++;
			cout << "\tMusic Num: " << success << endl;
			cout << "\t---------------" << endl;
			data.DisplayIDOnScreen();
			data.DisplayNameOnScreen();
			cout << endl;
		}
	}

	if (success > 0) return 1;
	else cout << "\tMusic doesn't exist" << endl;

	return 0;
}

int Application::SearchMusicByAlbum()
{
	MusicType music;

	music.SetAlbumFromKB();

	int success = 0;

	musicList.ResetList();
	cout << "\t---------------" << endl;

	for (int i = 0; i < musicList.GetLength(); i++)
	{
		MusicType data;
		musicList.GetNextItem(data);
		if ((int)data.GetAlbum().find(music.GetAlbum()) >= 0)
		{
			success++;
			cout << "\tMusic Num: " << success << endl;
			cout << "\t---------------" << endl;
			data.DisplayIDOnScreen();
			data.DisplayNameOnScreen();
			cout << endl;
		}
	}

	if (success > 0) return 1;
	else cout << "\tMusic doesn't exist" << endl;

	return 0;
}

int Application::SearchMusicByGenre()
{
	MusicType music;

	music.SetGenreFromKB();

	int success = 0;

	musicList.ResetList();
	cout << "\t---------------" << endl;

	for (int i = 0; i < musicList.GetLength(); i++)
	{
		MusicType data;
		musicList.GetNextItem(data);
		if ((int)data.GetGenre().find(music.GetGenre()) >= 0)
		{
			success++;
			cout << "\tMusic Num: " << success << endl;
			cout << "\t---------------" << endl;
			data.DisplayIDOnScreen();
			data.DisplayNameOnScreen();
			cout << endl;
		}
	}

	if (success > 0) return 1;
	else cout << "\tMusic doesn't exist" << endl;

	return 0;
}

void Application::MakeEmpty()
{
	musicList.MakeEmpty();
	nameList.MakeEmpty();
	genreList.MakeEmpty();
	albumList.MakeEmpty();
	artistList.MakeEmpty();
	newAddMusicList.MakeEmpty();
	recentPlayedList.MakeEmpty();
	mostPlayedList.MakeEmpty();
}

void Application::DisplayNewMusic()
{
	newAddMusicList.ResetQueue();
	SimpleMusicType music;

	cout << "\t최근 추가된 음악들" << endl;
	cout << "\t---------------" << endl;

	if (newAddMusicList.IsEmpty())
	{
		cout << "\tThere is no music recently added!" << endl;
		return;
	}

	for (int i = 0; i < 30; i++)
	{
		if (newAddMusicList.GetNextItem(music) < 0) break; //아이템이 더 없으면 종료

		music.DisplayIDOnScreen();
		music.DisplayNameOnScreen();
		cout << endl;
	}
}

void Application::DisplayMusicByGenre()
{
	cout << "\t장르별 음악들" << endl;
	genreList.ResetList();

	if (genreList.GetLength() == 0)
	{
		cout << "There is no genres added!" << endl;
		return;
	}

	for (int i = 0; i < genreList.GetLength(); i++)
	{
		GenreType genre;

		genreList.GetNextItem(genre);

		SimpleMusicType music;
		cout << "Genre : " << genre.GetGenre() << endl;
		cout << "\t---------------" << endl;

		DoublyLinkedList<SimpleMusicType> list = genre.GetList();
		list.ResetList();
		for (int j = 0; j < list.GetLength(); j++)
		{
			cout << "\tMusic Num: " << i + 1 << endl;
			cout << "\t---------------" << endl;
			list.GetNextItem(music);
			music.DisplayIDOnScreen();
			music.DisplayNameOnScreen();
			cout << endl;
		}

		cout << endl;
	}
}

void Application::RetreiveByNameNGenre()
{
	MusicType music;
	music.SetNameFromKB();
	music.SetGenreFromKB();

	MusicType temp;
	musicList.ResetList();

	int success = 0;

	cout << "\t---------------" << endl;
	for (int i = 0; i < musicList.GetLength(); i++)
	{
		musicList.GetNextItem(temp);
		if (music.GetName() != temp.GetName()) continue; //이름이 다르면 스킵
		if (music.GetGenre() != temp.GetGenre()) continue; //장르가 다르면 스킵
		
		temp.DisplayAllOnScreen(); //발견했으므로 출력
		cout << endl;
		success++;
	}

	if (success == 0) cout << "\tMusic doesn't exist" << endl;
}

int Application::ShowMusicMenu()
{
	MusicType music;
	SimpleMusicType simple;

	while (true)
	{
		cout << "\t-----Music Menu-----" << endl;
		int command = GetMusicCommand();
		switch (command)
		{
		case 1:
			AddMusic();
			break;

		case 2:
			DeleteMusic();
			break;

		case 3:
			ReplaceMusic();
			break;

		case 4:
			PlayMusic();
			break;

		case 5:
			music.SetIDFromKB();
			musicList.Get(music);
			music.DisplayLyricsOnScreen();
			cout << endl;
			break;

		case 6:
			music.SetIDFromKB();
			musicList.Get(music);
			music.DisplayNoteOnScreen();
			cout << endl;
			break;

		case 7:
			SearchMusicByID();
			break;

		case 8:
			DisplayAllMusic();
			break;

		case 9:
			SearchMusicByName();
			break;

		case 10:
			SearchMusicByArtist();
			break;

		case 11:
			SearchMusicByAlbum();
			break;

		case 12:
			SearchMusicByGenre();
			break;

		case 13:
			SearchMusicByComposer();
			break;

		case 14:
			DisplayNewMusic();
			break;

		case 15:
			DisplayRecentPlayed();
			break;

		case 16:
			DisplayMostPlayed();
			break;

		case 0:
			return 0;

		default:
			cout << "\tIllegal selection...\n";
			cin.ignore();
			break;
		}
	}
}

int Application::GetMusicCommand()
{
	int command;
	cout << endl << endl;
	cout << "\t---ID -- Command ----- " << endl;
	cout << "\t   1 : Add music" << endl;
	cout << "\t   2 : Delete music" << endl;
	cout << "\t   3 : Replace music" << endl;
	cout << "\t   4 : Play music" << endl;
	cout << "\t   5 : Show lyrics" << endl;
	cout << "\t   6 : Show notes" << endl;
	cout << "\t   7 : Display music" << endl;
	cout << "\t   8 : Display all musics" << endl;
	cout << "\t   9 : Search music by name" << endl;
	cout << "\t   10 : Search music by artist" << endl;
	cout << "\t   11 : Search music by album" << endl;
	cout << "\t   12 : Search music by genre" << endl;
	cout << "\t   13 : Search music by composer" << endl;
	cout << "\t   14 : Display recently added musics" << endl;
	cout << "\t   15 : Display recently played musics" << endl;
	cout << "\t   16 : Display most played musics" << endl;
	cout << "\t   0 : Return to the main menu" << endl;

	cout << endl << "\t Choose a Command--> ";
	cin >> command;

	while (cin.fail())
	{
		cout << "\t You should input integer value" << endl;
		cin.clear();
		cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
		cout << endl << "\t Choose a Command--> ";
		cin >> command;
	}

	cin.ignore();
	cout << endl;

	return command;
}

void Application::AddToRecentPlayed(const MusicType& music)
{
	SimpleMusicType simpleMusic;
	simpleMusic.SetID(music.GetID());
	simpleMusic.SetName(music.GetName());

	recentPlayedList.Delete(simpleMusic); //만약 있다면 제거
	recentPlayedList.Add(simpleMusic);

	if (recentPlayedList.GetLength() > 30)
	{
		recentPlayedList.ResetList();
		recentPlayedList.GetNextItem(simpleMusic); //첫번째 아이템 찾기
		recentPlayedList.Delete(simpleMusic);
	}
}

void Application::AddToMostPlayed(const MusicType& music)
{
	SimpleMusicType simpleMusic;
	simpleMusic.SetID(music.GetID());
	simpleMusic.SetName(music.GetName());
	simpleMusic.SetPlayedTime(music.GetPlayedTime());
;
	mostPlayedList.Delete(simpleMusic); //만약 있다면 제거
	mostPlayedList.Add(simpleMusic);

	if (mostPlayedList.GetLength() > 30)
	{
		mostPlayedList.ResetList();
		while (mostPlayedList.GetNextItem(simpleMusic)); //가장 마지막 아이템 찾기
		mostPlayedList.Delete(simpleMusic);
	}
}

void Application::DisplayRecentPlayed()
{
	cout << "\t최근 재생한 30곡" << endl;

	recentPlayedList.ResetList();
	SimpleMusicType music;

	if (recentPlayedList.GetLength() == 0)
	{
		cout << "\tThere is no music recently played!" << endl;
		return;
	}

	cout << "\t---------------" << endl;

	for (int i = 0; i < recentPlayedList.GetLength(); i++)
	{
		recentPlayedList.GetNextItem(music);

		cout << "\tMusic Num: " << i + 1<< endl;
		cout << "\t---------------" << endl;
		music.DisplayIDOnScreen();
		music.DisplayNameOnScreen();
		cout << endl;
	}
}

void Application::DisplayMostPlayed()
{
	cout << "\t가장 많이 재생한 30곡" << endl;
	mostPlayedList.ResetList();
	SimpleMusicType music;

	if (mostPlayedList.GetLength() == 0)
	{
		cout << "\tThere is no music most played!" << endl;
		return;
	}

	cout << "\t---------------" << endl;
	
	int count = 0;
	while (mostPlayedList.GetNextItem(music))
	{
		count++;
		cout << "\tMusic Num: " << count << endl;
		cout << "\t---------------" << endl;
		music.DisplayIDOnScreen();
		music.DisplayNameOnScreen();
		music.DisplayPlayedTimeOnScreen();
		cout << endl;
	}
}

int Application::ReplaceMusic(const MusicType& music)
{

	if (musicList.Replace(music) == 0) return 0;

	SimpleMusicType simple;
	simple.SetID(music.GetID());
	simple.SetName(music.GetName());
	simple.SetLength(music.GetLength());
	simple.SetPlayedTime(music.GetPlayedTime());

	nameList.Replace(simple);

	genreList.ResetList();

	GenreType genre;
	while (genreList.GetNextItem(genre))
	{
		if (!strcmp(genre.GetGenre().c_str(), music.GetGenre().c_str()))
		{
			genre.Replace(simple);
			break;
		}
	}

	albumList.ResetList();

	Album album;

	while (albumList.GetNextItem(album)) if (album.ReplaceMusic(simple)) albumList.Replace(album);

	recentPlayedList.Replace(simple);
	mostPlayedList.Replace(simple);

	return 1;
}

int Application::DeleteMusic(const MusicType& music)
{
	SimpleMusicType simpleMusic;
	simpleMusic.SetID(music.GetID());

	if (musicList.Delete(music) == 0) return 0;

	nameList.Delete(simpleMusic);

	GenreType genre;
	genre.SetGenre(music.GetGenre());
	genreList.Get(genre);
	genre.Delete(simpleMusic);
	if (genre.GetMusicNum() > 0) genreList.Replace(genre);
	else genreList.Delete(genre);

	Album album;
	
	albumList.ResetList();

	while (albumList.GetNextItem(album))
		if (album.DeleteMusic(simpleMusic)) albumList.Replace(album);

	recentPlayedList.Delete(simpleMusic);
	mostPlayedList.Delete(simpleMusic);

	return 1;
}

int Application::GetArtistByName(const string& name, Artist& data)
{
	artistList.ResetList();
	Artist artist;
	for (int i = 0; i < artistList.GetLength(); i++)
	{
		artistList.GetNextItem(artist);
		if (artist.GetName() == name)
		{
			data = artist;
			return 1;
		}
	}
	return 0;
}

void Application::ClearEmptyAlbums()
{
	albumList.ResetList();

	Album album;
	while (albumList.GetNextItem(album))
	{
		if (album.GetMusicNum() != 0) continue;

		albumList.Delete(album);
		Artist artist;

		if (GetArtistByName(album.GetArtist(), artist) == 0) continue;
		artist.DeleteAlbum(album);
		artistList.Replace(artist);
	}

	cout << "\tRemoved all empty album data" << endl;
}

int Application::SearchMusicByID()
{
	MusicType music;
	music.SetIDFromKB();

	if (musicList.Get(music) == 0)
	{
		cout << "\tThe music doesn't exist" << endl;
		return 0;
	}

	music.DisplayAllOnScreen();
	cout << endl;
	return 1;
}

int Application::SearchMusicByComposer()
{
	MusicType music;
	music.SetComposerFromKB();

	int success = 0;
	cout << "\t---------------" << endl;
	musicList.ResetList();


	for (int i = 0; i < musicList.GetLength(); i++)
	{
		MusicType temp;
		musicList.GetNextItem(temp);

		if ((int)(music.GetComposer().find(temp.GetComposer())) >= 0)
		{
			success++;
			cout << "\tMusic Num: " << success << endl;
			cout << "\t---------------" << endl;
			temp.DisplayIDOnScreen();
			temp.DisplayNameOnScreen();
			cout << endl;
		}
	}

	if (success > 0) return 1;

	cout << "\tThere is no music that the composer made" << endl;
	return 0;
}

int Application::SearchAlbumByID()
{
	Album album;
	album.SetIDFromKB();

	cout << "\t---------------" << endl;
	if (albumList.Get(album) == 0)
	{
		cout << "\tThe album doesn't exist" << endl;
		return 0;
	}

	album.DisplayAllOnScreen();
	cout << endl;
	return 1;
}

int Application::SearchAlbumByName()
{
	string albumName;

	cout << "\tAlbum Name: ";
	getline(cin, albumName);

	albumList.ResetList();

	int success = 0;

	cout << "\t---------------" << endl;
	for (int i = 0; i < albumList.GetLength(); i++)
	{
		Album temp;
		albumList.GetNextItem(temp);

		if ((int)(temp.GetAlbumName().find(albumName)) >= 0)
		{
			success++;
			cout << "\tAlbum Num: " << success << endl;
			cout << "\t---------------" << endl;
			temp.DisplayAllOnScreen();
			cout << endl;
		}
	}

	if (success > 0) return 1;
	cout << "\tNo album that has such name exists" << endl;
	return 0;
}

int Application::SearchAlbumByArtist()
{
	string artist;
	cout << "\tAlbum Artist: ";
	
	getline(cin, artist);

	albumList.ResetList();
	
	cout << "\t---------------" << endl;
	int success = 0;
	for (int i = 0; i < albumList.GetLength(); i++)
	{
		Album temp;
		albumList.GetNextItem(temp);

		if ((int)(temp.GetArtist().find(artist)) >= 0)
		{
			success++;
			cout << "\tAlbum Num: " << success << endl;
			cout << "\t---------------" << endl;
			temp.DisplayAllOnScreen();
			cout << endl;
		}
	}

	if (success > 0) return 1;
	cout << "\tNo album from the artist exists" << endl;
	return 0;
}

void Application::DisplayAlbumByName()
{
	if (albumList.GetLength() == 0)
	{
		cout << "\tThere is no album added" << endl;
		return;
	}

	//primary key가 이름 순이므로 그냥 출력
	albumList.ResetList();
	Album album;

	cout << "\t---------------" << endl;
	for (int i = 0; i < albumList.GetLength(); i++)
	{
		cout << "\tAlbum Num: " << i + 1 << endl;
		cout << "\t---------------" << endl;
		albumList.GetNextItem(album);

		album.DisplayIDOnScreen();
		album.DisplayAlbumNameOnScreen();
		album.DisplayArtistOnScreen();
		cout << endl;
	}
}

void Application::DisplayAlbumByArtist()
{
	if (albumList.GetLength() == 0)
	{
		cout << "\tThere is no album added" << endl;
		return;
	}

	DoublyLinkedList<Album> list;
	list.SetCompareFunction([](const Album& a1, const Album& a2) {
		int compare = strcmp(a1.GetArtist().c_str(), a2.GetArtist().c_str());
		if (compare == 0) return (a1 < a2) - (a1 > a2);
		return compare;
	}); //아티스트 순으로 정렬하도록 설정

	//개선할 여지가 있음

	Album album;

	albumList.ResetList();

	for (int i = 0; i < albumList.GetLength(); i++)
	{
		albumList.GetNextItem(album);
		list.Add(album);
	}


	cout << "\t---------------" << endl;
	list.ResetList();

	for (int i = 0; i < list.GetLength(); i++)
	{
		list.GetNextItem(album);
		cout << "\tAlbum Num: " << i + 1 << endl;
		cout << "\t---------------" << endl;
		album.DisplayIDOnScreen();
		album.DisplayAlbumNameOnScreen();
		album.DisplayArtistOnScreen();
		cout << endl;
	}

}

void Application::DisplayAlbumByDate()
{
	if (albumList.GetLength() == 0)
	{
		cout << "\tThere is no album added" << endl;
		return;
	}

	DoublyLinkedList<Album> list;
	list.SetCompareFunction([](const Album& a1, const Album& a2) {
		int compare = (a1.GetDate() > a2.GetDate()) - (a1.GetDate() < a2.GetDate());
		if (compare == 0) return (a1 < a2) - (a1 > a2);
		return compare;
	}); //날짜 순으로 정렬하도록 설정

	//개선할 여지가 있음
	Album album;

	albumList.ResetList();

	for (int i = 0; i < albumList.GetLength(); i++)
	{
		albumList.GetNextItem(album);
		list.Add(album);
	}

	cout << "\t---------------" << endl;
	list.ResetList();

	for (int i = 0; i < list.GetLength(); i++)
	{
		list.GetNextItem(album);
		cout << "\tAlbum Num: " << i + 1 << endl;
		cout << "\t---------------" << endl;
		album.DisplayIDOnScreen();
		album.DisplayAlbumNameOnScreen();
		album.DisplayArtistOnScreen();
		album.DisplayDateOnScreen();
		cout << endl;
	}
}

int Application::SearchAlbumByMusic()
{
	string musicName;
	cout << "\tMusic Name : ";
	
	getline(cin, musicName);

	albumList.ResetList();

	Album album;

	int success = 0;

	cout << "\t---------------" << endl;
	for (int i = 0; i < albumList.GetLength(); i++)
	{
		albumList.GetNextItem(album);
		album.ResetList();

		SimpleMusicType music;

		for (unsigned int j = 0; j < album.GetMusicNum(); j++)
		{
			album.GetNextMusic(music);

			if ((int)(music.GetName().find(musicName)) >= 0)
			{
				success++;
				cout << "\tAlbum Num: " << success << endl;
				cout << "\t---------------" << endl;
				album.DisplayAllOnScreen();
				cout << endl;
				break;
			}
		}
	}

	if (success > 0) return 1;
	cout << "\tNo album found" << endl;
	return 0;
}

int Application::SearchArtistByID()
{
	string artistID;
	cout << "\tArtist ID: ";
	
	getline(cin, artistID);

	cout << "\t---------------" << endl;
	Artist artist;
	artist.SetID(artistID);

	if (artistList.Get(artist) == 0)
	{
		cout << "\tThe artist doesn't exist" << endl;
		return 0;
	}

	artist.DisplayAllOnScreen();
	cout << endl;
	return 1;
}

int Application::SearchArtistByName()
{
	string artistName;
	cout << "\tArtist Name: ";
	
	getline(cin, artistName);

	cout << "\t---------------" << endl;

	int success = 0;
	artistList.ResetList();

	for (int i = 0; i < artistList.GetLength(); i++)
	{
		Artist temp;
		artistList.GetNextItem(temp);

		if ((int)(temp.GetName().find(artistName)) >= 0)
		{
			success++;
			cout << "\tArtist Num: " << success << endl;
			cout << "\t---------------" << endl;
			temp.DisplayAllOnScreen();
			cout << endl;
		}
	}

	if (success >= 0) return 1;
	cout << "\tNo artist found" << endl;
	return 0;
}

int Application::SearchArtistByAlbum()
{
	string albumName;
	cout << "\tAlbum Name: ";
	
	getline(cin, albumName);

	cout << "\t---------------" << endl;

	int success = 0;
	artistList.ResetList();

	for (int i = 0; i < artistList.GetLength(); i++)
	{
		Artist temp;
		artistList.GetNextItem(temp);

		Album album;

		temp.ResetList();

		while (temp.GetNextAlbum(album))
		{

			if ((int)(album.GetAlbumName().find(albumName)) >= 0)
			{
				success++;
				cout << "\tArtist Num: " << success << endl;
				cout << "\t---------------" << endl;
				temp.DisplayAllOnScreen();
				cout << endl;
				break;
			}
		}
	}

	if (success >= 0) return 1;
	cout << "\tNo artist found" << endl;
	return 0;
}

void Application::DisplayArtistByName()
{
	if (artistList.GetLength() == 0)
	{
		cout << "\tThere is no artist added" << endl;
		return;
	}

	artistList.ResetList();
	Artist artist;

	cout << "\t---------------" << endl;
	for (int i = 0; i < artistList.GetLength(); i++)
	{
		cout << "\tArtist Num: " << i + 1 << endl;
		cout << "\t---------------" << endl;
		artistList.GetNextItem(artist);

		artist.DisplayIDOnScreen();
		artist.DisplayNameOnScreen();
		artist.DisplayBirthDateOnScreen();
		cout << endl;
	}
}

int Application::GetAlbumCommand()
{
	int command;
	cout << endl << endl;
	cout << "\t---ID -- Command ----- " << endl;
	cout << "\t   1 : Add album" << endl;
	cout << "\t   2 : Delete album" << endl;
	cout << "\t   3 : Replace album" << endl;
	cout << "\t   4 : Add music to album" << endl;
	cout << "\t   5 : Delete music from album" << endl;
	cout << "\t   6 : Display album" << endl;
	cout << "\t   7 : Search album by name" << endl;
	cout << "\t   8 : Search album by artist" << endl;
	cout << "\t   9 : Search album by music name" << endl;
	cout << "\t   10 : Display all albums" << endl;
	cout << "\t   11 : Display all albums by artist" << endl;
	cout << "\t   12 : Display all albums by date" << endl;
	cout << "\t   13 : Clear empty albums" << endl;
	cout << "\t   0 : Return to the main menu" << endl;

	cout << endl << "\t Choose a Command--> ";
	cin >> command;

	while (cin.fail())
	{
		cout << "\t You should input integer value" << endl;
		cin.clear();
		cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
		cout << endl << "\t Choose a Command--> ";
		cin >> command;
	}

	cin.ignore();
	cout << endl;

	return command;
}

int Application::ShowAlbumMenu()
{
	while (true)
	{
		cout << "\t-----Album Menu-----" << endl;
		int command = GetAlbumCommand();
		switch (command)
		{
		case 1:
			AddAlbum();
			break;

		case 2:
			DeleteAlbum();
			break;

		case 3:
			ReplaceAlbum();
			break;

		case 4:
			AddMusicToAlbum();
			break;

		case 5:
			DeleteMusicFromAlbum();
			break;

		case 6:
			SearchAlbumByID();
			break;

		case 7:
			SearchAlbumByName();
			break;

		case 8:
			SearchAlbumByArtist();
			break;

		case 9:
			SearchAlbumByMusic();
			break;

		case 10:
			DisplayAlbumByName();
			break;

		case 11:
			DisplayAlbumByArtist();
			break;

		case 12:
			DisplayAlbumByDate();
			break;

		case 13:
			ClearEmptyAlbums();
			break;

		case 0:
			return 0;

		default:
			cout << "\tIllegal selection...\n";
			cin.ignore();
			break;
		}
	}
}

int Application::GetArtistCommand()
{
	int command;
	cout << endl << endl;

	cout << "\t---ID -- Command ----- " << endl;
	cout << "\t   1 : Add artist" << endl;
	cout << "\t   2 : Delete artist" << endl;
	cout << "\t   3 : Replace artist" << endl;
	cout << "\t   4 : Add album to artist" << endl;
	cout << "\t   5 : Delete album from artist" << endl;
	cout << "\t   6 : Display artist" << endl;
	cout << "\t   7 : Search artist by name" << endl;
	cout << "\t   8 : Search artist by album name" << endl;
	cout << "\t   9 : Display all artists" << endl;
	cout << "\t   10 : Clear empty artists" << endl;
	cout << "\t   0 : Return to the main menu" << endl;

	cout << endl << "\t Choose a Command--> ";
	cin >> command;
	cin.ignore();
	cout << endl;

	return command;
}

int Application::ShowArtistMenu()
{
	while (true)
	{
		cout << "\t-----Artist Menu-----" << endl;
		int command = GetArtistCommand();
		switch (command)
		{
		case 1:
			AddArtist();
			break;

		case 2:
			DeleteArtist();
			break;

		case 3:
			ReplaceArtist();
			break;

		case 4:
			AddAlbumToArtist();
			break;

		case 5:
			DeleteAlbumFromArtist();
			break;

		case 6:
			SearchArtistByID();
			break;

		case 7:
			SearchArtistByName();
			break;

		case 8:
			SearchArtistByAlbum();
			break;

		case 9:
			DisplayArtistByName();
			break;

		case 10:
			ClearEmptyArtists();
			break;

		case 0:
			return 0;

		default:
			cout << "\tIllegal selection...\n";
			cin.ignore();
			break;
		}
	}
}

int Application::AddAlbum()
{
	Album album;

	album.SetAlbumNameFromKB();
	album.SetArtistFromKB();
	album.SetRecordFromKB();
	album.SetDateFromKB();

	album.SetID(album.GetAlbumName() + '_' + album.GetArtist());

	if (albumList.Add(album) == 0)
	{
		cout << "\tAlbum that has the same ID already exists" << endl;
		return 0; //이미 존재하는 앨범이면 실패 
	}

	cout << endl;
	cout << "\tAlbum Added. ID = " << album.GetID() << endl << endl;
	return 1;
}

int Application::DeleteAlbum()
{
	Album album;

	album.SetIDFromKB();

	if (albumList.Delete(album))
	{
		Artist artist;
		artistList.ResetList();

		while (artistList.GetNextItem(artist)) if (artist.DeleteAlbum(album)) artistList.Replace(artist);

		cout << "\tAlbum has been successfully deleted" << endl;
		return 1;
	}

	cout << "\tThere is no album that has such ID" << endl;
	return 0;
}

int Application::ReplaceAlbum()
{
	Album album;

	album.SetAlbumNameFromKB();
	album.SetArtistFromKB();
	album.SetRecordFromKB();
	album.SetDateFromKB();

	album.SetID(album.GetAlbumName() + '_' + album.GetArtist());
	if (albumList.Replace(album))
	{
		cout << "\tAlbum has been successfully replaced" << endl;

		artistList.ResetList();
		Artist artist;

		while (artistList.GetNextItem(artist)) if (artist.ReplaceAlbum(album)) artistList.Replace(artist);
		return 1;
	}

	cout << "\tThere is no album that has such ID" << endl;
	return 0;
}

int Application::AddMusicToAlbum()
{
	Album album;
	cout << "\tSelect the album you want to add music" << endl;
	album.SetIDFromKB();

	if (!albumList.Get(album))
	{
		cout << "\tThere is no album that has such name" << endl;
		return 0;
	}

	cout << "\tCurrent Album" << endl;
	cout << "\t---------------" << endl;
	album.DisplayAllOnScreen();

	cout << endl;

	SimpleMusicType simpleMusic;
	cout << "\tSelect the music you want to add" << endl;
	simpleMusic.SetIDFromKB();

	if (nameList.Get(simpleMusic) == 0) //simpleMusic은 현재 유효한 이름을 가지고 있지 않으므로 ID 검색이 가능
	{
		cout << "\tThere is no music that has such ID" << endl;
		return 0;
	}

	if (album.AddMusic(simpleMusic))
	{
		albumList.Replace(album);
		cout << "\tAlbum successfully added the music" << endl;
		return 1;
	}
	
	cout << "\tAlbum already has the music" << endl;
	return 0;
}

int Application::DeleteMusicFromAlbum()
{
	Album album;
	cout << "\tSelect the album you want to delete music" << endl;
	album.SetIDFromKB();

	if (!albumList.Get(album))
	{
		cout << "\tThere is no album that has such name" << endl;
		return 0;
	}

	cout << "\tCurrent Album" << endl;
	cout << "\t---------------" << endl;
	album.DisplayAllOnScreen();

	cout << endl;

	SimpleMusicType simpleMusic;
	cout << "\tSelect the music you want to delete" << endl;
	simpleMusic.SetIDFromKB();

	if (album.DeleteMusic(simpleMusic))
	{
		cout << "\tAlbum successfully deleted the music" << endl;
		albumList.Replace(album);
		return 1;
	}

	cout << "\tAlbum doesn't have the music" << endl;
	return 0;
}

int Application::AddArtist()
{
	Artist artist;
	artist.SetNameFromKB();
	artist.SetBirthDateFromKB();

	artist.SetID(artist.GetName() + '_' + std::to_string(artist.GetBirthDate()));
	
	if (artistList.Add(artist) == 0)
	{
		cout << "\tArtist that has the same ID already exists" << endl;
		return 0; //이미 존재하는 아티스트면 실패 
	}

	cout << endl;
	cout << "\tArtist Added. ID = " << artist.GetID() << endl << endl;

	return 1;
}

int Application::DeleteArtist()
{
	Artist artist;
	artist.SetIDFromKB();
	if (artistList.Delete(artist))
	{
		cout << "\tArtist has been successfully deleted" << endl;
		return 1;
	}

	cout << "\tThere is no artist that has such ID" << endl;
	return 0;
}

int Application::ReplaceArtist()
{
	Artist artist;

	artist.SetNameFromKB();
	artist.SetBirthDateFromKB();

	artist.SetID(artist.GetName() + '_' + std::to_string(artist.GetBirthDate()));
	if (artistList.Replace(artist))
	{
		cout << "\tArtist has been successfully replaced" << endl;
		return 1;
	}

	cout << "\tThere is no artist that has such ID" << endl;
	return 0;
}

int Application::AddAlbumToArtist()
{
	Artist artist;
	cout << "\tSelect the artist you want to add album" << endl;
	artist.SetIDFromKB();

	if (!artistList.Get(artist))
	{
		cout << "\tThere is no artist that has such name" << endl;
		return 0;
	}

	cout << "\tCurrent Artist" << endl;
	cout << "\t---------------" << endl;
	artist.DisplayAllOnScreen();

	cout << endl;

	Album album;
	cout << "\tSelect the album you want to add" << endl;
	album.SetIDFromKB();

	if (!albumList.Get(album))
	{
		cout << "\tThere is no album that has such ID" << endl;
		return 0;
	}

	Album simpleAlbum;
	simpleAlbum.SetAlbumName(album.GetAlbumName());
	simpleAlbum.SetArtist(album.GetArtist());
	simpleAlbum.SetID(album.GetAlbumName() + '_' + album.GetArtist());
	simpleAlbum.SetDate(album.GetDate());

	if (artist.AddAlbum(simpleAlbum))
	{
		artistList.Replace(artist);
		cout << "\tArtist has successfully added the album" << endl;
		return 1;
	}

	cout << "\tArtist already has the album" << endl;
	return 0;
}

int Application::DeleteAlbumFromArtist()
{
	Artist artist;
	cout << "\tSelect the artist you want to delete album" << endl;
	artist.SetIDFromKB();

	if (!artistList.Get(artist))
	{
		cout << "\tThere is no artist that has such name" << endl;
		return 0;
	}

	cout << "\tCurrent Artist" << endl;
	cout << "\t---------------" << endl;
	artist.DisplayAllOnScreen();

	cout << endl;

	Album album;
	cout << "\tSelect the album you want to delete" << endl;
	album.SetIDFromKB();

	albumList.Get(album);

	if (artist.DeleteAlbum(album))
	{
		cout << "\tArtist successfully deleted the album" << endl;
		artistList.Replace(artist);
		return 1;
	}

	cout << "\tArtist doesn't have the album" << endl;
	return 0;
}

void Application::ClearEmptyArtists()
{
	artistList.ResetList();

	Artist artist;
	while (artistList.GetNextItem(artist))
	{
		if (artist.GetAlbumNum() != 0) continue;

		artistList.Delete(artist);
	}

	cout << "\tRemoved all empty artist data" << endl;
}

int Application::AddMusic(const MusicType& data)
{
	if (!musicList.Add(data)) return 0;

	SimpleMusicType music;
	music.SetID(data.GetID());
	music.SetName(data.GetName());
	music.SetLength(data.GetLength());
	music.SetPlayedTime(data.GetPlayedTime());

	nameList.Add(music);
	
	bool exist = false;

	genreList.ResetList();

	GenreType genre;

	while (genreList.GetNextItem(genre))
	{
		if (!strcmp(genre.GetGenre().c_str(), data.GetGenre().c_str()))
		{
			genre.Add(music);
			genreList.Replace(genre);
			exist = true;
			break;
		}
	}

	if (!exist)
	{
		GenreType newGenre;
		newGenre.SetGenre(data.GetGenre());
		newGenre.Add(music);
		genreList.Add(newGenre);
	}

	AddToMostPlayed(data);

	return 1;
}

int Application::PlayMusic()
{
	MusicType music;
	music.SetIDFromKB();
	if (musicList.Get(music) == 0)
	{
		cout << "\tThere is no music that has such ID" << endl;
		return 0;
	}

	music.SetPlayedTime(music.GetPlayedTime() + 1);
	musicList.Replace(music);
	AddToRecentPlayed(music);
	AddToMostPlayed(music);

	cout << "\tPlaying the music..." << endl;
	return 1;
}

void Application::OpenURLOnBrowser(const string& path)
{
	ShellExecuteA(0, 0, path.c_str(), 0, 0, SW_SHOW);
}