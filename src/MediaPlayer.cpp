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
	static const QITAB qit[] = //ã�� interface(���� Ŭ����) ���̺��� ����
	{
		QITABENT(MediaPlayer, IMFAsyncCallback), //iid �ĺ��ڰ� IMFAsyncCallback�� ��
		{ 0 } //�������� {0}���� �ʱ�ȭ�Ǿ� �־�� ��
	};

	return QISearch(this, qit, riid, ppv); //QISearch�� ���� ���̺� ���� �ĺ��ڸ� ã��, this �����͸� �ش� Ÿ������ ĳ�����Ͽ� ppv�� �����Ѵ�.
}

ULONG MediaPlayer::AddRef()
{
	return InterlockedIncrement(&refCount); //reference count ����

	//Interlocked�� �Լ��� ������ ����(Thread Safety)�� �����Ѵ�.

	//COM������ ���ÿ� ��ü�� �����ϴ� �����尡 ���� �� ���� �� �־�, �� �����尡 ���� �޸𸮿� ������ �� ������ �߻��� �� �ִ�.
	//�̷� �������� �����ϰ� �޸𸮿� ������ �� �ִ� �Լ����� Thread safe�� �Լ����̴�.
}

ULONG MediaPlayer::Release()
{
	ULONG uCount = InterlockedDecrement(&refCount); //reference count ����

	if (uCount == 0) //0�̸� �޸� ����
	{
		delete this;
	}
	return uCount;
}

HRESULT MediaPlayer::create(HWND playerWindow, HWND event, MediaPlayer** p_player)
{
	if (p_player == NULL)
	{
		return E_POINTER; //Player ��ü�� �����Ͱ� ��ȿ���� ����
	}

	MediaPlayer *player = new (std::nothrow) MediaPlayer(playerWindow, event); //���ܸ� throw�� ���� �ʴ� MediaPlayer ��ü�� ����

	if (player == NULL) //��ü ������ �����ϸ�
	{
		return E_OUTOFMEMORY; //�޸� ���� ����
	}

	HRESULT hr = player->Init();

	if (SUCCEEDED(hr))
	{
		*p_player = player; //�ʱ�ȭ�� �����ϸ� �Ҵ�
	}
	else
	{
		player->Release(); //�����ϸ� reference ����
	}

	return hr;
}

MediaPlayer::MediaPlayer(HWND playerWindow, HWND event) : session(NULL), source(NULL), hwndPlayer(playerWindow), hwndEvent(event), refCount(1), playState(Closed), closeEvent(NULL)
{
}

MediaPlayer::~MediaPlayer()
{
	assert(session == NULL); //session�� NULL�� �ƴ� ���� Shutdown Event�� �߻��� ���� �ƴϹǷ� ���α׷��� �ߴ�

	ShutDown();
}

HRESULT MediaPlayer::Init()
{
	HRESULT result = MFStartup(MF_VERSION);
	if (SUCCEEDED(result)) //Media Foundation�� �����ϴ� �� �����ϸ�
	{
		closeEvent = CreateEvent(NULL, FALSE, FALSE, NULL); //��ȣ�ۿ��� ���� �̺�Ʈ ����

		if (closeEvent == NULL) //�̺�Ʈ�� �������� ���ߴٸ� ������ �߻��� ���̹Ƿ�
			result = HRESULT_FROM_WIN32(GetLastError()); //���� �������� �߻��� ������ Ȯ��
	}

	return result;
}

bool MediaPlayer::openFile(const wstring& filepath)
{
	IMFTopology *topology = NULL;
	IMFPresentationDescriptor* sourcePD = NULL;

	HRESULT hr = CreateSession(); //media session (�̵�� ����� �����ϴ� ��ü)�� �����Ѵ�
	if (FAILED(hr))
	{
		goto done;
	}

	hr = CreateMediaSource(filepath.c_str(), &source); //���� ��ηκ��� �̵�� �ҽ��� �����Ѵ�

	if (FAILED(hr))
	{
		goto done;
	}

	hr = source->CreatePresentationDescriptor(&sourcePD); //�̵�� �ҽ����� presentation descriptor(��Ʈ�� ������ ���� ��ü)�� �����Ѵ�.
	if (FAILED(hr))
	{
		goto done;
	}

	
	hr = CreatePlaybackTopology(source, sourcePD, hwndPlayer, &topology); //�κ�ȭ�� topology(�̵�� �Է�, ������� ������ ������ ���� ��ü)�� �����Ѵ�.

	if (FAILED(hr))
	{
		goto done;
	}

	hr = session->SetTopology(0, topology); //media session�� topology�� �����Ѵ�
	if (FAILED(hr))
	{
		goto done;
	}

	playState = OpenPending; //���¸� [���� ��]���� ����

	//topology ������ ������ MESessionTopologySet �̺�Ʈ�� ȣ����.

done:
	if (FAILED(hr))
	{
		playState = Closed; //�� ������ �����ϸ� ���¸� [����]���� ����
	}

	SafeRelease(&sourcePD);
	SafeRelease(&topology);

	return SUCCEEDED(hr); //reference ���� �� ����
}

