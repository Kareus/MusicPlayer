#include "Application.h"
#include "PlayListWriter.h"
#include "GlobalFunctions.h"
#include "DirectoryReader.h"
#include "FileDialog.h"
#include "AVLTreeIterator.h"
#include <Dwmapi.h>

#pragma comment (lib, "Dwmapi.lib")

extern MediaPlayer* player;

Application::Application()
{
	focus = nullptr;
	m_Command = 0;
	recentPlayCount = 0;
	addedCount = 0;
	nameList.SetCompareFunction(compareMusicName);
	recentPlayedList.SetCompareFunction(compareToLast);
	mostPlayedList.SetCompareFunction(comparePlayedTime);

	drawings.SetCompareFunction(compareGraphics);
	edit_drawings.SetCompareFunction(compareGraphics);
	displayList.SetCompareFunction(compareGraphics);

	backColor = sf::Color::Transparent;
	drawings.MakeEmpty();
	edit_drawings.MakeEmpty();

	currentGroup = nullptr;
	running = false;
	editing = false;
	editMusic = nullptr;
	displayMode = 0;

	Resource::defaultFont = new sf::Font();
	Resource::defaultFont->loadFromFile("C:/Windows/Fonts/malgun.ttf");

	playerSprite = new Sprite("../../../graphic/player.png");
	minimizeSprite = new Sprite("../../../graphic/minimize.png");
	closeSprite = new Sprite("../../../graphic/exit.png");
	playSprite = new Sprite("../../../graphic/playpause.png");
	prevSprite = new Sprite("../../../graphic/prev.png");
	nextSprite = new Sprite("../../../graphic/next.png");
	searchSprite = new Sprite("../../../graphic/search.png");

	Resource::addSprite = new Sprite("../../../graphic/plus.png");
	Resource::editSprite = new Sprite("../../../graphic/edit.png");
	Resource::removeSprite = new Sprite("../../../graphic/minus.png");
	Resource::backSprite = new Sprite("../../../graphic/background.png");
	Resource::playSprite = new Sprite("../../../graphic/playmusic.png");
	addDirSprite = new Sprite("../../../graphic/folder.png");
	editorSprite = new Sprite("../../../graphic/editor.png");

	okSprite = new Sprite("../../../graphic/ok.png");
	cancelSprite = new Sprite("../../../graphic/cancel.png");
}

Application::~Application()
{
	player->Release(); //플레이어 메모리 해제
	
	ReleaseMainGraphic();
	ReleaseEditorGraphic(); //그래픽 할당 해제
	
	drawings.MakeEmpty();
	edit_drawings.MakeEmpty();

	Resource::ReleaseResource();
}

void Application::ReleaseMainGraphic()
{
	DoublyIterator<Graphic*> iter(drawings);

	while (iter.NotNull())
	{
		if (iter.Current() != nullptr) delete iter.Current();
		iter.Next();
	}
}

void Application::ReleaseEditorGraphic()
{
	DoublyIterator<Graphic*> iter(edit_drawings);

	while (iter.NotNull())
	{
		if (iter.Current() != nullptr) delete iter.Current();
		iter.Next();
	}
}

void Application::RenderMain()
{
	Sleep(100); //렌더링 시작 전 0.1초 대기

	while (window.isOpen())
	{
		if (!running) continue;

		window.clear(backColor);

		DoublyIterator<Graphic*> iter(drawings);

		while (iter.NotNull())
		{
			if (!running) return; //그 사이에 닫힌 경우 종료

			iter.Current()->draw(&window); //각 그래픽을 렌더링한다
			iter.Next();
		}

		window.display();
	}
}

void Application::RenderEditor()
{
	Sleep(100);

	while (editor.isOpen())
	{
		if (!running) continue;

		editor.clear(backColor);

		DoublyIterator<Graphic*> iter(edit_drawings);

		while (iter.NotNull())
		{

			if (!running) return;

			iter.Current()->draw(&editor); //각 그래픽을 렌더링한다
			iter.Next();
		}

		editor.display();
	}
}

void Application::Close()
{
	SendMessage(Handle, WM_CLOSE, NULL, NULL);
}

void Application::CloseEditor()
{
	ShowWindow(editHandle, SW_HIDE);
	editing = false;
	SendMessage(Handle, WM_ACTIVATE, NULL, NULL);
}

int Application::AddGraphicToMain(Graphic* graphic)
{
	graphic->setID(drawings.GetLength());
	if (graphic->getID() < 0) return 0; //오버플로우
	return drawings.Add(graphic);
}

