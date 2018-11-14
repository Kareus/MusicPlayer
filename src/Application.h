#ifndef __APPLICATION__
#define __APPLICATION__

#include "Artist.h"
#include "GenreType.h"
#include "CircularQueueType.h"
#include "DoublyLinkedList.h"
#include "SimpleMusicType.h"

#define FILENAMESIZE 1024

/**
*	자료구조를 이용한 음악 플레이어 클래스
*	@author	김성주
*	@date	2018.09.20
*/

class Application {
private:
	int m_Command; ///< 사용자로부터 입력받은 현재 커맨드
	std::ifstream m_inputFile; ///< 파일 입력을 받기 위한 스트림
	std::ofstream m_outFile; ///< 파일 출력을 하기 위한 스트림
	DoublyLinkedList<MusicType> musicList; ///< 모든 음악을 저장하는 리스트 (master list)
	DoublyLinkedList<GenreType> genreList; ///< 장르를 저장하는 리스트
	CircularQueueType<SimpleMusicType> newAddMusicList = CircularQueueType<SimpleMusicType>(31); ///< 최근 추가된 음악 리스트.
	//30개 데이터를 추가해야 하므로 MAXSIZE = 31
	
	DoublyLinkedList<SimpleMusicType> nameList;
	DoublyLinkedList<Album> albumList;
	DoublyLinkedList<Artist> artistList;
	DoublyLinkedList<SimpleMusicType> recentPlayedList;
	DoublyLinkedList<SimpleMusicType> mostPlayedList;
	int recentListCount; ///< 최근 재생한 음악 수 (<= 30)
	int addedCount; ///< 최근 추가한 음악 수 (<= 30)

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

		int time = (m1.GetPlayedTime() < m2.GetPlayedTime()) - (m1.GetPlayedTime() > m2.GetPlayedTime());
		if (time) return time; //횟수가 더 많은 순으로

		return (m1 < m2) - (m1 > m2); //횟수가 같으면 ID 순으로
	};

