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

	bool editor_opened;
	bool updating;

	TextBox* defaultSearch;
	Sprite* playerSprite;
	Sprite* minimizeSprite;
	Sprite* closeSprite;
	Sprite* playSprite;
	Sprite* prevSprite;
	Sprite* nextSprite;
	Sprite* searchSprite;
	Sprite* addDirSprite;
	Sprite* scrollbackSprite;
	Sprite* scrollSprite;

	Group* tab_song;
	Group* tab_album;
	Group* tab_artist;
	Group* tab_genre;
	Group* tab_folder;
	Group* tab_playlist;

	Sprite* song_sprite;
	Sprite* album_sprite;
	Sprite* artist_sprite;
	Sprite* genre_sprite;
	Sprite* folder_sprite;
	Sprite* playlist_sprite;

	HWND edit_nameLabel;
	HWND edit_nameEdit;
	HWND edit_artistLabel;
	HWND edit_artistEdit;
	HWND edit_albumLabel;
	HWND edit_albumEdit;
	HWND edit_genreLabel;
	HWND edit_genreEdit;
	HWND edit_composerLabel;
	HWND edit_composerEdit;
	HWND edit_writerLabel;
	HWND edit_writerEdit;
	HWND edit_dateLabel;
	HWND edit_dateEdit;
	HWND edit_lyricsLabel;
	HWND edit_lyricsEdit;
	HWND edit_timeLabel;
	HWND edit_pathLabel;
	HWND edit_ok;

	WNDPROC oldProc;

	int m_Command; ///< ����ڷκ��� �Է¹��� ���� Ŀ�ǵ�
	std::ifstream m_inputFile; ///< ���� �Է��� �ޱ� ���� ��Ʈ��
	std::ofstream m_outFile; ///< ���� ����� �ϱ� ���� ��Ʈ��
	AVLTree<MusicType> musicList; ///< ��� ������ �����ϴ� ����Ʈ (master list). ��� ������ ����.
	AVLTree<GenreType> genreList; ///< �帣�� �����ϴ� ����Ʈ
	CircularQueueType<SimpleMusicType> newAddMusicList = CircularQueueType<SimpleMusicType>(31); ///< �ֱ� �߰��� ���� ����Ʈ.
	//30�� �����͸� �߰��ؾ� �ϹǷ� MAXSIZE = 31
	
	AVLTree<SimpleMusicType> nameList; ///<�̸� ������ ������ �����ϴ� ����Ʈ. �̸�_��Ƽ��Ʈ ������ ����.
	AVLTree<Album> albumList;
	AVLTree<Artist> artistList;
	DoublyLinkedList<SimpleMusicType> recentPlayedList;
	DoublyLinkedList<SimpleMusicType> mostPlayedList;
	AVLTree<FolderType> folderList;
	AVLTree<PlayList> playLists;

	int recentPlayCount; ///<�ֱ� ����� ���� ��
	int addedCount; ///<�ֱ� �߰��� ���� ��

	sf::RenderWindow window; ///<���� �÷��̾��� ������ ��ü
	sf::Color backColor; ///<��׶��� �÷�
	DoublyLinkedList<Graphic*> drawings; ///<�������� �׷��� ����Ʈ. Ư�� ������ Ž�� ���� ��ü Ž���� ����ϹǷ� linked list ���.
	DoublyLinkedList<Graphic*> displayList; ///<�׷������� ����� ������ ����Ʈ.

	Graphic* focus; ///<��Ŀ�� ���� �׷���
	Group* currentGroup; ///<���� ������� �׷�
	bool running; ///<���ø����̼� ���� ����
	MusicType* editMusic; ///<������ �����쿡�� ������ ���� Ÿ��
	SimpleMusicType currentMusic; ///<���������� ����� ���� ������
	bool scrolling; ///<��ũ�ѹ� �巡�� ����
	bool mouseDown;
	
	int displayMode; ///<����� ����Ʈ ��� (0 : Music 1 : Album 2 : Artist 3 : Genre 4 : Playlist)


	function<int(const SimpleMusicType&, const SimpleMusicType&)> compareMusicName = [] (const SimpleMusicType& m1, const SimpleMusicType& m2) {
		if (m1.GetName().size() > 0 && m2.GetName().size() > 0) //�� ��� ��ȿ�� �̸��� ������ ���� ��
		{
			int compare = strcmp(m1.GetName().c_str(), m2.GetName().c_str());
			if (compare) return compare;
		}
		return strcmp(m1.GetID().c_str(), m2.GetID().c_str());
	};

	function<int(const SimpleMusicType&, const SimpleMusicType&)> compareToLast = [](const SimpleMusicType& m1, const SimpleMusicType& m2) {
		if (m1 == m2) return 0; //���� �׸��� ���ٰ� ��ȯ
		return -1; //�ƴ� ��� last����
	};

	function<int(const SimpleMusicType&, const SimpleMusicType&)> comparePlayedTime = [](const SimpleMusicType& m1, const SimpleMusicType& m2) {
		if (m1 == m2) return 0; //���� �׸��� ���ٰ� ��ȯ

		int time = (m1.GetPlayedTime() > m2.GetPlayedTime()) - (m1.GetPlayedTime() < m2.GetPlayedTime());
		if (time) return time; //Ƚ���� �� ���� ������

		return (m1 > m2) - (m1 < m2); //Ƚ���� ������ ID ������
	};

	function<int(Graphic* const&, Graphic* const&)> compareGraphics = [](Graphic* const& g1, Graphic* const& g2) {
		return (*g1 > *g2) - (*g1 < *g2); //�����ʹ� �ּҰ��� ���ϹǷ�, ����Ű�� ��ü�� �� ���踦 ��ȯ�Ѵ�.
	};

	function<void(Sprite*)> func_dragStart = [this](Sprite*) { SendMessage(Handle, WM_NCLBUTTONDOWN, HTCAPTION, 0); };
	function<void(Sprite*)> func_minimize = [this](Sprite*) { ShowWindow(Handle, SW_MINIMIZE); };
	function<void(Sprite*)> func_close = [this](Sprite*) { Close(); };
	function<void(Sprite*)> func_playMusic = [this](Sprite* sprite) {
		if (player->GetState() == PlayerState::Started)
		{
			sprite->SetTexturePos(0, 0);
			//pause
		}
		else
		{
			sprite->SetTexturePos(41, 0);
			PlayMusic(currentMusic);
		}
		sprite->ResetMouseOver(); //���콺 ���� ���¸� ����
		sprite->TriggerMouseOver(); //���콺 ���� �ٽ� Ʈ����
	};
	function<void(Sprite*)> func_addMusic = [this](Sprite*) {
		AddMusic();
	};
	function<void(Sprite*)> func_addDirMusic = [this](Sprite*) {
		AddMusicFromDirectory();
	};

	function<void(Sprite*)> func_editData = [this](Sprite* sprite) { 
		SimpleMusicType data;
		data.SetPath(String::StrToWstr(sprite->GetData()));
		EditMusic(data);
	};

	function<void(Sprite*)> func_removeData = [this](Sprite* sprite) { 
		SimpleMusicType data;
		data.SetPath(String::StrToWstr(sprite->GetData()));
		DeleteMusic(data);
	};

	function<void(Sprite*)> func_playData = [this](Sprite* sprite) {
		SimpleMusicType data;
		data.SetPath(String::StrToWstr(sprite->GetData()));
		PlayMusic(data);
	};

	function<void(Sprite*)> func_scrollDown = [this](Sprite*) {
		scrolling = true;
	};

	function<void(Sprite*)> func_scrollUp = [this](Sprite*) {
		scrolling = false;
	};

	function<void(Sprite*)> func_detail = [this](Sprite* sprite) {
		SendToDetail(sprite->GetData());
	};

	function<void(Sprite*)> func_search = [this](Sprite* sprite) {
		string data = String::WstrToStr(defaultSearch->getText());
		if (!data.empty()) Search(data, displayMode);
		else UpdateList();
	};

	//��Ƽ �����忡�� �����츦 �������� �� �� �Լ�
	void RenderMain();

	void initMainGraphic();
	void initEditor();

	void ReleaseMainGraphic();

	void initDisplay();

	void OpenEditor();

