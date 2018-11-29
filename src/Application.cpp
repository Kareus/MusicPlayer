#include "Application.h"
#include "PlayListWriter.h"
#include "GlobalFunctions.h"
#include "DirectoryReader.h"

extern MediaPlayer* player;

Application::Application()
{
	focus = nullptr;
	m_Command = 0;
	recentListCount = 0;
	addedCount = 0;
	nameList.SetCompareFunction(compareMusicName);
	recentPlayedList.SetCompareFunction(compareToLast);
	mostPlayedList.SetCompareFunction(comparePlayedTime);
	drawings.SetCompareFunction(compareGraphics);
	backColor = sf::Color::White;
	drawings.MakeEmpty();
	currentGroup = nullptr;
	running = false;

	backgroundSprite = new Sprite("background.png");
}

Application::~Application()
{
	player->Release(); //�÷��̾� �޸� ����
	DoublyIterator<Graphic*> iter(drawings);

	while (iter.NotNull())
	{
		if (iter.Current() != nullptr) delete iter.Current();
		iter.Next();
	} //�׷��� �Ҵ� ����

	drawings.MakeEmpty();

	delete backgroundSprite;
}

void Application::Render()
{
	while (window.isOpen())
	{
		window.clear(backColor);

		DoublyIterator<Graphic*> iter(drawings);

		while (iter.NotNull())
		{
			iter.Current()->draw(&window); //�� �׷����� �������Ѵ�
			iter.Next();
		}
		window.display();
	}
}

int Application::AddGraphic(Graphic* graphic)
{
	graphic->setID(drawings.GetLength());
	if (graphic->getID() < 0) return 0; //�����÷ο�
	return drawings.Add(graphic);
}

void Application::Run(HINSTANCE instance)
{
	WNDCLASS WindowClass;
	WindowClass.style = 0;
	WindowClass.lpfnWndProc = &WndProc; //global function�� ���ǵ� �������� �̺�Ʈ�� ���
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = instance;
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = 0;
	WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = L"MusicPlayer";
	RegisterClass(&WindowClass); //������ ������ ���� class ���
	HWND Window = CreateWindow(L"MusicPlayer", L"Music Player Application", WS_SYSMENU | WS_VISIBLE, 0, 0, 800, 600, NULL, NULL, instance, NULL); //������ ����

	DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS & ~WS_SIZEBOX;
	HWND  View1 = CreateWindow(L"STATIC", NULL, Style, 0, 0, 800, 600, Window, NULL, instance, NULL); //�����쿡 ���� SFML View ����.

	window.create(View1); //������ View�� �����쿡 �Ҵ�

	MediaPlayer::create(Window, Window, &player); //�÷��̾� ����

	MSG Message;
	Message.message = ~WM_QUIT; //�ý������κ��� �޾ƿ� �������� �޼���

	window.setActive(false); //������ �и��� ���� ��Ȱ��ȭ.

	std::thread renderer([this]() {
		this->Render();

	}); //���� �Լ��� �����带 �����Ͽ� ����. (������ �޼��� ���Ű� ������ �������� �۵��Ѵ�.)
	renderer.detach(); //���� ������κ��� ������Ų��. (������ ���ư��� �ϱ� ����)
	//join �Ǵ� detach�� ȣ�������Ƿ� �� ������� �Լ��� ����Ǹ� �����ϰ� �����ȴ�.

	//test functions
	Group* group = new Group();

	TextBox* box = new TextBox(0, 0, 300, 36, true);
	box->loadFontFrom("C:/Windows/Fonts/malgun.ttf");
	box->setText(L"TextBox test.");
	group->AddGraphic(box);

	TextLabel* label = new TextLabel(L"TextLabel Test\nMulti Line");
	label->setFont(box->getFont());
	label->setCharacterSize(16);
	label->SetPosition(0, 200);
	group->AddGraphic(label);
	/*
	Sprite* sprite = new Sprite("C:/test.png");
	sprite->SetPosition(300, 300);
	sprite->SetButton(true);
	std::function<void(Sprite*)> func = [](Sprite*) {OutputDebugStringA("hello\n"); };
	sprite->setClickFunction(func);
	group->AddGraphic(sprite);
	*/

	AddGraphic(group);

	MusicType* m = new MusicType();
	m->SetName("song");
	m->SetArtist("artist");
	m->SetAlbum("album");

	currentGroup = new Group();
	AddDisplayGraphic(m);
	AddGraphic(currentGroup);

	delete m;
	//test functions end.

	Sleep(500); //������ ������ ������ ���̿� �̺�Ʈ�� �߻��ϴ� ��찡 �־ �ذ��ϱ� ���� 0.5�� ���
	running = true;

	while (Message.message != WM_QUIT) //���� �޽����� �ƴ� ��� ���� ������ ����.
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			//������κ��� �̺�Ʈ ������ ������ �޽��� ����
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	window.close(); //SFML ���� ����

	running = false;

	DestroyWindow(Window); //����Ǹ� �����츦 �����Ѵ�.

	UnregisterClass(L"MusicPlayer", instance); //Ŭ���� ����� �����ϰ� ����.
}

