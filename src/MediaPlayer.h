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
* @author �輺��
*/

class MediaPlayer : public IMFAsyncCallback ///Media Foundation�� ������Ʈ ������Ʈ ��(COM)�� ����ϹǷ� ��ӹ���
{
protected:

	PlayerState playState; ///�̵�� �÷��̾� ����
	long refCount; ///reference count. COM������ �̵� �����ϴ� ����̽� ���� ���� ��ü�� ������ ���ɼ��� �־�, ��ü�� �޸� ���� ���θ� �����ϱ� ���� ���
	IMFMediaSession *session; ///�̵�� ����
	IMFMediaSource *source; ///�̵�� �ҽ�
	HWND hwndPlayer; ///�̵� ����� ������ ��ü
	HWND hwndEvent; ///�̺�Ʈ�� ������ ������ ��ü
	HANDLE closeEvent; ///�÷��̾� ���Ḧ ���� �̺�Ʈ ��ü

	MediaPlayer(HWND playerWindow, HWND event); ///access level�� �����Ͽ� ������ ȣ���� ������. ����ڴ� create()�� ����� ������

	virtual ~MediaPlayer(); ///�Ҹ��ڵ� �����ڿ� ����. ����ڴ� Release()�� ����� ������

	HRESULT Init(); ///<�ʱ�ȭ �Լ�
	HRESULT CreateSession(); ///<���� ���� �Լ�
	HRESULT CloseSession(); ///<���� ���� �Լ�
	HRESULT StartPlayback(); ///<�÷��̾� ���� �Լ�

	///Player���� ó���� �̵�� �̺�Ʈ��
	virtual HRESULT OnTopologyStatus(IMFMediaEvent *pEvent); ///<topology�� ���°� ������Ʈ �Ǿ��� ���� callback
	virtual HRESULT OnPresentationEnded(IMFMediaEvent *pEvent); ///<�̵�� ����� ������ ���� callback
	virtual HRESULT OnNewPresentation(IMFMediaEvent *pEvent); ///<�� �̵� ����� ���� callbackx

	// Override to handle additional session events.
	virtual HRESULT OnSessionEvent(IMFMediaEvent*, MediaEventType) ///<���ǿ� ���õ� callback
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

	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	//***

	//** IMFAsyncCallback���� �����ؾ��ϴ� ���� �Լ���

	STDMETHODIMP GetParameters(DWORD*, DWORD*)
	{
		return E_NOTIMPL; //�� �Լ��� ���� �������� ���� (optional)
	}

	STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult);

	//**

	static HRESULT create(HWND playerWindow, HWND event, MediaPlayer** p_player);

	bool openFile(const wstring& filepath);

	HRESULT HandleEvent(UINT_PTR ptr);

	bool play();

	bool pause();

	bool stop();

	HRESULT ShutDown();

	PlayerState GetState() { return playState; }

	unsigned int getLength();

	unsigned int getPosition();
};
#endif