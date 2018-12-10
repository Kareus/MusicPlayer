#ifndef __APPLICATION__
#define __APPLICATION__

#include "Artist.h"
#include "Album.h"
#include "MusicType.h"
#include "SimpleMusicType.h"
#include "GenreType.h"
#include "FolderType.h"
#include "PlayList.h"

#include "CircularQueueType.h"
#include "DoublyLinkedList.h"
#include "SimpleMusicType.h"
#include "AVLTree.h"
#include <SFML/Graphics.hpp>
#include "MediaPlayer.h"
#include "GraphicElements.h"
#include "CustomWinEvent.h"
#include "GlobalFunctions.h"
#include "DoublyIterator.h"

#define FILENAMESIZE 1024

extern MediaPlayer* player;

/**
*	�ڷᱸ���� �̿��� ���� �÷��̾� Ŭ����
*	@author	�輺��
*	@date	2018.09.20
*/

class Application {
private:
	HWND Handle; ///<���� ������ �ڵ�
	HWND editor; ///<������ �� �ܼ�

	bool editor_opened; ///<�����Ͱ� ���ȴ��� ����
	bool updating; ///<�����͸� �����Ͽ� �׷����� ������Ʈ ������ ����

	TextBox* defaultSearch; ///<�˻�â �� �ؽ�Ʈ �ڽ�
	TextLabel* playName; ///<����ϴ� ���� �̸��� ����ϴ� �ؽ�Ʈ
	TextLabel* playTime; ///<��� �ð�:���� ������ ����ϴ� �ؽ�Ʈ
	Sprite* playerSprite; ///<�÷��̾� ���
	Sprite* minimizeSprite; ///<�ּ�ȭ ��ư
	Sprite* closeSprite; ///<�ݱ� ��ư
	Sprite* playSprite; ///<��� ��ư
	Sprite* prevSprite; ///<���� ��ư
	Sprite* nextSprite; ///<���� ��ư
	Sprite* searchSprite; ///<�˻� ��ư
	Sprite* addDirSprite; ///<���� �߰� ��ư
	Sprite* scrollbackSprite; ///<��ũ�ѹ� ��׶���
	Sprite* scrollSprite; ///<��ũ�ѹ�
	Sprite* listSprite; ///<�÷��� ����Ʈ �߰� ���� ��ư
	Sprite* saveFileSprite; ///<���� ���� ��ư
	Sprite* loadFileSprite; ///<���� �ε� ��ư

	Group* tab_song; ///<���� ��
	Group* tab_album; ///<�ٹ� ��
	Group* tab_artist; ///<��Ƽ��Ʈ ��
	Group* tab_genre; ///<�帣 ��
	Group* tab_folder; ///<���� ��
	Group* tab_playlist; ///<�÷��� ����Ʈ ��

	Sprite* song_sprite; ///<���� �ǿ� ����ϴ� �׷���
	Sprite* album_sprite; ///�ٹ� �ǿ� ����ϴ� �׷���
	Sprite* artist_sprite; ///<��Ƽ��Ʈ �ǿ� ����ϴ� �׷���
	Sprite* genre_sprite; ///<�帣 �ǿ��� ����ϴ� �׷���
	Sprite* folder_sprite; ///<���� �ǿ��� ����ϴ� �׷���
	Sprite* playlist_sprite; ///<�÷��� ����Ʈ �ǿ��� ����ϴ� �׷���

