#include "MediaPlayer.h"

#pragma comment(lib, "shlwapi")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfplay.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "wmcodecdspuuid")
#pragma comment(lib, "strmiids.lib")

HRESULT MediaPlayer::QueryInterface(REFIID riid, void** ppv)
{
	static const QITAB qit[] = //찾을 interface(가상 클래스) 테이블을 정의
	{
		QITABENT(MediaPlayer, IMFAsyncCallback), //iid 식별자가 IMFAsyncCallback일 때
		{ 0 } //마지막은 {0}으로 초기화되어 있어야 함
	};

	return QISearch(this, qit, riid, ppv); //QISearch를 통해 테이블에 따라 식별자를 찾고, this 포인터를 해당 타입으로 캐스팅하여 ppv에 저장한다.
}

ULONG MediaPlayer::AddRef()
{
	return InterlockedIncrement(&refCount); //reference count 증가

	//Interlocked인 함수는 쓰레드 안전(Thread Safety)를 보장한다.

	//COM에서는 동시에 객체를 공유하는 쓰레드가 여러 개 있을 수 있어, 각 쓰레드가 같은 메모리에 접근할 때 문제가 발생할 수 있다.
	//이런 문제에서 안전하게 메모리에 접근할 수 있는 함수들이 Thread safe한 함수들이다.
}

ULONG MediaPlayer::Release()
{
	ULONG uCount = InterlockedDecrement(&refCount); //reference count 감소

	if (uCount == 0) //0이면 메모리 해제
	{
		delete this;
	}
	return uCount;
}

HRESULT MediaPlayer::create(HWND playerWindow, HWND event, MediaPlayer** p_player)
{
	if (p_player == NULL)
	{
		return E_POINTER; //Player 객체의 포인터가 유효하지 않음
	}

	MediaPlayer *player = new (std::nothrow) MediaPlayer(playerWindow, event); //예외를 throw를 하지 않는 MediaPlayer 객체를 생성

	if (player == NULL) //객체 생성에 실패하면
	{
		return E_OUTOFMEMORY; //메모리 부족 문제
	}

	HRESULT hr = player->Init();

	if (SUCCEEDED(hr))
	{
		*p_player = player; //초기화에 성공하면 할당
	}
	else
	{
		player->Release(); //실패하면 reference 해제
	}

	return hr;
}

MediaPlayer::MediaPlayer(HWND playerWindow, HWND event) : session(NULL), source(NULL), hwndPlayer(playerWindow), hwndEvent(event), refCount(1), playState(Closed), closeEvent(NULL)
{
}

MediaPlayer::~MediaPlayer()
{
	assert(session == NULL); //session이 NULL이 아닌 경우는 Shutdown Event가 발생한 것이 아니므로 프로그램을 중단

	ShutDown();
}

HRESULT MediaPlayer::Init()
{
	HRESULT result = MFStartup(MF_VERSION);
	if (SUCCEEDED(result)) //Media Foundation을 시작하는 데 성공하면
	{
		closeEvent = CreateEvent(NULL, FALSE, FALSE, NULL); //상호작용을 위한 이벤트 생성

		if (closeEvent == NULL) //이벤트가 생성되지 못했다면 문제가 발생한 것이므로
			result = HRESULT_FROM_WIN32(GetLastError()); //가장 마지막에 발생한 에러를 확인
	}

	return result;
}

