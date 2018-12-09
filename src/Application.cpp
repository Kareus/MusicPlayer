#include "Application.h"
#include "PlayListWriter.h"
#include "DirectoryReader.h"
#include "FileDialog.h"
#include "AVLTreeIterator.h"
#include <Dwmapi.h>
#include <conio.h>

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
	displayList.SetCompareFunction(compareGraphics);

	backColor = sf::Color::Transparent;
	drawings.MakeEmpty();

	currentGroup = nullptr;
	running = false;
	editMusic = nullptr;
	displayMode = 0;
	editor_opened = false;
	updating = false;
	scrolling = false;
	mouseDown = false;
	editList = false;

	//���� ��Ʈ���� ����ϴ� ���ҽ��� ��ü�鳢�� ������ �����ϱ� ������ clone�� ���� �����ϰ�, Resource���� �������� ������.
	Resource::defaultFont = new sf::Font();
	Resource::defaultFont->loadFromFile("C:/Windows/Fonts/malgun.ttf");
	Resource::addSprite = new Sprite("../../../graphic/plus.png");
	Resource::editSprite = new Sprite("../../../graphic/edit.png");
	Resource::removeSprite = new Sprite("../../../graphic/minus.png");
	Resource::backSprite = new Sprite("../../../graphic/background.png");
	Resource::playSprite = new Sprite("../../../graphic/playmusic.png");
	Resource::tabSprite = new Sprite("../../../graphic/tab.png");
	Resource::detailSprite = new Sprite("../../../graphic/detail.png");
	Resource::tolistSprite = new Sprite("../../../graphic/tolist.png");
	Resource::addlistSprite = new Sprite("../../../graphic/addlist.png");
	Resource::saveSprite = new Sprite("../../../graphic/save.png");

	defaultSearch = nullptr;
	playerSprite = new Sprite("../../../graphic/player.png");
	minimizeSprite = new Sprite("../../../graphic/minimize.png");
	closeSprite = new Sprite("../../../graphic/exit.png");
	playSprite = new Sprite("../../../graphic/playpause.png");
	prevSprite = new Sprite("../../../graphic/prev.png");
	nextSprite = new Sprite("../../../graphic/next.png");
	searchSprite = new Sprite("../../../graphic/search.png");

	addDirSprite = new Sprite("../../../graphic/folder.png");
	scrollbackSprite = new Sprite("../../../graphic/scroll_background.png");
	scrollSprite = new Sprite("../../../graphic/scroll.png");

	tab_song = nullptr;
	tab_album = nullptr;
	tab_artist = nullptr;
	tab_genre = nullptr;
	tab_folder = nullptr;
	tab_playlist = nullptr;

	song_sprite = nullptr;
	album_sprite = nullptr;
	artist_sprite = nullptr;
	genre_sprite = nullptr;
	folder_sprite = nullptr;
	playlist_sprite = nullptr;
}

Application::~Application()
{
	player->Release(); //�÷��̾� �޸� ����
	
	ReleaseMainGraphic(); //������ ���� ��� �׷��� ����. (������ ������ ���� �� Delete/Display �������� ���������� �����Ǿ� ����)

	Resource::ReleaseResource(); //���� ���ҽ� ����
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

void Application::RenderMain()
{
	if (updating) return;

	window.clear(backColor);

	DoublyIterator<Graphic*> iter(drawings);

	while (iter.NotNull())
	{
		if (updating) break; //�� ���̿� ���� ��� ����

		if (iter.Current()->isVisible()) iter.Current()->draw(&window); //�� �׷����� �������Ѵ�
		iter.Next();
	}

	if (updating) return; //������Ʈ ���� �̿ϼ��� ȭ�� ������ ����
	window.display();
}

void Application::Close()
{
	SendMessage(Handle, WM_CLOSE, NULL, NULL);
}

int Application::AddGraphicToMain(Graphic* graphic)
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
	WindowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = L"MusicPlayer";
	RegisterClass(&WindowClass); //������ ������ ���� class ���

	WNDCLASS EditClass;
	EditClass.style = 0;
	EditClass.lpfnWndProc = &EditProc;
	EditClass.cbClsExtra = 0;
	EditClass.cbWndExtra = 0;
	EditClass.hInstance = instance;
	EditClass.hIcon = NULL;
	EditClass.hCursor = 0;
	EditClass.hbrBackground = CreateSolidBrush(RGB(0xbd, 0xbd, 0xbd));
	EditClass.lpszMenuName = NULL;
	EditClass.lpszClassName = L"MusicEditor";
	RegisterClass(&EditClass); //������ ������ ���� class ���

	MARGINS margins;
	margins.cxLeftWidth = -1; //aero�� ����� ������ ��׶��带 �����ϰ� �ϱ� ���� ����

	Handle = CreateWindow(L"MusicPlayer", L"Music Player Application", WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, 360, 600, NULL, NULL, instance, NULL); //���� ������ ����
	DwmExtendFrameIntoClientArea(Handle, &margins); //����� �����ϰ� �Ѵ�.

	editor = CreateWindow(L"MusicEditor", L"Music Data Editor", WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, 360, 420, NULL, NULL, instance, NULL); //������ ������ ����
	ShowWindow(editor, SW_HIDE); //������ �����

	window.create(Handle); //������ �����츦 SFML ���� �����쿡 �Ҵ�
	window.setActive(false); //������ �и��� ���� ��Ȱ��ȭ.

	MediaPlayer::create(Handle, Handle, &player); //�÷��̾� ����

	MSG Message;
	Message.message = ~WM_QUIT; //�ý������κ��� �޾ƿ� �������� �޼���

	initMainGraphic();
	initEditor(); //�ʱ� �׷��� ����

	running = true;

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	while (Message.message != WM_QUIT) //���� �޽����� �ƴ� ��� ���� ������ ����.
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			//������κ��� �̺�Ʈ ������ ������ �޽��� ����
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		RenderMain();
	}

	running = false;

	DestroyWindow(editor); //������ ����
	window.close(); //SFML ���� ����

	UnregisterClass(L"MusicEditor", instance);
	UnregisterClass(L"MusicPlayer", instance); //Ŭ���� ����� �����ϰ� ����.
}