	HWND edit_nameLabel; ///<�������� �̸� �±�
	HWND edit_nameEdit; ///<�������� �̸� ���� â
	HWND edit_artistLabel; ///<�������� ��Ƽ��Ʈ �±�
	HWND edit_artistEdit; ///<�������� ��Ƽ��Ʈ ���� â
	HWND edit_albumLabel; ///<�������� �ٹ� �±�
	HWND edit_albumEdit; ///<�������� �ٹ� ���� â
	HWND edit_genreLabel; ///<�������� �帣 �±�
	HWND edit_genreEdit; ///<�������� �帣 ���� â
	HWND edit_composerLabel; ///<�������� �۰ �±�
	HWND edit_composerEdit; ///<�������� �۰ ���� â
	HWND edit_writerLabel; ///<�������� �ۻ簡 �±�
	HWND edit_writerEdit; ///<�������� �ۻ簡 ���� â
	HWND edit_dateLabel; ///<�������� ��¥ �±�
	HWND edit_dateEdit; ///<�������� ��¥ ���� â
	HWND edit_lyricsLabel; ///<�������� ���� �±�
	HWND edit_lyricsEdit; ///<�������� ���� ���� â 
	HWND edit_timeLabel; ///<�������� ���Ƚ�� �±�
	HWND edit_pathLabel; ///<�������� ��� �±�
	HWND edit_ok; ///<�������� Ȯ�� ��ư

	WNDPROC oldProc; ///<�������� ��Ʈ�ѿ��� ȣ���� ���ν��� �Լ�

	int m_Command; ///< ����ڷκ��� �Է¹��� ���� Ŀ�ǵ�
	std::ifstream m_inputFile; ///< ���� �Է��� �ޱ� ���� ��Ʈ��
	std::ofstream m_outFile; ///< ���� ����� �ϱ� ���� ��Ʈ��
	AVLTree<MusicType> musicList; ///< ��� ������ �����ϴ� ����Ʈ (master list). ��� ������ ����.
	AVLTree<GenreType> genreList; ///< �帣�� �����ϴ� ����Ʈ
	CircularQueueType<SimpleMusicType> newAddMusicList = CircularQueueType<SimpleMusicType>(31); ///< �ֱ� �߰��� ���� ����Ʈ.
	//30�� �����͸� �߰��ؾ� �ϹǷ� MAXSIZE = 31
	
	AVLTree<SimpleMusicType> nameList; ///<�̸� ������ ������ �����ϴ� ����Ʈ. �̸�_��Ƽ��Ʈ ������ ����.
	AVLTree<Album> albumList; ///<�ٹ����� ������ �����ϴ� ����Ʈ. �ٹ��� ������ ����
	AVLTree<Artist> artistList; ///<��Ƽ��Ʈ���� ������ �����ϴ� ����Ʈ. ��Ƽ��Ʈ�� ������ ����
	DoublyLinkedList<SimpleMusicType> mostPlayedList; ///<���� ���� ����� ������ �����ϴ� ����Ʈ. ���Ƚ�� ��, ID ������ ����.
	AVLTree<FolderType> folderList; ///<�������� ������ �����ϴ� ����Ʈ. ��� ������ ����.
	AVLTree<PlayList> playLists; ///<�÷��̸���Ʈ�� �����ϴ� ����Ʈ. ID ������ ����.

	int addedCount; ///<�ֱ� �߰��� ���� ��

	sf::RenderWindow window; ///<���� �÷��̾��� ������ ��ü
	sf::Color backColor; ///<��׶��� �÷�
	DoublyLinkedList<Graphic*> drawings; ///<�������� �׷��� ����Ʈ. Ư�� ������ Ž�� ���� ��ü Ž���� ����ϹǷ� linked list ���.
	DoublyLinkedList<Graphic*> displayList; ///<�׷������� ����� ������ ����Ʈ.

	Graphic* focus; ///<��Ŀ�� ���� �׷���
	Group* currentGroup; ///<���� ������� �׷�
	bool running; ///<���ø����̼� ���� ����
	MusicType* editMusic; ///<������ �����쿡�� ������ ���� Ÿ��
	PlayList currentEdit; ///<���� �����ϴ� �÷��̸���Ʈ
	PlayList currentPlay; ///<���� ����ϴ� �÷��̸���Ʈ
	DoublyIterator<SimpleMusicType> currentPlayIterator = currentPlay.GetIterator(); ///<���� ����ϴ� �÷��̸���Ʈ�� �ݺ���
	SimpleMusicType currentMusic; ///<���� ����ϴ� ����
	bool scrolling; ///<��ũ�ѹ� �巡�� ����
	bool mouseDown; ///<���콺 Ŭ�� ����
	
