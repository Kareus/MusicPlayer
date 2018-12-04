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
#include <thread>
#include "CustomWinEvent.h"

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
	HWND editHandle; ///<정보 수정 핸들

	sf::Font defaultFont;

	Sprite* playerSprite;
	Sprite* minimizeSprite;
	Sprite* closeSprite;
	Sprite* playSprite;
	Sprite* prevSprite;
	Sprite* nextSprite;
	Sprite* searchSprite;
	Sprite* addSprite;
	Sprite* addDirSprite;

	Sprite* editorSprite;
	Sprite* okSprite;
	Sprite* cancelSprite;

	TextBox* nameEdit;
	TextBox* artistEdit;
	TextBox* composerEdit;
	TextBox* writerEdit;
	TextBox* dateEdit;
	TextBox* albumEdit;
	TextBox* genreEdit;
	TextBox* lyricsEdit;
	TextLabel* lengthLabel;
	TextLabel* timeLabel;
	TextLabel* pathLabel;

	int m_Command; ///< 사용자로부터 입력받은 현재 커맨드
	std::ifstream m_inputFile; ///< 파일 입력을 받기 위한 스트림
	std::ofstream m_outFile; ///< 파일 출력을 하기 위한 스트림
	DoublyLinkedList<MusicType> musicList; ///< 모든 음악을 저장하는 리스트 (master list). 경로 순으로 정렬.
	DoublyLinkedList<GenreType> genreList; ///< 장르를 저장하는 리스트
	CircularQueueType<SimpleMusicType> newAddMusicList = CircularQueueType<SimpleMusicType>(31); ///< 최근 추가된 음악 리스트.
	//30개 데이터를 추가해야 하므로 MAXSIZE = 31
	
	AVLTree<SimpleMusicType> nameList; ///<이름 순으로 음악을 저장하는 리스트. 이름_아티스트 순으로 정렬.
	DoublyLinkedList<Album> albumList;
	DoublyLinkedList<Artist> artistList;
	DoublyLinkedList<SimpleMusicType> recentPlayedList;
	DoublyLinkedList<SimpleMusicType> mostPlayedList;
	AVLTree<FolderType> folderList;
	AVLTree<PlayList> playLists;

	int recentPlayCount; ///<최근 재생한 음악 수
	int addedCount; ///<최근 추가한 음악 수

	sf::RenderWindow window; ///<뮤직 플레이어의 윈도우 객체
	sf::RenderWindow editor;
	sf::Color backColor; ///<백그라운드 컬러
	DoublyLinkedList<Graphic*> drawings; ///<렌더링할 그래픽 리스트. 특정 아이템 탐색 보다 전체 탐색이 빈번하므로 linked list 사용.
	DoublyLinkedList<Graphic*> edit_drawings; ///<에디터의 그래픽 리스트.
	DoublyLinkedList<Graphic*> displayList;

	Graphic* focus; ///<포커싱 중인 그래픽
	Group* currentGroup; ///<현재 출력중인 그룹
	bool running; ///<애플리케이션 구동 여부
	bool editing; ///<정보 에디터 구동 여부
	MusicType* editMusic; ///<에디터 윈도우에서 수정할 음악 타입
	
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
			PlayMusic();
		}
		sprite->ResetMouseOver(); //마우스 오버 상태를 리셋
		sprite->TriggerMouseOver(); //마우스 오버 다시 트리거
	};
	function<void(Sprite*)> func_addMusic = [this](Sprite*) {
		AddMusic();
	};
	function<void(Sprite*)> func_dragStart_edit = [this](Sprite*) { SendMessage(editHandle, WM_NCLBUTTONDOWN, HTCAPTION, 0); };
	function<void(Sprite*)> func_ok_edit = [this](Sprite*) { ReplaceMusic(); };
	function<void(Sprite*)> func_cancel_edit = [this](Sprite*) {
		CloseEditor(); };

	//멀티 쓰레드에서 윈도우를 렌더링할 때 쓸 함수
	void RenderMain();
	void RenderEditor();

	void initMainGraphic();
	void initEditorGraphic();

	void ReleaseMainGraphic();
	void ReleaseEditorGraphic();

	void CloseEditor();

public:

	HWND GetHandle() { return Handle; }
	HWND GetEditor() { return editHandle; }

	bool IsEditing();
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

	/**
	*	@brief	사용자로부터 정보를 입력받아 아이템을 삭제한다.
	*	@pre	없음.
	*	@post	찾은 아이템이 리스트에서 삭제된다.
	*	@param	data	삭제할 음악 데이터
	*	@return 성공하면 1, 아니면 0을 반환.
	*/
	int DeleteMusic(const MusicType& data);

	/**
	*	@brief	사용자로부터 정보를 입력받아 아이템을 교체한다.
	*	@pre	없음.
	*	@post	찾은 아이템이 리스트에서 교체된다.
	*	@return 성공하면 1, 아니면 0을 반환.
	*/
	int ReplaceMusic();

	void DisplayResult();

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
	*	@brief	음악을 앨범에 추가한다.
	*	@pre	없음.
	*	@post	음악이 앨범에 추가된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int AddMusicToAlbum();

	/**
	*	@brief	음악을 앨범에서 삭제한다.
	*	@pre	없음.
	*	@post	음악이 앨범에서 삭제된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int DeleteMusicFromAlbum();

	/**
	*	@brief	앨범을 아티스트에 추가한다.
	*	@pre	없음.
	*	@post	앨범이 아티스트에 추가된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int AddAlbumToArtist();

	/**
	*	@brief	앨범을 아티스트에서 삭제한다.
	*	@pre	없음.
	*	@post	앨범이 아티스트에서 삭제한다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int DeleteAlbumFromArtist();

	/**
	*	@brief	parameter로 받은 음악을 최근 재생한 리스트에 추가한다.
	*	@pre	paramter는 유효한 값이어야 한다.
	*	@post	parameter의 음악이 최근 재생한 음악 리스트에 추가된다.
	*	@param	music	추가할 음악.
	*/
	void AddToRecentPlayed(const MusicType& music);

	/**
	*	@brief	parameter로 받은 음악을 가장 많이 재생한 리스트에 추가한다.
	*	@pre	paramter는 유효한 값이어야 한다.
	*	@post	parameter의 음악이 가장 많이 재생한 음악 리스트에 추가된다.
	*	@param	music	추가할 음악.
	*/
	void AddToMostPlayed(const MusicType& music);

	/**
	*	@brief	빈 앨범들을 찾아 삭제한다.
	*	@pre	없음.
	*	@post	수록곡이 없는 앨범들이 리스트에서 삭제된다.
	*/
	void ClearEmptyAlbums();

	/**
	*	@brief	빈 아티스트들을 찾아 삭제한다.
	*	@pre	없음.
	*	@post	앨범이 없는 아티스트들이 리스트에서 삭제된다.
	*/
	void ClearEmptyArtists();

	/**
	*	@brief	음악을 재생한다.
	*	@pre	없음.
	*	@post	찾은 음악을 재생한다.
	*	@return	성공하면 1, 실패하면 0을 반환한다.
	*/
	int PlayMusic();

	bool pollEvent(CustomWinEvent e);

	int AddGraphicToMain(Graphic* graphic);

	int AddGraphicToEditor(Graphic* graphic);

	Group* AddDisplayGraphic(MusicType* data);

	Group* AddEditGraphic(MusicType* data);

	void Close();

	int EditMusic();
};
#pragma once
#endif
