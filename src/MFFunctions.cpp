#include "MFFunctions.h"

//경로로부터 media source를 가져오는 함수
HRESULT CreateMediaSource(PCWSTR sURL, IMFMediaSource **ppSource)
{
	MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;

	IMFSourceResolver* pSourceResolver = NULL;
	IUnknown* pSource = NULL;

	HRESULT hr = MFCreateSourceResolver(&pSourceResolver); //source를 읽어올 resolver을 생성

	if (FAILED(hr))
	{
		goto done;
	}

	//파일에서만 소스를 읽어올 것이므로, asynchronous를 추가할 이유가 없다고 판단하여 synchronous method를 그대로 사용함.

	hr = pSourceResolver->CreateObjectFromURL(
		sURL,                       // 소스의 경로 (wide char string)
		MF_RESOLUTION_MEDIASOURCE,  // source 객체를 생성함
		NULL,                       // 불러올 source의 특성 (property)
		&ObjectType,        // 소스의 타입을 저장 (스트림 혹은 파일 소스 등)
		&pSource            // 미디어 소스를 저장
	);

	if (FAILED(hr))
	{
		goto done;
	}

	hr = pSource->QueryInterface(IID_PPV_ARGS(ppSource)); //인터페이스의 포인터를 이용해 타입을 추적하여 인터페이스를 찾음

done:
	SafeRelease(&pSourceResolver);
	SafeRelease(&pSource);
	return hr; //메모리를 해제하고 반환
}

//미디어 재생을 위해 topology를 생성하는 함수
HRESULT CreatePlaybackTopology(IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, HWND hVideoWnd, IMFTopology **ppTopology)
{
	IMFTopology *pTopology = NULL;
	DWORD cSourceStreams = 0;

	HRESULT hr = MFCreateTopology(&pTopology); //topology (stream에서 data flow를 관리) 생성

	if (FAILED(hr))
	{
		goto done;
	}

	hr = pPD->GetStreamDescriptorCount(&cSourceStreams); //미디어 소스에 있는 스트림 수를 가져옴 (단일 혹은 여러 개일 수 있음)

	if (FAILED(hr))
	{
		goto done;
	}

	for (DWORD i = 0; i < cSourceStreams; i++)
	{
		hr = AddBranchToPartialTopology(pTopology, pSource, pPD, i, hVideoWnd); //각 스트림에 대해 topology node를 추가.
		//이 node에서는 스트림 소스에서 각 과정 (디코딩 등)을 진행해 오디오 출력을 보조함

		if (FAILED(hr))
		{
			goto done;
		}
	}

	*ppTopology = pTopology; //topology를 반환
	(*ppTopology)->AddRef();

done:
	SafeRelease(&pTopology);
	return hr;
}

//topology를 소스 입력, 출력 등으로 세분화하는 함수

HRESULT AddBranchToPartialTopology(IMFTopology *pTopology, IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, DWORD iStream, HWND hVideoWnd)
{
	IMFStreamDescriptor *pSD = NULL;
	IMFActivate         *pSinkActivate = NULL;
	IMFTopologyNode     *pSourceNode = NULL;
	IMFTopologyNode     *pOutputNode = NULL;

	BOOL fSelected = FALSE;

	HRESULT hr = pPD->GetStreamDescriptorByIndex(iStream, &fSelected, &pSD); //인덱스에 해당하는 presentation descriptor를 가져옴
	//presentation은 영화에서 사운드와 그래픽처럼, 같은 시간동안 흐르는 오디오/비디오 스트림의 set을 말함.
	//presentation descriptor(pd)는 이러한 presentation에 대한 정보를 가지고 있음.
	//pd는 selected 또는 deselected일 수 있으며, selected인 pd만 스트리밍에 유효한 데이터를 가지고 있음 (다국어 더빙된 영화 DVD에서 한국어로 재생하면 한국어 음성만이 selected된다)

	if (FAILED(hr))
	{
		goto done;
	}

	//selected인 경우

	if (fSelected)
	{
		//media sink(미디어 정보를 받아오는 파이프라인)을 활성화한다
		hr = CreateMediaSinkActivate(pSD, hVideoWnd, &pSinkActivate);
		if (FAILED(hr))
		{
			goto done;
		}

		//source node(소스에서 미디어를 읽는 담당)을 추가한다
		hr = AddSourceNode(pTopology, pSource, pPD, pSD, &pSourceNode);
		if (FAILED(hr))
		{
			goto done;
		}

		//output node(미디어를 사운드로 출력하는 담당)을 추가한다.
		hr = AddOutputNode(pTopology, pSinkActivate, 0, &pOutputNode);
		if (FAILED(hr))
		{
			goto done;
		}

		//output node에 source node를 연결한다.
		hr = pSourceNode->ConnectOutput(0, pOutputNode, 0);
	}

	//selected가 아닌 경우는 무시됨

done:
	SafeRelease(&pSD);
	SafeRelease(&pSinkActivate);
	SafeRelease(&pSourceNode);
	SafeRelease(&pOutputNode);
	return hr;
}