	int displayMode; ///<����� ����Ʈ ��� (0 : Music 1 : Album 2 : Artist 3 : Genre 4 : Playlist)

	function<int(const SimpleMusicType&, const SimpleMusicType&)> comparePlayedTime = [](const SimpleMusicType& m1, const SimpleMusicType& m2) {
		if (m1 == m2) return 0; //���� �׸��� ���ٰ� ��ȯ

		int time = (m1.GetPlayedTime() > m2.GetPlayedTime()) - (m1.GetPlayedTime() < m2.GetPlayedTime());
		if (time) return time; //Ƚ���� �� ���� ������

		return (m1 > m2) - (m1 < m2); //Ƚ���� ������ ID ������
	}; ///<���Ƚ���� ���ϴ� �Լ�

	function<int(Graphic* const&, Graphic* const&)> compareGraphics = [](Graphic* const& g1, Graphic* const& g2) {
		return (*g1 > *g2) - (*g1 < *g2); //�����ʹ� �ּҰ��� ���ϹǷ�, ����Ű�� ��ü�� �� ���踦 ��ȯ�Ѵ�.
	}; ///<�׷����� �������̹Ƿ� ��ü�� ���ϴ� �Լ�

	function<void(Sprite*)> func_dragStart = [this](Sprite*) { SendMessage(Handle, WM_NCLBUTTONDOWN, HTCAPTION, 0); }; ///<�÷��̾� �巡�׸� �����ϴ� �Լ�
	function<void(Sprite*)> func_minimize = [this](Sprite*) { ShowWindow(Handle, SW_MINIMIZE); }; ///<�÷��̾ �ּ�ȭ�ϴ� �Լ�
	function<void(Sprite*)> func_close = [this](Sprite*) { Close(); }; ///<�÷��̾ �����ϴ� �Լ�
	function<void(Sprite*)> func_playMusic = [this](Sprite* sprite) {
		if (player->GetState() == PlayerState::Started)
		{
			sprite->SetTexturePos(0, 0);
			PausePlay();
		}
		else
		{
			sprite->SetTexturePos(41, 0);
			ResumePlay();
		}
		sprite->ResetMouseOver(); //���콺 ���� ���¸� ����
		sprite->TriggerMouseOver(); //���콺 ���� �ٽ� Ʈ����
	}; ///<���� ���/�����ϴ� �Լ�

	function<void(Sprite*)> func_prevMusic = [this](Sprite* sprite) {
		Prev();
		sprite->ResetMouseOver();
		sprite->TriggerMouseOver();
	}; ///<���� �������� �̵��ϴ� �Լ�

	function<void(Sprite*)> func_nextMusic = [this](Sprite* sprite) {
		Next();
		sprite->ResetMouseOver();
		sprite->TriggerMouseOver();
	}; ///<���� �������� �̵��ϴ� �Լ�

	function<void(Sprite*)> func_addMusic = [this](Sprite*) {
		AddMusic();
	}; ///<������ ���Ϻ��� �߰��ϴ� �Լ�

	function<void(Sprite*)> func_addDirMusic = [this](Sprite*) {
		AddMusicFromDirectory();
	}; ///<������ �������� �߰��ϴ� �Լ�

	function<void(Sprite*)> func_editData = [this](Sprite* sprite) { 
		SimpleMusicType data;
		data.SetID(sprite->GetData());
		EditMusic(data);
	}; ///<���� ������ �����ϴ� �Լ�

	function<void(Sprite*)> func_removeData = [this](Sprite* sprite) { 
		SimpleMusicType data;
		data.SetID(sprite->GetData());
		DeleteMusic(data);
	}; ///<������ �����ϴ� �Լ�

