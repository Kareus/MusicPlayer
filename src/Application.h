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
*	�ڷᱸ���� �̿��� ���� �÷��̾� Ŭ����
*	@author	�輺��
*	@date	2018.09.20
*/

class Application {
private:
	HWND Handle; ///<���� ������ �ڵ�
	HWND editHandle; ///<���� ���� �ڵ�

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

	int m_Command; ///< ����ڷκ��� �Է¹��� ���� Ŀ�ǵ�
	std::ifstream m_inputFile; ///< ���� �Է��� �ޱ� ���� ��Ʈ��
	std::ofstream m_outFile; ///< ���� ����� �ϱ� ���� ��Ʈ��
	DoublyLinkedList<MusicType> musicList; ///< ��� ������ �����ϴ� ����Ʈ (master list). ��� ������ ����.
	DoublyLinkedList<GenreType> genreList; ///< �帣�� �����ϴ� ����Ʈ
	CircularQueueType<SimpleMusicType> newAddMusicList = CircularQueueType<SimpleMusicType>(31); ///< �ֱ� �߰��� ���� ����Ʈ.
	//30�� �����͸� �߰��ؾ� �ϹǷ� MAXSIZE = 31
	
	AVLTree<SimpleMusicType> nameList; ///<�̸� ������ ������ �����ϴ� ����Ʈ. �̸�_��Ƽ��Ʈ ������ ����.
	DoublyLinkedList<Album> albumList;
	DoublyLinkedList<Artist> artistList;
	DoublyLinkedList<SimpleMusicType> recentPlayedList;
	DoublyLinkedList<SimpleMusicType> mostPlayedList;
	AVLTree<FolderType> folderList;
	AVLTree<PlayList> playLists;

	int recentPlayCount; ///<�ֱ� ����� ���� ��
	int addedCount; ///<�ֱ� �߰��� ���� ��

	sf::RenderWindow window; ///<���� �÷��̾��� ������ ��ü
	sf::RenderWindow editor;
	sf::Color backColor; ///<��׶��� �÷�
	DoublyLinkedList<Graphic*> drawings; ///<�������� �׷��� ����Ʈ. Ư�� ������ Ž�� ���� ��ü Ž���� ����ϹǷ� linked list ���.
	DoublyLinkedList<Graphic*> edit_drawings; ///<�������� �׷��� ����Ʈ.
	DoublyLinkedList<Graphic*> displayList;

	Graphic* focus; ///<��Ŀ�� ���� �׷���
	Group* currentGroup; ///<���� ������� �׷�
	bool running; ///<���ø����̼� ���� ����
	bool editing; ///<���� ������ ���� ����
	MusicType* editMusic; ///<������ �����쿡�� ������ ���� Ÿ��
	
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
			PlayMusic();
		}
		sprite->ResetMouseOver(); //���콺 ���� ���¸� ����
		sprite->TriggerMouseOver(); //���콺 ���� �ٽ� Ʈ����
	};
	function<void(Sprite*)> func_addMusic = [this](Sprite*) {
		AddMusic();
	};
	function<void(Sprite*)> func_dragStart_edit = [this](Sprite*) { SendMessage(editHandle, WM_NCLBUTTONDOWN, HTCAPTION, 0); };
	function<void(Sprite*)> func_ok_edit = [this](Sprite*) { ReplaceMusic(); };
	function<void(Sprite*)> func_cancel_edit = [this](Sprite*) {
		CloseEditor(); };

	//��Ƽ �����忡�� �����츦 �������� �� �� �Լ�
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
	*	@brief	����ڷκ��� ������ �Է¹޾� �������� �����Ѵ�.
	*	@pre	����.
	*	@post	ã�� �������� ����Ʈ���� �����ȴ�.
	*	@param	data	������ ���� ������
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int DeleteMusic(const MusicType& data);

	/**
	*	@brief	����ڷκ��� ������ �Է¹޾� �������� ��ü�Ѵ�.
	*	@pre	����.
	*	@post	ã�� �������� ����Ʈ���� ��ü�ȴ�.
	*	@return �����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReplaceMusic();

	void DisplayResult();

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