public:
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
	*/
	void Run();

	/**
	*	@brief	커맨드를 화면에 출력하고 키보드로부터 커맨드를 입력받는다.
	*	@pre	없음.
	*	@post	없음.
	*	@return	사용자가 입력한 커맨드.
	*/
	int GetCommand();

	/**
	*	@brief	리스트에 새로운 아이템을 추가한다.
	*	@pre	리스트가 초기화되어 있어야 한다.
	*	@post	새 아이템이 리스트에 추가된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int AddMusic();

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
	*	@brief	사용자로부터 정보를 입력받아 아이템을 삭제한다.
	*	@pre	없음.
	*	@post	찾은 아이템이 리스트에서 삭제된다.
	*	@return 성공하면 1, 아니면 0을 반환.
	*/
	int DeleteMusic();

	/**
	*	@brief	사용자로부터 정보를 입력받아 아이템을 교체한다.
	*	@pre	없음.
	*	@post	찾은 아이템이 리스트에서 교체된다.
	*	@return 성공하면 1, 아니면 0을 반환.
	*/
	int ReplaceMusic();

	/**
	*	@brief	사용자로부터 곡명을 입력받아 현재 리스트에서 그 이름이 포함된 모든 음악을 찾아 출력한다.
	*	@pre	없음.
	*	@post	찾은 음악들의 정보를 출력한다.
	*	@return 성공하면 1, 아니면 0을 반환.
	*/
	int SearchMusicByName();

	/**
	*	@brief	사용자로부터 가수의 이름을 입력받아 현재 리스트에서 그 가수의 모든 음악을 찾아 출력한다.
	*	@pre	없음.
	*	@post	찾은 음악들의 정보를 출력한다.
	*	@return 성공하면 1, 아니면 0을 반환.
	*/
	int SearchMusicByArtist();

	/**
	*	@brief	사용자로부터 앨범을 입력받아 현재 리스트에서 그 앨범에 포함된 모든 음악을 찾아 출력한다.
	*	@pre	없음.
	*	@post	찾은 음악들의 정보를 출력한다.
	*	@return 성공하면 1, 아니면 0을 반환.
	*/
	int SearchMusicByAlbum();

	/**
	*	@brief	사용자로부터 장르를 입력받아 현재 리스트에서 그 장르의 모든 음악을 찾아 출력한다.
	*	@pre	없음.
	*	@post	찾은 음악들의 정보를 출력한다.
	*	@return 성공하면 1, 아니면 0을 반환.
	*/
	int SearchMusicByGenre();

	/**
	*	@brief	리스트의 아이템을 모두 비운다.
	*	@pre	없음.
	*	@post	리스트의 아이템이 모두 비워진다.
	*/
	void MakeEmpty();

	/**
	*	@brief	최근에 추가된 음악의 정보를 출력한다.
	*	@pre	없음.
	*	@post	최근에 추가된 30곡의 정보를 출력한다.
	*/
	void DisplayNewMusic();

	/**
	*	@brief	음악을 장르별로 출력한다.
	*	@pre	없음.
	*	@post	각 장르별로 음악이 출력된다.
	*/
	void DisplayMusicByGenre();

	/**
	*	@brief	키보드로부터 이름과 장르를 입력받고 음악을 찾아 출력한다.
	*	@pre	없음.
	*	@post	이름과 장르가 일치하는 음악을 찾아 출력한다.
	*/
	void RetreiveByNameNGenre();

	/**
	*	@brief	parameter로 받은 음악을 추가한다.
	*	@pre	paramter는 유효한 값이어야 한다.
	*	@post	parameter의 음악이 리스트에 추가된다.
	*	@param	data	추가할 음악.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int AddMusic(const MusicType& data);

	/**
	*	@brief	parameter로 받은 음악을 삭제한다.
	*	@pre	paramter는 유효한 값이어야 한다.
	*	@post	parameter의 음악이 리스트에서 삭제된다.
	*	@param	data	삭제할 음악.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int DeleteMusic(const MusicType& music);

	/**
	*	@brief	parameter로 받은 음악을 교체한다.
	*	@pre	paramter는 유효한 값이어야 한다.
	*	@post	parameter의 음악이 리스트에서 교체된다.
	*	@param	data	교체할 음악.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int ReplaceMusic(const MusicType& music);

	/**
	*	@brief	앨범을 추가한다.
	*	@pre	없음.
	*	@post	앨범이 리스트에 추가된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int AddAlbum();

	/**
	*	@brief	앨범을 삭제한다.
	*	@pre	없음.
	*	@post	앨범이 리스트에서 삭제된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int DeleteAlbum();

	/**
	*	@brief	앨범을 교체한다.
	*	@pre	없음.
	*	@post	앨범이 리스트에서 교체된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int ReplaceAlbum();

	/**
	*	@brief	아티스트를 추가한다.
	*	@pre	없음.
	*	@post	아티스트가 리스트에 추가된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int AddArtist();

	/**
	*	@brief	아티스트를 삭제한다.
	*	@pre	없음.
	*	@post	아티스트가 리스트에서 삭제된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int DeleteArtist();
	
	/**
	*	@brief	아티스트를 교체한다.
	*	@pre	없음.
	*	@post	아티스트가 리스트에서 교체된다.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int ReplaceArtist();

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
	*	@brief	음악 메뉴를 보여준다.
	*	@pre	없음.
	*	@post	음악 메뉴로 진입한다.
	*	@return	음악 메뉴에서 나가면 0을 반환.
	*/
	int ShowMusicMenu();

	/**
	*	@brief	앨범 메뉴를 보여준다.
	*	@pre	없음.
	*	@post	앨범 메뉴로 진입한다.
	*	@return	앨범 메뉴에서 나가면 0을 반환.
	*/
	int ShowAlbumMenu();

	/**
	*	@brief	아티스트 메뉴를 보여준다.
	*	@pre	없음.
	*	@post	아티스트 메뉴로 진입한다.
	*	@return 아티스트 메뉴에서 나가면 0을 반환.
	*/
	int ShowArtistMenu();

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
	*	@brief	최근 재생한 음악들을 출력한다.
	*	@pre	없음.
	*	@post	최근 재생한 음악 리스트가 출력된다.
	*/
	void DisplayRecentPlayed();

	/**
	*	@brief	가장 많이 재생한 음악들을 출력한다.
	*	@pre	없음.
	*	@post	가장 많이 재생한 음악 리스트가 출력된다.
	*/
	void DisplayMostPlayed();

	/**
	*	@brief	음악 메뉴 관련 커맨드를 입력받는다.
	*	@pre	없음.
	*	@post	사용자로부터 음악 메뉴 커맨드를 입력받고 반환한다.
	*	@return	사용자의 음악 메뉴 커맨드.
	*/
	int GetMusicCommand();

	/**
	*	@brief	앨범 메뉴 관련 커맨드를 입력받는다.
	*	@pre	없음.
	*	@post	사용자로부터 앨범 메뉴 커맨드를 입력받고 반환한다.
	*	@return	사용자의 앨범 메뉴 커맨드.
	*/
	int GetAlbumCommand();

	/**
	*	@brief	아티스트 메뉴 관련 커맨드를 입력받는다.
	*	@pre	없음.
	*	@post	사용자로부터 아티스트 메뉴 커맨드를 입력받고 반환한다.
	*	@return	사용자의 아티스트 메뉴 커맨드.
	*/
	int GetArtistCommand();

	/**
	*	@brief	아티스트를 이름으로 찾아 반환한다.
	*	@pre	이름은 유효한 값이어야 한다.
	*	@post	data에 찾은 아티스트를 할당한다.
	*	@param	name	찾을 아티스트의 이름.
	*	@param	data	찾은 데이터를 할당할 아티스트.
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int GetArtistByName(const string& name, Artist& data);

	/**
	*	@brief	빈 앨범들을 찾아 삭제한다.
	*	@pre	없음.
	*	@post	수록곡이 없는 앨범들이 리스트에서 삭제된다.
	*/
	void ClearEmptyAlbums();

	/**
	*	@brief	primiary key를 통해 음악을 찾는다.
	*	@pre	없음.
	*	@post	찾은 음악의 정보를 출력한다.
	*	@return	찾으면 1, 아니면 0을 반환한다.
	*/
	int SearchMusicByID();

	/**
	*	@brief	작곡가의 이름을 통해 음악을 찾는다.
	*	@pre	없음.
	*	@post	찾은 음악의 정보를 출력한다.
	*	@return	찾으면 1, 아니면 0을 반환한다.
	*/
	int SearchMusicByComposer();

	/**
	*	@brief	primiary key를 통해 앨범을 찾는다.
	*	@pre	없음.
	*	@post	찾은 앨범의 정보를 출력한다.
	*	@return	찾으면 1, 아니면 0을 반환한다.
	*/
	int SearchAlbumByID();

	/**
	*	@brief	이름를 통해 앨범을 찾는다.
	*	@pre	없음.
	*	@post	찾은 앨범의 정보를 출력한다.
	*	@return	찾으면 1, 아니면 0을 반환한다.
	*/
	int SearchAlbumByName();

	/**
	*	@brief	아티스트의 이름을 통해 앨범을 찾는다.
	*	@pre	없음.
	*	@post	찾은 앨범의 정보를 출력한다.
	*	@return	찾으면 1, 아니면 0을 반환한다.
	*/
	int SearchAlbumByArtist();

	/**
	*	@brief	앨범을 아티스트의 이름 순으로 출력한다.
	*	@pre	없음.
	*	@post	앨범의 정보가 아티스트의 이름 순으로 출력된다.
	*/
	void DisplayAlbumByArtist();

	/**
	*	@brief	앨범을 이름 순으로 출력한다.
	*	@pre	없음.
	*	@post	앨범의 정보가 이름 순으로 출력된다.
	*/
	void DisplayAlbumByName();

	/**
	*	@brief	앨범을 앨범의 발표 날짜 순으로 출력한다.
	*	@pre	없음.
	*	@post	앨범의 정보가 발표 날짜 순으로 출력된다.
	*/
	void DisplayAlbumByDate();

	/**
	*	@brief	음악의 이름을 통해 앨범을 찾는다.
	*	@pre	없음.
	*	@post	찾은 앨범의 정보를 출력한다.
	*	@return	찾으면 1, 아니면 0을 반환한다.
	*/
	int SearchAlbumByMusic();

	/**
	*	@brief	primary key을 통해 아티스트를 찾는다.
	*	@pre	없음.
	*	@post	찾은 아티스트의 정보를 출력한다.
	*	@return	찾으면 1, 아니면 0을 반환한다.
	*/
	int SearchArtistByID();

	/**
	*	@brief	이름을 통해 아티스트를 찾는다.
	*	@pre	없음.
	*	@post	찾은 아티스트의 정보를 출력한다.
	*	@return	찾으면 1, 아니면 0을 반환한다.
	*/
	int SearchArtistByName();

	/**
	*	@brief	앨범의 이름을 통해 아티스트를 찾는다.
	*	@pre	없음.
	*	@post	찾은 아티스트의 정보를 출력한다.
	*	@return	찾으면 1, 아니면 0을 반환한다.
	*/
	int SearchArtistByAlbum();

	/**
	*	@brief	아티스트를 이름 순으로 출력한다.
	*	@pre	없음.
	*	@post	아티스트의 정보가 이름 순으로 출력된다.
	*/
	void DisplayArtistByName();

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

	void OpenURLOnBrowser(const string& path);
};
#pragma once
#endif
