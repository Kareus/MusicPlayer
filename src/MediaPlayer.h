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
* 이 클래스는 다음 튜토리얼 페이지를 참고하여 작성하였습니다. - https://docs.microsoft.com/en-us/windows/desktop/medfound/audio-video-playback
* 주석은 코드 동작에 대해 본인의 주관적인 해석을 작성한 것이므로, 사실이 아닐 수가 있습니다.
* @date 2018.11.26
* @author 김성주
*/

class MediaPlayer : public IMFAsyncCallback ///Media Foundation은 컴포넌트 오브젝트 모델(COM)을 사용하므로 상속받음
{
protected:

	PlayerState playState; ///미디어 플레이어 상태
	long refCount; ///reference count. COM에서는 미디어에 접근하는 디바이스 여러 개가 객체를 공유할 가능성이 있어, 객체의 메모리 해제 여부를 결정하기 위해 사용
	IMFMediaSession *session; ///미디어 세션
	IMFMediaSource *source; ///미디어 소스
	HWND hwndPlayer; ///미디어를 재생할 윈도우 객체
	HWND hwndEvent; ///이벤트를 수신할 윈도우 객체
	HANDLE closeEvent; ///플레이어 종료를 위한 이벤트 객체

	MediaPlayer(HWND playerWindow, HWND event); ///access level을 조정하여 생성자 호출을 제한함. 사용자는 create()를 사용해 생성함

	virtual ~MediaPlayer(); ///소멸자도 생성자와 동일. 사용자는 Release()를 사용해 해제함

	HRESULT Init(); ///<초기화 함수
	HRESULT CreateSession(); ///<세션 생성 함수
	HRESULT CloseSession(); ///<세션 종료 함수
	HRESULT StartPlayback(); ///<플레이어 시작 함수

	///Player에서 처리할 미디어 이벤트들
	virtual HRESULT OnTopologyStatus(IMFMediaEvent *pEvent); ///<topology의 상태가 업데이트 되었을 때의 callback
	virtual HRESULT OnPresentationEnded(IMFMediaEvent *pEvent); ///<미디어 재생이 끝났을 때의 callback
	virtual HRESULT OnNewPresentation(IMFMediaEvent *pEvent); ///<새 미디어를 재생할 때의 callbackx

	// Override to handle additional session events.
	virtual HRESULT OnSessionEvent(IMFMediaEvent*, MediaEventType) ///<세션에 관련된 callback
	{
		return S_OK;
	}

	unsigned int startTime; ///<재생을 시작한 시간
	unsigned int startPosition; ///<재생을 시작한 위치

public:

	//***COM의 IUnknown으로 인해 다음 세 함수(IUnkown의 가상 함수)를 정의해야 함.

	/**
	*	@brief	가상 인터페이스를 찾는 함수
	*	@param	iid	인터페이스의 참조 식별자
	*	@param	ppv	인터페이스가 지원 중일 경우 저장되는 인터페이스의 포인터의 주소
	*	@return	인터페이스를 찾았는지 여부를 반환한다.
	*/
	STDMETHODIMP QueryInterface(REFIID iid, void** ppv);

	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	//***

	//** IMFAsyncCallback에서 정의해야하는 가상 함수들

	STDMETHODIMP GetParameters(DWORD*, DWORD*)
	{
		return E_NOTIMPL; //이 함수는 따로 정의하지 않음 (optional)
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