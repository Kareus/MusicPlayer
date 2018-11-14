#ifndef __APPLICATION__
#define __APPLICATION__

#include "Artist.h"
#include "GenreType.h"
#include "CircularQueueType.h"
#include "DoublyLinkedList.h"
#include "SimpleMusicType.h"

#define FILENAMESIZE 1024

/**
*	�ڷᱸ���� �̿��� ���� �÷��̾� Ŭ����
*	@author	�輺��
*	@date	2018.09.20
*/

class Application {
private:
	int m_Command; ///< ����ڷκ��� �Է¹��� ���� Ŀ�ǵ�
	std::ifstream m_inputFile; ///< ���� �Է��� �ޱ� ���� ��Ʈ��
	std::ofstream m_outFile; ///< ���� ����� �ϱ� ���� ��Ʈ��
	DoublyLinkedList<MusicType> musicList; ///< ��� ������ �����ϴ� ����Ʈ (master list)
	DoublyLinkedList<GenreType> genreList; ///< �帣�� �����ϴ� ����Ʈ
	CircularQueueType<SimpleMusicType> newAddMusicList = CircularQueueType<SimpleMusicType>(31); ///< �ֱ� �߰��� ���� ����Ʈ.
	//30�� �����͸� �߰��ؾ� �ϹǷ� MAXSIZE = 31
	
	DoublyLinkedList<SimpleMusicType> nameList;
	DoublyLinkedList<Album> albumList;
	DoublyLinkedList<Artist> artistList;
	DoublyLinkedList<SimpleMusicType> recentPlayedList;
	DoublyLinkedList<SimpleMusicType> mostPlayedList;
	int recentListCount; ///< �ֱ� ����� ���� �� (<= 30)
	int addedCount; ///< �ֱ� �߰��� ���� �� (<= 30)

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

		int time = (m1.GetPlayedTime() < m2.GetPlayedTime()) - (m1.GetPlayedTime() > m2.GetPlayedTime());
		if (time) return time; //Ƚ���� �� ���� ������

		return (m1 < m2) - (m1 > m2); //Ƚ���� ������ ID ������
	};

