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
	player->Release(); //플레이어 메모리 해제
	DoublyIterator<Graphic*> iter(drawings);

	while (iter.NotNull())
	{
		if (iter.Current() != nullptr) delete iter.Current();
		iter.Next();
	} //그래픽 할당 해제

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
			iter.Current()->draw(&window); //각 그래픽을 렌더링한다
			iter.Next();
		}
		window.display();
	}
}

int Application::AddGraphic(Graphic* graphic)
{
	graphic->setID(drawings.GetLength());
	if (graphic->getID() < 0) return 0; //오버플로우
	return drawings.Add(graphic);
}

void Application::Run(HINSTANCE instance)
{
	WNDCLASS WindowClass;
	WindowClass.style = 0;
	WindowClass.lpfnWndProc = &WndProc; //global function에 정의된 프로토콜 이벤트를 사용
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = instance;
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = 0;
	WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = L"MusicPlayer";
	RegisterClass(&WindowClass); //윈도우 생성을 위한 class 등록
	HWND Window = CreateWindow(L"MusicPlayer", L"Music Player Application", WS_SYSMENU | WS_VISIBLE, 0, 0, 800, 600, NULL, NULL, instance, NULL); //윈도우 생성

	DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS & ~WS_SIZEBOX;
	HWND  View1 = CreateWindow(L"STATIC", NULL, Style, 0, 0, 800, 600, Window, NULL, instance, NULL); //윈도우에 대한 SFML View 생성.

	window.create(View1); //생성한 View를 윈도우에 할당

	MediaPlayer::create(Window, Window, &player); //플레이어 생성

	MSG Message;
	Message.message = ~WM_QUIT; //시스템으로부터 받아올 윈도우의 메세지

	window.setActive(false); //렌더링 분리를 위해 비활성화.

	std::thread renderer([this]() {
		this->Render();

	}); //렌더 함수를 쓰레드를 생성하여 실행. (윈도우 메세지 수신과 별개로 렌더링이 작동한다.)
	renderer.detach(); //현재 쓰레드로부터 독립시킨다. (별개로 돌아가야 하기 때문)
	//join 또는 detach를 호출했으므로 이 쓰레드는 함수가 종료되면 안전하게 해제된다.

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

	Sleep(500); //윈도우 생성과 렌더링 사이에 이벤트가 발생하는 경우가 있어서 해결하기 위해 0.5초 대기
	running = true;

	while (Message.message != WM_QUIT) //종료 메시지가 아닌 경우 무한 루프를 돈다.
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			//윈도우로부터 이벤트 응답을 받으면 메시지 전달
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	window.close(); //SFML 렌더 종료

	running = false;

	DestroyWindow(Window); //종료되면 윈도우를 해제한다.

	UnregisterClass(L"MusicPlayer", instance); //클래스 등록을 해제하고 종료.
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
	iter.ResetToLastPointer(); //id가 z-order 역할을 하므로 역순 검색
	Graphic* g;

	switch (e.type)
	{
	case sf::Event::MouseButtonPressed:

		if (focus) //기존의 포커스가 있을 경우
		{
			focus->setFocus(false); //기존의 포커싱을 해제
			focus = nullptr; //포커스 객체를 null로 설정
		}

		while (iter.NotNull()) //포커싱할 그래픽을 찾는다.
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouseButton.x, e.mouseButton.y)))
			{
				focus = g; // 포커싱에 성공하면 포커싱 객체로 설정
				break;
			}

			iter.Prev();
		}

		if (focus)
		{
			focus->setFocus(true); //포커싱 객체의 포커스 설정
			focus->pollEvent(e);
		}
		break;

	case sf::Event::MouseMoved:
		while (iter.NotNull()) //마우스가 올려진 경우는 포커싱이 아니어도 처리해야 함
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouseMove.x, e.mouseMove.y)))
			{
				custom.type = CustomWinEvent::MouseOver;
				custom.mouseOver = CustomWinEvent::MouseOverEvent();
				custom.mouseOver.x = e.mouseMove.x;
				custom.mouseOver.y = e.mouseMove.y;
				g->pollEvent(custom); // 포커싱으로 설정하지는 않는다.
				break;
			}
			else g->pollEvent(e); //마우스 움직임에 대한 이벤트 수행

			iter.Prev();
		}
		break;

	case sf::Event::MouseLeft:
		while (iter.NotNull()) //마우스가 윈도우 밖으로 나갔을 때 이벤트 처리
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
	DoublyIterator<GenreType> iter(genreList);
	GenreType* genre;

	SimpleMusicType simpleMusic; //간단한 정보만 저장한 음악 데이터
	simpleMusic.SetName(music.GetName());
	simpleMusic.SetID(music.GetID());
	simpleMusic.SetLength(music.GetLength());
	simpleMusic.SetPlayedTime(music.GetPlayedTime());

	while (iter.NotNull())
	{
		genre = iter.CurrentPtr();
		
		if (music.GetGenre() == genre->GetGenre()) //장르가 이미 있다면
		{
			exist = true;

			genre->Add(simpleMusic);

			//pointer이므로 replace 등의 추가 작업이 필요하지 않음
			break;
		}

		iter.Next();
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
	//have to work with genre file i/o

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
	MusicType* data;	// 쓰기용 임시 변수
	Album* album;
	Artist* artist;
	SimpleMusicType music;

	char filename[FILENAMESIZE];
	cout << "\n\tEnter Output file Name : ";
	cin.getline(filename, FILENAMESIZE);

	// file open, open error가 발생하면 0을 리턴
	if (!OpenOutFile(filename))
		return 0;

	m_outFile << musicList.GetLength() << endl; //음악 수 기록

	// iterator 생성
	DoublyIterator<MusicType> iter_m(musicList);
	DoublyIterator<Album> iter_al(albumList);
	DoublyIterator<Artist> iter_ar(artistList);

	// list의 모든 데이터를 파일에 쓰기
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
	MusicType* data = NULL; //리스트에서 찾은 아이템에 대한 포인터 변수

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

	musicList.Delete(iter); //iterator를 이용해 빠르게 아이템을 제거한다.

	if (ReplaceMusic(music)) //다시 정렬하여 추가
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

	recentPlayedList.Delete(simpleMusic); //만약 있다면 제거
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
	mostPlayedList.Delete(simpleMusic); //만약 있다면 제거
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