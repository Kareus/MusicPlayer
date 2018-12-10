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
*	자료구조를 이용한 음악 플레이어 클래스
*	@author	김성주
*	@date	2018.09.20
*/

class Application {
private:
	HWND Handle; ///<실제 윈도우 핸들
	HWND editor; ///<에디터 용 콘솔

	bool editor_opened; ///<에디터가 열렸는지 여부
	bool updating; ///<데이터를 수정하여 그래픽을 업데이트 중인지 여부

	TextBox* defaultSearch; ///<검색창 용 텍스트 박스
	TextLabel* playName; ///<재생하는 음악 이름을 출력하는 텍스트
	TextLabel* playTime; ///<재생 시간:길이 정보를 출력하는 텍스트
	Sprite* playerSprite; ///<플레이어 배경
	Sprite* minimizeSprite; ///<최소화 버튼
	Sprite* closeSprite; ///<닫기 버튼
	Sprite* playSprite; ///<재생 버튼
	Sprite* prevSprite; ///<이전 버튼
	Sprite* nextSprite; ///<다음 버튼
	Sprite* searchSprite; ///<검색 버튼
	Sprite* addDirSprite; ///<폴더 추가 버튼
	Sprite* scrollbackSprite; ///<스크롤바 백그라운드
	Sprite* scrollSprite; ///<스크롤바
	Sprite* listSprite; ///<플레이 리스트 추가 관련 버튼
	Sprite* saveFileSprite; ///<파일 저장 버튼
	Sprite* loadFileSprite; ///<파일 로드 버튼

	Group* tab_song; ///<음악 탭
	Group* tab_album; ///<앨범 탭
	Group* tab_artist; ///<아티스트 탭
	Group* tab_genre; ///<장르 탭
	Group* tab_folder; ///<폴더 탭
	Group* tab_playlist; ///<플레이 리스트 탭

	Sprite* song_sprite; ///<음악 탭에 사용하는 그래픽
	Sprite* album_sprite; ///앨범 탭에 사용하는 그래픽
	Sprite* artist_sprite; ///<아티스트 탭에 사용하는 그래픽
	Sprite* genre_sprite; ///<장르 탭에서 사용하는 그래픽
	Sprite* folder_sprite; ///<폴더 탭에서 사용하는 그래픽
	Sprite* playlist_sprite; ///<플레이 리스트 탭에서 사용하는 그래픽

	HWND edit_nameLabel; ///<에디터의 이름 태그
	HWND edit_nameEdit; ///<에디터의 이름 수정 창
	HWND edit_artistLabel; ///<에디터의 아티스트 태그
	HWND edit_artistEdit; ///<에디터의 아티스트 수정 창
	HWND edit_albumLabel; ///<에디터의 앨범 태그
	HWND edit_albumEdit; ///<에디터의 앨범 수정 창
	HWND edit_genreLabel; ///<에디터의 장르 태그
	HWND edit_genreEdit; ///<에디터의 장르 수정 창
	HWND edit_composerLabel; ///<에디터의 작곡가 태그
	HWND edit_composerEdit; ///<에디터의 작곡가 수정 창
	HWND edit_writerLabel; ///<에디터의 작사가 태그
	HWND edit_writerEdit; ///<에디터의 작사가 수정 창
	HWND edit_dateLabel; ///<에디터의 날짜 태그
	HWND edit_dateEdit; ///<에디터의 날짜 수정 창
	HWND edit_lyricsLabel; ///<에디터의 가사 태그
	HWND edit_lyricsEdit; ///<에디터의 가사 수정 창 
	HWND edit_timeLabel; ///<에디터의 재생횟수 태그
	HWND edit_pathLabel; ///<에디터의 경로 태그
	HWND edit_ok; ///<에디터의 확인 버튼

	WNDPROC oldProc; ///<에디터의 컨트롤에서 호출할 프로시저 함수

	int m_Command; ///< 사용자로부터 입력받은 현재 커맨드
	std::ifstream m_inputFile; ///< 파일 입력을 받기 위한 스트림
	std::ofstream m_outFile; ///< 파일 출력을 하기 위한 스트림
	AVLTree<MusicType> musicList; ///< 모든 음악을 저장하는 리스트 (master list). 경로 순으로 정렬.
	AVLTree<GenreType> genreList; ///< 장르를 저장하는 리스트
	CircularQueueType<SimpleMusicType> newAddMusicList = CircularQueueType<SimpleMusicType>(31); ///< 최근 추가된 음악 리스트.
	//30개 데이터를 추가해야 하므로 MAXSIZE = 31
	
	AVLTree<SimpleMusicType> nameList; ///<이름 순으로 음악을 저장하는 리스트. 이름_아티스트 순으로 정렬.
	AVLTree<Album> albumList; ///<앨범별로 음악을 저장하는 리스트. 앨범명 순으로 정렬
	AVLTree<Artist> artistList; ///<아티스트별로 음악을 저장하는 리스트. 아티스트명 순으로 정렬
	DoublyLinkedList<SimpleMusicType> mostPlayedList; ///<가장 많이 재생한 음악을 저장하는 리스트. 재생횟수 순, ID 순으로 저장.
	AVLTree<FolderType> folderList; ///<폴더별로 음악을 저장하는 리스트. 경로 순으로 정렬.
	AVLTree<PlayList> playLists; ///<플레이리스트를 저장하는 리스트. ID 순으로 정렬.