bool MediaPlayer::openFile(const wstring& filepath)
{
	IMFTopology *topology = NULL;
	IMFPresentationDescriptor* sourcePD = NULL;

	HRESULT hr = CreateSession(); //media session (미디어 재생을 관리하는 객체)를 생성한다
	if (FAILED(hr))
	{
		goto done;
	}

	hr = CreateMediaSource(filepath.c_str(), &source); //파일 경로로부터 미디어 소스를 생성한다

	if (FAILED(hr))
	{
		goto done;
	}

	hr = source->CreatePresentationDescriptor(&sourcePD); //미디어 소스에서 presentation descriptor(스트림 정보를 가진 객체)를 생성한다.
	if (FAILED(hr))
	{
		goto done;
	}

	
	hr = CreatePlaybackTopology(source, sourcePD, hwndPlayer, &topology); //부분화된 topology(미디어 입력, 출력으로 나눠진 데이터 관리 객체)를 생성한다.

	if (FAILED(hr))
	{
		goto done;
	}

	hr = session->SetTopology(0, topology); //media session의 topology로 설정한다
	if (FAILED(hr))
	{
		goto done;
	}

	playState = OpenPending; //상태를 [여는 중]으로 설정

	//topology 설정이 끝나면 MESessionTopologySet 이벤트를 호출함.

done:
	if (FAILED(hr))
	{
		playState = Closed; //위 과정에 실패하면 상태를 [닫음]으로 설정
	}

	SafeRelease(&sourcePD);
	SafeRelease(&topology);

	return SUCCEEDED(hr); //reference 해제 후 종료
}

HRESULT MediaPlayer::CreateSession()
{

	HRESULT hr = CloseSession(); //이전에 생성된 세션이 남아있다면 종료한다

	if (FAILED(hr)) //이 과정에서 문제가 발생하면 종료
	{
		goto done;
	}

	assert(playState == Closed); //session이 열려있다면 문제가 있으므로 프로그램을 중단

	hr = MFCreateMediaSession(NULL, &session); //media session을 생성한다
	if (FAILED(hr))
	{
		goto done;
	}

	hr = session->BeginGetEvent((IMFAsyncCallback*)this, NULL); //미디어 재생 중 발생하는 이벤트들을 비동기적으로 수신한다
	if (FAILED(hr))
	{
		goto done;
	}

	playState = Ready; //미디어를 재생할 준비가 됨

	startTime = 0;
	startPosition = 0;

done:
	return hr;
}

HRESULT MediaPlayer::Invoke(IMFAsyncResult *pResult)
{
	MediaEventType meType = MEUnknown; //수신한 이벤트의 타입

	IMFMediaEvent *pEvent = NULL; //수신한 이벤트

	HRESULT hr = session->EndGetEvent(pResult, &pEvent); //이벤트를 수신한다

	if (FAILED(hr))
	{
		goto done;
	}

	hr = pEvent->GetType(&meType); //이벤트의 타입(종류)를 가져온다
	if (FAILED(hr))
	{
		goto done;
	}

	if (meType == MESessionClosed) //세션 종료인 경우
	{
		SetEvent(closeEvent); //세션이 종료되었으므로, application에 이벤트를 넘겨준다
	}
	else //다른 모든 이벤트의 경우
	{
		hr = session->BeginGetEvent(this, NULL); //딱히 처리할 내용이 없으므로, 다음 이벤트를 수신한다

		if (FAILED(hr))
		{
			goto done;
		}
	}

	if (playState != Closing) //종료하는 중이라면, application에 세션 종료 이벤트를 전달해야 하므로 무시해야 한다. (application이 종료 이벤트 수신을 기다리므로 메시지를 받을 수 없다)
	{
		pEvent->AddRef(); //이벤트에 대한 reference count 증가

		PostMessage(hwndEvent, WM_APP_PLAYER_EVENT, (WPARAM)pEvent, (LPARAM)meType); //application에 해당 이벤트에 대한 메시지를 전달한다
	}

done:
	SafeRelease(&pEvent);
	return S_OK; //성공하면 이벤트에 대한 reference 해제 후 종료
}

HRESULT MediaPlayer::OnTopologyStatus(IMFMediaEvent *pEvent)
{
	//topology의 상태가 바뀐 경우에 대한 이벤트 처리

	UINT32 status;

	HRESULT hr = pEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, &status); //이벤트 정보를 가져온다

	if (SUCCEEDED(hr) && (status == MF_TOPOSTATUS_READY)) //topology의 사용이 준비되었다면
	{
		hr = StartPlayback(); //플레이어를 시작한다
	}
	return hr;
}