bool Application::IsRunning()
{
	return running;
}

bool Application::pollEvent(CustomWinEvent e)
{
	DoublyIterator<Graphic*> iter(drawings);
	iter.ResetToLastPointer();
	Graphic* g;

	switch (e.type)
	{
	default:
		if (focus) focus->pollEvent(e);
		break;
	}

	return true;
}

bool Application::pollEvent(sf::Event e)
{
	CustomWinEvent custom;
	DoublyIterator<Graphic*> iter(drawings);
	iter.ResetToLastPointer(); //id�� z-order ������ �ϹǷ� ���� �˻�
	Graphic* g;

	switch (e.type)
	{
	case sf::Event::MouseButtonPressed:

		if (focus) //������ ��Ŀ���� ���� ���
		{
			focus->setFocus(false); //������ ��Ŀ���� ����
			focus = nullptr; //��Ŀ�� ��ü�� null�� ����
		}

		while (iter.NotNull()) //��Ŀ���� �׷����� ã�´�.
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouseButton.x, e.mouseButton.y)))
			{
				focus = g; // ��Ŀ�̿� �����ϸ� ��Ŀ�� ��ü�� ����
				break;
			}

			iter.Prev();
		}

		if (focus)
		{
			focus->setFocus(true); //��Ŀ�� ��ü�� ��Ŀ�� ����
			focus->pollEvent(e);
		}
		break;

	case sf::Event::MouseMoved:
		while (iter.NotNull()) //���콺�� �÷��� ���� ��Ŀ���� �ƴϾ ó���ؾ� ��
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouseMove.x, e.mouseMove.y)))
			{
				custom.type = CustomWinEvent::MouseOver;
				custom.mouseOver = CustomWinEvent::MouseOverEvent();
				custom.mouseOver.x = e.mouseMove.x;
				custom.mouseOver.y = e.mouseMove.y;
				g->pollEvent(custom); // ��Ŀ������ ���������� �ʴ´�.
				break;
			}
			else g->pollEvent(e); //���콺 �����ӿ� ���� �̺�Ʈ ����

			iter.Prev();
		}
		break;

	case sf::Event::MouseLeft:
		while (iter.NotNull()) //���콺�� ������ ������ ������ �� �̺�Ʈ ó��
		{
			iter.Current()->pollEvent(e);
			iter.Prev();
		}
		break;

	default:
		if (focus) focus->pollEvent(e);
		break;
	}

	return true;
}

Group* Application::AddDisplayGraphic(MusicType* data)
{
	Group* group = new Group();
	Sprite* play_nodeSprite = nullptr;
	Sprite* edit_nodeSprite = nullptr;

	Sprite* background = backgroundSprite->clone();
	background->SetButton(true);
	std::wstring str = String::StrToWstr(data->GetName() + '\n' + data->GetArtist() + " - " + data->GetAlbum());
	TextLabel* info = new TextLabel(str);
	//Sprite* playButton = play_nodeSprite->clone();
	//Sprite* editButton = edit_nodeSprite->clone();

	group->AddGraphic(background);
	group->AddGraphic(info);
	//group->AddGraphic(playButton);
	//group->AddGraphic(editButton);

	currentGroup->AddGraphic(group);

	return group;
}

void Application::DisplayAllMusic()
{
	
	if (currentGroup == nullptr)
	{
		currentGroup = new Group();
		AddGraphic(currentGroup);
	}

	currentGroup->MakeEmpty();

	//avltree do function does here


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
		Stream::IgnoreJunk(cin);
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