int Application::AddGraphicToEditor(Graphic* graphic)
{
	graphic->setID(edit_drawings.GetLength());
	if (graphic->getID() < 0) return 0;
	return edit_drawings.Add(graphic);
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

	//에디터 윈도우 생성
	editHandle = CreateWindow(L"MusicPlayer", L"Music Player Editor", WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, 360, 400, NULL, NULL, instance, NULL);
	DwmExtendFrameIntoClientArea(editHandle, &margins);

	editor.create(editHandle);
	editor.setActive(false);

	ShowWindow(editHandle, SW_HIDE);

	MediaPlayer::create(Handle, Handle, &player); //플레이어 생성

	MSG Message;
	Message.message = ~WM_QUIT; //시스템으로부터 받아올 윈도우의 메세지
	
	std::thread renderer([this]() {
		this->RenderMain();
	}); //렌더 함수를 쓰레드를 생성하여 실행. (윈도우 메세지 수신과 별개로 렌더링이 작동한다.)
	renderer.detach(); //현재 쓰레드로부터 독립시킨다. (별개로 돌아가야 하기 때문)
	//join 또는 detach를 호출했으므로 이 쓰레드는 함수가 종료되면 안전하게 해제된다.

	std::thread renderer2([this]() {
		this->RenderEditor();
	});
	renderer2.detach();

	initMainGraphic();
	initEditorGraphic();

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

	editor.close();
	window.close(); //SFML 렌더 종료

	DestroyWindow(editHandle);
	DestroyWindow(Handle); //종료되면 윈도우를 해제한다.

	UnregisterClass(L"MusicPlayer", instance); //클래스 등록을 해제하고 종료.
}

void Application::initMainGraphic()
{
	//메인 윈도우에 사용할 그래픽 로드 및 배치

	playerSprite->SetMouseDownFunction(func_dragStart);
	AddGraphicToMain(playerSprite);

	minimizeSprite->SetPosition(297, 10);
	minimizeSprite->SetButton(true);
	minimizeSprite->SetMouseUpFunction(func_minimize);
	AddGraphicToMain(minimizeSprite);

	closeSprite->SetPosition(326, 10);
	closeSprite->SetButton(true);
	closeSprite->SetMouseUpFunction(func_close);
	AddGraphicToMain(closeSprite);

	TextLabel* playName = new TextLabel(L"Artist - Song");
	playName->setAlign(TextAlign::MIDDLE);
	playName->setDisplayRect(250, 40);
	playName->setFont(*Resource::defaultFont);
	playName->setCharacterSize(24);
	playName->setTextColor(sf::Color::White);
	playName->SetPosition(55, 45);
	AddGraphicToMain(playName);

	playSprite->SetPosition(161, 87);
	playSprite->SetButton(true);
	playSprite->SetTextureRect(41, 46);
	playSprite->SetMouseUpFunction(func_playMusic);
	AddGraphicToMain(playSprite);

	prevSprite->SetPosition(78, 95);
	prevSprite->SetButton(true);
	AddGraphicToMain(prevSprite);

	nextSprite->SetPosition(249, 95);
	nextSprite->SetButton(true);
	AddGraphicToMain(nextSprite);

	TextBox* defaultSearch = new TextBox(10, 174, 260, 24, false);
	defaultSearch->setMaxLength(100);
	defaultSearch->setFont(*Resource::defaultFont);
	defaultSearch->setBackgroundColor(sf::Color(0x17, 0x21, 0x29));
	defaultSearch->setBorderSize(0);
	defaultSearch->setCharacterSize(16);
	defaultSearch->setTextColor(sf::Color::White);
	AddGraphicToMain(defaultSearch);

	searchSprite->SetPosition(271, 172);
	searchSprite->SetButton(true);
	AddGraphicToMain(searchSprite);

	Sprite* addSprite = Resource::addSprite->clone();
	addSprite->SetPosition(296, 172);
	addSprite->SetButton(true);
	addSprite->SetMouseUpFunction(func_addMusic);
	AddGraphicToMain(addSprite);

	addDirSprite->SetPosition(321, 172);
	addDirSprite->SetButton(true);
	addDirSprite->SetMouseUpFunction(func_addDirMusic);
	AddGraphicToMain(addDirSprite);

	currentGroup = new Group();
	currentGroup->SetPositionY(300);
	AddGraphicToMain(currentGroup);
}