	function<void(Sprite*)> func_playData = [this](Sprite* sprite) {
		SimpleMusicType data;
		data.SetID(sprite->GetData());
		PlayMusic(data);
	}; ///<������ ����ϴ� �Լ�

	function<void(Sprite*)> func_scrollDown = [this](Sprite*) {
		scrolling = true;
	}; ///<��ũ���� �����ϴ� �Լ�

	function<void(Sprite*)> func_scrollUp = [this](Sprite*) {
		scrolling = false;
	}; ///<��ũ�� ���� �Լ�

	function<void(Sprite*)> func_detail = [this](Sprite* sprite) {
		SendToDetail(sprite->GetData());
	}; ///<�ڼ��� ������ �˻��ϴ� �Լ�

	function<void(Sprite*)> func_search = [this](Sprite* sprite) {
		string data = String::WstrToStr(defaultSearch->getText());
		if (displayMode == 7)
		{
			SearchInPlayList(data);
			return;
		}
		if (!data.empty()) Search(data, displayMode);
		else UpdateList();
	}; ///<�Է¹��� �����͸� �Ľ��Ͽ� �˻��ϴ� �Լ�

	function<void(Sprite*)> func_tolist = [this](Sprite* sprite) {
		CreatePlayList(sprite->GetData());
	}; ///<���� ����� �÷��� ����Ʈ�� ��ȯ�ϴ� �Լ�

	function<void(Sprite*)> func_editlist = [this](Sprite* sprite) {
		DisplayMusicForList(stoi(sprite->GetData()));
	}; ///<�÷��� ����Ʈ�� �����ϴ� �Լ�

	function<void(Sprite*)> func_removelist = [this](Sprite* sprite) {
		DeletePlayList(stoi(sprite->GetData()));
	}; ///<�÷��� ����Ʈ�� �����ϴ� �Լ�

	function<void(Sprite*)> func_playlist = [this](Sprite* sprite) {
		currentPlay.SetID(stoi(sprite->GetData()));
		playLists.Get(currentPlay);
		PlayMusicList();
	}; ///<�÷��� ����Ʈ�� ����ϴ� �Լ�

	function<void(Sprite*)> func_savelist = [this](Sprite* sprite) {
		SaveMusicList(stoi(sprite->GetData()));
	}; ///<�÷��� ����Ʈ�� ���Ϸ� �����ϴ� �Լ�

	function<void(Sprite*)> func_togglelist = [this](Sprite* sprite) {
		SimpleMusicType simple;
		simple.SetID(sprite->GetData());
		int x = sprite->GetTexturePos().x;
		if (x > 0) DeleteMusicFromPlayList(simple);
		else AddMusicToPlayList(simple);

		sprite->SetTexturePos(25 - x, 0);
	}; ///<�÷��� ����Ʈ�� ���� ���� ��ȯ�ϴ� �Լ�

	function<void(Sprite*)> func_addlist = [this](Sprite*) {
		if (displayMode == 0) CreatePlayList();
		else if (displayMode == 5) AddPlayList();
		else if (displayMode == 7)
		{
			displayMode = 5;
			UpdateMode();
			UpdateList();
		}
	}; ///<�÷��� ����Ʈ�� �����ϴ� �Լ�

	function<void(Sprite*)> func_savefile = [this](Sprite*) { WriteDataToFile(); }; ///<���� �����͸� �����ϴ� �Լ�
	function<void(Sprite*)> func_loadfile = [this](Sprite*) { ReadDataFromFile(); }; ///<�����͸� �ҷ����� �Լ�

	/**
	*	@brief	�����츦 �������� �� �� �Լ�
	*/
	void RenderMain();

	/**
	*	@brief	���� �׷����� �����ϴ� �Լ�
	*/
	void initMainGraphic();

	/**
	*	@brief	������ �׷����� �����ϴ� �Լ�
	*/
	void initEditor();

	/**
	*	@brief	���� �׷����� �����ϴ� �Լ�
	*/
	void ReleaseMainGraphic();