HRESULT MediaPlayer::OnPresentationEnded(IMFMediaEvent *pEvent)
{
	//파일의 끝에 도달한 경우 (미디어의 재생이 끝난 경우)에 대한 이벤트 처리

	playState = Stopped; //플레이어 상태를 [정지됨]으로 설정
	return S_OK;
}

HRESULT MediaPlayer::OnNewPresentation(IMFMediaEvent *pEvent)
{
	//새로운 topology가 요청되었을 경우에 대한 이벤트 처리 (다른 미디어를 추가로 재생하는 등)

	IMFPresentationDescriptor *pPD = NULL;
	IMFTopology *pTopology = NULL;

	HRESULT hr = GetEventObject(pEvent, &pPD); //이벤트에서 presentation descriptor을 가져온다
	if (FAILED(hr))
	{
		goto done;
	}

	hr = CreatePlaybackTopology(source, pPD, hwndPlayer, &pTopology); //topology 생성. video를 재생하지 않으므로 NULL
	if (FAILED(hr))
	{
		goto done;
	}

	hr = session->SetTopology(0, pTopology); //세션의 topology로 설정
	if (FAILED(hr))
	{
		goto done;
	}

	playState = OpenPending; //새로운 topology를 여는 중

done:
	SafeRelease(&pTopology);
	SafeRelease(&pPD);
	return S_OK;
}

HRESULT MediaPlayer::StartPlayback()
{
	assert(session != NULL); //세션이 반드시 존재해야 하므로, 그렇지 않으면 프로그램을 중단

	PROPVARIANT varStart;
	PropVariantInit(&varStart); //데이터 초기화 (Start에 해당함)

	HRESULT hr = session->Start(&GUID_NULL, &varStart); //재생 시작

	Sleep(100); //재생을 위해 대기

	if (SUCCEEDED(hr))
	{
		playState = Started; //세션 시작은 비동기적으로 작동하므로, 여기서는 [시작했음]으로 설정함
		// 후에 만약 결과가 실패했을 경우에는 MESessionStarted Event에서 문제가 발생했음을 수신하므로, 그때 처리함
		
		if (playState != Paused) startPosition = 0;
		startTime = time(NULL);
	}

	PropVariantClear(&varStart);
	return hr;
}

bool MediaPlayer::play()
{
	if (session == NULL || source == NULL) //session이나 source가 유효하지 않은 경우 또한 실패
	{
		return 0;
	}

	return SUCCEEDED(StartPlayback());
}

bool MediaPlayer::pause()
{
	if (playState != Started) //상태가 재생 중 (시작했음)이 아니라면 실패
	{
		return 0;
	}

	if (session == NULL || source == NULL) //session이나 source가 유효하지 않은 경우 또한 실패
	{
		return 0;
	}

	HRESULT hr = session->Pause(); //일시 정지

	Sleep(100); //정지를 위해 대기

	if (SUCCEEDED(hr))
	{
		startPosition = getPosition();
		startTime = 0;
		playState = Paused; //성공하면 상태를 [일시 정지]로 변경
		return 1;
	}

	return 0;
}

bool MediaPlayer::stop()
{
	if (playState != Started && playState != Paused) //상태가 재생 중 혹은 일시 정지가 아니라면 실패
	{
		return 0;
	}
	if (session == NULL) //session이 유효하지 않은 경우 또한 실패 (source는 더 이상 읽을 수 없을 수도 있으므로 체크하지 않음)
	{
		return 0;
	}

	HRESULT hr = session->Stop(); //정지

	Sleep(100); //정지를 위해 대기

	if (SUCCEEDED(hr))
	{
		playState = Stopped; //성공하면 상태를 [정지]로 변경
		startTime = 0;
		startPosition = 0;
		return 1;
	}
	return 0;
}