void Application::initMainGraphic()
{
	//���� �����쿡 ����� �׷��� �ε� �� ��ġ

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
	playName->setDisplayRect(240, 40);
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

	defaultSearch = new TextBox(10, 174, 260, 24, false);
	defaultSearch->setMaxLength(100);
	defaultSearch->setFont(*Resource::defaultFont);
	defaultSearch->setBackgroundColor(sf::Color(0x17, 0x21, 0x29));
	defaultSearch->setBorderSize(0);
	defaultSearch->setCharacterSize(16);
	defaultSearch->setTextColor(sf::Color::White);
	AddGraphicToMain(defaultSearch);

	searchSprite->SetPosition(271, 172);
	searchSprite->SetButton(true);
	searchSprite->SetMouseUpFunction(func_search);
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

	scrollbackSprite->SetPosition(328, 300);
	scrollbackSprite->Hide();
	AddGraphicToMain(scrollbackSprite);

	scrollSprite->SetPosition(328, 300);
	scrollSprite->SetButton(true);
	scrollSprite->SetMouseDownFunction(func_scrollDown);
	scrollSprite->SetMouseUpFunction(func_scrollUp);
	scrollSprite->Hide();
	AddGraphicToMain(scrollSprite);

	tab_song = new Group();
	tab_song->SetPosition(0, 230);
	song_sprite = Resource::tabSprite->clone();
	song_sprite->SetTextureRect(60, 30);
	song_sprite->SetButton(true);
	song_sprite->SetMouseUpFunction([this](Sprite*) { ChangeState(0); });
	TextLabel* song_label = new TextLabel(L"Song");
	song_label->setFont(*Resource::defaultFont);
	song_label->setCharacterSize(16);
	song_label->setTextColor(sf::Color::White);
	song_label->SetPosition(14, 7);
	song_label->SetMouseUpFunction([this](TextLabel*) { ChangeState(0); });
	tab_song->AddGraphic(song_sprite);
	tab_song->AddGraphic(song_label);
	AddGraphicToMain(tab_song);

	tab_album = new Group();
	tab_album->SetPosition(60, 230);
	album_sprite = Resource::tabSprite->clone();
	album_sprite->SetTextureRect(60, 30);
	album_sprite->SetButton(true);
	album_sprite->SetMouseUpFunction([this](Sprite*) { ChangeState(1); });
	TextLabel* album_label = new TextLabel(L"Album");
	album_label->setFont(*Resource::defaultFont);
	album_label->setCharacterSize(16);
	album_label->setTextColor(sf::Color::White);
	album_label->SetPosition(8, 7);
	album_label->SetMouseUpFunction([this](TextLabel*) { ChangeState(1); });
	tab_album->AddGraphic(album_sprite);
	tab_album->AddGraphic(album_label);
	AddGraphicToMain(tab_album);

	tab_artist = new Group();
	tab_artist->SetPosition(120, 230);
	artist_sprite = Resource::tabSprite->clone();
	artist_sprite->SetTextureRect(60, 30);
	artist_sprite->SetButton(true);
	artist_sprite->SetMouseUpFunction([this](Sprite*) { ChangeState(2); });
	TextLabel* artist_label = new TextLabel(L"Artist");
	artist_label->SetPosition(10, 7);
	artist_label->setFont(*Resource::defaultFont);
	artist_label->setCharacterSize(16);
	artist_label->setTextColor(sf::Color::White);
	artist_label->SetMouseUpFunction([this](TextLabel*) { ChangeState(2); });
	tab_artist->AddGraphic(artist_sprite);
	tab_artist->AddGraphic(artist_label);
	AddGraphicToMain(tab_artist);

	tab_genre = new Group();
	tab_genre->SetPosition(180, 230);
	genre_sprite = Resource::tabSprite->clone();
	genre_sprite->SetTextureRect(60, 30);
	genre_sprite->SetButton(true);
	genre_sprite->SetMouseUpFunction([this](Sprite*) { ChangeState(3); });
	TextLabel* genre_label = new TextLabel(L"Genre");
	genre_label->SetPosition(10, 7);
	genre_label->setFont(*Resource::defaultFont);
	genre_label->setCharacterSize(16);
	genre_label->setTextColor(sf::Color::White);
	genre_label->SetMouseUpFunction([this](TextLabel*) { ChangeState(3); });
	tab_genre->AddGraphic(genre_sprite);
	tab_genre->AddGraphic(genre_label);
	AddGraphicToMain(tab_genre);

	tab_folder = new Group();
	tab_folder->SetPosition(240, 230);
	folder_sprite = Resource::tabSprite->clone();
	folder_sprite->SetTextureRect(60, 30);
	folder_sprite->SetButton(true);
	folder_sprite->SetMouseUpFunction([this](Sprite*) { ChangeState(4); });
	TextLabel* folder_label = new TextLabel(L"Folder");
	folder_label->SetPosition(8, 7);
	folder_label->setFont(*Resource::defaultFont);
	folder_label->setCharacterSize(16);
	folder_label->setTextColor(sf::Color::White);
	folder_label->SetMouseUpFunction([this](TextLabel*) { ChangeState(4); });
	tab_folder->AddGraphic(folder_sprite);
	tab_folder->AddGraphic(folder_label);
	AddGraphicToMain(tab_folder);

	tab_playlist = new Group();
	tab_playlist->SetPosition(300, 230);
	playlist_sprite = Resource::tabSprite->clone();
	playlist_sprite->SetTextureRect(60, 30);
	playlist_sprite->SetButton(true);
	playlist_sprite->SetMouseUpFunction([this](Sprite*) { ChangeState(5); });
	TextLabel* playlist_label = new TextLabel(L"Play");
	playlist_label->SetPosition(14, 7);
	playlist_label->setFont(*Resource::defaultFont);
	playlist_label->setCharacterSize(16);
	playlist_label->setTextColor(sf::Color::White);
	playlist_label->SetMouseUpFunction([this](TextLabel*) { ChangeState(5); });
	tab_playlist->AddGraphic(playlist_sprite);
	tab_playlist->AddGraphic(playlist_label);
	AddGraphicToMain(tab_playlist);

	currentGroup = new Group();
	currentGroup->SetPositionY(300);
	currentGroup->SetViewRect(0, 0, 320, 280);
	AddGraphicToMain(currentGroup);

	ChangeState(0);
}

void Application::initEditor()
{
	//������ �����쿡 ����� Win32 ��Ʈ�� ���� �� ��ġ

	edit_nameLabel = CreateWindow(L"STATIC", L"Name :", WS_VISIBLE | WS_CHILD, 5, 5, 90, 20, editor, NULL, NULL, NULL);
	edit_nameEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 100, 5, 240, 20, editor, NULL, NULL, NULL);
	edit_artistLabel = CreateWindow(L"STATIC", L"Artist :", WS_VISIBLE | WS_CHILD, 5, 30, 90, 20, editor, NULL, NULL, NULL);
	edit_artistEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 100, 30, 240, 20, editor, NULL, NULL, NULL);
	edit_albumLabel = CreateWindow(L"STATIC", L"Album :", WS_VISIBLE | WS_CHILD, 5, 55, 90, 20, editor, NULL, NULL, NULL);
	edit_albumEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 100, 55, 240, 20, editor, NULL, NULL, NULL);
	edit_genreLabel = CreateWindow(L"STATIC", L"Genre :", WS_VISIBLE | WS_CHILD, 5, 80, 90, 20, editor, NULL, NULL, NULL);
	edit_genreEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 100, 80, 240, 20, editor, NULL, NULL, NULL);
	edit_composerLabel = CreateWindow(L"STATIC", L"Composer :", WS_VISIBLE | WS_CHILD, 5, 105, 90, 20, editor, NULL, NULL, NULL);
	edit_composerEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 100, 105, 240, 20, editor, NULL, NULL, NULL);
	edit_writerLabel = CreateWindow(L"STATIC", L"Writer :", WS_VISIBLE | WS_CHILD, 5, 130, 90, 20, editor, NULL, NULL, NULL);
	edit_writerEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 100, 130, 240, 20, editor, NULL, NULL, NULL);
	edit_dateLabel = CreateWindow(L"STATIC", L"Date :", WS_VISIBLE | WS_CHILD, 5, 155, 90, 20, editor, NULL, NULL, NULL);
	edit_dateEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 100, 155, 240, 20, editor, NULL, NULL, NULL);
	edit_lyricsLabel = CreateWindow(L"STATIC", L"Lyrics :", WS_VISIBLE | WS_CHILD, 5, 180, 90, 20, editor, NULL, NULL, NULL);
	edit_lyricsEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 100, 180, 240, 120, editor, NULL, NULL, NULL);
	edit_timeLabel = CreateWindow(L"STATIC", L"Played Time :", WS_VISIBLE | WS_CHILD, 5, 320, 330, 20, editor, NULL, NULL, NULL);
	edit_pathLabel = CreateWindow(L"EDIT", L"Path :", WS_VISIBLE | WS_CHILD | ES_READONLY | ES_AUTOHSCROLL, 5, 345, 330, 20, editor, NULL, NULL, NULL);
	edit_ok = CreateWindow(L"BUTTON", L"OK", WS_VISIBLE | WS_CHILD, 300, 360, 40, 20, editor, NULL, NULL, NULL);

	oldProc = (WNDPROC)SetWindowLong(edit_dateEdit, GWL_WNDPROC, (LONG)NumericProc); //ó�� �Լ��� �ٲٰ� ���� ������ ����
}

bool Application::IsRunning()
{
	return running;
}

bool Application::IsEditing()
{
	return editor_opened;
}