	/**
	*	@brief	���� ������ ���� �׷��� ����Ʈ�� �ʱ�ȭ �� �����ϴ� �Լ�
	*/
	void initDisplay();

public:

	/**
	*	@brief	�÷��̾��� ������ �ڵ��� ��ȯ�ϴ� �Լ�
	*	@return	�÷��̾��� ������ �ڵ�
	*/
	HWND GetHandle();

	/**
	*	@brief	���� ������ ���� ������ ���θ� ��ȯ�ϴ� �Լ�
	*	@return	���� ���̸� true, �ƴϸ� false�� ��ȯ�Ѵ�
	*/
	bool IsEditing();

	/**
	*	@brief	�÷��̾� ���� ������ �����찡 ������ �����ϴ� �Լ�
	*/
	void SwapEditor();

	/**
	*	�⺻ ������
	*/
	Application();

	/**
	*	�⺻ �Ҹ���
	*/
	~Application();

	/**
	*	@brief	���α׷��� �����Ѵ�.
	*	@pre	���α׷��� ���۵Ǿ�� �Ѵ�.
	*	@post	���α׷��� ������.
	*	@param	instance	���� �������� �ν��Ͻ� ��ü.
	*/
	void Run(HINSTANCE instance);

	/**
	*	@brief	�÷��̾ �۵� ������ ���θ� ��ȯ�ϴ� �Լ�
	*	@return	�÷��̾ �۵� ���̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool IsRunning();

	/**
	*	@brief	���� ���ڸ� ���� ���� ������ �ҷ��� �߰��Ѵ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int AddMusic();

	/**
	*	@brief	���� ���ڸ� ���� ���� ��θ� �ҷ��� �� ������ ������ ��� �߰��Ѵ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int AddMusicFromDirectory();

	/**
	*	@brief	�Է¹��� ������ ������ �̿��� ����Ʈ���� ã�� �� ������ �����ϴ� �Լ�
	*	@pre	�Է¹޴� ���ǿ� primary key�� �ԷµǾ� �־�� �Ѵ�.
	*	@post	ã�� �������� ����Ʈ���� �����ȴ�.
	*	@param	music	������ ���� ������
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int DeleteMusic(const SimpleMusicType& music);

	/**
	*	@brief	������ �����츦 ���� ������ ���� �����͸� ����Ʈ���� ��ü�Ѵ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReplaceMusic();

	/**
	*	@brief	��� ���� ������ �÷��̾ ǥ���Ѵ�.
	*/
	void DisplayAllMusic();

	/**
	*	@brief	������ �Է¹��� ������ ����.
	*	@param	fileName	���� ������ �̸�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int OpenInFile(wstring fileName);

	/**
	*	@brief	������ �����ϱ� ���� ������ ����.
	*	@param	fileName	�� ������ �̸�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int OpenOutFile(wstring fileName);

	/**
	*	@brief	������ ���� �����͸� ��� �а�, ����Ʈ�� �����Ѵ�.
	*	@pre	������ �̹� �������� �ʾƾ� �Ѵ�.
	*	@post	����Ʈ�� ������ ��� ������ ����ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReadDataFromFile();

	/**
	*	@brief	������ ���� ���� ����Ʈ�� �����͸� ��� �����Ѵ�.
	*	@pre	������ �̹� �������� �ʾƾ� �Ѵ�.
	*	@post	���Ͽ� ����Ʈ�� ��� ������ ����ȴ�.
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int WriteDataToFile();

	/**
	*	@brief	����Ʈ�� �������� ��� ����.
	*	@pre	����.
	*	@post	����Ʈ�� �������� ��� �������.
	*/
	void MakeEmpty();

	/**
	*	@brief	parameter�� ���� ������ ���� ���� ����� ����Ʈ�� �߰��Ѵ�.
	*	@pre	paramter�� ��ȿ�� ���̾�� �Ѵ�.
	*	@post	parameter�� ������ ���� ���� ����� ���� ����Ʈ�� �߰��ȴ�.
	*	@param	music	�߰��� ����.
	*/
	void AddToMostPlayed(const SimpleMusicType& music);