void Application::initEditorGraphic()
{
	//에디터 윈도우에 사용할 그래픽 로드 및 배치

	editorSprite->SetMouseDownFunction(func_dragStart_edit);
	AddGraphicToEditor(editorSprite);

	okSprite->SetPosition(183, 368);
	okSprite->SetMouseUpFunction(func_ok_edit);
	okSprite->SetButton(true);
	AddGraphicToEditor(okSprite);

	cancelSprite->SetPosition(238, 368);
	cancelSprite->SetMouseUpFunction(func_cancel_edit);
	cancelSprite->SetButton(true);
	AddGraphicToEditor(cancelSprite);

	//name
	TextLabel* nameTag = new TextLabel(L"Name :");
	nameTag->setFont(*Resource::defaultFont);
	nameTag->setCharacterSize(16);
	nameTag->SetPosition(12, 10);
	AddGraphicToEditor(nameTag);

	nameEdit = new TextBox(110, 10, 190, 24, false);
	nameEdit->setFont(*Resource::defaultFont);
	nameEdit->setCharacterSize(16);
	nameEdit->setBackgroundColor(sf::Color(0x17, 0x21, 0x29));
	nameEdit->setBorderSize(0);
	nameEdit->setCharacterSize(16);
	nameEdit->setTextColor(sf::Color::White);
	AddGraphicToEditor(nameEdit);

	//artist
	TextLabel* artistTag = new TextLabel(L"Artist :");
	artistTag->setFont(*Resource::defaultFont);
	artistTag->setCharacterSize(16);
	artistTag->SetPosition(12, 38);
	AddGraphicToEditor(artistTag);

	artistEdit = new TextBox(110, 38, 190, 24, false);
	artistEdit->setFont(*Resource::defaultFont);
	artistEdit->setBackgroundColor(sf::Color(0x17, 0x21, 0x29));
	artistEdit->setBorderSize(0);
	artistEdit->setCharacterSize(16);
	artistEdit->setTextColor(sf::Color::White);
	AddGraphicToEditor(artistEdit);

	//album
	TextLabel* albumTag = new TextLabel(L"Album :");
	albumTag->setFont(*Resource::defaultFont);
	albumTag->setCharacterSize(16);
	albumTag->SetPosition(12, 66);
	AddGraphicToEditor(albumTag);

	albumEdit = new TextBox(110, 66, 190, 24, false);
	albumEdit->setFont(*Resource::defaultFont);
	albumEdit->setBackgroundColor(sf::Color(0x17, 0x21, 0x29));
	albumEdit->setBorderSize(0);
	albumEdit->setCharacterSize(16);
	albumEdit->setTextColor(sf::Color::White);
	AddGraphicToEditor(albumEdit);

	//genre
	TextLabel* genreTag = new TextLabel(L"Genre :");
	genreTag->setFont(*Resource::defaultFont);
	genreTag->setCharacterSize(16);
	genreTag->SetPosition(12, 94);
	AddGraphicToEditor(genreTag);

	genreEdit = new TextBox(110, 94, 190, 24, false);
	genreEdit->setFont(*Resource::defaultFont);
	genreEdit->setBackgroundColor(sf::Color(0x17, 0x21, 0x29));
	genreEdit->setBorderSize(0);
	genreEdit->setCharacterSize(16);
	genreEdit->setTextColor(sf::Color::White);
	AddGraphicToEditor(genreEdit);

	//composer
	TextLabel* composerTag = new TextLabel(L"Composer :");
	composerTag->setFont(*Resource::defaultFont);
	composerTag->setCharacterSize(16);
	composerTag->SetPosition(12, 122);
	AddGraphicToEditor(composerTag);

	composerEdit = new TextBox(110, 122, 190, 24, false);
	composerEdit->setFont(*Resource::defaultFont);
	composerEdit->setBackgroundColor(sf::Color(0x17, 0x21, 0x29));
	composerEdit->setBorderSize(0);
	composerEdit->setCharacterSize(16);
	composerEdit->setTextColor(sf::Color::White);
	AddGraphicToEditor(composerEdit);

	//writer
	TextLabel* writerTag = new TextLabel(L"Writer :");
	writerTag->setFont(*Resource::defaultFont);
	writerTag->setCharacterSize(16);
	writerTag->SetPosition(12, 150);
	AddGraphicToEditor(writerTag);

	writerEdit = new TextBox(110, 150, 190, 24, false);
	writerEdit->setFont(*Resource::defaultFont);
	writerEdit->setBackgroundColor(sf::Color(0x17, 0x21, 0x29));
	writerEdit->setBorderSize(0);
	writerEdit->setCharacterSize(16);
	writerEdit->setTextColor(sf::Color::White);
	AddGraphicToEditor(writerEdit);

	//date
	TextLabel* dateTag = new TextLabel(L"Date :");
	dateTag->setFont(*Resource::defaultFont);
	dateTag->setCharacterSize(16);
	dateTag->SetPosition(12, 178);
	AddGraphicToEditor(dateTag);

	dateEdit = new TextBox(110, 178, 190, 24, false);
	dateEdit->setFont(*Resource::defaultFont);
	dateEdit->setBackgroundColor(sf::Color(0x17, 0x21, 0x29));
	dateEdit->setBorderSize(0);
	dateEdit->setCharacterSize(16);
	dateEdit->setTextColor(sf::Color::White);
	AddGraphicToEditor(dateEdit);

	//lyrics
	TextLabel* lyricsTag = new TextLabel(L"Lyrics :");
	lyricsTag->setFont(*Resource::defaultFont);
	lyricsTag->setCharacterSize(16);
	lyricsTag->SetPosition(12, 266);
	AddGraphicToEditor(lyricsTag);

	lyricsEdit = new TextBox(110, 266, 190, 96, true);
	lyricsEdit->setFont(*Resource::defaultFont);
	lyricsEdit->setBackgroundColor(sf::Color(0x17, 0x21, 0x29));
	lyricsEdit->setBorderSize(0);
	lyricsEdit->setCharacterSize(16);
	lyricsEdit->setTextColor(sf::Color::White);
	AddGraphicToEditor(lyricsEdit);
}

