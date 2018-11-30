#ifndef __APPLICATION__
#define __APPLICATION__

#include "Artist.h"
#include "GenreType.h"
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

/**
*	�ڷᱸ���� �̿��� ���� �÷��̾� Ŭ����
*	@author	�輺��
*	@date	2018.09.20
*/

class Application {
private:
	Sprite* backgroundSprite;

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

	sf::RenderWindow window; ///<���� �÷��̾��� ������ ��ü
	sf::Color backColor; ///<��׶��� �÷�
	DoublyLinkedList<Graphic*> drawings; ///<�������� �׷��� ����Ʈ. Ư�� ������ Ž�� ���� ��ü Ž���� ����ϹǷ� linked list ���.
	Graphic* focus; ///<��Ŀ�� ���� �׷���
	Group* currentGroup; ///<���� ������� �׷�
	bool running; ///<���ø����̼� ���� ����

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

	//��Ƽ �����忡�� �����츦 �������� �� �� �Լ�
	void Render();

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
	*	@brief	����Ʈ�� �������� ��� ����.
	*	@pre	����.
	*	@post	����Ʈ�� �������� ��� �������.
	*/
	void MakeEmpty();

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
	*	@brief	�� �ٹ����� ã�� �����Ѵ�.
	*	@pre	����.
	*	@post	���ϰ��� ���� �ٹ����� ����Ʈ���� �����ȴ�.
	*/
	void ClearEmptyAlbums();

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

	bool pollEvent(sf::Event e);

	bool pollEvent(CustomWinEvent e);

	int AddGraphic(Graphic* graphic);

	Group* AddDisplayGraphic(MusicType* data);

	Group* AddEditGraphic(MusicType* data);
};
#pragma once
#endif