	/**
	*	@brief	������ ����Ѵ�.
	*	@pre	����.
	*	@post	������ ����ϰ� ���� ���� ����� ���� ����Ʈ�� �߰��Ѵ�.
	*	@param	music	����� ���� ������
	*	@return	�����ϸ� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int PlayMusic(const SimpleMusicType& music);

	/**
	*	@brief	������ ���ν����κ��� ������ �̺�Ʈ�� �����Ѵ�.
	*	@param	e	������ �̺�Ʈ
	*	@return	�̺�Ʈ�� ���������� ����Ǹ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool pollEvent(CustomWinEvent e);

	/**
	*	@brief	�ش� �׷��� ��ü�� ������ ����Ʈ�� �߰��Ѵ�.
	*	@param	graphic	�߰��� �׷��� ��ü
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int AddGraphicToMain(Graphic* graphic);

	/**
	*	@brief	�÷��̾ �����Ѵ�.
	*/
	void Close();

	/**
	*	@brief	���� ���� ��ü�� ���� ������ �����츦 ����.
	*	@pre	������ ������ primary key�� ��ȿ�ؾ� �Ѵ�.
	*	@post	������ �����츦 ����.
	*	@param	music	������ ������ primary key�� ���� ���� ������
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int EditMusic(const SimpleMusicType& music);

	/**
	*	@brief	parameter�� ���� �����͸� ����Ʈ�� �߰��Ѵ�.
	*	@pre	parameter�� �ʿ��� �����͸� ��� ������ �־�� �Ѵ�.
	*	@post	�ش� ������ ����Ʈ�� �߰��ȴ�.
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int AddMusic(MusicType& music);

	/**
	*	@brief	����� ������ �������� �������� �׷����� �ٽ� �߰��Ѵ�.
	*/
	void UpdateList();

	/**
	*	@brief	���� �����͸� �������� ���� �׷����� �����Ѵ�.
	*	@param	data	������ �ش��ϴ� ���� ������
	*	@return	������ �׷��� �׷�
	*/
	Group* CreateDisplayGraphic(const SimpleMusicType& data);

	/**
	*	@brief	�ٹ� �����͸� �������� ���� �׷����� �����Ѵ�.
	*	@param	data	������ �ش��ϴ� �ٹ� ������
	*	@return	������ �׷��� �׷�
	*/
	Group* CreateDisplayGraphic(const Album& data);

	/**
	*	@brief	��Ƽ��Ʈ �����͸� �������� ���� �׷����� �����Ѵ�.
	*	@param	data	������ �ش��ϴ� ��Ƽ��Ʈ ������
	*	@return	������ �׷��� �׷�
	*/
	Group* CreateDisplayGraphic(const Artist& data);

	/**
	*	@brief	�帣 �����͸� �������� ���� �׷����� �����Ѵ�.
	*	@param	data	������ �ش��ϴ� �帣 ������
	*	@return	������ �׷��� �׷�
	*/
	Group* CreateDisplayGraphic(const GenreType& data);

	/**
	*	@brief	���� �����͸� �������� ���� �׷����� �����Ѵ�.
	*	@param	data	������ �ش��ϴ� ���� ������
	*	@return	������ �׷��� �׷�
	*/
	Group* CreateDisplayGraphic(const FolderType& data);

	/**
	*	@brief	�÷��� ����Ʈ �����͸� �������� ���� �׷����� �����Ѵ�.
	*	@param	data	������ �ش��ϴ� �÷��� ����Ʈ ������
	*	@return	������ �׷��� �׷�
	*/
	Group* CreateDisplayGraphic(const PlayList& data);

