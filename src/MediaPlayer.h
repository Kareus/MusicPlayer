#pragma once
#ifndef __MEDIA_PLAYER__
#define __MEDIA_PLAYER__

#include "MFFunctions.h"
#include <ctime>

using namespace std;

/**
Player States
*/

enum PlayerState
{
	Closed = 0,     // No session.
	Ready,          // Session was created, ready to open a file. 
	OpenPending,    // Session is opening a file.
	Started,        // Session is playing a file.
	Paused,         // Session is paused.
	Stopped,        // Session is stopped (ready to play). 
	Closing         // Application has closed the session, but is waiting for MESessionClosed.
};

/**
* Media Player class with Media Foundation. Media Foundation is from Microsoft APIs, so this class only works in Windows
* �� Ŭ������ ���� Ʃ�丮�� �������� �����Ͽ� �ۼ��Ͽ����ϴ�. - https://docs.microsoft.com/en-us/windows/desktop/medfound/audio-video-playback
* �ּ��� �ڵ� ���ۿ� ���� ������ �ְ����� �ؼ��� �ۼ��� ���̹Ƿ�, ����� �ƴ� ���� �ֽ��ϴ�.
* @date 2018.11.26
* @author Kareus
*/

class MediaPlayer : public IMFAsyncCallback ///<Media Foundation�� ������Ʈ ������Ʈ ��(COM)�� ����ϹǷ� ��ӹ���
{
protected:

	PlayerState playState; ///<�̵�� �÷��̾� ����
	long refCount; ///<reference count. COM������ �̵� �����ϴ� ����̽� ���� ���� ��ü�� ������ ���ɼ��� �־�, ��ü�� �޸� ���� ���θ� �����ϱ� ���� ���
	IMFMediaSession *session; ///<�̵�� ����
	IMFMediaSource *source; ///<�̵�� �ҽ�
	HWND hwndPlayer; ///<�̵� ����� ������ ��ü
	HWND hwndEvent; ///<�̺�Ʈ�� ������ ������ ��ü
	HANDLE closeEvent; ///<�÷��̾� ���Ḧ ���� �̺�Ʈ ��ü

	/**
	*	@brief	�⺻ ������. access level�� �����Ͽ� ������ ȣ���� �����Ѵ�. ����ڴ� create()�� ����� �����ؾ� �Ѵ�.
	*	@param	playerWindow	������ ����� ������
	*	@param	event	�̺�Ʈ�� �ۼ����� ������
	*/
	MediaPlayer(HWND playerWindow, HWND event);

	virtual ~MediaPlayer(); ///<�Ҹ��ڵ� �����ڿ� ����. ����ڴ� Release()�� ����� ������

	/**
	*	@brief	�ʱ�ȭ �Լ�
	*	@return	�Լ� ���� ���
	*/
	HRESULT Init();

	/**
	*	@brief	���� ���� �Լ�
	*	@return	�Լ� ���� ���
	*/
	HRESULT CreateSession();

	/**
	*	@brief	���� ���� �Լ�
	*	@return	�Լ� ���� ���
	*/
	HRESULT CloseSession();

	/**
	*	@brief	�÷��̾� ����(���) �Լ�
	*	@return	�Լ� ���� ���
	*/
	HRESULT StartPlayback();

	//Player���� ó���� �̵�� �̺�Ʈ��

	/**
	*	@brief	topology�� ���°� ������Ʈ �Ǿ��� ���� callback �Լ�
	*	@param	pEvent	������ �̺�Ʈ
	*/
	virtual HRESULT OnTopologyStatus(IMFMediaEvent *pEvent);

	/**
	*	@brief	�̵�� ����� ������ ���� callback �Լ�
	*	@param	pEvent	������ �̺�Ʈ
	*/
	virtual HRESULT OnPresentationEnded(IMFMediaEvent *pEvent);

	/**
	*	@brief	�� �̵� ����� ���� callback �Լ�
	*	@param	pEvent	������ �̺�Ʈ
	*/
	virtual HRESULT OnNewPresentation(IMFMediaEvent *pEvent);

