#include "Application.h"
#include <Windows.h>
#include "PlayListWriter.h"
#include "AVLTree.h"

#undef max //max macro ȥ�� ����

Application::Application()
{
	m_Command = 0;
	recentListCount = 0;
	addedCount = 0;
	nameList.SetCompareFunction(compareMusicName);
	recentPlayedList.SetCompareFunction(compareToLast);
	mostPlayedList.SetCompareFunction(comparePlayedTime);
	/*
	ID�� ����_�����̸� ������, �� ���� ����
	1. ABC_ARTIST1
	2. ABCA_ARTIST2
	�϶� ���� �����δ� 1 -> 2������
	ID�����δ� 2 -> 1 (A�� ascii code�� 65, _�� ascii code�� 95)�̹Ƿ�
	�̷� ��Ȳ�� ���� ���� compare function�� ���� ����.
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
		case 1:	// ���� �޴�
			ShowMusicMenu();
			break;
		case 2:	// �ٹ� �޴�
			ShowAlbumMenu();
			break;
		case 3:	// ��Ƽ��Ʈ �޴�
			ShowArtistMenu();
			break;
		case 4:	// txt ���Ͽ��� ���������� �о�´�
			ReadDataFromFile();
			break;
		case 5:	// txt ���Ͽ� ���������� ����
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
	// �Է¹��� ���ڵ带 ����Ʈ�� add

	MusicType music;

	music.SetAllFromKB();
	music.SetID(music.GetName() + '_' + music.GetArtist());

	if (musicList.Add(music) == 0)
	{
		cout << "\tMusic that has the same ID already exists" << endl;
		return 0; //�̹� �����ϴ� �����̸� ���� 
	}

	cout << endl;
	cout << "\tMusic Added. ID = " << music.GetID() << endl << endl;

	//�帣 Ȯ��
	bool exist = false;
	DoublyIterator<GenreType> iter(genreList);
	GenreType* genre;

	SimpleMusicType simpleMusic; //������ ������ ������ ���� ������
	simpleMusic.SetName(music.GetName());
	simpleMusic.SetID(music.GetID());
	simpleMusic.SetLength(music.GetLength());
	simpleMusic.SetPlayedTime(music.GetPlayedTime());

	while (iter.NotNull())
	{
		genre = iter.CurrentPtr();
		
		if (music.GetGenre() == genre->GetGenre()) //�帣�� �̹� �ִٸ�
		{
			exist = true;

			genre->Add(simpleMusic);

			//pointer�̹Ƿ� replace ���� �߰� �۾��� �ʿ����� ����
			break;
		}

		iter.Next();
	}

	if (!exist) //����Ʈ�� ���ٸ�
	{
		GenreType newGenre;
		newGenre.SetGenre(music.GetGenre());
		newGenre.Add(simpleMusic);
		genreList.Add(newGenre);
	}

	if (newAddMusicList.IsFull()) //�̹� �� �� �ִٸ�
	{
		SimpleMusicType temp;
		newAddMusicList.DeQueue(temp);
	}
	else addedCount++;

	newAddMusicList.EnQueue(simpleMusic); //�ֱ� ���� �߰�
	
	nameList.Add(simpleMusic); //��� ������ ����

	AddToMostPlayed(music); //���� ���� ����� ��Ͽ� �߰�

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
	MusicType* data;

	cout << "\n\tCurrent music list" << endl;

	// list�� ��� �����͸� ȭ�鿡 ���
	
	DoublyIterator<MusicType> iter(musicList);

	int curIndex = 1;
	while (iter.NotNull())
	{
		data = iter.CurrentPtr();
		cout << "\tMusic Num: " << curIndex << endl;
		cout << "\t---------------" << endl;
		data->DisplayAllOnScreen();
		cout << endl;
		curIndex++;
		iter.Next();
	}

	if (musicList.GetLength() == 0) cout << "\tThere is no music added" << endl;
}

int Application::OpenInFile(char *fileName)
{
	m_inputFile.open(fileName);	// file open for reading.

	// ���� ���¿� �����ϸ� 1, �׷��� �ʴٸ� 0�� ����.
	if (!m_inputFile)	return 0;
	return 1;
}

int Application::OpenOutFile(char *fileName)
{
	m_outFile.open(fileName);	// file open for writing.

	// ���� ���¿� �����ϸ� 1, �׷��� �ʴٸ� 0�� ����.
	if (!m_outFile)	return 0;
	return 1;
}

int Application::ReadDataFromFile()
{
	//have to work with genre file i/o

	int index = 0;
	MusicType data;	// �б�� �ӽ� ����
	Album album;
	Artist artist;
	SimpleMusicType music;

	char filename[FILENAMESIZE];
	cout << "\n\tEnter Input file Name : ";
	cin.getline(filename, FILENAMESIZE);

	// file open, open error�� �߻��ϸ� 0�� ����
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

	// ���� list ���
	DisplayAllMusic();

	return 1;
}

int Application::WriteDataToFile()
{
	MusicType* data;	// ����� �ӽ� ����
	Album* album;
	Artist* artist;
	SimpleMusicType music;

	char filename[FILENAMESIZE];
	cout << "\n\tEnter Output file Name : ";
	cin.getline(filename, FILENAMESIZE);

	// file open, open error�� �߻��ϸ� 0�� ����
	if (!OpenOutFile(filename))
		return 0;

	m_outFile << musicList.GetLength() << endl; //���� �� ���

	// iterator ����
	DoublyIterator<MusicType> iter_m(musicList);
	DoublyIterator<Album> iter_al(albumList);
	DoublyIterator<Artist> iter_ar(artistList);

	// list�� ��� �����͸� ���Ͽ� ����
	while (iter_m.NotNull())
	{
		data = iter_m.CurrentPtr();
		data->WriteDataToFile(m_outFile);
		iter_m.Next();
	}

	m_outFile << albumList.GetLength() << endl;

	while (iter_al.NotNull())
	{
		album = iter_al.CurrentPtr();
		album->WriteDataToFile(m_outFile);
		iter_al.Next();
	}

	m_outFile << artistList.GetLength() << endl;

	while (iter_ar.NotNull())
	{
		artist = iter_ar.CurrentPtr();
		artist->WriteDataToFile(m_outFile);
		iter_ar.Next();
	}

	m_outFile << addedCount << endl;

	newAddMusicList.ResetQueue();

	for (int i = 0; i < addedCount; i++)
	{
		newAddMusicList.GetNextItem(music);
		music.WriteDataToFile(m_outFile);
	}

	m_outFile << recentListCount << endl;

	SimpleMusicType* simple;

	DoublyIterator<SimpleMusicType> iter_s(recentPlayedList);

	while (iter_s.NotNull())
	{
		simple = iter_s.CurrentPtr();
		simple->WriteDataToFile(m_outFile);
		iter_s.Next();
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

	DoublyIterator<MusicType> iter(musicList);
	MusicType* data = NULL; //����Ʈ���� ã�� �����ۿ� ���� ������ ����

	while (iter.NotNull())
	{
		data = iter.CurrentPtr();
		if (music == *data) break;
		iter.Next();
	}

	if (data == NULL)
	{
		cout << "\tThere is no music that has such ID" << endl;
		return 0;
	}
	
	data->SetAllFromKB();

	data->SetID(data->GetName() + '_' + data->GetArtist());

	music = *data; //backup

	musicList.Delete(iter); //iterator�� �̿��� ������ �������� �����Ѵ�.

	if (ReplaceMusic(music)) //�ٽ� �����Ͽ� �߰�
	{
		cout << "\tMusic has been successfully replaced" << endl;
		return 1;
	}

	cout << "\tFailed to replace the music" << endl;
	return 0;
}

int Application::SearchMusicByName()
{
	string name;
	cout << "\t Name: ";
	getline(cin, name);

	int success = 0;
	
	DoublyIterator<SimpleMusicType> iter(nameList);

	cout << "\t---------------" << endl;
	
	SimpleMusicType* data;
	while (iter.NotNull())
	{
		data = iter.CurrentPtr();

		if ((int)data->GetName().find(name) >= 0)
		{
			success++;
			cout << "\tMusic num : " << success << endl;
			data->DisplayIDOnScreen();
			data->DisplayNameOnScreen();
			cout << endl;
		}
		iter.Next();
	}

	if (success > 0) return 1;
	else cout << "\tMusic doesn't exist" << endl;

	return 0;
}

int Application::SearchMusicByArtist()
{
	string artist;
	cout << "\t Artist: ";
	getline(cin, artist);

	int success = 0;

	DoublyIterator<MusicType> iter(musicList);
	cout << "\t---------------" << endl;

	MusicType* data;
	while (iter.NotNull())
	{
		data = iter.CurrentPtr();

		if ((int)data->GetArtist().find(artist) >= 0)
		{
			success++;
			cout << "\tMusic Num: " << success << endl;
			cout << "\t---------------" << endl;
			data->DisplayIDOnScreen();
			data->DisplayNameOnScreen();
			cout << endl;
		}
		iter.Next();
	}

	if (success > 0) return 1;
	else cout << "\tMusic doesn't exist" << endl;

	return 0;
}

int Application::SearchMusicByAlbum()
{
	string album;
	cout << "\t Album: ";
	getline(cin, album);

	int success = 0;

	DoublyIterator<MusicType> iter(musicList);
	cout << "\t---------------" << endl;

	MusicType* data;
	while (iter.NotNull())
	{
		data = iter.CurrentPtr();
		if ((int)data->GetAlbum().find(album) >= 0)
		{
			success++;
			cout << "\tMusic Num: " << success << endl;
			cout << "\t---------------" << endl;
			data->DisplayIDOnScreen();
			data->DisplayNameOnScreen();
			cout << endl;
		}
		iter.Next();
	}

	if (success > 0) return 1;
	else cout << "\tMusic doesn't exist" << endl;

	return 0;
}

int Application::SearchMusicByGenre()
{
	string genre;
	cout << "\t Genre: ";
	getline(cin, genre);

	int success = 0;

	DoublyIterator<MusicType> iter(musicList);
	cout << "\t---------------" << endl;

	MusicType* data;
	while (iter.NotNull())
	{
		data = iter.CurrentPtr();
		if ((int)data->GetGenre().find(genre) >= 0)
		{
			success++;
			cout << "\tMusic Num: " << success << endl;
			cout << "\t---------------" << endl;
			data->DisplayIDOnScreen();
			data->DisplayNameOnScreen();
			cout << endl;
		}
		iter.Next();
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

	cout << "\t�ֱ� �߰��� ���ǵ�" << endl;
	cout << "\t---------------" << endl;

	if (newAddMusicList.IsEmpty())
	{
		cout << "\tThere is no music recently added!" << endl;
		return;
	}

	for (int i = 0; i < 30; i++)
	{
		if (newAddMusicList.GetNextItem(music) < 0) break; //�������� �� ������ ����

		music.DisplayIDOnScreen();
		music.DisplayNameOnScreen();
		cout << endl;
	}
}

void Application::DisplayMusicByGenre()
{
	cout << "\t�帣�� ���ǵ�" << endl;

	if (genreList.GetLength() == 0)
	{
		cout << "There is no genres added!" << endl;
		return;
	}

	DoublyIterator<GenreType> iter(genreList);
	GenreType* genre;
	SimpleMusicType* music;

	while (iter.NotNull())
	{
		genre = iter.CurrentPtr();

		cout << "Genre : " << genre->GetGenre() << endl;
		cout << "\t---------------" << endl;

		DoublyIterator<SimpleMusicType> iter_s = genre->GetIterator();

		int count = 1;
		while (iter_s.NotNull())
		{
			cout << "\tMusic Num: " << count << endl;
			cout << "\t---------------" << endl;
			music = iter_s.CurrentPtr();
			music->DisplayIDOnScreen();
			music->DisplayNameOnScreen();
			cout << endl;
			count++;
			iter_s.Next();
		}

		cout << endl;
		iter.Next();
	}
}

void Application::RetreiveByNameNGenre()
{
	string name, genre;
	cout << "\t Name: ";
	getline(cin, name);
	cout << "\t Genre: ";
	getline(cin, genre);

	DoublyIterator<MusicType> iter(musicList);
	MusicType* music;

	int success = 0;

	cout << "\t---------------" << endl;
	while (iter.NotNull())
	{
		music = iter.CurrentPtr();
		if (music->GetName() != name) continue; //�̸��� �ٸ��� ��ŵ
		if (music->GetGenre() != genre) continue; //�帣�� �ٸ��� ��ŵ
		
		music->DisplayAllOnScreen(); //�߰������Ƿ� ���
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

	recentPlayedList.Delete(simpleMusic); //���� �ִٸ� ����
	recentPlayedList.Add(simpleMusic);

	if (recentPlayedList.GetLength() > 30)
	{
		DoublyIterator<SimpleMusicType> iter(recentPlayedList);
		recentPlayedList.Delete(iter.First());
	}
}

void Application::AddToMostPlayed(const MusicType& music)
{
	SimpleMusicType simpleMusic;
	simpleMusic.SetID(music.GetID());
	simpleMusic.SetName(music.GetName());
	simpleMusic.SetPlayedTime(music.GetPlayedTime());
;
	mostPlayedList.Delete(simpleMusic); //���� �ִٸ� ����
	mostPlayedList.Add(simpleMusic);

	if (mostPlayedList.GetLength() > 30)
	{
		DoublyIterator<SimpleMusicType> iter(mostPlayedList);
		iter.ResetToLastPointer();
		mostPlayedList.Delete(iter);
	}
}

void Application::DisplayRecentPlayed()
{
	cout << "\t�ֱ� ����� 30��" << endl;

	SimpleMusicType* music;
	DoublyIterator<SimpleMusicType> iter(recentPlayedList);

	if (recentPlayedList.GetLength() == 0)
	{
		cout << "\tThere is no music recently played!" << endl;
		return;
	}

	cout << "\t---------------" << endl;

	int count = 1;
	while (iter.NotNull())
	{
		music = iter.CurrentPtr();

		cout << "\tMusic Num: " << count << endl;
		cout << "\t---------------" << endl;
		music->DisplayIDOnScreen();
		music->DisplayNameOnScreen();
		count++;
		cout << endl;
		iter.Next();
	}
}

void Application::DisplayMostPlayed()
{
	cout << "\t���� ���� ����� 30��" << endl;
	SimpleMusicType* music;
	DoublyIterator<SimpleMusicType> iter(mostPlayedList);

	if (mostPlayedList.GetLength() == 0)
	{
		cout << "\tThere is no music most played!" << endl;
		return;
	}

	cout << "\t---------------" << endl;
	
	int count = 1;
	while (iter.NotNull())
	{
		music = iter.CurrentPtr();
		cout << "\tMusic Num: " << count << endl;
		cout << "\t---------------" << endl;
		music->DisplayIDOnScreen();
		music->DisplayNameOnScreen();
		music->DisplayPlayedTimeOnScreen();
		iter.Next();
		count++;
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

	DoublyIterator<GenreType> iter(genreList);

	GenreType* genre;

	while (iter.NotNull())
	{
		genre = iter.CurrentPtr();
		if (!strcmp(genre->GetGenre().c_str(), music.GetGenre().c_str()))
		{
			genre->Replace(simple);
			break;
		}
		iter.Next();
	}

	Album* album;
	DoublyIterator<Album> iter_a(albumList);

	while (iter_a.NotNull())
	{
		album = iter_a.CurrentPtr();
		album->ReplaceMusic(simple);
		iter_a.Next();
	}

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

	GenreType* genre = NULL;

	DoublyIterator<GenreType> iter(genreList);

	while (iter.NotNull())
	{
		genre = iter.CurrentPtr();
		if (genre->Delete(simpleMusic)) break;
		iter.Next();
	}

	if (genre != NULL && genre->GetMusicNum() == 0) genreList.Delete(iter);

	Album* album;
	
	DoublyIterator<Album> iter_a(albumList);

	while (iter_a.NotNull())
	{
		album = iter_a.CurrentPtr();
		if (album->DeleteMusic(simpleMusic)) break;
		iter_a.Next();
	}

	recentPlayedList.Delete(simpleMusic);
	mostPlayedList.Delete(simpleMusic);

	return 1;
}

int Application::GetArtistByName(const string& name, Artist& data)
{
	Artist* artist;

	DoublyIterator<Artist> iter(artistList);

	while (iter.NotNull())
	{
		artist = iter.CurrentPtr();

		if (artist->GetName() == name)
		{
			data = *artist;
			return 1;
		}
		iter.Next();
	}
	return 0;
}

void Application::ClearEmptyAlbums()
{
	Album* album;
	Artist* artist;

	DoublyIterator<Album> iter(albumList);

	while (iter.NotNull())
	{
		album = iter.CurrentPtr();

		if (album->GetMusicNum() != 0) continue;

		albumList.Delete(iter);

		DoublyIterator<Artist> iter_a(artistList);

		while (iter_a.NotNull())
		{
			artist = iter_a.CurrentPtr();
			if (!strcmp(artist->GetName().c_str(), album->GetArtist().c_str())) artist->DeleteAlbum(*album);
			iter_a.Next();
		}
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
	cout << "\tComposer : ";
	string composer;
	getline(cin, composer);

	int success = 0;
	cout << "\t---------------" << endl;

	DoublyIterator<MusicType> iter(musicList);
	MusicType* music;
	
	while (iter.NotNull())
	{
		music = iter.CurrentPtr();

		if ((int)(music->GetComposer().find(composer)) >= 0)
		{
			success++;
			cout << "\tMusic Num: " << success << endl;
			cout << "\t---------------" << endl;
			music->DisplayIDOnScreen();
			music->DisplayNameOnScreen();
			cout << endl;
		}

		iter.Next();
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

	DoublyIterator<Album> iter(albumList);
	Album* temp;

	int success = 0;

	cout << "\t---------------" << endl;
	
	while (iter.NotNull())
	{
		temp = iter.CurrentPtr();

		if ((int)(temp->GetAlbumName().find(albumName)) >= 0)
		{
			success++;
			cout << "\tAlbum Num: " << success << endl;
			cout << "\t---------------" << endl;
			temp->DisplayAllOnScreen();
			cout << endl;
		}

		iter.Next();
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

	DoublyIterator<Album> iter(albumList);
	Album* temp;
	
	cout << "\t---------------" << endl;
	int success = 0;
	while (iter.NotNull())
	{
		temp = iter.CurrentPtr();

		if ((int)(temp->GetArtist().find(artist)) >= 0)
		{
			success++;
			cout << "\tAlbum Num: " << success << endl;
			cout << "\t---------------" << endl;
			temp->DisplayAllOnScreen();
			cout << endl;
		}

		iter.Next();
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

	//primary key�� �̸� ���̹Ƿ� �׳� ���
	Album* album;
	DoublyIterator<Album> iter(albumList);

	int count = 1;
	cout << "\t---------------" << endl;
	while (iter.NotNull())
	{
		album = iter.CurrentPtr();
		cout << "\tAlbum Num: " << count << endl;
		cout << "\t---------------" << endl;

		album->DisplayIDOnScreen();
		album->DisplayAlbumNameOnScreen();
		album->DisplayArtistOnScreen();
		cout << endl;

		iter.Next();
		count++;
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
	}); //��Ƽ��Ʈ ������ �����ϵ��� ����

	//������ ������ ����

	Album album;

	DoublyIterator<Album> iter(albumList);

	while (iter.NotNull())
	{
		album = iter.Current();
		list.Add(album);
		iter.Next();
	}


	cout << "\t---------------" << endl;
	
	DoublyIterator<Album> iter2(list);
	int count = 1;

	while (iter2.NotNull())
	{
		album = iter2.Current();
		cout << "\tAlbum Num: " << count << endl;
		cout << "\t---------------" << endl;
		album.DisplayIDOnScreen();
		album.DisplayAlbumNameOnScreen();
		album.DisplayArtistOnScreen();
		cout << endl;

		iter2.Next();
		count++;
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
	}); //��¥ ������ �����ϵ��� ����

	//������ ������ ����
	Album album;

	DoublyIterator<Album> iter(albumList);

	while (iter.NotNull())
	{
		album = iter.Current();
		list.Add(album);
		iter.Next();
	}


	cout << "\t---------------" << endl;

	DoublyIterator<Album> iter2(list);
	int count = 1;

	while (iter2.NotNull())
	{
		album = iter2.Current();
		cout << "\tAlbum Num: " << count << endl;
		cout << "\t---------------" << endl;
		album.DisplayIDOnScreen();
		album.DisplayAlbumNameOnScreen();
		album.DisplayArtistOnScreen();
		album.DisplayDateOnScreen();
		cout << endl;

		iter2.Next();
		count++;
	}
}

int Application::SearchAlbumByMusic()
{
	string musicName;
	cout << "\tMusic Name : ";
	
	getline(cin, musicName);

	DoublyIterator<Album> iter(albumList);
	Album* album;

	int success = 0;

	cout << "\t---------------" << endl;
	while (iter.NotNull())
	{
		album = iter.CurrentPtr();

		DoublyIterator<SimpleMusicType> iter_s = album->GetIterator();

		SimpleMusicType* music;

		while (iter_s.NotNull())
		{
			music = iter_s.CurrentPtr();
			if ((int)(music->GetName().find(musicName)) >= 0)
			{
				success++;
				cout << "\tAlbum Num: " << success << endl;
				cout << "\t---------------" << endl;
				album->DisplayAllOnScreen();
				cout << endl;
				break;
			}
			iter_s.Next();
		}

		iter.Next();
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

	DoublyIterator<Artist> iter(artistList);
	Artist* temp;
	
	while (iter.NotNull())
	{
		temp = iter.CurrentPtr();

		if ((int)(temp->GetName().find(artistName)) >= 0)
		{
			success++;
			cout << "\tArtist Num: " << success << endl;
			cout << "\t---------------" << endl;
			temp->DisplayAllOnScreen();
			cout << endl;
		}

		iter.Next();
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
	DoublyIterator<Artist> iter(artistList);
	Artist* temp;

	while (iter.NotNull())
	{
		temp = iter.CurrentPtr();
		DoublyIterator<Album> iter_a = temp->GetIterator();

		Album* album;

		while (iter_a.NotNull())
		{
			album = iter_a.CurrentPtr();

			if ((int)(album->GetAlbumName().find(albumName)) >= 0)
			{
				success++;
				cout << "\tArtist Num: " << success << endl;
				cout << "\t---------------" << endl;
				temp->DisplayAllOnScreen();
				cout << endl;
				break;
			}

			iter_a.Next();
		}

		iter.Next();
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

	DoublyIterator<Artist> iter(artistList);
	Artist* artist;

	cout << "\t---------------" << endl;
	int count = 1;

	while (iter.NotNull())
	{
		artist = iter.CurrentPtr();
		cout << "\tArtist Num: " << count << endl;
		cout << "\t---------------" << endl;

		artist->DisplayIDOnScreen();
		artist->DisplayNameOnScreen();
		artist->DisplayBirthDateOnScreen();
		cout << endl;
		count++;
		iter.Next();
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
		return 0; //�̹� �����ϴ� �ٹ��̸� ���� 
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
		Artist* artist;
		DoublyIterator<Artist> iter(artistList);

		while (iter.NotNull())
		{
			artist = iter.CurrentPtr();
			artist->DeleteAlbum(album);
			iter.Next();
		}

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

		DoublyIterator<Artist> iter(artistList);
		Artist* artist;

		while (iter.NotNull())
		{
			artist = iter.CurrentPtr();
			artist->ReplaceAlbum(album);
			iter.Next();
		}
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

	if (nameList.Get(simpleMusic) == 0) //simpleMusic�� ���� ��ȿ�� �̸��� ������ ���� �����Ƿ� ID �˻��� ����
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
		return 0; //�̹� �����ϴ� ��Ƽ��Ʈ�� ���� 
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
	DoublyIterator<Artist> iter(artistList);
	Artist* artist;

	while (iter.NotNull())
	{
		artist = iter.CurrentPtr();
		if (artist->GetAlbumNum() != 0) continue;

		artistList.Delete(iter);
		iter.Next();
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

	DoublyIterator<GenreType> iter(genreList);
	GenreType* genre;

	while (iter.NotNull())
	{
		genre = iter.CurrentPtr();
		if (!strcmp(genre->GetGenre().c_str(), data.GetGenre().c_str()))
		{
			genre->Add(music);
			exist = true;
			break;
		}
		iter.Next();
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