bool Application::IsRunning()
{
	return running;
}

bool Application::IsEditing()
{
	return editing;
}

bool Application::pollEvent(CustomWinEvent e)
{
	DoublyLinkedList<Graphic*>* toDraw;
	if (e.hWnd == Handle) toDraw = &drawings;
	else toDraw = &edit_drawings; //윈도우에 따라 탐색할 렌더 리스트 선택

	DoublyIterator<Graphic*> iter(*toDraw);
	iter.ResetToLastPointer();
	Graphic* g;
	CustomWinEvent custom;

	switch (e.type)
	{
	case CustomWinEvent::MouseDown:

		if (focus) //기존의 포커스가 있을 경우
		{
			focus->setFocus(false); //기존의 포커싱을 해제
			focus = nullptr; //포커스 객체를 null로 설정
		}

		while (iter.NotNull()) //포커싱할 그래픽을 찾는다.
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouse.x, e.mouse.y)))
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

	case CustomWinEvent::MouseMoved:

		while (iter.NotNull()) //마우스가 올려진 경우는 포커싱이 아니어도 처리해야 함
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouse.x, e.mouse.y)))
			{
				custom.type = CustomWinEvent::MouseOver;
				custom.mouse = CustomWinEvent::MouseEvent();
				custom.mouse.x = e.mouse.x;
				custom.mouse.y = e.mouse.y;
				g->pollEvent(custom); // 포커싱으로 설정하지는 않는다.
				break;
			}
			else g->pollEvent(e); //마우스 움직임에 대한 이벤트 수행

			iter.Prev();
		}
		break;

	case CustomWinEvent::MouseLeft:
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

void Application::initDisplay()
{
	currentGroup->MakeEmpty();

	switch (displayMode)
	{
	default:
		break;
	}

	DoublyIterator<Graphic*> iter(displayList);

	while (iter.NotNull())
	{
		currentGroup->AddGraphic(iter.Current());
		iter.Next();
	}
}

Group* Application::CreateDisplayGraphic(const MusicType& data)
{
	Group* graphic = new Group();
	graphic->SetData(data.GetPath());
	graphic->SetPositionY(300);

	int y = 300 + 45 * displayList.GetLength();

	Sprite* background = Resource::backSprite->clone();
	background->SetPositionY(y);

	graphic->AddGraphic(background);

	TextLabel* nameLabel = new TextLabel(L"nonname");
	nameLabel->setCharacterSize(12);
	nameLabel->setTextColor(sf::Color::White);
	nameLabel->setAlign(TextAlign::LEFT);
	nameLabel->setFont(*Resource::defaultFont);
	nameLabel->SetPosition(7, 4 + y);
	graphic->AddGraphic(nameLabel);

	TextLabel* idLabel = new TextLabel(L"nonid");
	idLabel->setCharacterSize(12);
	idLabel->setTextColor(sf::Color::White);
	idLabel->setAlign(TextAlign::LEFT);
	idLabel->setFont(*Resource::defaultFont);
	idLabel->SetPosition(7, 25 + y);
	graphic->AddGraphic(idLabel);

	std::wstring wstr = String::StrToWstr(data.GetName());
	if (wstr.size() >= 25) wstr = wstr.substr(0, 22) + L"...";
	nameLabel->setText(wstr);

	wstr = String::StrToWstr(data.GetArtist() + " - " + data.GetAlbum());

	if (wstr.size() >= 25) wstr = wstr.substr(0, 22) + L"...";
	idLabel->setText(wstr);

	Sprite* button1 = Resource::editSprite->clone();
	button1->SetButton(true);
	button1->SetPosition(261, 20 + y);
	button1->SetMouseUpFunction(func_editData);

	Sprite* button2 = Resource::removeSprite->clone();
	button2->SetButton(true);
	button2->SetPosition(292, 20 + y);
	button2->SetMouseUpFunction(func_removeData);

	Sprite* player = Resource::playSprite->clone();
	player->SetButton(true);
	player->SetPosition(230, 20 + y);
	player->SetMouseUpFunction(func_playData);

	graphic->AddGraphic(button1);
	graphic->AddGraphic(button2);
	graphic->AddGraphic(player);

	return graphic;
}

void Application::DisplayAllMusic()
{
	displayList.MakeEmpty();

	function<void(SimpleMusicType&)> func = [this](SimpleMusicType& data)
	{
		MusicType music;
		music.SetID(String::WstrToStr(data.GetPath()));
		musicList.Get(music);
		displayList.Add(CreateDisplayGraphic(music));
	};

	displayMode = 0;
	nameList.Do(func);

	initDisplay();
}