	/**
	*	@brief	���� �Է� ��� ���� ���ν��� �Լ�
	*	@param	hWnd	���ν��� ȣ�� ������
	*	@param	iMessage	���ν��� �޽���
	*	@param	wParam	ȣ�� �Ķ����
	*	@param	lParam	ȣ�� �Ķ����
	*	@return	���ν��� ȣ�� ��
	*/
	int InputNumeric(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	/**
	*	@brief	�����͸� ���� �Լ�
	*/
	void OpenEditor();

	/**
	*	@brief	�����͸� �ݴ� �Լ�
	*/
	void CloseEditor();

	/**
	*	@brief	��ũ�ѹٿ� ���� �׷����� ������Ʈ�ϴ� �Լ�
	*/
	void UpdateScroll();

	/**
	*	@brief	�Է¹��� ���¿� ���� �׷����� �ʱ�ȭ�ϰ� �������ϴ� �Լ�
	*	@param	state	������ �׷��� ���� (����, �ٹ�, ��Ƽ��Ʈ ��)
	*/
	void ChangeState(int state);

	/**
	*	@brief	�׷��� ���¿� ���� �׷��� �̹����� �����ϴ� �Լ�
	*/
	void UpdateMode();

	/**
	*	@brief	��� �ٹ��� ������ ����Ʈ�� �߰��ϴ� �Լ�
	*/
	void DisplayAllAlbum();

	/**
	*	@brief	��� ��Ƽ��Ʈ�� ������ ����Ʈ�� �߰��ϴ� �Լ�
	*/
	void DisplayAllArtist();

	/**
	*	@brief	��� �帣�� ������ ����Ʈ�� �߰��ϴ� �Լ�
	*/
	void DisplayAllGenre();

	/**
	*	@brief	��� ������ ������ ����Ʈ�� �߰��ϴ� �Լ�
	*/
	void DisplayAllFolder();

	/**
	*	@brief	��� �÷��̸���Ʈ�� ������ ����Ʈ�� �߰��ϴ� �Լ�
	*/
	void DisplayAllPlayList();

	/**
	*	@brief	�Է¹��� id�� �ش��ϴ� �÷��� ����Ʈ�� ���Ǹ��� ������ ����Ʈ�� �߰��ϴ� �Լ�
	*	@param	id	�÷��� ����Ʈ�� primary key
	*/
	void DisplayMusicForList(int id);

	/**
	*	@brief	�Է¹��� Ű����� �˻��� ������ �������� �˻��Ͽ� ����� ������ ����Ʈ�� �߰��ϴ� �Լ�
	*	@param	keyword	�Ľ̵��� ���� �˻��� Ű����
	*	@param	mode	�˻��� ���� (����, �ٹ�, ��Ƽ��Ʈ ��)
	*/
	void Search(const std::string& keyword, int mode = 0);

	/**
	*	@brief	�ٸ� �ǿ��� ���� ���� �˻��ϱ� ���� ȣ���ϴ� �Լ�
	*	@param	data	�˻��� ������
	*/
	void SendToDetail(const std::string& data);

	/**
	*	@brief	���� �������Ǵ� ������ ����Ʈ �߿��� �ش� ������ �����ϴ� �����͸� ����� �Լ�
	*	@param	label	�˻��� �������� ���� (�ٹ�, �۰, ���� ��)
	*	@param	content	�˻��� ������
	*	@param	notLabel	��ȿ�� ������ �������� ����
	*/
	void FilterDisplay(const std::string& label, const std::string& content, bool& notLabel);

	/**
	*	@brief	�Է¹��� id�� ���� ������ ������ ���� �÷��� ����Ʈ�� �����ϴ� �Լ�
	*	@param	id	���� �̿��� ���� �����Ϳ� ���� primary key
	*/
	void CreatePlayList(const std::string& id = "");

	/**
	*	@brief	�÷��� ����Ʈ�� ����� �߰�/�����ϱ� ���� ���� �����͸� �׷��� �������ϴ� �Լ�
	*	@param	data	�������� ���� ������
	*	@param	found	�ش� �����Ͱ� �÷��� ����Ʈ�� ���ԵǾ� �ִ��� ����
	*	@return	������ �׷��� �׷�
	*/
	Group* CreatePlayGraphic(const SimpleMusicType& data, bool found);

	/**
	*	@brief	�÷��� ����Ʈ�� �ش� ������ �߰��ϴ� �Լ�
	*	@param	data	�߰��� ����
	*/
	void AddMusicToPlayList(const SimpleMusicType& data);

	/**
	*	@brief	�÷��� ����Ʈ�� �ش� ������ �����ϴ� �Լ�
	*	@param	data	������ ����
	*/
	void DeleteMusicFromPlayList(const SimpleMusicType& data);

	/**
	*	@brief	�÷��� ����Ʈ�� ���� �����ϴ� �Լ�
	*/
	void AddPlayList();

	/**
	*	@brief	�÷��� ����Ʈ�� ����ϴ� �Լ�
	*/
	void PlayMusicList();

	/**
	*	@brief	�÷��� ����Ʈ ����� �����ϴ� �Լ�
	*/
	void StopMusicList();

	/**
	*	@brief	�ش� �÷��� ����Ʈ�� ���� ��� ��� ���Ϸ� �����ϴ� �Լ�
	*	@param	id	������ �÷��� ����Ʈ�� primary key
	*/
	void SaveMusicList(int id);

	/**
	*	@brief	�÷��� ����Ʈ �߰�/���� ��忡�� ������ �˻��� �� ����ϴ� �Լ�
	*	@param	keyword	�˻� Ű����
	*/
	void SearchInPlayList(const std::string& keyword);

	/**
	*	@brief	�÷��� ����Ʈ�� �����ϴ� �Լ�
	*	@param	id	������ �÷��� ����Ʈ�� primary key
	*/
	void DeletePlayList(int id);

	/**
	*	@brief	�Ͻ������� ������ �ٽ� ����ϴ� �Լ�
	*	@pre	����.
	*	@post	�Ͻ������� ������ ������ �� ������, �ƴϸ� ���Ƿ� �÷��� ����Ʈ�� �����Ͽ� ó�� ������ ����Ѵ�.
	*/
	void ResumePlay();

	/**
	*	@brief	����ϰ� �ִ� ������ �Ͻ� �����ϴ� �Լ�
	&	@pre	������ ����ǰ� �־�� �Ѵ�.
	*	@post	������ �Ͻ������ȴ�.
	*/
	void PausePlay();

	/**
	*	@brief	����ϰ� �ִ� ������ ���� �������� �̵��Ͽ� ����ϴ� �Լ�
	*	@pre	���� ������ �����ؾ� �Ѵ�.
	*	@post	���� ��� ���� ������ 5�� �̻� ����Ǹ� �� ������ ó������, �ƴϸ� ���� ������ ����Ѵ�. ���� ������ ������ ����� �����Ѵ�.
	*/
	void Prev();

	/**
	*	@brief	����ϰ� �ִ� ������ ���� ������ �̵��Ͽ� ����ϴ� �Լ�
	*	@pre	���� ������ �����ؾ� �Ѵ�.
	*	@post	���� ������ �����ϸ� ���� ������ ����ϰ�, �ƴϸ� ����� �����Ѵ�.
	*/
	void Next();

	/**
	*	���� ��� ���� ������ �����Ѵ�.
	*/
	void StopMusic();

	/**
	*	@brief	�ش� �÷��� ����Ʈ�� ����Ʈ�� �߰��Ѵ�.
	*	@pre	����Ʈ�� ��ȿ�� primary key�� ������ �־�� �Ѵ�.
	*	@post	�ش� �÷��� ����Ʈ�� ����Ʈ�� �߰��ȴ�.
	*	@param	play	�߰��� �÷��� ����Ʈ
	*/
	void AddPlayList(PlayList& play);
};
#pragma once
#endif