HRESULT MediaPlayer::CreateSession()
{

	HRESULT hr = CloseSession(); //������ ������ ������ �����ִٸ� �����Ѵ�

	if (FAILED(hr)) //�� �������� ������ �߻��ϸ� ����
	{
		goto done;
	}

	assert(playState == Closed); //session�� �����ִٸ� ������ �����Ƿ� ���α׷��� �ߴ�

	hr = MFCreateMediaSession(NULL, &session); //media session�� �����Ѵ�
	if (FAILED(hr))
	{
		goto done;
	}

	hr = session->BeginGetEvent((IMFAsyncCallback*)this, NULL); //�̵�� ��� �� �߻��ϴ� �̺�Ʈ���� �񵿱������� �����Ѵ�
	if (FAILED(hr))
	{
		goto done;
	}

	playState = Ready; //�̵� ����� �غ� ��

	startTime = 0;
	startPosition = 0;

done:
	return hr;
}

HRESULT MediaPlayer::Invoke(IMFAsyncResult *pResult)
{
	MediaEventType meType = MEUnknown; //������ �̺�Ʈ�� Ÿ��

	IMFMediaEvent *pEvent = NULL; //������ �̺�Ʈ

	HRESULT hr = session->EndGetEvent(pResult, &pEvent); //�̺�Ʈ�� �����Ѵ�

	if (FAILED(hr))
	{
		goto done;
	}

	hr = pEvent->GetType(&meType); //�̺�Ʈ�� Ÿ��(����)�� �����´�
	if (FAILED(hr))
	{
		goto done;
	}

	if (meType == MESessionClosed) //���� ������ ���
	{
		SetEvent(closeEvent); //������ ����Ǿ����Ƿ�, application�� �̺�Ʈ�� �Ѱ��ش�
	}
	else //�ٸ� ��� �̺�Ʈ�� ���
	{
		hr = session->BeginGetEvent(this, NULL); //���� ó���� ������ �����Ƿ�, ���� �̺�Ʈ�� �����Ѵ�

		if (FAILED(hr))
		{
			goto done;
		}
	}

	if (playState != Closing) //�����ϴ� ���̶��, application�� ���� ���� �̺�Ʈ�� �����ؾ� �ϹǷ� �����ؾ� �Ѵ�. (application�� ���� �̺�Ʈ ������ ��ٸ��Ƿ� �޽����� ���� �� ����)
	{
		pEvent->AddRef(); //�̺�Ʈ�� ���� reference count ����

		PostMessage(hwndEvent, WM_APP_PLAYER_EVENT, (WPARAM)pEvent, (LPARAM)meType); //application�� �ش� �̺�Ʈ�� ���� �޽����� �����Ѵ�
	}

done:
	SafeRelease(&pEvent);
	return S_OK; //�����ϸ� �̺�Ʈ�� ���� reference ���� �� ����
}

HRESULT MediaPlayer::OnTopologyStatus(IMFMediaEvent *pEvent)
{
	//topology�� ���°� �ٲ� ��쿡 ���� �̺�Ʈ ó��

	UINT32 status;

	HRESULT hr = pEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, &status); //�̺�Ʈ ������ �����´�

	if (SUCCEEDED(hr) && (status == MF_TOPOSTATUS_READY)) //topology�� ����� �غ�Ǿ��ٸ�
	{
		hr = StartPlayback(); //�÷��̾ �����Ѵ�
	}
	return hr;
}

HRESULT MediaPlayer::OnPresentationEnded(IMFMediaEvent *pEvent)
{
	//������ ���� ������ ��� (�̵���� ����� ���� ���)�� ���� �̺�Ʈ ó��

	playState = Stopped; //�÷��̾� ���¸� [������]���� ����
	return S_OK;
}

HRESULT MediaPlayer::OnNewPresentation(IMFMediaEvent *pEvent)
{
	//���ο� topology�� ��û�Ǿ��� ��쿡 ���� �̺�Ʈ ó�� (�ٸ� �̵� �߰��� ����ϴ� ��)

	IMFPresentationDescriptor *pPD = NULL;
	IMFTopology *pTopology = NULL;

	HRESULT hr = GetEventObject(pEvent, &pPD); //�̺�Ʈ���� presentation descriptor�� �����´�
	if (FAILED(hr))
	{
		goto done;
	}

	hr = CreatePlaybackTopology(source, pPD, hwndPlayer, &pTopology); //topology ����. video�� ������� �����Ƿ� NULL
	if (FAILED(hr))
	{
		goto done;
	}

	hr = session->SetTopology(0, pTopology); //������ topology�� ����
	if (FAILED(hr))
	{
		goto done;
	}

	playState = OpenPending; //���ο� topology�� ���� ��

done:
	SafeRelease(&pTopology);
	SafeRelease(&pPD);
	return S_OK;
}