int Application::AddMusic()
{
	FileDialog dialog;
	std::wstring path;
	if (dialog.openFileDialog(path, L"Music Files", L"mp3") != DIALOG_SUCCESS) return 0;

	MusicType music;
	music.SetPath(path);
	music.ReadDataFromID3(); //id3 태그를 읽어온다.
	int dir = path.find_last_of(L'\\'); //마지막 폴더에 해당하는 인덱스

	if (music.GetName().empty()) //이름 정보가 없으면
	{
		std::wstring file = path.substr(dir + 1);
		file = file.substr(0, file.size() - 4);
		music.SetName(String::WstrToStr(file)); //파일 이름으로 설정
	}

	music.SetID(String::WstrToStr(music.GetPath()));
	
	if (!musicList.Add(music))
	{
		System::AlertError("해당 음악이 이미 존재합니다.", "Music Already Exists", MB_OK);
		return 0;
	}

	SimpleMusicType simple = music; //형변환 연산자로 SimpleMusicType으로 변환

	simple.SetPath(music.GetPath());
	string simpleID = music.GetName() + '_' + music.GetArtist();
	simple.SetID(simpleID);

	int count = 0;

	while (!nameList.Add(simple)) //경로는 다르지만 이름_아티스트가 같을 수 있으므로 중복 방지 처리
	{
		simple.SetID(simpleID + '_' + std::to_string(count));
		count++;
	}

	if (!music.GetAlbum().empty()) //앨범 타입 추가
	{
		Album album;
		album.SetAlbumName(music.GetAlbum());
		album.SetArtist(music.GetArtist());
		album.SetID(music.GetAlbum() + '_' + music.GetArtist());

		if (albumList.Get(album)) //앨범이 이미 존재하는 경우
		{
			album.AddMusic(simple);
			albumList.Replace(album);
		}
		else
		{
			album.AddMusic(simple);
			albumList.Add(album);
		}
	}

	if (!music.GetArtist().empty()) //아티스트 타입 추가
	{
		Artist artist;
		artist.SetName(music.GetArtist());
		artist.SetID(music.GetArtist());

		if (artistList.Get(artist)) //아티스트가 이미 존재하는 경우
		{
			artist.AddMusic(simple);
			artistList.Replace(artist);
		}
		else
		{
			artist.AddMusic(simple);
			artistList.Add(artist);
		}
	}

	if (!music.GetGenre().empty()) //장르 타입 추가
	{
		GenreType genre;
		genre.SetGenre(music.GetGenre());
		
		if (genreList.Get(genre)) //장르가 이미 존재하는 경우
		{
			genre.AddMusic(simple);
			genreList.Replace(genre);
		}
		else
		{
			genre.AddMusic(simple);
			genreList.Add(genre);
		}
	}

	FolderType folder; //폴더 타입 추가 (경로는 비어있을 수 없으므로 empty 체크 안함)
	folder.SetPath(path.substr(0, dir));
	
	if (folderList.Get(folder)) //폴더 타입이 이미 존재하는 경우
	{
		folder.AddMusic(simple);
		folderList.Replace(folder);
	}
	else
	{
		folder.AddMusic(simple);
		folderList.Add(folder);
	}

	if (newAddMusicList.IsFull()) //최근 추가한 리스트가 꽉 차 있으면
	{
		SimpleMusicType temp;
		newAddMusicList.DeQueue(temp);
		addedCount--;
	}

	simple.SetID(music.GetID()); //primary key를 경로로 재설정
	newAddMusicList.EnQueue(simple);
	addedCount++;

	UpdateList();
	return 1;
}

int Application::AddMusic(MusicType& music)
{
	wstring path = music.GetPath();
	int dir = path.find_last_of(L'\\'); //마지막 폴더에 해당하는 인덱스

	if (music.GetName().empty()) //이름 정보가 없으면
	{
		std::wstring file = path.substr(dir + 1);
		file = file.substr(0, file.size() - 4);
		music.SetName(String::WstrToStr(file)); //파일 이름으로 설정
	}

	music.SetID(String::WstrToStr(music.GetPath()));

	if (!musicList.Add(music)) return 0;

	SimpleMusicType simple = music; //형변환 연산자로 SimpleMusicType으로 변환

	string simpleID = music.GetName() + '_' + music.GetArtist();
	simple.SetID(simpleID);

	int count = 0;

	while (!nameList.Add(simple)) //경로는 다르지만 이름_아티스트가 같을 수 있으므로 중복 방지 처리
	{
		simple.SetID(simpleID + '_' + std::to_string(count));
		count++;
	}

	if (!music.GetAlbum().empty()) //앨범 타입 추가
	{
		Album album;
		album.SetAlbumName(music.GetAlbum());
		album.SetArtist(music.GetArtist());
		album.SetID(music.GetAlbum() + '_' + music.GetArtist());

		if (albumList.Get(album)) //앨범이 이미 존재하는 경우
		{
			album.AddMusic(simple);
			albumList.Replace(album);
		}
		else
		{
			album.AddMusic(simple);
			albumList.Add(album);
		}
	}

	if (!music.GetArtist().empty()) //아티스트 타입 추가
	{
		Artist artist;
		artist.SetName(music.GetArtist());
		artist.SetID(music.GetArtist());

		if (artistList.Get(artist)) //아티스트가 이미 존재하는 경우
		{
			artist.AddMusic(simple);
			artistList.Replace(artist);
		}
		else
		{
			artist.AddMusic(simple);
			artistList.Add(artist);
		}
	}

	if (!music.GetGenre().empty()) //장르 타입 추가
	{
		GenreType genre;
		genre.SetGenre(music.GetGenre());

		if (genreList.Get(genre)) //장르가 이미 존재하는 경우
		{
			genre.AddMusic(simple);
			genreList.Replace(genre);
		}
		else
		{
			genre.AddMusic(simple);
			genreList.Add(genre);
		}
	}

	FolderType folder; //폴더 타입 추가 (경로는 비어있을 수 없으므로 empty 체크 안함)
	folder.SetPath(path.substr(0, dir));

	if (folderList.Get(folder)) //폴더 타입이 이미 존재하는 경우
	{
		folder.AddMusic(simple);
		folderList.Replace(folder);
	}
	else
	{
		folder.AddMusic(simple);
		folderList.Add(folder);
	}

	if (newAddMusicList.IsFull()) //최근 추가한 리스트가 꽉 차 있으면
	{
		SimpleMusicType temp;
		newAddMusicList.DeQueue(temp);
		addedCount--;
	}

	simple.SetID(music.GetID()); //primary key를 경로로 재설정
	newAddMusicList.EnQueue(simple);
	addedCount++;

	return 1;
}