bool Application::pollEvent(CustomWinEvent e)
{
	DoublyIterator<Graphic*> iter(drawings);
	iter.ResetToLastPointer();
	Graphic* g;
	CustomWinEvent custom;

	switch (e.type)
	{
	case CustomWinEvent::MouseDown:
		mouseDown = true;
		if (focus) //������ ��Ŀ���� ���� ���
		{
			focus->setFocus(false); //������ ��Ŀ���� ����
			focus = nullptr; //��Ŀ�� ��ü�� null�� ����
		}

		while (iter.NotNull()) //��Ŀ���� �׷����� ã�´�.
		{
			g = iter.Current();

			if (g->isVisible() && g->hasPoint(sf::Vector2f(e.mouse.x, e.mouse.y)))
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

	case CustomWinEvent::MouseMoved:
		if (scrolling)
		{
			if (mouseDown)
			{
				float y = e.mouse.y;
				if (y < 300) y = 300;
				if (y > 540) y = 540;
				scrollSprite->SetPositionY(y);
				
			}
			else scrolling = false;
			UpdateScroll();
		}

		custom.type = CustomWinEvent::MouseOver;
		custom.mouse = CustomWinEvent::MouseEvent();
		custom.mouse.x = e.mouse.x;
		custom.mouse.y = e.mouse.y;

		while (iter.NotNull()) //���콺�� �÷��� ���� ��Ŀ���� �ƴϾ ó���ؾ� ��
		{
			g = iter.Current();

			if (g->isVisible() && g->hasPoint(sf::Vector2f(e.mouse.x, e.mouse.y)))
				g->pollEvent(custom); // ��Ŀ������ ���������� �ʴ´�.
			else g->pollEvent(e); //���콺 �����ӿ� ���� �̺�Ʈ ����

			iter.Prev();
		}
		break;

	case CustomWinEvent::MouseLeft:
		while (iter.NotNull()) //���콺�� ������ ������ ������ �� �̺�Ʈ ó��
		{
			iter.Current()->pollEvent(e);
			iter.Prev();
		}
		break;

	case CustomWinEvent::MouseUp:
		mouseDown = false; //������ �������ְ� �������� default�� ����

	default:
		if (focus) focus->pollEvent(e);
		break;
	}

	return true;
}

void Application::initDisplay()
{
	updating = true;

	DoublyIterator<Graphic*> iter(displayList);

	while (iter.NotNull())
	{
		currentGroup->AddGraphic(iter.Current(), false);
		iter.Next();
	}

	//��ũ�ѹ�
	float height = currentGroup->GetSize().y;

	if (height > 280)
	{
		if (!scrollSprite->isVisible())
		{
			scrollbackSprite->Show();
			scrollSprite->Show();
		}

		UpdateScroll();
	}
	else if (scrollSprite->isVisible())
	{
		scrollbackSprite->Hide();
		scrollSprite->Hide();
		scrollSprite->SetPositionY(300);

		currentGroup->SetPositionY(300);
		currentGroup->SetViewRect(0, 0, 320, 280);
	}
}

Group* Application::CreateDisplayGraphic(const SimpleMusicType& data)
{
	Group* graphic = new Group();
	graphic->SetData(String::WstrToStr(data.GetPath()));
	int id = displayList.GetLength();
	graphic->setID(id);
	graphic->SetPositionY(45 * id);

	Sprite* background = Resource::backSprite->clone();

	graphic->AddGraphic(background);

	TextLabel* nameLabel = new TextLabel(L"nonname");
	nameLabel->setCharacterSize(12);
	nameLabel->setTextColor(sf::Color::White);
	nameLabel->setAlign(TextAlign::LEFT);
	nameLabel->setFont(*Resource::defaultFont);
	nameLabel->SetPosition(7, 4);
	graphic->AddGraphic(nameLabel);

	std::wstring wstr = String::StrToWstr(data.GetName());
	if (wstr.size() >= 25) wstr = wstr.substr(0, 22) + L'\n' + wstr.substr(22);
	nameLabel->setText(wstr);

	Sprite* editButton = Resource::editSprite->clone();
	editButton->SetButton(true);
	editButton->SetPosition(261, 20);
	editButton->SetMouseUpFunction(func_editData);

	Sprite* removeButton = Resource::removeSprite->clone();
	removeButton->SetButton(true);
	removeButton->SetPosition(292, 20);
	removeButton->SetMouseUpFunction(func_removeData);

	Sprite* player = Resource::playSprite->clone();
	player->SetButton(true);
	player->SetPosition(230, 20);
	player->SetMouseUpFunction(func_playData);

	graphic->AddGraphic(editButton);
	graphic->AddGraphic(removeButton);
	graphic->AddGraphic(player);

	return graphic;
}

void Application::DisplayAllMusic()
{
	currentGroup->MakeEmpty();

	displayList.MakeEmpty();

	function<void(SimpleMusicType&)> func = [this](SimpleMusicType& data)
	{
		displayList.Add(CreateDisplayGraphic(data));
	};

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
	music.ReadDataFromID3(); //id3 �±׸� �о�´�.
	int dir = path.find_last_of(L'\\'); //������ ������ �ش��ϴ� �ε���

	if (music.GetName().empty()) //�̸� ������ ������
	{
		std::wstring file = path.substr(dir + 1);
		file = file.substr(0, file.size() - 4);
		music.SetName(String::WstrToStr(file)); //���� �̸����� ����
	}

	music.SetID(String::WstrToStr(music.GetPath()));
	
	if (!musicList.Add(music))
	{
		System::AlertError("�ش� ������ �̹� �����մϴ�.", "Music Already Exists", MB_OK);
		return 0;
	}

	SimpleMusicType simple = music; //����ȯ �����ڷ� SimpleMusicType���� ��ȯ

	simple.SetPath(music.GetPath());
	string simpleID = music.GetName() + '_' + music.GetArtist();
	simple.SetID(simpleID);

	int count = 0;

	while (!nameList.Add(simple)) //��δ� �ٸ����� �̸�_��Ƽ��Ʈ�� ���� �� �����Ƿ� �ߺ� ���� ó��
	{
		simple.SetID(simpleID + '_' + std::to_string(count));
		count++;
	}

	if (!music.GetAlbum().empty()) //�ٹ� Ÿ�� �߰�
	{
		Album album;
		album.SetAlbumName(music.GetAlbum());
		album.SetArtist(music.GetArtist());
		album.SetID(music.GetAlbum() + '_' + music.GetArtist());

		if (albumList.Get(album)) //�ٹ��� �̹� �����ϴ� ���
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

	if (!music.GetArtist().empty()) //��Ƽ��Ʈ Ÿ�� �߰�
	{
		Artist artist;
		artist.SetName(music.GetArtist());
		artist.SetID(music.GetArtist());

		if (artistList.Get(artist)) //��Ƽ��Ʈ�� �̹� �����ϴ� ���
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

	if (!music.GetGenre().empty()) //�帣 Ÿ�� �߰�
	{
		GenreType genre;
		genre.SetGenre(music.GetGenre());
		
		if (genreList.Get(genre)) //�帣�� �̹� �����ϴ� ���
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

	FolderType folder; //���� Ÿ�� �߰� (��δ� ������� �� �����Ƿ� empty üũ ����)
	folder.SetPath(path.substr(0, dir));
	
	if (folderList.Get(folder)) //���� Ÿ���� �̹� �����ϴ� ���
	{
		folder.AddMusic(simple);
		folderList.Replace(folder);
	}
	else
	{
		folder.AddMusic(simple);
		folderList.Add(folder);
	}

	if (newAddMusicList.IsFull()) //�ֱ� �߰��� ����Ʈ�� �� �� ������
	{
		SimpleMusicType temp;
		newAddMusicList.DeQueue(temp);
		addedCount--;
	}

	simple.SetID(music.GetID()); //primary key�� ��η� �缳��
	newAddMusicList.EnQueue(simple);
	addedCount++;

	UpdateList();
	return 1;
}

int Application::AddMusic(MusicType& music)
{
	wstring path = music.GetPath();
	int dir = path.find_last_of(L'\\'); //������ ������ �ش��ϴ� �ε���

	if (music.GetName().empty()) //�̸� ������ ������
	{
		std::wstring file = path.substr(dir + 1);
		file = file.substr(0, file.size() - 4);
		music.SetName(String::WstrToStr(file)); //���� �̸����� ����
	}

	music.SetID(String::WstrToStr(music.GetPath()));

	if (!musicList.Add(music)) return 0;

	SimpleMusicType simple = music; //����ȯ �����ڷ� SimpleMusicType���� ��ȯ

	string simpleID = music.GetName() + '_' + music.GetArtist();
	simple.SetID(simpleID);

	int count = 0;

	while (!nameList.Add(simple)) //��δ� �ٸ����� �̸�_��Ƽ��Ʈ�� ���� �� �����Ƿ� �ߺ� ���� ó��
	{
		simple.SetID(simpleID + '_' + std::to_string(count));
		count++;
	}

	if (!music.GetAlbum().empty()) //�ٹ� Ÿ�� �߰�
	{
		Album album;
		album.SetAlbumName(music.GetAlbum());
		album.SetArtist(music.GetArtist());
		album.SetID(music.GetAlbum() + '_' + music.GetArtist());

		if (albumList.Get(album)) //�ٹ��� �̹� �����ϴ� ���
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

	if (!music.GetArtist().empty()) //��Ƽ��Ʈ Ÿ�� �߰�
	{
		Artist artist;
		artist.SetName(music.GetArtist());
		artist.SetID(music.GetArtist());

		if (artistList.Get(artist)) //��Ƽ��Ʈ�� �̹� �����ϴ� ���
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

	if (!music.GetGenre().empty()) //�帣 Ÿ�� �߰�
	{
		GenreType genre;
		genre.SetGenre(music.GetGenre());

		if (genreList.Get(genre)) //�帣�� �̹� �����ϴ� ���
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

	FolderType folder; //���� Ÿ�� �߰� (��δ� ������� �� �����Ƿ� empty üũ ����)
	folder.SetPath(path.substr(0, dir));

	if (folderList.Get(folder)) //���� Ÿ���� �̹� �����ϴ� ���
	{
		folder.AddMusic(simple);
		folderList.Replace(folder);
	}
	else
	{
		folder.AddMusic(simple);
		folderList.Add(folder);
	}

	if (newAddMusicList.IsFull()) //�ֱ� �߰��� ����Ʈ�� �� �� ������
	{
		SimpleMusicType temp;
		newAddMusicList.DeQueue(temp);
		addedCount--;
	}

	simple.SetID(music.GetID()); //primary key�� ��η� �缳��
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

	while (iter.NotNull())
	{
		wstring newPath = iter.Current();
		if (newPath.size() > 4 && newPath.substr(newPath.size() - 4, 4) == L".mp3")
		{
			newPath = path + L'\\' + newPath; //���� ��� �����ֱ�

			MusicType music;
			music.SetPath(newPath);
			music.ReadDataFromID3(); //id3 �±׸� �о�´�.
			int dir = newPath.find_last_of(L'\\');

			if (music.GetName().empty()) //�̸� ������ ������
			{
				std::wstring file = newPath.substr(dir + 1);
				file = file.substr(0, file.size() - 4);
				music.SetName(String::WstrToStr(file)); //���� �̸����� ����
			}

			music.SetID(String::WstrToStr(music.GetPath()));

			if (!musicList.Add(music))
			{
				iter.Next();
				continue;
			}

			SimpleMusicType simple = music; //����ȯ �����ڷ� SimpleMusicType���� ��ȯ

			string simpleID = music.GetName() + '_' + music.GetArtist();
			simple.SetID(simpleID);

			int count = 0;

			while (!nameList.Add(simple)) //��δ� �ٸ����� �̸�_��Ƽ��Ʈ�� ���� �� �����Ƿ� �ߺ� ���� ó��
			{
				simple.SetID(simpleID + '_' + std::to_string(count));
				count++;
			}

			if (!music.GetAlbum().empty()) //�ٹ� Ÿ�� �߰�
			{
				Album album;
				album.SetAlbumName(music.GetAlbum());
				album.SetArtist(music.GetArtist());
				album.SetID(music.GetAlbum() + '_' + music.GetArtist());

				if (albumList.Get(album)) //�ٹ��� �̹� �����ϴ� ���
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

			if (!music.GetArtist().empty()) //��Ƽ��Ʈ Ÿ�� �߰�
			{
				Artist artist;
				artist.SetName(music.GetArtist());
				artist.SetID(music.GetArtist());

				if (artistList.Get(artist)) //��Ƽ��Ʈ�� �̹� �����ϴ� ���
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

			if (!music.GetGenre().empty()) //�帣 Ÿ�� �߰�
			{
				GenreType genre;
				genre.SetGenre(music.GetGenre());

				if (genreList.Get(genre)) //�帣�� �̹� �����ϴ� ���
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

			if (newAddMusicList.IsFull()) //�ֱ� �߰��� ����Ʈ�� �� �� ������
			{
				SimpleMusicType temp;
				newAddMusicList.DeQueue(temp);
				addedCount--;
			}

			simple.SetID(music.GetID()); //primary key�� ��η� �缳��
			newAddMusicList.EnQueue(simple);
			addedCount++;
			
		}
		iter.Next();
	}

	if (folderExist) folderList.Replace(folder);
	else folderList.Add(folder);

	UpdateList();
	return 1;
}

void Application::SwapEditor()
{
	BringWindowToTop(editor);
}

void Application::OpenEditor()
{
	if (editor_opened) return;

	editor_opened = true;
	
	ShowWindow(editor, SW_SHOW);
	SwapEditor();
}

void Application::CloseEditor()
{
	if (!editor_opened) return;

	ReplaceMusic();
	editor_opened = false;

	ShowWindow(editor, SW_HIDE);
}

int Application::EditMusic(const SimpleMusicType& music)
{
	if (editor_opened) return 0;

	editMusic = nullptr;
	AVLTreeIterator<MusicType> iter(musicList);
	MusicType* data = nullptr;

	while (iter.NotNull())
	{
		data = iter.CurrentPtr();
		int compare = wcscmp(data->GetPath().c_str(), music.GetPath().c_str());
		if (compare > 0) iter.GotoLeft();
		else if (compare < 0) iter.GotoRight();
		else break; //ã���� ����
	}

	if (data == nullptr) return 0; //�� ã���� 0 ��ȯ

	editMusic = data;

	SetWindowTextA(edit_nameEdit, editMusic->GetName().c_str());
	SetWindowTextA(edit_artistEdit, editMusic->GetArtist().c_str());
	SetWindowTextA(edit_albumEdit, editMusic->GetAlbum().c_str());
	SetWindowTextA(edit_genreEdit, editMusic->GetGenre().c_str());
	SetWindowTextA(edit_composerEdit, editMusic->GetComposer().c_str());
	SetWindowTextA(edit_writerEdit, editMusic->GetWriter().c_str());
	SetWindowTextA(edit_dateEdit, to_string(editMusic->GetDate()).c_str());
	SetWindowTextA(edit_lyricsEdit, editMusic->GetLyrics().c_str());
	SetWindowTextA(edit_timeLabel, ("Played Time : " + to_string(editMusic->GetPlayedTime())).c_str());
	SetWindowTextW(edit_pathLabel, (L"Path : " + editMusic->GetPath()).c_str());
	OpenEditor();
	return 1;
}

int Application::ReplaceMusic()
{
	if (editMusic == nullptr) return 0; //������ ���� Ÿ���� ������ ���� ������ 0 ��ȯ

	updating = true;

	char* newName;
	int len = GetWindowTextLengthA(edit_nameEdit) + 1; //������ +1
	newName = new char[len];
	newName[len - 1] = 0;
	GetWindowTextA(edit_nameEdit, newName, len);

	char* newArtist;
	len = GetWindowTextLengthA(edit_artistEdit) + 1;
	newArtist = new char[len];
	newArtist[len - 1] = 0;
	GetWindowTextA(edit_artistEdit, newArtist, len);

	bool idChange = strcmp(newName, editMusic->GetName().c_str()) || strcmp(newArtist, editMusic->GetArtist().c_str());

	wstring path = editMusic->GetPath();
	path = path.substr(0, path.find_last_of(L'\\'));

	SimpleMusicType simple = *editMusic;
	simple.SetID(editMusic->GetName() + '_' + editMusic->GetArtist());

	SimpleMusicType check = simple;
	nameList.Get(check);

	int count = 0;

	while (simple.GetPath() != check.GetPath())
	{
		check.SetID(simple.GetID() + '_' + to_string(count));
		count++;
		nameList.Get(check);
	}

	simple.SetID(check.GetID());

	if (idChange)
	{
		Album album;
		Artist artist;
		GenreType genre;
		FolderType folder;

		album.SetID(editMusic->GetAlbum() + '_' + editMusic->GetArtist());
		artist.SetID(editMusic->GetArtist());
		genre.SetGenre(editMusic->GetGenre());
		folder.SetPath(path);

		//�̸��̳� ��Ƽ��Ʈ�� �ٲ� ���, ID�� ����Ǿ����Ƿ� ����Ʈ���� ����
		nameList.Delete(simple);

		albumList.Get(album);
		album.DeleteMusic(simple);
		if (album.GetMusicNum() > 0) albumList.Replace(album);
		else albumList.Delete(album);

		artistList.Get(artist);
		artist.DeleteMusic(simple);
		if (artist.GetMusicNum() > 0) artistList.Replace(artist);
		else artistList.Delete(artist);
		
		genreList.Get(genre);
		genre.DeleteMusic(simple);
		if (genre.GetMusicNum() > 0) genreList.Replace(genre);
		else genreList.Delete(genre);
		
		folder.DeleteMusic(simple);
	}

	MusicType backup = *editMusic;

	editMusic->SetName(newName);
	editMusic->SetArtist(newArtist);
	delete[] newName;
	delete[] newArtist;

	char* newValue;

	len = GetWindowTextLengthA(edit_composerEdit) + 1;
	newValue = new char[len];
	newValue[len - 1] = 0;
	GetWindowTextA(edit_composerEdit, newValue, len);
	editMusic->SetComposer(newValue);
	delete[] newValue;

	len = GetWindowTextLengthA(edit_writerEdit) + 1;
	newValue = new char[len];
	newValue[len - 1] = 0;
	GetWindowTextA(edit_writerEdit, newValue, len);
	editMusic->SetWriter(newValue);
	delete[] newValue;

	len = GetWindowTextLengthA(edit_dateEdit) + 1;
	newValue = new char[len];
	newValue[len - 1] = 0;
	GetWindowTextA(edit_dateEdit, newValue, len);
	editMusic->SetDate(stoi(newValue));
	delete[] newValue;

	len = GetWindowTextLengthA(edit_albumEdit) + 1;
	newValue = new char[len];
	newValue[len - 1] = 0;
	GetWindowTextA(edit_albumEdit, newValue, len);
	editMusic->SetAlbum(newValue);
	delete[] newValue;

	len = GetWindowTextLengthA(edit_genreEdit) + 1;
	newValue = new char[len];
	newValue[len - 1] = 0;
	GetWindowTextA(edit_genreEdit, newValue, len);
	editMusic->SetGenre(newValue);
	delete[] newValue;

	len = GetWindowTextLengthA(edit_lyricsEdit) + 1;
	newValue = new char[len];
	newValue[len - 1] = 0;
	GetWindowTextA(edit_lyricsEdit, newValue, len);
	editMusic->SetLyrics(newValue);
	delete[] newValue;

	//���� ���� üũ

	if (!idChange)
	{
		backup.SetID(backup.GetName() + '_' + backup.GetArtist());

		if (editMusic->GetAlbum() != backup.GetAlbum())
		{
			Album oldAlbum;
			oldAlbum.SetID(backup.GetAlbum()+'_'+backup.GetArtist());
			if (albumList.Get(oldAlbum))
			{
				oldAlbum.DeleteMusic(backup);
				if (oldAlbum.GetMusicNum() > 0) albumList.Replace(oldAlbum);
				else albumList.Delete(oldAlbum);
			}
		}

		if (editMusic->GetArtist() != backup.GetArtist())
		{
			Artist oldArtist;
			oldArtist.SetID(backup.GetArtist());
			if (artistList.Get(oldArtist))
			{
				oldArtist.DeleteMusic(backup);
				if (oldArtist.GetMusicNum() > 0) artistList.Replace(oldArtist);
				else artistList.Delete(oldArtist);
			}
		}

		if (editMusic->GetGenre() != backup.GetGenre())
		{
			GenreType oldGenre;
			oldGenre.SetGenre(backup.GetGenre());
			if (genreList.Get(oldGenre))
			{
				if (oldGenre.GetMusicNum() > 0) oldGenre.DeleteMusic(backup);
				else genreList.Replace(oldGenre);
			}
		}
	}
	
	//����� ���� ����

	//musicList�� �����͸� �̿��� ���������Ƿ� Replace�� �ʿ� ���� (Path�� ID�ε� Path�� �״����)
	
	simple = *editMusic;
	Album album;
	Artist artist;
	GenreType genre;
	FolderType folder;

	album.SetID(editMusic->GetAlbum() + '_' + editMusic->GetArtist());
	artist.SetID(editMusic->GetArtist());
	genre.SetGenre(editMusic->GetGenre());
	folder.SetPath(path);

	if (!idChange)
	{
		simple.SetID(check.GetID());

		nameList.Replace(simple);

		if (albumList.Get(album))
		{
			if (!album.ReplaceMusic(simple)) album.AddMusic(simple);
			albumList.Replace(album);
		}
		else
		{
			album.SetAlbumName(editMusic->GetAlbum());
			album.SetArtist(editMusic->GetArtist());
			album.AddMusic(simple);
			albumList.Add(album);
		}

		if (artistList.Get(artist))
		{
			if (!artist.ReplaceMusic(simple)) artist.AddMusic(simple);
			artistList.Replace(artist);
		}
		else
		{
			artist.SetName(editMusic->GetArtist());
			artist.AddMusic(simple);
			artistList.Add(artist);
		}
		

		if (genreList.Get(genre))
		{
			if (!genre.ReplaceMusic(simple)) genre.AddMusic(simple);
			genreList.Replace(genre);
		}
		else
		{
			genre.AddMusic(simple);
			genreList.Add(genre);
		}

		folderList.Get(folder);

		if (!folder.ReplaceMusic(simple)) folder.AddMusic(simple);
		folderList.Replace(folder);

		UpdateList();
		return 1;
	}

	check = simple;

	if (nameList.Get(check)) //���� �׸��� �����ϸ�
	{
		//id �ߺ� üũ

		int count = 0;

		while (simple.GetPath() != check.GetPath())
		{
			check.SetID(simple.GetID() + '_' + to_string(count));
			count++;
			nameList.Get(check);
		}

		simple.SetID(check.GetID());
	}

	nameList.Add(simple);
	
	album.SetID(editMusic->GetAlbum() + '_' + editMusic->GetArtist());
	artist.SetID(editMusic->GetArtist());
	genre.SetGenre(editMusic->GetGenre());

	if (albumList.Get(album))
	{
		album.AddMusic(simple);
		albumList.Replace(album);
	}
	else
	{
		album.SetAlbumName(editMusic->GetAlbum());
		album.SetArtist(editMusic->GetArtist());
		album.AddMusic(simple);
		albumList.Add(album);
	}

	if (artistList.Get(artist))
	{
		artist.SetName(editMusic->GetArtist());
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

	folder.AddMusic(simple);
	folderList.Replace(folder);

	UpdateList();

	editMusic = nullptr;
	return 1;
}

int Application::DeleteMusic(const SimpleMusicType& music)
{	
	updating = true;

	MusicType data;
	data.SetID(String::WstrToStr(music.GetPath()));

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
	wstring path = data.GetPath();
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

	newAddMusicList = newQueue; //������ �������� ť�� �ִ��� �˻��Ͽ� �����.

	playLists.Do([simple](PlayList& p) { p.DeleteMusic(simple); }); //�÷��̸���Ʈ���� �����͸� ã�� ����

	UpdateList();
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

	m_inputFile >> recentPlayCount;
	m_inputFile.ignore();

	for (int i = 0; i < recentPlayCount; i++)
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

	// list�� ��� �����͸� ���Ͽ� ����
	musicList.Do([this](MusicType& music) { music.WriteDataToFile(m_outFile); });

	m_outFile << albumList.GetLength() << endl;

	albumList.Do([this](Album& album) { album.WriteDataToFile(m_outFile); });

	m_outFile << artistList.GetLength() << endl;

	artistList.Do([this](Artist& artist) { artist.WriteDataToFile(m_outFile); });

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
	recentPlayedList.Delete(music); //���� �ִٸ� ����
	recentPlayedList.Add(music);

	if (recentPlayedList.GetLength() > 30)
	{
		DoublyIterator<SimpleMusicType> iter(recentPlayedList);
		recentPlayedList.Delete(iter.First());
	}
}

void Application::AddToMostPlayed(const SimpleMusicType& music)
{
	mostPlayedList.Delete(music); //���� �ִٸ� ����
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
	updating = true;

	switch (displayMode)
	{
	case 0:
		DisplayAllMusic();
		break;

	case 1:
		DisplayAllAlbum();
		break;

	case 2:
		DisplayAllArtist();
		break;

	case 3:
		DisplayAllGenre();
		break;

	case 4:
		DisplayAllFolder();
		break;

	case 5:
		DisplayAllPlayList();
		break;
	}

	updating = false;
}

int Application::InputNumeric(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(oldProc, hWnd, iMessage, wParam, lParam);
}

void Application::UpdateScroll()
{
	if (!scrollSprite->isVisible()) return;

	float moveY = (scrollSprite->GetPosition().y - 300) / 240 * (currentGroup->GetSize().y - 282); //������ ũ�� 47 * ȭ�鿡 ���̴� �� 6
	currentGroup->SetPositionY(300 - moveY);
	currentGroup->SetViewRect(0, moveY, 320, 280);
}

void Application::ChangeState(int state)
{
	displayMode = state;
	UpdateMode();
	UpdateList();
}

void Application::DisplayAllAlbum()
{
	currentGroup->MakeEmpty();

	displayList.MakeEmpty();

	function<void(Album&)> func = [this](Album& data)
	{
		displayList.Add(CreateDisplayGraphic(data));
	};

	albumList.Do(func);

	initDisplay();
}

void Application::DisplayAllArtist()
{
	currentGroup->MakeEmpty();

	displayList.MakeEmpty();

	function<void(Artist&)> func = [this](Artist& data)
	{
		displayList.Add(CreateDisplayGraphic(data));
	};

	artistList.Do(func);

	initDisplay();
}

void Application::DisplayAllGenre()
{
	currentGroup->MakeEmpty();

	displayList.MakeEmpty();

	function<void(GenreType&)> func = [this](GenreType& data)
	{
		displayList.Add(CreateDisplayGraphic(data));
	};

	genreList.Do(func);

	initDisplay();
}

void Application::DisplayAllFolder()
{
	currentGroup->MakeEmpty();

	displayList.MakeEmpty();

	function<void(FolderType&)> func = [this](FolderType& data)
	{
		displayList.Add(CreateDisplayGraphic(data));
	};

	folderList.Do(func);

	initDisplay();
}

void Application::DisplayAllPlayList()
{
	currentGroup->MakeEmpty();

	displayList.MakeEmpty();

	function<void(PlayList&)> func = [this](PlayList& play)
	{
		displayList.Add(CreateDisplayGraphic(play));
	};

	playLists.Do(func);

	initDisplay();
}

Group* Application::CreateDisplayGraphic(const Album& data)
{
	Group* graphic = new Group();
	graphic->SetData(data.GetID());
	int id = displayList.GetLength();
	graphic->setID(id);
	graphic->SetPositionY(45 * id);

	Sprite* background = Resource::backSprite->clone();

	graphic->AddGraphic(background);

	TextLabel* nameLabel = new TextLabel(L"nonname");
	nameLabel->setCharacterSize(12);
	nameLabel->setTextColor(sf::Color::White);
	nameLabel->setAlign(TextAlign::LEFT);
	nameLabel->setFont(*Resource::defaultFont);
	nameLabel->SetPosition(7, 4);
	graphic->AddGraphic(nameLabel);

	TextLabel* artistLabel = new TextLabel(L"nonartist");
	artistLabel->setCharacterSize(12);
	artistLabel->setTextColor(sf::Color::White);
	artistLabel->setAlign(TextAlign::LEFT);
	artistLabel->setFont(*Resource::defaultFont);
	artistLabel->SetPosition(7, 25);
	graphic->AddGraphic(artistLabel);

	std::wstring wstr = String::StrToWstr(data.GetAlbumName());
	if (wstr.size() >= 25) wstr = wstr.substr(0, 22) + L"...";
	nameLabel->setText(wstr);

	wstr = String::StrToWstr(data.GetArtist() + " - num : " + to_string(data.GetMusicNum()));

	if (wstr.size() >= 25) wstr = wstr.substr(0, 22) + L"...";
	artistLabel->setText(wstr);

	Sprite* detailButton = Resource::detailSprite->clone();
	detailButton->SetButton(true);
	detailButton->SetPosition(261, 20);
	detailButton->SetMouseUpFunction(func_detail);

	Sprite* listButton = Resource::tolistSprite->clone();
	listButton->SetButton(true);
	listButton->SetPosition(292, 20);
	listButton->SetMouseUpFunction(func_tolist);

	graphic->AddGraphic(listButton);
	graphic->AddGraphic(detailButton);

	return graphic;
}

Group* Application::CreateDisplayGraphic(const Artist& data)
{
	Group* graphic = new Group();
	graphic->SetData(data.GetID());
	int id = displayList.GetLength();
	graphic->setID(id);
	graphic->SetPositionY(45 * id);

	Sprite* background = Resource::backSprite->clone();

	graphic->AddGraphic(background);

	TextLabel* nameLabel = new TextLabel(L"nonname");
	nameLabel->setCharacterSize(12);
	nameLabel->setTextColor(sf::Color::White);
	nameLabel->setAlign(TextAlign::LEFT);
	nameLabel->setFont(*Resource::defaultFont);
	nameLabel->SetPosition(7, 4);
	graphic->AddGraphic(nameLabel);

	TextLabel* numLabel = new TextLabel(L"nonnum");
	numLabel->setCharacterSize(12);
	numLabel->setTextColor(sf::Color::White);
	numLabel->setAlign(TextAlign::LEFT);
	numLabel->setFont(*Resource::defaultFont);
	numLabel->SetPosition(7, 25);
	graphic->AddGraphic(numLabel);

	std::wstring wstr = String::StrToWstr(data.GetName());
	if (wstr.size() >= 25) wstr = wstr.substr(0, 22) + L"...";
	nameLabel->setText(wstr);

	wstr = L"Music num : " + to_wstring(data.GetMusicNum());
	numLabel->setText(wstr);

	Sprite* detailButton = Resource::detailSprite->clone();
	detailButton->SetButton(true);
	detailButton->SetPosition(261, 20);
	detailButton->SetMouseUpFunction(func_detail);

	Sprite* listButton = Resource::tolistSprite->clone();
	listButton->SetButton(true);
	listButton->SetPosition(292, 20);
	listButton->SetMouseUpFunction(func_tolist);

	graphic->AddGraphic(listButton);
	graphic->AddGraphic(detailButton);

	return graphic;
}

Group* Application::CreateDisplayGraphic(const GenreType& data)
{
	Group* graphic = new Group();
	graphic->SetData(data.GetGenre());
	int id = displayList.GetLength();
	graphic->setID(id);
	graphic->SetPositionY(45 * id);

	Sprite* background = Resource::backSprite->clone();

	graphic->AddGraphic(background);

	TextLabel* nameLabel = new TextLabel(L"nonname");
	nameLabel->setCharacterSize(12);
	nameLabel->setTextColor(sf::Color::White);
	nameLabel->setAlign(TextAlign::LEFT);
	nameLabel->setFont(*Resource::defaultFont);
	nameLabel->SetPosition(7, 4);
	graphic->AddGraphic(nameLabel);

	TextLabel* numLabel = new TextLabel(L"nonnum");
	numLabel->setCharacterSize(12);
	numLabel->setTextColor(sf::Color::White);
	numLabel->setAlign(TextAlign::LEFT);
	numLabel->setFont(*Resource::defaultFont);
	numLabel->SetPosition(7, 25);
	graphic->AddGraphic(numLabel);

	std::wstring wstr = String::StrToWstr(data.GetGenre());
	if (wstr.size() >= 25) wstr = wstr.substr(0, 22) + L"...";
	nameLabel->setText(wstr);

	wstr = L"Music num : " + to_wstring(data.GetMusicNum());
	numLabel->setText(wstr);

	Sprite* detailButton = Resource::detailSprite->clone();
	detailButton->SetButton(true);
	detailButton->SetPosition(261, 20);
	detailButton->SetMouseUpFunction(func_detail);

	Sprite* listButton = Resource::tolistSprite->clone();
	listButton->SetButton(true);
	listButton->SetPosition(292, 20);
	listButton->SetMouseUpFunction(func_tolist);

	graphic->AddGraphic(listButton);
	graphic->AddGraphic(detailButton);

	return graphic;
}

Group* Application::CreateDisplayGraphic(const FolderType& data)
{
	Group* graphic = new Group();
	graphic->SetData(String::WstrToStr(data.GetPath()));
	int id = displayList.GetLength();
	graphic->setID(id);
	graphic->SetPositionY(45 * id);

	Sprite* background = Resource::backSprite->clone();

	graphic->AddGraphic(background);

	TextLabel* nameLabel = new TextLabel(L"nonname");
	nameLabel->setCharacterSize(12);
	nameLabel->setTextColor(sf::Color::White);
	nameLabel->setAlign(TextAlign::LEFT);
	nameLabel->setFont(*Resource::defaultFont);
	nameLabel->SetPosition(7, 4);
	graphic->AddGraphic(nameLabel);

	TextLabel* numLabel = new TextLabel(L"nonnum");
	numLabel->setCharacterSize(12);
	numLabel->setTextColor(sf::Color::White);
	numLabel->setAlign(TextAlign::LEFT);
	numLabel->setFont(*Resource::defaultFont);
	numLabel->SetPosition(7, 25);
	graphic->AddGraphic(numLabel);

	std::wstring wstr = data.GetPath();
	if (wstr.size() >= 25) wstr = wstr.substr(0, 22) + L"...";
	nameLabel->setText(wstr);

	wstr = L"Music num : " + to_wstring(data.GetLength());
	numLabel->setText(wstr);

	Sprite* detailButton = Resource::detailSprite->clone();
	detailButton->SetButton(true);
	detailButton->SetPosition(261, 20);
	detailButton->SetMouseUpFunction(func_detail);

	Sprite* listButton = Resource::tolistSprite->clone();
	listButton->SetButton(true);
	listButton->SetPosition(292, 20);
	listButton->SetMouseUpFunction(func_tolist);

	graphic->AddGraphic(listButton);
	graphic->AddGraphic(detailButton);

	return graphic;
}

Group* Application::CreateDisplayGraphic(const PlayList& play)
{
	Group* graphic = new Group();
	graphic->SetData(to_string(play.GetID()));
	int id = displayList.GetLength();
	graphic->setID(id);
	graphic->SetPositionY(45 * id);

	Sprite* background = Resource::backSprite->clone();

	graphic->AddGraphic(background);

	TextLabel* nameLabel = new TextLabel(L"nonname");
	nameLabel->setCharacterSize(12);
	nameLabel->setTextColor(sf::Color::White);
	nameLabel->setAlign(TextAlign::LEFT);
	nameLabel->setFont(*Resource::defaultFont);
	nameLabel->SetPosition(7, 4);
	graphic->AddGraphic(nameLabel);

	TextLabel* numLabel = new TextLabel(L"nonnum");
	numLabel->setCharacterSize(12);
	numLabel->setTextColor(sf::Color::White);
	numLabel->setAlign(TextAlign::LEFT);
	numLabel->setFont(*Resource::defaultFont);
	numLabel->SetPosition(7, 25);
	graphic->AddGraphic(numLabel);

	std::wstring wstr = L"PlayList " + to_wstring(play.GetID());
	if (wstr.size() >= 25) wstr = wstr.substr(0, 22) + L"...";
	nameLabel->setText(wstr);

	wstr = L"Music num : " + to_wstring(play.GetMusicNum());
	numLabel->setText(wstr);

	Sprite* editButton = Resource::editSprite->clone();
	editButton->SetButton(true);
	editButton->SetPosition(229, 20);
	editButton->SetMouseUpFunction(func_editlist);

	Sprite* saveButton = Resource::saveSprite->clone();
	saveButton->SetButton(true);
	saveButton->SetPosition(292, 20);
	saveButton->SetMouseUpFunction(func_savelist);

	Sprite* removeButton = Resource::removeSprite->clone();
	removeButton->SetButton(true);
	removeButton->SetPosition(292, 20);
	removeButton->SetMouseUpFunction(func_removelist);

	Sprite* player = Resource::playSprite->clone();
	player->SetButton(true);
	player->SetPosition(198, 20);
	player->SetMouseUpFunction(func_playlist);

	Sprite* detailButton = Resource::detailSprite->clone();
	detailButton->SetButton(true);
	detailButton->SetPosition(166, 20);
	detailButton->SetMouseUpFunction(func_detail);

	graphic->AddGraphic(editButton);
	graphic->AddGraphic(removeButton);
	graphic->AddGraphic(player);
	graphic->AddGraphic(detailButton);

	return graphic;
}

void Application::Search(const string& keyword, int mode)
{
	DoublyLinkedList<string> parser;

	int index = 0;
	int size = 0;

	for (int i = 0; i < keyword.size(); i++)
	{
		if (keyword.at(i) != '|') size++;
		else
		{
			parser.Add(keyword.substr(index, size));
			index = i+1;
			size = 0;
		}
	}

	if (size > 0) parser.Add(keyword.substr(index, size)); //���� ������ �߰�

	DoublyIterator<string> iter(parser);

	bool notLabel = true;

	if (mode == 6) //�ٸ� �ǿ��� music �˻�
	{
		currentGroup->MakeEmpty();
		displayList.MakeEmpty();
		string label = iter.First();
		string content;
		int tag = label.find(':');
		if (tag > 0)
		{
			content = label.substr(tag + 1);
			label = label.substr(0, tag);
		}
		else content = label;
			
		label = String::Strip(label); //���� ����
		content = String::Strip_LR(content);

		if (label == "album")
		{
			Album album;
			album.SetID(content);
			if (albumList.Get(album))
			{
				DoublyIterator<SimpleMusicType> iter_al = album.GetIterator();
				
				while (iter_al.NotNull())
				{
					displayList.Add(CreateDisplayGraphic(iter_al.Current()));
					iter_al.Next();
				}
			}
		}
		else if (label == "artist")
		{
			Artist artist;
			artist.SetID(content);
			if (artistList.Get(artist))
			{
				DoublyIterator<SimpleMusicType> iter_ar = artist.GetIterator();

				while (iter_ar.NotNull())
				{
					displayList.Add(CreateDisplayGraphic(iter_ar.Current()));
					iter_ar.Next();
				}
			}
		}
		else if (label == "genre")
		{
			GenreType genre;
			genre.SetGenre(content);
			if (genreList.Get(genre))
			{
				DoublyIterator<SimpleMusicType> iter_g = genre.GetIterator();

				while (iter_g.NotNull())
				{
					displayList.Add(CreateDisplayGraphic(iter_g.Current()));
					iter_g.Next();
				}
			}
		}
		else if (label == "folder")
		{
			FolderType folder;
			folder.SetPath(String::StrToWstr(content));

			if (folderList.Get(folder))
			{
				DoublyIterator<SimpleMusicType> iter_f = folder.GetIterator();

				while (iter_f.NotNull())
				{
					displayList.Add(CreateDisplayGraphic(iter_f.Current()));
					iter_f.Next();
				}
			}
		}
		else if (label == "playlist")
		{
			PlayList play;
			play.SetID(stoi(content));

			if (playLists.Get(play))
			{
				DoublyIterator<SimpleMusicType> iter_p = play.GetIterator();

				while (iter_p.NotNull())
				{
					displayList.Add(CreateDisplayGraphic(iter_p.Current()));
					iter_p.Next();
				}
			}
		}

		iter.Next();
	}

	currentGroup->MakeEmpty(false); //�� �Ʒ����ʹ� displaylist�� �ʱ�ȭ���� ����

	if (mode == 1) //album �˻�
	{
		string content = String::Strip_LR(iter.First());

		albumList.Do([&](Album& data) {
			if ((int)(data.GetAlbumName().find(content)) >= 0)
			{
				displayList.Add(CreateDisplayGraphic(data));
			}
		});
	}
	else if (mode == 2) //artist �˻�
	{
		string content = String::Strip_LR(iter.First());

		artistList.Do([&](Artist& data) {
			if ((int)(data.GetName().find(content)) >= 0)
			{
				displayList.Add(CreateDisplayGraphic(data));
			}
		});
	}
	else if (mode == 3) //genre �˻�
	{
		string content = String::Strip_LR(iter.First());

		genreList.Do([&](GenreType& data) {
			if ((int)(data.GetGenre().find(content)) >= 0)
			{
				displayList.Add(CreateDisplayGraphic(data));
			}
		});
	}
	else if (mode == 4) //folder �˻�
	{
		wstring content = String::StrToWstr(String::Strip_LR(iter.First()));

		folderList.Do([&](FolderType& data) {
			if ((int)(data.GetPath().find(content)) >= 0)
			{
				displayList.Add(CreateDisplayGraphic(data));
			}
		});
	}

	else if (mode == 5) //playlist �˻�
	{
		string content = String::Strip_LR(iter.First());

		int id = 0;
		for (int i = 0; i < content.size(); i++)
		{
			char ch = content.at(i);
			if (ch < 48 || ch > 57) continue; //���ڰ� �ƴ� ���� ����
			id = id * 10 + ch - 48;
		}

		PlayList play;
		play.SetID(id);

		playLists.Get(play);

		displayList.Add(CreateDisplayGraphic(play));
	}
	else if (mode == 0) //music �� �˻�
	{
		while (iter.NotNull())
		{
			string label = iter.Current();
			string content;
			int tag = label.find(':');
			if (tag > 0)
			{
				content = label.substr(tag + 1);
				label = label.substr(0, tag);
			}
			else content = label;

			label = String::Strip(label);
			content = String::Strip_LR(content);

			FilterDisplay(label, content, notLabel);
			iter.Next();
		}
	}

	if (mode == 6) mode = 0;
	displayMode = mode;
	UpdateMode();

	initDisplay();
	updating = false;
}

void Application::SendToDetail(const std::string& data)
{
	switch (displayMode)
	{
	case 1:
		Search("album:" + data, 6);
		break;

	case 2:
		Search("artist:" + data, 6);
		break;

	case 3:
		Search("genre:" + data, 6);
		break;

	case 4:
		Search("folder:" + data, 6);
		break;

	case 5:
		Search("playlist:" + data, 6);
		break;
	}
}

void Application::UpdateMode()
{
	song_sprite->SetTexturePos(0, 0);
	album_sprite->SetTexturePos(0, 0);
	artist_sprite->SetTexturePos(0, 0);
	genre_sprite->SetTexturePos(0, 0);
	folder_sprite->SetTexturePos(0, 0);
	playlist_sprite->SetTexturePos(0, 0);

	switch (displayMode)
	{
	case 0:
		song_sprite->SetTexturePos(60, 0);
		break;

	case 1:
		album_sprite->SetTexturePos(60, 0);
		break;

	case 2:
		artist_sprite->SetTexturePos(60, 0);
		break;

	case 3:
		genre_sprite->SetTexturePos(60, 0);
		break;

	case 4:
		folder_sprite->SetTexturePos(60, 0);
		break;

	case 5:
		playlist_sprite->SetTexturePos(60, 0);
		break;
	}
}

void Application::FilterDisplay(const std::string& filter, const std::string& content, bool& notLabel)
{
	DoublyIterator<Graphic*> iter(displayList);

	if (filter == "album")
	{
		while (iter.NotNull())
		{
			MusicType music;
			music.SetID(iter.Current()->GetData());
			musicList.Get(music);

			if ((int)music.GetAlbum().find(content) >= 0) iter.Next();
			else
			{
				displayList.Delete(iter);
				delete iter.Current();
			}
		}
	}
	else if (filter == "artist")
	{
		while (iter.NotNull())
		{
			MusicType music;
			music.SetID(iter.Current()->GetData());
			musicList.Get(music);

			if ((int)music.GetArtist().find(content) >= 0) iter.Next();
			else
			{
				displayList.Delete(iter);
				delete iter.Current();
			}
		}
	}
	else if (filter == "genre")
	{
		while (iter.NotNull())
		{
			MusicType music;
			music.SetID(iter.Current()->GetData());
			musicList.Get(music);

			if ((int)music.GetGenre().find(content) >= 0) iter.Next();
			else
			{
				displayList.Delete(iter);
				delete iter.Current();
			}
		}
	}
	else if (filter == "folder")
	{
		wstring path = String::StrToWstr(content);
		while (iter.NotNull())
		{
			MusicType music;
			music.SetID(iter.Current()->GetData());
			musicList.Get(music);

			if ((int)music.GetPath().find(path) >= 0) iter.Next();
			else
			{
				displayList.Delete(iter);
				delete iter.Current();
			}
		}
	}
	else if (filter == "composer")
	{
		while (iter.NotNull())
		{
			MusicType music;
			music.SetID(iter.Current()->GetData());
			musicList.Get(music);

			if ((int)music.GetComposer().find(content) >= 0) iter.Next();
			else
			{
				displayList.Delete(iter);
				delete iter.Current();
			}
		}
	}
	else if (filter == "writer")
	{
		while (iter.NotNull())
		{
			MusicType music;
			music.SetID(iter.Current()->GetData());
			musicList.Get(music);

			if ((int)music.GetWriter().find(content) >= 0) iter.Next();
			else
			{
				displayList.Delete(iter);
				delete iter.Current();
			}
		}
	}
	else if (filter == "lyrics")
	{
		while (iter.NotNull())
		{
			MusicType music;
			music.SetID(iter.Current()->GetData());
			musicList.Get(music);

			if ((int)music.GetLyrics().find(content) >= 0) iter.Next();
			else
			{
				displayList.Delete(iter);
				delete iter.Current();
			}
		}
	}
	else if (notLabel)
	{
		notLabel = false;
		
		int num = 0;

		while (iter.NotNull())
		{
			MusicType music;
			music.SetID(iter.Current()->GetData());
			musicList.Get(music);

			if ((int)music.GetName().find(content) >= 0)
			{
				iter.Current()->SetPosition(0, num * 45);
				num++;
				iter.Next();
			}
			else
			{
				delete iter.Current();
				displayList.Delete(iter);
			}
		}
	}
}

void Application::CreatePlayList(const std::string& id)
{
	displayMode = 7;

	PlayList play;
	play.SetID(playLists.GetLength()+1);

	if (id.empty()) //���� ���� ����Ʈ�� ������� ����
	{

	}
	else if (displayMode == 1) //�ٹ��� ������� ����
	{
		Album album;
		album.SetID(id);
		albumList.Get(album);

		DoublyIterator<SimpleMusicType> iter_al = album.GetIterator();

		while (iter_al.NotNull())
		{
			play.AddMusic(iter_al.Current());
			iter_al.Next();
		}
	}
	else if (displayMode == 2) //��Ƽ��Ʈ�� ������� ����
	{
		Artist artist;
		artist.SetID(id);
		artistList.Get(artist);

		DoublyIterator<SimpleMusicType> iter_ar = artist.GetIterator();

		while (iter_ar.NotNull())
		{
			play.AddMusic(iter_ar.Current());
			iter_ar.Next();
		}
	}
	else if (displayMode == 3) //�帣�� ������� ����
	{
		GenreType genre;
		genre.SetGenre(id);
		genreList.Get(genre);

		DoublyIterator<SimpleMusicType> iter_g = genre.GetIterator();

		while (iter_g.NotNull())
		{
			play.AddMusic(iter_g.Current());
			iter_g.Next();
		}
	}
	else if (displayMode == 4) //������ ������� ����
	{
		FolderType folder;
		folder.SetPath(String::StrToWstr(id));
		folderList.Get(folder);

		DoublyIterator<SimpleMusicType> iter_f = folder.GetIterator();

		while (iter_f.NotNull())
		{
			play.AddMusic(iter_f.Current());
			iter_f.Next();
		}
	}

	playLists.Add(play);
}

void Application::DisplayMusicForList(int id)
{
	PlayList play;
	play.SetID(id);

	if (!playLists.Get(play)) return;

	editList = true;
	currentEdit = play;
	currentGroup->MakeEmpty();

	displayList.MakeEmpty();

	DoublyIterator<SimpleMusicType> iter = play.GetIterator();

	function<void(SimpleMusicType&)> func = [&](SimpleMusicType& data)
	{
		bool found = false;
		if (iter.NotNull()) found = iter.Current().GetID() == data.GetID();
		if (found) iter.Next(); //iterator�� nameList�� id ������ �����Ƿ� ���������� Ž��

		displayList.Add(CreatePlayGraphic(data, found));
	};

	nameList.Do(func);

	initDisplay();
	updating = false;
}

Group* Application::CreatePlayGraphic(const SimpleMusicType& data, bool found)
{
	Group* graphic = new Group();
	graphic->SetData(data.GetID());
	int id = displayList.GetLength();
	graphic->setID(id);
	graphic->SetPositionY(45 * id);

	Sprite* background = Resource::backSprite->clone();

	graphic->AddGraphic(background);

	TextLabel* nameLabel = new TextLabel(L"nonname");
	nameLabel->setCharacterSize(12);
	nameLabel->setTextColor(sf::Color::White);
	nameLabel->setAlign(TextAlign::LEFT);
	nameLabel->setFont(*Resource::defaultFont);
	nameLabel->SetPosition(7, 4);
	graphic->AddGraphic(nameLabel);

	std::wstring wstr = String::StrToWstr(data.GetName());
	if (wstr.size() >= 25) wstr = wstr.substr(0, 22) + L'\n' + wstr.substr(22);
	nameLabel->setText(wstr);

	Sprite* listButton = Resource::addlistSprite->clone();
	listButton->SetTextureRect(25, 25);
	if (found) listButton->SetTexturePos(25, 0);
	listButton->SetButton(true);
	listButton->SetPosition(261, 20);
	listButton->SetMouseUpFunction(func_togglelist);

	graphic->AddGraphic(listButton);

	return graphic;
}

void Application::AddMusicToPlayList(const SimpleMusicType& data)
{
	SimpleMusicType simple;
	simple.SetID(data.GetID());
	nameList.Get(simple);

	if (currentEdit.AddMusic(simple)) playLists.Replace(currentEdit);
}

void Application::DeleteMusicFromPlayList(const SimpleMusicType& data)
{
	if (currentEdit.DeleteMusic(data)) playLists.Replace(currentEdit);
}

void Application::SearchInPlayList(const string& keyword)
{
	updating = true;
	DoublyLinkedList<string> parser;

	int index = 0;
	int size = 0;

	for (int i = 0; i < keyword.size(); i++)
	{
		if (keyword.at(i) != '|') size++;
		else
		{
			parser.Add(keyword.substr(index, size));
			index = i + 1;
			size = 0;
		}
	}

	if (size > 0) parser.Add(keyword.substr(index, size));

	DoublyIterator<string> iter(parser);

	bool notLabel = true;

	currentGroup->MakeEmpty();
	displayList.MakeEmpty();

	DoublyIterator<SimpleMusicType> iter_p = currentEdit.GetIterator();

	function<void(SimpleMusicType&)> func = [&](SimpleMusicType& data)
	{
		bool found = false;
		if (iter_p.NotNull()) found = iter_p.Current().GetID() == data.GetID();
		if (found) iter_p.Next();

		displayList.Add(CreatePlayGraphic(data, found));
	};

	nameList.Do(func); //�׸� ��� �߰�

	//���͸��ϸ鼭 ����
	while (iter.NotNull())
	{
		string label = iter.Current();
		string content;
		int tag = label.find(':');
		if (tag > 0)
		{
			content = label.substr(tag + 1);
			label = label.substr(0, tag);
		}
		else content = label;

		label = String::Strip(label);
		content = String::Strip_LR(content);

		FilterDisplay(label, content, notLabel);
		iter.Next();
	}

	initDisplay();
	updating = false;
}

void Application::DeletePlayList(int id)
{
	PlayList play;
	play.SetID(id);

	if (currentPlay.GetID() == id) StopMusicList(); //��� ���̴� ���� ����Ʈ�� ����

	if (playLists.Delete(play)) UpdateList();
}

void Application::StopMusicList()
{
	StopMusic();
	AVLTreeIterator<PlayList> iter(playLists);
	currentPlay = iter.Current(); //ó�� �÷��� ����Ʈ�� �ӽ� ��ü
}

void Application::SaveMusicList(int id)
{
	PlayList play;
	play.SetID(id);

	if (playLists.Get(play))
	{
		FileDialog dialog;
		wstring path;
		dialog.saveFileDialog(path, L"Window Media Play List;Zune Play List;M3U8 File", L"*.wpl;*.zpl;*.m3u8");

		if (path.size() < 4)
		{
			System::AlertError("�ùٸ��� ���� ����Դϴ�.", "Invalid Path", MB_OK);
			return;
		}
		
		wstring format = path.substr(path.size() - 4, 4);
		if (format == L".wpl" || format == L".zpl")
		{
			WPLWriter writer;
			writer.open(path);

			DoublyIterator<SimpleMusicType> iter = play.GetIterator();

			while (iter.NotNull())
			{
				writer.addFile(iter.CurrentPtr()->GetPath());
				iter.Next();
			}

			writer.close();
		}
		else if (format == L".m3u8")
		{
			M3U8Writer writer;
			writer.open(path);

			DoublyIterator<SimpleMusicType> iter = play.GetIterator();

			while (iter.NotNull())
			{
				MusicType music;
				music.SetID(String::WstrToStr(iter.CurrentPtr()->GetPath()));
				musicList.Get(music);

				writer.addMedia(music); //m3u8������ ��� �ÿ� artist - song_name ������ ��Ű�� ���ʰ� �־� musictype�� ������
				iter.Next();
			}

			writer.close();
		}
		else
		{
			System::AlertError("�ùٸ��� ���� ���� �����Դϴ�.", "Invalid File Format", MB_OK);
			return;
		}		
	}

	System::AlertError("�÷��� ����Ʈ�� ã�� ���߽��ϴ�.", "Cannot Find Playlist", MB_OK);
}