//읽고 출력할 미디어의 종류에 따라 적합한 렌더러를 활성화하는 함수
HRESULT CreateMediaSinkActivate(IMFStreamDescriptor *pSourceSD, HWND hVideoWindow, IMFActivate **ppActivate)
{
	IMFMediaTypeHandler *pHandler = NULL;
	IMFActivate *pActivate = NULL;

	//스트림에서 미디어 타입을 가져올 핸들러 생성
	HRESULT hr = pSourceSD->GetMediaTypeHandler(&pHandler);
	if (FAILED(hr))
	{
		goto done;
	}

	//major type(오디오인지, 혹은 비디오인지)를 확인한다
	GUID guidMajorType;
	hr = pHandler->GetMajorType(&guidMajorType);
	if (FAILED(hr))
	{
		goto done;
	}

	//미디어 종류에 따라 렌더러 활성화를 결정한다

	if (MFMediaType_Audio == guidMajorType) //미디어가 오디오인 경우
	{
		//오디오 렌더러 생성
		hr = MFCreateAudioRendererActivate(&pActivate);
	}

	//비디오는 사용하지 않을 것이므로 주석 처리

	/*
	else if (MFMediaType_Video == guidMajorType)
	{
		//비디오 렌더러 생성
		hr = MFCreateVideoRendererActivate(hVideoWindow, &pActivate);
	}
	*/

	else //아닌 경우 (이 코드에선, 비디오인 경우와 알 수 없는 경우를 포함함)
	{
		hr = E_FAIL;
	}

	if (FAILED(hr))
	{
		goto done;
	}

	//활성화 객체를 반환
	*ppActivate = pActivate;
	(*ppActivate)->AddRef();

done:
	SafeRelease(&pHandler);
	SafeRelease(&pActivate);
	return hr;
}

//topology에 source node를 추가하는 함수
HRESULT AddSourceNode(IMFTopology *pTopology, IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, IMFStreamDescriptor *pSD, IMFTopologyNode **ppNode)
{
	IMFTopologyNode *pNode = NULL;

	//source node를 생성한다
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	//속성을 설정한다. (source, descriptor 등)

	hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, pSource);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD);
	if (FAILED(hr))
	{
		goto done;
	}

	//topology에 node를 추가한다.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	//노드를 반환
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(&pNode);
	return hr;
}

//topology에 output node를 추가하는 함수
HRESULT AddOutputNode(IMFTopology *pTopology, IMFActivate *pActivate, DWORD dwId, IMFTopologyNode **ppNode)
{
	IMFTopologyNode *pNode = NULL;

	//output node를 생성한다
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	//미디어 렌더러를 설정한다
	hr = pNode->SetObject(pActivate);
	if (FAILED(hr))
	{
		goto done;
	}

	//미디어 정보를 가져오는 media sink의 식별자(id)를 설정한다
	hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId);
	if (FAILED(hr))
	{
		goto done;
	}

	//node를 제거했을 때 shutdown할 여부를 결정 -> shutdown을 하도록 설정함

	hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE);
	if (FAILED(hr))
	{
		goto done;
	}

	//topology에 node를 추가한다.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	//노드를 반환
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(&pNode);
	return hr;
}