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
*	자료구조를 이용한 음악 플레이어 클래스
*	@author	김성주
*	@date	2018.09.20
*/

class Application {
private:
	HWND Handle; ///<실제 윈도우 핸들
	HWND editor; ///<에디터 용 콘솔

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

	int m_Command; ///< 사용자로부터 입력받은 현재 커맨드
	std::ifstream m_inputFile; ///< 파일 입력을 받기 위한 스트림
	std::ofstream m_outFile; ///< 파일 출력을 하기 위한 스트림
	AVLTree<MusicType> musicList; ///< 모든 음악을 저장하는 리스트 (master list). 경로 순으로 정렬.
	AVLTree<GenreType> genreList; ///< 장르를 저장하는 리스트
	CircularQueueType<SimpleMusicType> newAddMusicList = CircularQueueType<SimpleMusicType>(31); ///< 최근 추가된 음악 리스트.
	//30개 데이터를 추가해야 하므로 MAXSIZE = 31
	
	AVLTree<SimpleMusicType> nameList; ///<이름 순으로 음악을 저장하는 리스트. 이름_아티스트 순으로 정렬.
	AVLTree<Album> albumList;
	AVLTree<Artist> artistList;
	DoublyLinkedList<SimpleMusicType> recentPlayedList;
	DoublyLinkedList<SimpleMusicType> mostPlayedList;
	AVLTree<FolderType> folderList;
	AVLTree<PlayList> playLists;

	int recentPlayCount; ///<최근 재생한 음악 수
	int addedCount; ///<최근 추가한 음악 수

	sf::RenderWindow window; ///<뮤직 플레이어의 윈도우 객체
	sf::Color backColor; ///<백그라운드 컬러
	DoublyLinkedList<Graphic*> drawings; ///<렌더링할 그래픽 리스트. 특정 아이템 탐색 보다 전체 탐색이 빈번하므로 linked list 사용.
	DoublyLinkedList<Graphic*> displayList; ///<그래픽으로 출력할 데이터 리스트.

	Graphic* focus; ///<포커싱 중인 그래픽
	Group* currentGroup; ///<현재 출력중인 그룹
	bool running; ///<애플리케이션 구동 여부
	MusicType* editMusic; ///<에디터 윈도우에서 수정할 음악 타입
	SimpleMusicType currentMusic; ///<마지막으로 재생한 음악 데이터
	bool scrolling; ///<스크롤바 드래그 여부
	bool mouseDown;
	
	int displayMode; ///<출력할 리스트 모드 (0 : Music 1 : Album 2 : Artist 3 : Genre 4 : Playlist)


	function<int(const SimpleMusicType&, const SimpleMusicType&)> compareMusicName = [] (const SimpleMusicType& m1, const SimpleMusicType& m2) {
		if (m1.GetName().size() > 0 && m2.GetName().size() > 0) //둘 모두 유효한 이름을 가지고 있을 때
		{
			int compare = strcmp(m1.GetName().c_str(), m2.GetName().c_str());
			if (compare) return compare;
		}
		return strcmp(m1.GetID().c_str(), m2.GetID().c_str());
	};

	function<int(const SimpleMusicType&, const SimpleMusicType&)> compareToLast = [](const SimpleMusicType& m1, const SimpleMusicType& m2) {
		if (m1 == m2) return 0; //같은 항목은 같다고 반환
		return -1; //아닌 경우 last까지
	};

	function<int(const SimpleMusicType&, const SimpleMusicType&)> comparePlayedTime = [](const SimpleMusicType& m1, const SimpleMusicType& m2) {
		if (m1 == m2) return 0; //같은 항목은 같다고 반환

		int time = (m1.GetPlayedTime() > m2.GetPlayedTime()) - (m1.GetPlayedTime() < m2.GetPlayedTime());
		if (time) return time; //횟수가 더 많은 순으로

		return (m1 > m2) - (m1 < m2); //횟수가 같으면 ID 순으로
	};