int Application::AddMusicFromDirectory()
{
	FileDialog dialog;
	std::wstring path;
	if (dialog.openDirectoryDialog(path) != DIALOG_SUCCESS) return 0;

	DirectoryReader dirReader;
	dirReader.openDirectory(path);

	FolderType folder;
	folder.SetPath(path);

	bool folderExist = folderList.Get(folder);

	DoublyIterator<wstring> iter = dirReader.getIterator();
	MusicType music;

	while (iter.NotNull())
	{
		wstring newPath = iter.Current();
		if (newPath.size() > 4 && newPath.substr(newPath.size() - 4, 4) == L".mp3")
		{
			music.SetPath(newPath);
			music.ReadDataFromID3(); //id3 태그를 읽어온다.
			int dir = newPath.find_last_of(L'\\');

			if (music.GetName().empty()) //이름 정보가 없으면
			{
				std::wstring file = newPath.substr(dir + 1);
				file = file.substr(0, file.size() - 4);
				music.SetName(String::WstrToStr(file)); //파일 이름으로 설정
			}

			music.SetID(String::WstrToStr(music.GetPath()));

			if (!musicList.Add(music))
			{
				iter.Next();
				continue;
			}

			SimpleMusicType simple = music; //형변환 연산자로 SimpleMusicType으로 변환

			string simpleID = music.GetName() + '_' + music.GetArtist();
			simple.SetID(simpleID);

			int count = 0;

			while (!nameList.Add(simple)) //경로는 다르지만 이름_아티스트가 같을 수 있으므로 중복 방지 처리
			{
				simple.SetID(simpleID + '_' + std::to_string(count));
				count++;
			}

			if (!music.GetAlbum().empty()) //앨범 타입 추가
			{
				Album album;
				album.SetAlbumName(music.GetAlbum());
				album.SetArtist(music.GetArtist());
				album.SetID(music.GetAlbum() + '_' + music.GetArtist());

				if (albumList.Get(album)) //앨범이 이미 존재하는 경우
				{
					album.AddMusic(simple);
					albumList.Replace(album);
				}
				else
				{
					album.AddMusic(simple);
					albumList.Add(album);
				}
			}

			if (!music.GetArtist().empty()) //아티스트 타입 추가
			{
				Artist artist;
				artist.SetName(music.GetArtist());
				artist.SetID(music.GetArtist());

				if (artistList.Get(artist)) //아티스트가 이미 존재하는 경우
				{
					artist.AddMusic(simple);
					artistList.Replace(artist);
				}
				else
				{
					artist.AddMusic(simple);
					artistList.Add(artist);
				}
			}

			if (!music.GetGenre().empty()) //장르 타입 추가
			{
				GenreType genre;
				genre.SetGenre(music.GetGenre());

				if (genreList.Get(genre)) //장르가 이미 존재하는 경우
				{
					genre.AddMusic(simple);
					genreList.Replace(genre);
				}
				else
				{
					genre.AddMusic(simple);
					genreList.Add(genre);
				}
			}

			folder.AddMusic(simple);

			if (newAddMusicList.IsFull()) //최근 추가한 리스트가 꽉 차 있으면
			{
				SimpleMusicType temp;
				newAddMusicList.DeQueue(temp);
				addedCount--;
			}

			simple.SetID(music.GetID()); //primary key를 경로로 재설정
			newAddMusicList.EnQueue(simple);
			addedCount++;
			
		}
		iter.Next();
	}

	if (folderExist) folderList.Replace(folder);
	else folderList.Add(folder);
	return 1;
}

