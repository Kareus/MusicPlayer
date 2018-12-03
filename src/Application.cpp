#include "Application.h"
#include "PlayListWriter.h"
#include "GlobalFunctions.h"
#include "DirectoryReader.h"
#include "FileDialog.h"
#include <Dwmapi.h>

#pragma comment (lib, "Dwmapi.lib")

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
	backColor = sf::Color::Transparent;
	drawings.MakeEmpty();
	currentGroup = nullptr;
	running = false;

	defaultFont.loadFromFile("C:/Windows/Fonts/malgun.ttf");

	playerSprite = new Sprite("../../../graphic/player.png");
	minimizeSprite = new Sprite("../../../graphic/minimize.png");
	closeSprite = new Sprite("../../../graphic/exit.png");
	playSprite = new Sprite("../../../graphic/playpause.png");
	prevSprite = new Sprite("../../../graphic/prev.png");
	nextSprite = new Sprite("../../../graphic/next.png");
	searchSprite = new Sprite("../../../graphic/search.png");
	addSprite = new Sprite("../../../graphic/plus.png");
	addDirSprite = new Sprite("../../../graphic/folder.png");
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
}

void Application::Render()
{
	Sleep(100); //렌더링 시작 전 0.1초 대기

	while (window.isOpen())
	{
		window.clear(backColor);

		if (!running) continue;

		DoublyIterator<Graphic*> iter(drawings);

		while (iter.NotNull())
		{
			Sleep(5); //draw가 thread-detached로 작동하므로, 각 draw의 처리를 위해 5 밀리초 대기
			//대기가 없을 경우 특히 text box 렌더링에서 오류가 발생함

			if (!running) return; //그 사이에 닫힌 경우 종료

			iter.Current()->draw(&window); //각 그래픽을 렌더링한다
			iter.Next();
		}

		window.display();
	}
}

void Application::Close()
{
	SendMessage(Handle, WM_CLOSE, NULL, NULL);
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
	WindowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = L"MusicPlayer";
	RegisterClass(&WindowClass); //윈도우 생성을 위한 class 등록

	MARGINS margins;
	margins.cxLeftWidth = -1; //aero를 사용해 윈도우 백그라운드를 투명하게 하기 위한 변수

	Handle = CreateWindow(L"MusicPlayer", L"Music Player Application", WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, 360, 600, NULL, NULL, instance, NULL); //메인 윈도우 생성
	DwmExtendFrameIntoClientArea(Handle, &margins); //배경을 투명하게 한다.

	window.create(Handle); //생성한 윈도우를 SFML 렌더 윈도우에 할당
	window.setActive(false); //렌더링 분리를 위해 비활성화.

	MediaPlayer::create(Handle, Handle, &player); //플레이어 생성

	MSG Message;
	Message.message = ~WM_QUIT; //시스템으로부터 받아올 윈도우의 메세지
	
	std::thread renderer([this]() {
		this->Render();

	}); //렌더 함수를 쓰레드를 생성하여 실행. (윈도우 메세지 수신과 별개로 렌더링이 작동한다.)
	renderer.detach(); //현재 쓰레드로부터 독립시킨다. (별개로 돌아가야 하기 때문)
	//join 또는 detach를 호출했으므로 이 쓰레드는 함수가 종료되면 안전하게 해제된다.

	/*//test functions
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

	Sprite* sprite = new Sprite("C:/test.png");
	sprite->SetPosition(300, 300);
	sprite->SetButton(true);
	std::function<void(Sprite*)> func = [](Sprite*) {OutputDebugStringA("hello\n"); };
	sprite->setClickFunction(func);
	group->AddGraphic(sprite);

	AddGraphic(group);

	//test functions end. */

	playerSprite->SetMouseDownFunction(func_dragStart);
	AddGraphic(playerSprite);

	minimizeSprite->SetPosition(297, 10);
	minimizeSprite->SetButton(true);
	minimizeSprite->SetMouseUpFunction(func_minimize);
	AddGraphic(minimizeSprite);

	closeSprite->SetPosition(326, 10);
	closeSprite->SetButton(true);
	closeSprite->SetMouseUpFunction(func_close);
	AddGraphic(closeSprite);

	TextLabel* playName = new TextLabel(L"Artist - Song");
	playName->setAlign(TextAlign::MIDDLE);
	playName->setDisplayRect(250, 40);
	playName->setFont(defaultFont);
	playName->setCharacterSize(24);
	playName->setTextColor(sf::Color::White);
	playName->SetPosition(55, 45);
	AddGraphic(playName);

	playSprite->SetPosition(161, 87);
	playSprite->SetButton(true);
	playSprite->SetTextureRect(41, 46);
	playSprite->SetMouseUpFunction(func_playMusic);
	AddGraphic(playSprite);

	prevSprite->SetPosition(78, 95);
	prevSprite->SetButton(true);
	AddGraphic(prevSprite);

	nextSprite->SetPosition(249, 95);
	nextSprite->SetButton(true);
	AddGraphic(nextSprite);

	TextBox* defaultSearch = new TextBox(10, 174, 260, 24, false);
	defaultSearch->setCharacterSize(32);
	defaultSearch->setMaxLength(100);
	defaultSearch->setFont(defaultFont);
	defaultSearch->setBackgroundColor(sf::Color(0x17, 0x21, 0x29));
	defaultSearch->setBorderSize(0);
	defaultSearch->setCharacterSize(16);
	defaultSearch->setTextColor(sf::Color::White);
	AddGraphic(defaultSearch);

	searchSprite->SetPosition(271, 172);
	searchSprite->SetButton(true);
	AddGraphic(searchSprite);

	addSprite->SetPosition(296, 172);
	addSprite->SetButton(true);
	addSprite->SetMouseUpFunction(func_addMusic);
	AddGraphic(addSprite);

	addDirSprite->SetPosition(321, 172);
	addDirSprite->SetButton(true);
	AddGraphic(addDirSprite);

	Sleep(100); //윈도우 생성과 렌더링 사이에 이벤트가 발생하는 경우가 있어서 해결하기 위해 0.1초 대기
	running = true;

	SetCursor(LoadCursor(NULL, IDC_ARROW));
	while (Message.message != WM_QUIT) //종료 메시지가 아닌 경우 무한 루프를 돈다.
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			//윈도우로부터 이벤트 응답을 받으면 메시지 전달
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	running = false;

	Sleep(50); //thread 종료를 위해 기다림

	window.close(); //SFML 렌더 종료

	DestroyWindow(Handle); //종료되면 윈도우를 해제한다.

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
	/*
	Sprite* background = backgroundSprite->clone();
	background->SetButton(true);
	std::wstring str = String::StrToWstr(data->GetName() + '\n' + data->GetArtist() + " - " + data->GetAlbum());
	TextLabel* info = new TextLabel(str);
	info->loadFontFrom("C:/Windows/Fonts/malgun.ttf");
	info->SetPosition(200, 150);
	Sprite* playButton = play_nodeSprite->clone();
	Sprite* editButton = edit_nodeSprite->clone();
	Sprite* removeButton = remove_nodeSprite->clone();

	group->AddGraphic(background);
	group->AddGraphic(info);
	group->AddGraphic(playButton);
	group->AddGraphic(editButton);
	*/
	currentGroup->AddGraphic(group);

	return group;
}