HRESULT MediaPlayer::StartPlayback()
{
	assert(session != NULL); //������ �ݵ�� �����ؾ� �ϹǷ�, �׷��� ������ ���α׷��� �ߴ�

	PROPVARIANT varStart;
	PropVariantInit(&varStart); //������ �ʱ�ȭ (Start�� �ش���)

	HRESULT hr = session->Start(&GUID_NULL, &varStart); //��� ����

	Sleep(100); //����� ���� ���

	if (SUCCEEDED(hr))
	{
		playState = Started; //���� ������ �񵿱������� �۵��ϹǷ�, ���⼭�� [��������]���� ������
		// �Ŀ� ���� ����� �������� ��쿡�� MESessionStarted Event���� ������ �߻������� �����ϹǷ�, �׶� ó����
		
		if (playState != Paused) startPosition = 0;
		startTime = time(NULL);
	}

	PropVariantClear(&varStart);
	return hr;
}

bool MediaPlayer::play()
{
	if (session == NULL || source == NULL) //session�̳� source�� ��ȿ���� ���� ��� ���� ����
	{
		return 0;
	}

	return SUCCEEDED(StartPlayback());
}

bool MediaPlayer::pause()
{
	if (playState != Started) //���°� ��� �� (��������)�� �ƴ϶�� ����
	{
		return 0;
	}

	if (session == NULL || source == NULL) //session�̳� source�� ��ȿ���� ���� ��� ���� ����
	{
		return 0;
	}

	HRESULT hr = session->Pause(); //�Ͻ� ����

	Sleep(100); //������ ���� ���

	if (SUCCEEDED(hr))
	{
		startPosition = getPosition();
		startTime = 0;
		playState = Paused; //�����ϸ� ���¸� [�Ͻ� ����]�� ����
		return 1;
	}

	return 0;
}

bool MediaPlayer::stop()
{
	if (playState != Started && playState != Paused) //���°� ��� �� Ȥ�� �Ͻ� ������ �ƴ϶�� ����
	{
		return 0;
	}
	if (session == NULL) //session�� ��ȿ���� ���� ��� ���� ���� (source�� �� �̻� ���� �� ���� ���� �����Ƿ� üũ���� ����)
	{
		return 0;
	}

	HRESULT hr = session->Stop(); //����

	Sleep(100); //������ ���� ���

	if (SUCCEEDED(hr))
	{
		playState = Stopped; //�����ϸ� ���¸� [����]�� ����
		startTime = 0;
		startPosition = 0;
		return 1;
	}
	return 0;
}

HRESULT MediaPlayer::CloseSession()
{
	//������ �����ϴ� �Լ�

	HRESULT hr = S_OK;

	if (session) //������ ��ȿ�ϸ�
	{
		DWORD dwWaitResult = 0;

		playState = Closing; //���� ����

		hr = session->Close();

		if (SUCCEEDED(hr)) //���ῡ �����ϸ�
		{
			dwWaitResult = WaitForSingleObject(closeEvent, 5000); //5�� ���� �̺�Ʈ ���� ���ŵǴ��� ��ٸ�

			if (dwWaitResult == WAIT_TIMEOUT) //5�� ���� �̺�Ʈ ���� ���� ������
			{
				assert(FALSE); //���α׷��� �ߴ���
			}
		}
	}

	if (SUCCEEDED(hr)) //������ ������ ���, ���� ���ᰡ �����ϸ�
	{
		if (source) //source�� ��ȿ�� ���
		{
			(void)source->Shutdown(); //source�� shutdown�Ѵ�. (shutdown : ��ȣ �ۿ��� �� �ϵ��� ���´�)
		}

		if (session) //������ ��ȿ�ϸ�
		{
			(void)session->Shutdown(); //������ shutdown�Ѵ�.
		}
	}

	SafeRelease(&source);
	SafeRelease(&session);
	playState = Closed; //���¸� [�����]���� ����
	return hr;
}

HRESULT MediaPlayer::ShutDown()
{
	//��� ���ҽ�/�޸𸮸� �����Ѵ�

	HRESULT hr = CloseSession(); //���� ����

	MFShutdown(); //MF ����

	if (closeEvent) //���ῡ �� �̺�Ʈ ��ü�� �ִ� ���
	{
		CloseHandle(closeEvent); //�̺�Ʈ�� �ݴ´�
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

		return (unsigned int)(duration / 10000000); //100ns �����̹Ƿ� /10 (����ũ��) /1000 (�и�) / 1000 (��)
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