	int addedCount; ///<최근 추가한 음악 수

	sf::RenderWindow window; ///<뮤직 플레이어의 윈도우 객체
	sf::Color backColor; ///<백그라운드 컬러
	DoublyLinkedList<Graphic*> drawings; ///<렌더링할 그래픽 리스트. 특정 아이템 탐색 보다 전체 탐색이 빈번하므로 linked list 사용.
	DoublyLinkedList<Graphic*> displayList; ///<그래픽으로 출력할 데이터 리스트.

	Graphic* focus; ///<포커싱 중인 그래픽
	Group* currentGroup; ///<현재 출력중인 그룹
	bool running; ///<애플리케이션 구동 여부
	MusicType* editMusic; ///<에디터 윈도우에서 수정할 음악 타입
	PlayList currentEdit; ///<현재 수정하는 플레이리스트
	PlayList currentPlay; ///<현재 재생하는 플레이리스트
	DoublyIterator<SimpleMusicType> currentPlayIterator = currentPlay.GetIterator(); ///<현재 재생하는 플레이리스트의 반복자
	SimpleMusicType currentMusic; ///<현재 재생하는 음악
	bool scrolling; ///<스크롤바 드래그 여부
	bool mouseDown; ///<마우스 클릭 여부
	
	int displayMode; ///<출력할 리스트 모드 (0 : Music 1 : Album 2 : Artist 3 : Genre 4 : Playlist)

	function<int(const SimpleMusicType&, const SimpleMusicType&)> comparePlayedTime = [](const SimpleMusicType& m1, const SimpleMusicType& m2) {
		if (m1 == m2) return 0; //같은 항목은 같다고 반환

		int time = (m1.GetPlayedTime() > m2.GetPlayedTime()) - (m1.GetPlayedTime() < m2.GetPlayedTime());
		if (time) return time; //횟수가 더 많은 순으로

		return (m1 > m2) - (m1 < m2); //횟수가 같으면 ID 순으로
	}; ///<재생횟수를 비교하는 함수

	function<int(Graphic* const&, Graphic* const&)> compareGraphics = [](Graphic* const& g1, Graphic* const& g2) {
		return (*g1 > *g2) - (*g1 < *g2); //포인터는 주소값을 비교하므로, 가리키는 객체의 비교 관계를 반환한다.
	}; ///<그래픽이 포인터이므로 객체를 비교하는 함수