	function<int(Graphic* const&, Graphic* const&)> compareGraphics = [](Graphic* const& g1, Graphic* const& g2) {
		return (*g1 > *g2) - (*g1 < *g2); //포인터는 주소값을 비교하므로, 가리키는 객체의 비교 관계를 반환한다.
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
		sprite->ResetMouseOver(); //마우스 오버 상태를 리셋
		sprite->TriggerMouseOver(); //마우스 오버 다시 트리거
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

	//멀티 쓰레드에서 윈도우를 렌더링할 때 쓸 함수
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
	*	기본 생성자
	*/
	Application();

	/**
	*	기본 소멸자
	*/
	~Application();

	/**
	*	@brief	프로그램을 실행한다.
	*	@pre	프로그램이 시작되어야 한다.
	*	@post	프로그램이 끝난다.
	*	@param	instance	현재 윈도우의 인스턴스 객체.
	*/
	void Run(HINSTANCE instance);

	bool IsRunning();

	/**
	*	@brief	리스트에 새로운 아이템을 추가한다.
	*	@pre	리스트가 초기화되어 있어야 한다.
	*	@post	새 아이템이 리스트에 추가된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int AddMusic();

	int AddMusicFromDirectory();

	/**
	*	@brief	사용자로부터 정보를 입력받아 아이템을 삭제한다.
	*	@pre	없음.
	*	@post	찾은 아이템이 리스트에서 삭제된다.
	*	@param	music	삭제할 음악 데이터
	*	@return 성공하면 1, 아니면 0을 반환.
	*/
	int DeleteMusic(const SimpleMusicType& music);

	/**
	*	@brief	사용자로부터 정보를 입력받아 아이템을 교체한다.
	*	@pre	없음.
	*	@post	찾은 아이템이 리스트에서 교체된다.
	*	@return 성공하면 1, 아니면 0을 반환.
	*/
	int ReplaceMusic();

	/**
	*	@brief	모든 정보를 화면에 출력한다.
	*	@pre	없음.
	*	@post	없음.
	*/
	void DisplayAllMusic();

	/**
	*	@brief	정보를 입력받을 파일을 연다.
	*	@pre	읽을 파일이 존재해야 한다.
	*	@post	파일을 읽기 모드로 연다.
	*	@param	fileName	읽을 파일의 이름.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int OpenInFile(char *fileName);

	/**
	*	@brief	정보를 저장하기 위한 파일을 연다.
	*	@pre	리스트가 초기화되어 있어야 한다.
	*	@post	파일을 쓰기 모드로 연다.
	*	@param	fileName	쓸 파일의 이름.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int OpenOutFile(char *fileName);

	/**
	*	@brief	파일을 열어 데이터를 모두 읽고, 리스트에 저장한다.
	*	@pre	파일이 이미 열려있지 않아야 한다.
	*	@post	리스트에 파일의 모든 정보가 저장된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int ReadDataFromFile();

	/**
	*	@brief	파일을 열어 현재 리스트의 데이터를 모두 저장한다.
	*	@pre	파일이 이미 열려있지 않아야 한다.
	*	@post	파일에 리스트의 모든 정보가 저장된다.
	*	@return 성공하면 1, 아니면 0을 반환.
	*/
	int WriteDataToFile();

	/**
	*	@brief	리스트의 아이템을 모두 비운다.
	*	@pre	없음.
	*	@post	리스트의 아이템이 모두 비워진다.
	*/
	void MakeEmpty();

	/**
	*	@brief	parameter로 받은 음악을 최근 재생한 리스트에 추가한다.
	*	@pre	paramter는 유효한 값이어야 한다.
	*	@post	parameter의 음악이 최근 재생한 음악 리스트에 추가된다.
	*	@param	music	추가할 음악.
	*/
	void AddToRecentPlayed(const SimpleMusicType& music);

	/**
	*	@brief	parameter로 받은 음악을 가장 많이 재생한 리스트에 추가한다.
	*	@pre	paramter는 유효한 값이어야 한다.
	*	@post	parameter의 음악이 가장 많이 재생한 음악 리스트에 추가된다.
	*	@param	music	추가할 음악.
	*/
	void AddToMostPlayed(const SimpleMusicType& music);

	/**
	*	@brief	음악을 재생한다.
	*	@pre	없음.
	*	@post	음악을 재생하고 최근 재생한/많이 재생한 음악 리스트에 추가한다.
	*	@param	music	재생할 음악 데이터
	*	@return	성공하면 1, 실패하면 0을 반환한다.
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