Group* Application::AddEditGraphic(MusicType* data)
{
	Group* group = new Group();
	/*
	Sprite* background = backgroundSprite->clone();
	background->SetButton(true);
	std::wstring str = String::StrToWstr(data->GetName() + '\n' + data->GetArtist() + " - " + data->GetAlbum());
	TextLabel* info = new TextLabel(str);
	info->loadFontFrom("C:/Windows/Fonts/malgun.ttf");
	info->SetPosition(200, 150);
	Sprite* addButton = add_nodeSprite->clone();
	Sprite* editButton = edit_nodeSprite->clone();

	group->AddGraphic(background);
	group->AddGraphic(info);
	group->AddGraphic(playButton);
	group->AddGraphic(editButton);
	*/
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
	FileDialog dialog;
	std::wstring path;
	if (dialog.openFileDialog(path, L"Music Files", L"mp3,wav") != DIALOG_SUCCESS) return 0;

	MusicType music;
	music.SetPath(path);
	music.ReadDataFromID3(); //id3 태그를 읽어온다.

	if (music.GetName().empty()) //이름 정보가 없으면
	{
		int dir = path.find_last_of(L'\\');
		std::wstring file = path.substr(dir + 1);
		file = file.substr(0, file.size() - 4);
		music.SetName(String::WstrToStr(file)); //파일 이름으로 설정
	}

	if (music.GetName().empty() || music.GetArtist().empty()) //ID에 필요한 정보가 없는 경우
	{
		EditMusic(&music);
	}
	
	if (!musicList.Add(music))
	{
		System::AlertError("해당 경로의 음악이 이미 존재합니다.", "Music Already Exists", MB_OK);
		return 0;
	}

	SimpleMusicType simple = music; //형변환 연산자로 SimpleMusicType으로 변환
	simple.SetID(music.GetName() + '_' + music.GetArtist()); //ID를 이름_아티스트로 변경
	nameList.Add(simple);
	return 1;
}

int Application::EditMusic(MusicType* music)
{
	sf::RenderWindow editWindow;
	editWindow.create(sf::VideoMode(400, 500), L"Music Edit Window", sf::Style::Default & ~sf::Style::Resize);

	while (editWindow.isOpen())
	{
		Sleep(5000);
		editWindow.close();
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
	return 1;
}