HRESULT MediaPlayer::CloseSession()
{
	//세션을 종료하는 함수

	HRESULT hr = S_OK;

	if (session) //세션이 유효하면
	{
		DWORD dwWaitResult = 0;

		playState = Closing; //종료 시작

		hr = session->Close();

		if (SUCCEEDED(hr)) //종료에 성공하면
		{
			dwWaitResult = WaitForSingleObject(closeEvent, 5000); //5초 동안 이벤트 등이 수신되는지 기다림

			if (dwWaitResult == WAIT_TIMEOUT) //5초 동안 이벤트 수신 없이 끝나면
			{
				assert(FALSE); //프로그램을 중단함
			}
		}
	}

	if (SUCCEEDED(hr)) //세션을 종료한 경우, 세션 종료가 성공하면
	{
		if (source) //source가 유효한 경우
		{
			(void)source->Shutdown(); //source를 shutdown한다. (shutdown : 상호 작용을 못 하도록 막는다)
		}

		if (session) //세션이 유효하면
		{
			(void)session->Shutdown(); //세션을 shutdown한다.
		}
	}

	SafeRelease(&source);
	SafeRelease(&session);
	playState = Closed; //상태를 [종료됨]으로 변경
	return hr;
}

HRESULT MediaPlayer::ShutDown()
{
	//모든 리소스/메모리를 해제한다

	HRESULT hr = CloseSession(); //세션 종료

	MFShutdown(); //MF 종료

	if (closeEvent) //종료에 쓸 이벤트 객체가 있는 경우
	{
		CloseHandle(closeEvent); //이벤트를 닫는다
		closeEvent = NULL;
	}

	return hr;
}

HRESULT MediaPlayer::HandleEvent(UINT_PTR pEventPtr)
{
	HRESULT hrStatus = S_OK;
	MediaEventType meType = MEUnknown;

	IMFMediaEvent *pEvent = (IMFMediaEvent*)pEventPtr;

	if (pEvent == NULL)
	{
		return E_POINTER;
	}

	// Get the event type.
	HRESULT hr = pEvent->GetType(&meType);
	if (FAILED(hr))
	{
		goto done;
	}

	// Get the event status. If the operation that triggered the event 
	// did not succeed, the status is a failure code.
	hr = pEvent->GetStatus(&hrStatus);

	// Check if the async operation succeeded.
	if (SUCCEEDED(hr) && FAILED(hrStatus))
	{
		hr = hrStatus;
	}
	if (FAILED(hr))
	{
		goto done;
	}

	switch (meType)
	{
	case MESessionTopologyStatus:
		hr = OnTopologyStatus(pEvent);
		break;

	case MEEndOfPresentation:
		hr = OnPresentationEnded(pEvent);
		break;

	case MENewPresentation:
		hr = OnNewPresentation(pEvent);
		break;

	default:
		hr = OnSessionEvent(pEvent, meType);
		break;
	}

done:
	SafeRelease(&pEvent);
	return hr;
}

unsigned int MediaPlayer::getLength()
{
	if (source == NULL) return 0;

	IMFPresentationDescriptor *pPD = NULL;

	HRESULT hr = source->CreatePresentationDescriptor(&pPD);
	if (SUCCEEDED(hr))
	{
		UINT64 duration;
		hr = pPD->GetUINT64(MF_PD_DURATION, &duration);
		pPD->Release();

		return (unsigned int)(duration / 10000000); //100ns 단위이므로 /10 (마이크로) /1000 (밀리) / 1000 (초)
	}
	return 0;
}

unsigned int MediaPlayer::getPosition()
{
	if (source == NULL) return 0;

	if (playState == Started) return time(NULL) - startTime + startPosition;
	else if (playState == Paused) return startPosition;
	
	return 0;
}