public:

	HWND GetHandle() { return Handle; }

	bool IsEditing();

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

	bool IsRunning();

	/**
	*	@brief	����Ʈ�� ���ο� �������� �߰��Ѵ�.
	*	@pre	����Ʈ�� �ʱ�ȭ�Ǿ� �־�� �Ѵ�.
	*	@post	�� �������� ����Ʈ�� �߰��ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int AddMusic();

	int AddMusicFromDirectory();

	/**
	*	@brief	����ڷκ��� ������ �Է¹޾� �������� �����Ѵ�.
	*	@pre	����.
	*	@post	ã�� �������� ����Ʈ���� �����ȴ�.
	*	@param	music	������ ���� ������
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int DeleteMusic(const SimpleMusicType& music);

	/**
	*	@brief	����ڷκ��� ������ �Է¹޾� �������� ��ü�Ѵ�.
	*	@pre	����.
	*	@post	ã�� �������� ����Ʈ���� ��ü�ȴ�.
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReplaceMusic();

	/**
	*	@brief	��� ������ ȭ�鿡 ����Ѵ�.
	*	@pre	����.
	*	@post	����.
	*/
	void DisplayAllMusic();

	/**
	*	@brief	������ �Է¹��� ������ ����.
	*	@pre	���� ������ �����ؾ� �Ѵ�.
	*	@post	������ �б� ���� ����.
	*	@param	fileName	���� ������ �̸�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int OpenInFile(char *fileName);

	/**
	*	@brief	������ �����ϱ� ���� ������ ����.
	*	@pre	����Ʈ�� �ʱ�ȭ�Ǿ� �־�� �Ѵ�.
	*	@post	������ ���� ���� ����.
	*	@param	fileName	�� ������ �̸�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int OpenOutFile(char *fileName);

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
	*	@brief	parameter�� ���� ������ �ֱ� ����� ����Ʈ�� �߰��Ѵ�.
	*	@pre	paramter�� ��ȿ�� ���̾�� �Ѵ�.
	*	@post	parameter�� ������ �ֱ� ����� ���� ����Ʈ�� �߰��ȴ�.
	*	@param	music	�߰��� ����.
	*/
	void AddToRecentPlayed(const SimpleMusicType& music);

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
	*	@post	������ ����ϰ� �ֱ� �����/���� ����� ���� ����Ʈ�� �߰��Ѵ�.
	*	@param	music	����� ���� ������
	*	@return	�����ϸ� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int PlayMusic(const SimpleMusicType& music);

	bool pollEvent(CustomWinEvent e);

	int AddGraphicToMain(Graphic* graphic);

	void Close();

	int EditMusic(const SimpleMusicType& music);

	int AddMusic(MusicType& music);

	void UpdateList();

	Group* CreateDisplayGraphic(const SimpleMusicType& data);

	Group* CreateDisplayGraphic(const Album& data);

	Group* CreateDisplayGraphic(const Artist& data);

	Group* CreateDisplayGraphic(const GenreType& data);

	Group* CreateDisplayGraphic(const FolderType& data);

	int InputNumeric(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	void CloseEditor();

	void UpdateScroll();

	void ChangeState(int state);

	void UpdateMode();

	void DisplayAllAlbum();

	void DisplayAllArtist();

	void DisplayAllGenre();

	void DisplayAllFolder();

	void Search(const std::string& keyword, int mode = 0);

	void SendToDetail(const std::string& data);

	void FilterDisplay(const std::string& label, const std::string& content, bool& notLabel);

};
#pragma once
#endif