	/**
	*	@brief	������ ���ǿ� ���õ� callback �Լ� (override ������, ���� ���������� �ʴ´�)
	*/
	virtual HRESULT OnSessionEvent(IMFMediaEvent*, MediaEventType)
	{
		return S_OK;
	}

	unsigned int startTime; ///<����� ������ �ð�
	unsigned int startPosition; ///<����� ������ ��ġ

public:

	//***COM�� IUnknown���� ���� ���� �� �Լ�(IUnkown�� ���� �Լ�)�� �����ؾ� ��.

	/**
	*	@brief	���� �������̽��� ã�� �Լ�
	*	@param	iid	�������̽��� ���� �ĺ���
	*	@param	ppv	�������̽��� ���� ���� ��� ����Ǵ� �������̽��� �������� �ּ�
	*	@return	�������̽��� ã�Ҵ��� ���θ� ��ȯ�Ѵ�.
	*/
	STDMETHODIMP QueryInterface(REFIID iid, void** ppv);

	/**
	*	@brief	reference�� �߰��Ѵ�.
	*/
	STDMETHODIMP_(ULONG) AddRef();

	/**
	*	@brief	reference�� �����Ѵ�.
	*/
	STDMETHODIMP_(ULONG) Release();

	//** IMFAsyncCallback���� �����ؾ��ϴ� ���� �Լ���

	/**
	*	@brief	�Ķ���͸� �������� �Լ�
	*/
	STDMETHODIMP GetParameters(DWORD*, DWORD*)
	{
		return E_NOTIMPL; //�� �Լ��� ���� �������� ���� (not implemented)
	}

	/**
	*	@brief	�񵿱��� ����� �޾ƿ� ���� ������ �����ϴ� �Լ�
	*	@param	pAsyncResult	�񵿱� �Լ� ���
	*/
	STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult);

	/**
	*	@brief	�̵�� �÷��̾ �����ϴ� static �Լ�
	*	@param	playerWindow	�÷��̾ ����� ������
	*	@param	event	�̺�Ʈ�� �ۼ����� ������
	*	@param	p_player	������ �÷��̾ �Ҵ��� �������� ������
	*/
	static HRESULT create(HWND playerWindow, HWND event, MediaPlayer** p_player);

	/**
	*	@brief	���� ������ ���� �Լ� (��������� ����)
	*	@param	filepath	����� ���� ������ ���
	*	@return	���� �� �����ϸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool openFile(const wstring& filepath);

	/**
	*	@brief	������ �̺�Ʈ�� ���� �� callback�� �����ϴ� �Լ�
	*	@param	ptr	�̺�Ʈ ������ ������ �ִ� ������
	*/
	HRESULT HandleEvent(UINT_PTR ptr);

	/**
	*	@brief	������ ����Ѵ�.
	*	@pre	���� ������ ���� �־�� �Ѵ�.
	*	@post	������ ����ȴ�.
	*	@return	����� �����ϸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool play();

	/**
	*	@brief	������ �Ͻ� �����Ѵ�.
	*	@pre	���� ������ ���� �־�� �Ѵ�.
	*	@post	������ �Ͻ� �����ȴ�.
	*	@return	������ �����ϸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool pause();

	/**
	*	@brief	������ �����Ѵ�.
	*	@return	������ �����ϸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool stop();

	/**
	*	@brief	�÷��̾ �����ϰ� �޸𸮸� �����Ѵ�.
	*	@return	�Լ� ���� ���
	*/
	HRESULT ShutDown();

	/**
	*	@brief	�÷��̾� ���¸� ��ȯ�Ѵ�.
	*	@return	�÷��̾� ����
	*/
	PlayerState GetState() { return playState; }

	/**
	*	@brief	������ ���̸� ��ȯ�Ѵ�.
	*	@return	������ ����
	*/
	unsigned int getLength();

	/**
	*	@brief	���� ������ ��ġ�� ��ȯ�Ѵ�.
	*	@return	������ ��ġ
	*/
	unsigned int getPosition();
};
#endif