	function<void(Sprite*)> func_dragStart = [this](Sprite*) { SendMessage(Handle, WM_NCLBUTTONDOWN, HTCAPTION, 0); }; ///<플레이어 드래그를 시작하는 함수
	function<void(Sprite*)> func_minimize = [this](Sprite*) { ShowWindow(Handle, SW_MINIMIZE); }; ///<플레이어를 최소화하는 함수
	function<void(Sprite*)> func_close = [this](Sprite*) { Close(); }; ///<플레이어를 종료하는 함수
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
		sprite->ResetMouseOver(); //마우스 오버 상태를 리셋
		sprite->TriggerMouseOver(); //마우스 오버 다시 트리거
	}; ///<음악 재생/정지하는 함수

	function<void(Sprite*)> func_prevMusic = [this](Sprite* sprite) {
		Prev();
		sprite->ResetMouseOver();
		sprite->TriggerMouseOver();
	}; ///<이전 음악으로 이동하는 함수

	function<void(Sprite*)> func_nextMusic = [this](Sprite* sprite) {
		Next();
		sprite->ResetMouseOver();
		sprite->TriggerMouseOver();
	}; ///<다음 음악으로 이동하는 함수

	function<void(Sprite*)> func_addMusic = [this](Sprite*) {
		AddMusic();
	}; ///<음악을 파일별로 추가하는 함수

	function<void(Sprite*)> func_addDirMusic = [this](Sprite*) {
		AddMusicFromDirectory();
	}; ///<음악을 폴더별로 추가하는 함수

	function<void(Sprite*)> func_editData = [this](Sprite* sprite) { 
		SimpleMusicType data;
		data.SetID(sprite->GetData());
		EditMusic(data);
	}; ///<음악 정보를 수정하는 함수

	function<void(Sprite*)> func_removeData = [this](Sprite* sprite) { 
		SimpleMusicType data;
		data.SetID(sprite->GetData());
		DeleteMusic(data);
	}; ///<음악을 삭제하는 함수

	function<void(Sprite*)> func_playData = [this](Sprite* sprite) {
		SimpleMusicType data;
		data.SetID(sprite->GetData());
		PlayMusic(data);
	}; ///<음악을 재생하는 함수

	function<void(Sprite*)> func_scrollDown = [this](Sprite*) {
		scrolling = true;
	}; ///<스크롤을 시작하는 함수

	function<void(Sprite*)> func_scrollUp = [this](Sprite*) {
		scrolling = false;
	}; ///<스크롤 종료 함수

	function<void(Sprite*)> func_detail = [this](Sprite* sprite) {
		SendToDetail(sprite->GetData());
	}; ///<자세한 정보를 검색하는 함수

	function<void(Sprite*)> func_search = [this](Sprite* sprite) {
		string data = String::WstrToStr(defaultSearch->getText());
		if (displayMode == 7)
		{
			SearchInPlayList(data);
			return;
		}
		if (!data.empty()) Search(data, displayMode);
		else UpdateList();
	}; ///<입력받은 데이터를 파싱하여 검색하는 함수

	function<void(Sprite*)> func_tolist = [this](Sprite* sprite) {
		CreatePlayList(sprite->GetData());
	}; ///<현재 목록을 플레이 리스트로 변환하는 함수

	function<void(Sprite*)> func_editlist = [this](Sprite* sprite) {
		DisplayMusicForList(stoi(sprite->GetData()));
	}; ///<플레이 리스트를 수정하는 함수

	function<void(Sprite*)> func_removelist = [this](Sprite* sprite) {
		DeletePlayList(stoi(sprite->GetData()));
	}; ///<플레이 리스트를 제거하는 함수

	function<void(Sprite*)> func_playlist = [this](Sprite* sprite) {
		currentPlay.SetID(stoi(sprite->GetData()));
		playLists.Get(currentPlay);
		PlayMusicList();
	}; ///<플레이 리스트를 재생하는 함수

	function<void(Sprite*)> func_savelist = [this](Sprite* sprite) {
		SaveMusicList(stoi(sprite->GetData()));
	}; ///<플레이 리스트를 파일로 저장하는 함수

	function<void(Sprite*)> func_togglelist = [this](Sprite* sprite) {
		SimpleMusicType simple;
		simple.SetID(sprite->GetData());
		int x = sprite->GetTexturePos().x;
		if (x > 0) DeleteMusicFromPlayList(simple);
		else AddMusicToPlayList(simple);

		sprite->SetTexturePos(25 - x, 0);
	}; ///<플레이 리스트와 음악 탭을 전환하는 함수

	function<void(Sprite*)> func_addlist = [this](Sprite*) {
		if (displayMode == 0) CreatePlayList();
		else if (displayMode == 5) AddPlayList();
		else if (displayMode == 7)
		{
			displayMode = 5;
			UpdateMode();
			UpdateList();
		}
	}; ///<플레이 리스트를 생성하는 함수

	function<void(Sprite*)> func_savefile = [this](Sprite*) { WriteDataToFile(); }; ///<현재 데이터를 저장하는 함수
	function<void(Sprite*)> func_loadfile = [this](Sprite*) { ReadDataFromFile(); }; ///<데이터를 불러오는 함수

	//윈도우를 렌더링할 때 쓸 함수
	void RenderMain();

	//메인 그래픽을 생성하는 함수
	void initMainGraphic();

	//에디터 그래픽을 생성하는 함수
	void initEditor();

	//메인 그래픽을 해제하는 함수
	void ReleaseMainGraphic();

	//음악 데이터 관련 그래픽 리스트를 초기화 및 생성하는 함수
	void initDisplay();

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
	int OpenInFile(wstring fileName);

	/**
	*	@brief	정보를 저장하기 위한 파일을 연다.
	*	@pre	리스트가 초기화되어 있어야 한다.
	*	@post	파일을 쓰기 모드로 연다.
	*	@param	fileName	쓸 파일의 이름.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int OpenOutFile(wstring fileName);

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

	Group* CreateDisplayGraphic(const PlayList& data);

	int InputNumeric(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	//에디터를 여는 함수
	void OpenEditor();

	void CloseEditor();

	void UpdateScroll();

	void ChangeState(int state);

	void UpdateMode();

	void DisplayAllAlbum();

	void DisplayAllArtist();

	void DisplayAllGenre();

	void DisplayAllFolder();

	void DisplayAllPlayList();

	void DisplayMusicForList(int id);

	void Search(const std::string& keyword, int mode = 0);

	void SendToDetail(const std::string& data);

	void FilterDisplay(const std::string& label, const std::string& content, bool& notLabel);

	void CreatePlayList(const std::string& id = "");

	Group* CreatePlayGraphic(const SimpleMusicType& data, bool found);

	void AddMusicToPlayList(const SimpleMusicType& data);

	void DeleteMusicFromPlayList(const SimpleMusicType& data);

	void AddPlayList();

	void PlayMusicList();

	void StopMusicList();

	void SaveMusicList(int id);

	void SearchInPlayList(const std::string& keyword);

	void DeletePlayList(int id);

	void ResumePlay();

	void PausePlay();

	void Prev();

	void Next();

	void StopMusic();

	void AddPlayList(PlayList& play);
};
#pragma once
#endif