int Application::EditMusic(const SimpleMusicType& music)
{
	AVLTreeIterator<MusicType> iter(musicList);
	MusicType* data = nullptr;

	while (iter.NotNull())
	{
		data = iter.CurrentPtr();
		int compare = wcscmp(data->GetPath().c_str(), music.GetPath().c_str());
		if (compare < 0) iter.GotoLeft();
		else if (compare > 0) iter.GotoRight();

		break; //찾으면 종료
	}

	if (data == nullptr) return 0; //못 찾으면 0 반환

	editMusic = data;

	nameEdit->setText(String::StrToWstr(editMusic->GetName()));
	artistEdit->setText(String::StrToWstr(editMusic->GetArtist()));
	albumEdit->setText(String::StrToWstr(editMusic->GetAlbum()));
	composerEdit->setText(String::StrToWstr(editMusic->GetComposer()));
	writerEdit->setText(String::StrToWstr(editMusic->GetWriter()));
	dateEdit->setText(to_wstring(editMusic->GetDate()));
	genreEdit->setText(String::StrToWstr(editMusic->GetGenre()));
	//lengthLabel->setText(to_wstring(editMusic->GetLength()));
	//timeLabel->setText(to_wstring(editMusic->GetPlayedTime()));
	lyricsEdit->setText(String::StrToWstr(editMusic->GetLyrics()));
	//pathLabel->setText(editMusic->GetPath());
	//텍스트 설정

	ShowWindow(editHandle, SW_SHOW);
	editing = true;

	RECT rect;

	GetWindowRect(Handle, &rect);
	int x = rect.left + 30;
	int y = rect.top + 100; //메인 윈도우 기준 위치

	GetWindowRect(GetDesktopWindow(), &rect);

	if (x + 300 >= rect.right || y + 200 >= rect.bottom)
	{
		x = rect.right / 2 - 150;
		y = rect.bottom / 2 - 200;
	} //화면 영역 밖으로 벗어나면 화면 정중앙으로 설정

	SetWindowPos(editHandle, NULL, x, y, 0, 0, SWP_NOSIZE);

	return 1;
}

int Application::ReplaceMusic()
{
	if (editMusic == nullptr) return 0; //수정할 음악 타입이 정해져 있지 않으면 0 반환

	string newName = String::WstrToStr(nameEdit->getText());
	string newArtist = String::WstrToStr(artistEdit->getText());
	
	bool idChange = strcmp(editMusic->GetName().c_str(), newName.c_str()) || strcmp(editMusic->GetArtist().c_str(), newArtist.c_str());

	Album album;
	Artist artist;
	GenreType genre;
	FolderType folder;

	album.SetID(editMusic->GetAlbum() + '_' + editMusic->GetArtist());
	artist.SetID(editMusic->GetArtist());
	genre.SetGenre(editMusic->GetGenre());
	
	wstring path = editMusic->GetPath();
	path = path.substr(0, path.find_last_of(L'\\'));
	folder.SetPath(path);
	folderList.Get(folder);

	SimpleMusicType simple = *editMusic;

	if (idChange)
	{
		//이름이나 아티스트가 바뀐 경우, ID가 변경되었으므로 리스트에서 삭제
		nameList.Delete(simple);

		albumList.Get(album);
		album.DeleteMusic(simple);
		albumList.Replace(album);

		artistList.Get(artist);
		artist.DeleteMusic(simple);
		artistList.Replace(artist);
		
		genreList.Get(genre);
		genre.DeleteMusic(simple);
		genreList.Replace(genre);
		
		folder.DeleteMusic(simple);
	}

	editMusic->SetName(newName);
	editMusic->SetArtist(newArtist);
	editMusic->SetComposer(String::WstrToStr(composerEdit->getText()));
	editMusic->SetWriter(String::WstrToStr(writerEdit->getText()));

	unsigned int newDate = std::stoi(dateEdit->getText());
	while (newDate < 10000000) newDate *= 10; //YYYYMMDD 형식에 맞춘다.
	editMusic->SetDate(newDate);
	editMusic->SetAlbum(String::WstrToStr(albumEdit->getText()));
	editMusic->SetGenre(String::WstrToStr(genreEdit->getText()));
	editMusic->SetLyrics(String::WstrToStr(lyricsEdit->getText()));

	//musicList는 포인터를 이용해 수정했으므로 Replace할 필요 없음
	
	simple = *editMusic;

	if (!idChange)
	{
		nameList.Replace(simple);
		
		albumList.Get(album);
		album.ReplaceMusic(simple);
		albumList.Replace(album);

		artistList.Get(artist);
		artist.ReplaceMusic(simple);
		artistList.Replace(artist);

		genreList.Get(genre);
		genre.ReplaceMusic(simple);
		genreList.Replace(genre);

		folder.ReplaceMusic(simple);
		folderList.Replace(folder);

		return 1;
	}

	nameList.Add(simple);
	
	album.SetID(editMusic->GetAlbum() + '_' + editMusic->GetArtist());
	artist.SetID(editMusic->GetArtist());
	genre.SetGenre(editMusic->GetGenre());

	folder.AddMusic(simple);
	folderList.Replace(folder);

	if (albumList.Get(album))
	{
		album.AddMusic(simple);
		albumList.Replace(album);
	}
	else
	{
		album.AddMusic(simple);
		albumList.Add(album);
	}

	if (artistList.Get(artist))
	{
		artist.AddMusic(simple);
		artistList.Replace(artist);
	}
	else
	{
		artist.AddMusic(simple);
		artistList.Add(artist);
	}

	if (genreList.Get(genre))
	{
		genre.AddMusic(simple);
		genreList.Replace(genre);
	}
	else
	{
		genre.AddMusic(simple);
		genreList.Add(genre);
	}

	UpdateList();
	return 1;
}