public:
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
	*/
	void Run();

	/**
	*	@brief	Ŀ�ǵ带 ȭ�鿡 ����ϰ� Ű����κ��� Ŀ�ǵ带 �Է¹޴´�.
	*	@pre	����.
	*	@post	����.
	*	@return	����ڰ� �Է��� Ŀ�ǵ�.
	*/
	int GetCommand();

	/**
	*	@brief	����Ʈ�� ���ο� �������� �߰��Ѵ�.
	*	@pre	����Ʈ�� �ʱ�ȭ�Ǿ� �־�� �Ѵ�.
	*	@post	�� �������� ����Ʈ�� �߰��ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int AddMusic();

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
	*	@brief	����ڷκ��� ������ �Է¹޾� �������� �����Ѵ�.
	*	@pre	����.
	*	@post	ã�� �������� ����Ʈ���� �����ȴ�.
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int DeleteMusic();

	/**
	*	@brief	����ڷκ��� ������ �Է¹޾� �������� ��ü�Ѵ�.
	*	@pre	����.
	*	@post	ã�� �������� ����Ʈ���� ��ü�ȴ�.
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReplaceMusic();

	/**
	*	@brief	����ڷκ��� ����� �Է¹޾� ���� ����Ʈ���� �� �̸��� ���Ե� ��� ������ ã�� ����Ѵ�.
	*	@pre	����.
	*	@post	ã�� ���ǵ��� ������ ����Ѵ�.
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int SearchMusicByName();

	/**
	*	@brief	����ڷκ��� ������ �̸��� �Է¹޾� ���� ����Ʈ���� �� ������ ��� ������ ã�� ����Ѵ�.
	*	@pre	����.
	*	@post	ã�� ���ǵ��� ������ ����Ѵ�.
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int SearchMusicByArtist();

	/**
	*	@brief	����ڷκ��� �ٹ��� �Է¹޾� ���� ����Ʈ���� �� �ٹ��� ���Ե� ��� ������ ã�� ����Ѵ�.
	*	@pre	����.
	*	@post	ã�� ���ǵ��� ������ ����Ѵ�.
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int SearchMusicByAlbum();

	/**
	*	@brief	����ڷκ��� �帣�� �Է¹޾� ���� ����Ʈ���� �� �帣�� ��� ������ ã�� ����Ѵ�.
	*	@pre	����.
	*	@post	ã�� ���ǵ��� ������ ����Ѵ�.
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int SearchMusicByGenre();

	/**
	*	@brief	����Ʈ�� �������� ��� ����.
	*	@pre	����.
	*	@post	����Ʈ�� �������� ��� �������.
	*/
	void MakeEmpty();

	/**
	*	@brief	�ֱٿ� �߰��� ������ ������ ����Ѵ�.
	*	@pre	����.
	*	@post	�ֱٿ� �߰��� 30���� ������ ����Ѵ�.
	*/
	void DisplayNewMusic();

	/**
	*	@brief	������ �帣���� ����Ѵ�.
	*	@pre	����.
	*	@post	�� �帣���� ������ ��µȴ�.
	*/
	void DisplayMusicByGenre();

	/**
	*	@brief	Ű����κ��� �̸��� �帣�� �Է¹ް� ������ ã�� ����Ѵ�.
	*	@pre	����.
	*	@post	�̸��� �帣�� ��ġ�ϴ� ������ ã�� ����Ѵ�.
	*/
	void RetreiveByNameNGenre();

	/**
	*	@brief	parameter�� ���� ������ �߰��Ѵ�.
	*	@pre	paramter�� ��ȿ�� ���̾�� �Ѵ�.
	*	@post	parameter�� ������ ����Ʈ�� �߰��ȴ�.
	*	@param	data	�߰��� ����.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int AddMusic(const MusicType& data);

	/**
	*	@brief	parameter�� ���� ������ �����Ѵ�.
	*	@pre	paramter�� ��ȿ�� ���̾�� �Ѵ�.
	*	@post	parameter�� ������ ����Ʈ���� �����ȴ�.
	*	@param	data	������ ����.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int DeleteMusic(const MusicType& music);

	/**
	*	@brief	parameter�� ���� ������ ��ü�Ѵ�.
	*	@pre	paramter�� ��ȿ�� ���̾�� �Ѵ�.
	*	@post	parameter�� ������ ����Ʈ���� ��ü�ȴ�.
	*	@param	data	��ü�� ����.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReplaceMusic(const MusicType& music);

	/**
	*	@brief	�ٹ��� �߰��Ѵ�.
	*	@pre	����.
	*	@post	�ٹ��� ����Ʈ�� �߰��ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int AddAlbum();

	/**
	*	@brief	�ٹ��� �����Ѵ�.
	*	@pre	����.
	*	@post	�ٹ��� ����Ʈ���� �����ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int DeleteAlbum();

	/**
	*	@brief	�ٹ��� ��ü�Ѵ�.
	*	@pre	����.
	*	@post	�ٹ��� ����Ʈ���� ��ü�ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReplaceAlbum();

	/**
	*	@brief	��Ƽ��Ʈ�� �߰��Ѵ�.
	*	@pre	����.
	*	@post	��Ƽ��Ʈ�� ����Ʈ�� �߰��ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int AddArtist();

	/**
	*	@brief	��Ƽ��Ʈ�� �����Ѵ�.
	*	@pre	����.
	*	@post	��Ƽ��Ʈ�� ����Ʈ���� �����ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int DeleteArtist();
	
	/**
	*	@brief	��Ƽ��Ʈ�� ��ü�Ѵ�.
	*	@pre	����.
	*	@post	��Ƽ��Ʈ�� ����Ʈ���� ��ü�ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReplaceArtist();

	/**
	*	@brief	������ �ٹ��� �߰��Ѵ�.
	*	@pre	����.
	*	@post	������ �ٹ��� �߰��ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int AddMusicToAlbum();

	/**
	*	@brief	������ �ٹ����� �����Ѵ�.
	*	@pre	����.
	*	@post	������ �ٹ����� �����ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int DeleteMusicFromAlbum();

	/**
	*	@brief	�ٹ��� ��Ƽ��Ʈ�� �߰��Ѵ�.
	*	@pre	����.
	*	@post	�ٹ��� ��Ƽ��Ʈ�� �߰��ȴ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int AddAlbumToArtist();

	/**
	*	@brief	�ٹ��� ��Ƽ��Ʈ���� �����Ѵ�.
	*	@pre	����.
	*	@post	�ٹ��� ��Ƽ��Ʈ���� �����Ѵ�.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int DeleteAlbumFromArtist();

	/**
	*	@brief	���� �޴��� �����ش�.
	*	@pre	����.
	*	@post	���� �޴��� �����Ѵ�.
	*	@return	���� �޴����� ������ 0�� ��ȯ.
	*/
	int ShowMusicMenu();

	/**
	*	@brief	�ٹ� �޴��� �����ش�.
	*	@pre	����.
	*	@post	�ٹ� �޴��� �����Ѵ�.
	*	@return	�ٹ� �޴����� ������ 0�� ��ȯ.
	*/
	int ShowAlbumMenu();

	/**
	*	@brief	��Ƽ��Ʈ �޴��� �����ش�.
	*	@pre	����.
	*	@post	��Ƽ��Ʈ �޴��� �����Ѵ�.
	*	@return ��Ƽ��Ʈ �޴����� ������ 0�� ��ȯ.
	*/
	int ShowArtistMenu();

	/**
	*	@brief	parameter�� ���� ������ �ֱ� ����� ����Ʈ�� �߰��Ѵ�.
	*	@pre	paramter�� ��ȿ�� ���̾�� �Ѵ�.
	*	@post	parameter�� ������ �ֱ� ����� ���� ����Ʈ�� �߰��ȴ�.
	*	@param	music	�߰��� ����.
	*/
	void AddToRecentPlayed(const MusicType& music);

	/**
	*	@brief	parameter�� ���� ������ ���� ���� ����� ����Ʈ�� �߰��Ѵ�.
	*	@pre	paramter�� ��ȿ�� ���̾�� �Ѵ�.
	*	@post	parameter�� ������ ���� ���� ����� ���� ����Ʈ�� �߰��ȴ�.
	*	@param	music	�߰��� ����.
	*/
	void AddToMostPlayed(const MusicType& music);
	
	/**
	*	@brief	�ֱ� ����� ���ǵ��� ����Ѵ�.
	*	@pre	����.
	*	@post	�ֱ� ����� ���� ����Ʈ�� ��µȴ�.
	*/
	void DisplayRecentPlayed();

	/**
	*	@brief	���� ���� ����� ���ǵ��� ����Ѵ�.
	*	@pre	����.
	*	@post	���� ���� ����� ���� ����Ʈ�� ��µȴ�.
	*/
	void DisplayMostPlayed();

	/**
	*	@brief	���� �޴� ���� Ŀ�ǵ带 �Է¹޴´�.
	*	@pre	����.
	*	@post	����ڷκ��� ���� �޴� Ŀ�ǵ带 �Է¹ް� ��ȯ�Ѵ�.
	*	@return	������� ���� �޴� Ŀ�ǵ�.
	*/
	int GetMusicCommand();

	/**
	*	@brief	�ٹ� �޴� ���� Ŀ�ǵ带 �Է¹޴´�.
	*	@pre	����.
	*	@post	����ڷκ��� �ٹ� �޴� Ŀ�ǵ带 �Է¹ް� ��ȯ�Ѵ�.
	*	@return	������� �ٹ� �޴� Ŀ�ǵ�.
	*/
	int GetAlbumCommand();

	/**
	*	@brief	��Ƽ��Ʈ �޴� ���� Ŀ�ǵ带 �Է¹޴´�.
	*	@pre	����.
	*	@post	����ڷκ��� ��Ƽ��Ʈ �޴� Ŀ�ǵ带 �Է¹ް� ��ȯ�Ѵ�.
	*	@return	������� ��Ƽ��Ʈ �޴� Ŀ�ǵ�.
	*/
	int GetArtistCommand();

	/**
	*	@brief	��Ƽ��Ʈ�� �̸����� ã�� ��ȯ�Ѵ�.
	*	@pre	�̸��� ��ȿ�� ���̾�� �Ѵ�.
	*	@post	data�� ã�� ��Ƽ��Ʈ�� �Ҵ��Ѵ�.
	*	@param	name	ã�� ��Ƽ��Ʈ�� �̸�.
	*	@param	data	ã�� �����͸� �Ҵ��� ��Ƽ��Ʈ.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int GetArtistByName(const string& name, Artist& data);

	/**
	*	@brief	�� �ٹ����� ã�� �����Ѵ�.
	*	@pre	����.
	*	@post	���ϰ��� ���� �ٹ����� ����Ʈ���� �����ȴ�.
	*/
	void ClearEmptyAlbums();

	/**
	*	@brief	primiary key�� ���� ������ ã�´�.
	*	@pre	����.
	*	@post	ã�� ������ ������ ����Ѵ�.
	*	@return	ã���� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int SearchMusicByID();

	/**
	*	@brief	�۰�� �̸��� ���� ������ ã�´�.
	*	@pre	����.
	*	@post	ã�� ������ ������ ����Ѵ�.
	*	@return	ã���� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int SearchMusicByComposer();

	/**
	*	@brief	primiary key�� ���� �ٹ��� ã�´�.
	*	@pre	����.
	*	@post	ã�� �ٹ��� ������ ����Ѵ�.
	*	@return	ã���� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int SearchAlbumByID();

	/**
	*	@brief	�̸��� ���� �ٹ��� ã�´�.
	*	@pre	����.
	*	@post	ã�� �ٹ��� ������ ����Ѵ�.
	*	@return	ã���� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int SearchAlbumByName();

	/**
	*	@brief	��Ƽ��Ʈ�� �̸��� ���� �ٹ��� ã�´�.
	*	@pre	����.
	*	@post	ã�� �ٹ��� ������ ����Ѵ�.
	*	@return	ã���� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int SearchAlbumByArtist();

	/**
	*	@brief	�ٹ��� ��Ƽ��Ʈ�� �̸� ������ ����Ѵ�.
	*	@pre	����.
	*	@post	�ٹ��� ������ ��Ƽ��Ʈ�� �̸� ������ ��µȴ�.
	*/
	void DisplayAlbumByArtist();

	/**
	*	@brief	�ٹ��� �̸� ������ ����Ѵ�.
	*	@pre	����.
	*	@post	�ٹ��� ������ �̸� ������ ��µȴ�.
	*/
	void DisplayAlbumByName();

	/**
	*	@brief	�ٹ��� �ٹ��� ��ǥ ��¥ ������ ����Ѵ�.
	*	@pre	����.
	*	@post	�ٹ��� ������ ��ǥ ��¥ ������ ��µȴ�.
	*/
	void DisplayAlbumByDate();

	/**
	*	@brief	������ �̸��� ���� �ٹ��� ã�´�.
	*	@pre	����.
	*	@post	ã�� �ٹ��� ������ ����Ѵ�.
	*	@return	ã���� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int SearchAlbumByMusic();

	/**
	*	@brief	primary key�� ���� ��Ƽ��Ʈ�� ã�´�.
	*	@pre	����.
	*	@post	ã�� ��Ƽ��Ʈ�� ������ ����Ѵ�.
	*	@return	ã���� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int SearchArtistByID();

	/**
	*	@brief	�̸��� ���� ��Ƽ��Ʈ�� ã�´�.
	*	@pre	����.
	*	@post	ã�� ��Ƽ��Ʈ�� ������ ����Ѵ�.
	*	@return	ã���� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int SearchArtistByName();

	/**
	*	@brief	�ٹ��� �̸��� ���� ��Ƽ��Ʈ�� ã�´�.
	*	@pre	����.
	*	@post	ã�� ��Ƽ��Ʈ�� ������ ����Ѵ�.
	*	@return	ã���� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int SearchArtistByAlbum();

	/**
	*	@brief	��Ƽ��Ʈ�� �̸� ������ ����Ѵ�.
	*	@pre	����.
	*	@post	��Ƽ��Ʈ�� ������ �̸� ������ ��µȴ�.
	*/
	void DisplayArtistByName();

	/**
	*	@brief	�� ��Ƽ��Ʈ���� ã�� �����Ѵ�.
	*	@pre	����.
	*	@post	�ٹ��� ���� ��Ƽ��Ʈ���� ����Ʈ���� �����ȴ�.
	*/
	void ClearEmptyArtists();

	/**
	*	@brief	������ ����Ѵ�.
	*	@pre	����.
	*	@post	ã�� ������ ����Ѵ�.
	*	@return	�����ϸ� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int PlayMusic();

	void OpenURLOnBrowser(const string& path);
};
#pragma once
#endif