int Application::DeleteMusic(const SimpleMusicType& music)
{	
	MusicType data;
	data.SetPath(music.GetPath());

	if (!musicList.Get(data)) return 0;
	musicList.Delete(data);

	SimpleMusicType simple;
	simple.SetID(data.GetName() + '_' + data.GetArtist());

	Album album;
	Artist artist;
	GenreType genre;
	FolderType folder;

	album.SetID(data.GetAlbum() + '_' + data.GetArtist());
	artist.SetID(data.GetArtist());
	genre.SetGenre(data.GetGenre());
	wstring path = editMusic->GetPath();
	path = path.substr(0, path.find_last_of(L'\\'));
	folder.SetPath(path);

	albumList.Get(album);
	artistList.Get(artist);
	genreList.Get(genre);
	folderList.Get(folder);

	nameList.Delete(simple);
	album.DeleteMusic(simple);
	artist.DeleteMusic(simple);
	genre.DeleteMusic(simple);
	folder.DeleteMusic(simple);

	if (album.GetMusicNum() > 0) albumList.Replace(album);
	else albumList.Delete(album);

	if (artist.GetMusicNum() > 0) artistList.Replace(artist);
	else artistList.Delete(artist);

	if (genre.GetMusicNum() > 0) genreList.Replace(genre);
	else genreList.Delete(genre);

	if (folder.GetLength() > 0) folderList.Replace(folder);
	else folderList.Delete(folder);

	CircularQueueType<SimpleMusicType> newQueue = CircularQueueType<SimpleMusicType>(31);

	newAddMusicList.ResetQueue();
	SimpleMusicType temp;

	for (int i = 0; i < addedCount; i++)
	{
		newAddMusicList.GetNextItem(temp);
		if (temp.GetID() == simple.GetID()) continue;
		newQueue.EnQueue(temp);
	}

	newAddMusicList = newQueue; //삭제한 아이템이 큐에 있는지 검사하여 덮어쓰기.

	playLists.Do([simple](PlayList& p) { p.DeleteMusic(simple); }); //플레이리스트에서 데이터를 찾아 삭제

	UpdateList();
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

	m_inputFile >> recentPlayCount;
	m_inputFile.ignore();

	for (int i = 0; i < recentPlayCount; i++)
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
	DoublyIterator<Album> iter_al(albumList);
	DoublyIterator<Artist> iter_ar(artistList);

	// list의 모든 데이터를 파일에 쓰기
	musicList.Do([this](MusicType& music) { music.WriteDataToFile(m_outFile); });

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

	m_outFile << recentPlayCount << endl;

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

void Application::AddToRecentPlayed(const SimpleMusicType& music)
{
	recentPlayedList.Delete(music); //만약 있다면 제거
	recentPlayedList.Add(music);

	if (recentPlayedList.GetLength() > 30)
	{
		DoublyIterator<SimpleMusicType> iter(recentPlayedList);
		recentPlayedList.Delete(iter.First());
	}
}

void Application::AddToMostPlayed(const SimpleMusicType& music)
{
	mostPlayedList.Delete(music); //만약 있다면 제거
	SimpleMusicType simple = music;
	simple.SetPlayedTime(music.GetPlayedTime() + 1);
	mostPlayedList.Add(music);

	if (mostPlayedList.GetLength() > 30)
	{
		DoublyIterator<SimpleMusicType> iter(mostPlayedList);
		iter.ResetToLastPointer();
		mostPlayedList.Delete(iter);
	}
}

int Application::PlayMusic(const SimpleMusicType& music)
{
	SimpleMusicType simple;

	if (simple.GetID().empty())
	{
		AVLTreeIterator<SimpleMusicType> iter(nameList);
		simple = iter.Current();
	}
	else if (!nameList.Get(simple)) return 0;

	currentMusic = simple;

	//play
	AddToRecentPlayed(simple);
	AddToMostPlayed(simple);

	simple.SetPlayedTime(simple.GetPlayedTime() + 1);

	nameList.Replace(simple);

	MusicType data;
	data.SetPath(simple.GetPath());
	musicList.Get(data);
	data.SetPlayedTime(simple.GetPlayedTime());
	musicList.Replace(data);
	return 1;
}

void Application::UpdateList()
{
	switch (displayMode)
	{
	case 0:
		DisplayAllMusic();
		break;
	}
}