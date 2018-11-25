#include "MFFunctions.h"

//��ηκ��� media source�� �������� �Լ�
HRESULT CreateMediaSource(PCWSTR sURL, IMFMediaSource **ppSource)
{
	MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;

	IMFSourceResolver* pSourceResolver = NULL;
	IUnknown* pSource = NULL;

	HRESULT hr = MFCreateSourceResolver(&pSourceResolver); //source�� �о�� resolver�� ����

	if (FAILED(hr))
	{
		goto done;
	}

	//���Ͽ����� �ҽ��� �о�� ���̹Ƿ�, asynchronous�� �߰��� ������ ���ٰ� �Ǵ��Ͽ� synchronous method�� �״�� �����.

	hr = pSourceResolver->CreateObjectFromURL(
		sURL,                       // �ҽ��� ��� (wide char string)
		MF_RESOLUTION_MEDIASOURCE,  // source ��ü�� ������
		NULL,                       // �ҷ��� source�� Ư�� (property)
		&ObjectType,        // �ҽ��� Ÿ���� ���� (��Ʈ�� Ȥ�� ���� �ҽ� ��)
		&pSource            // �̵�� �ҽ��� ����
	);

	if (FAILED(hr))
	{
		goto done;
	}

	hr = pSource->QueryInterface(IID_PPV_ARGS(ppSource)); //�������̽��� �����͸� �̿��� Ÿ���� �����Ͽ� �������̽��� ã��

done:
	SafeRelease(&pSourceResolver);
	SafeRelease(&pSource);
	return hr; //�޸𸮸� �����ϰ� ��ȯ
}

//�̵�� ����� ���� topology�� �����ϴ� �Լ�
HRESULT CreatePlaybackTopology(IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, HWND hVideoWnd, IMFTopology **ppTopology)
{
	IMFTopology *pTopology = NULL;
	DWORD cSourceStreams = 0;

	HRESULT hr = MFCreateTopology(&pTopology); //topology (stream���� data flow�� ����) ����

	if (FAILED(hr))
	{
		goto done;
	}

	hr = pPD->GetStreamDescriptorCount(&cSourceStreams); //�̵�� �ҽ��� �ִ� ��Ʈ�� ���� ������ (���� Ȥ�� ���� ���� �� ����)

	if (FAILED(hr))
	{
		goto done;
	}

	for (DWORD i = 0; i < cSourceStreams; i++)
	{
		hr = AddBranchToPartialTopology(pTopology, pSource, pPD, i, hVideoWnd); //�� ��Ʈ���� ���� topology node�� �߰�.
		//�� node������ ��Ʈ�� �ҽ����� �� ���� (���ڵ� ��)�� ������ ����� ����� ������

		if (FAILED(hr))
		{
			goto done;
		}
	}

	*ppTopology = pTopology; //topology�� ��ȯ
	(*ppTopology)->AddRef();

done:
	SafeRelease(&pTopology);
	return hr;
}

//topology�� �ҽ� �Է�, ��� ������ ����ȭ�ϴ� �Լ�

HRESULT AddBranchToPartialTopology(IMFTopology *pTopology, IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, DWORD iStream, HWND hVideoWnd)
{
	IMFStreamDescriptor *pSD = NULL;
	IMFActivate         *pSinkActivate = NULL;
	IMFTopologyNode     *pSourceNode = NULL;
	IMFTopologyNode     *pOutputNode = NULL;

	BOOL fSelected = FALSE;

	HRESULT hr = pPD->GetStreamDescriptorByIndex(iStream, &fSelected, &pSD); //�ε����� �ش��ϴ� presentation descriptor�� ������
	//presentation�� ��ȭ���� ����� �׷���ó��, ���� �ð����� �帣�� �����/���� ��Ʈ���� set�� ����.
	//presentation descriptor(pd)�� �̷��� presentation�� ���� ������ ������ ����.
	//pd�� selected �Ǵ� deselected�� �� ������, selected�� pd�� ��Ʈ���ֿ� ��ȿ�� �����͸� ������ ���� (�ٱ��� ������ ��ȭ DVD���� �ѱ���� ����ϸ� �ѱ��� �������� selected�ȴ�)

	if (FAILED(hr))
	{
		goto done;
	}

	//selected�� ���

	if (fSelected)
	{
		//media sink(�̵�� ������ �޾ƿ��� ����������)�� Ȱ��ȭ�Ѵ�
		hr = CreateMediaSinkActivate(pSD, hVideoWnd, &pSinkActivate);
		if (FAILED(hr))
		{
			goto done;
		}

		//source node(�ҽ����� �̵� �д� ���)�� �߰��Ѵ�
		hr = AddSourceNode(pTopology, pSource, pPD, pSD, &pSourceNode);
		if (FAILED(hr))
		{
			goto done;
		}

		//output node(�̵� ����� ����ϴ� ���)�� �߰��Ѵ�.
		hr = AddOutputNode(pTopology, pSinkActivate, 0, &pOutputNode);
		if (FAILED(hr))
		{
			goto done;
		}

		//output node�� source node�� �����Ѵ�.
		hr = pSourceNode->ConnectOutput(0, pOutputNode, 0);
	}

	//selected�� �ƴ� ���� ���õ�

done:
	SafeRelease(&pSD);
	SafeRelease(&pSinkActivate);
	SafeRelease(&pSourceNode);
	SafeRelease(&pOutputNode);
	return hr;
}

//�а� ����� �̵���� ������ ���� ������ �������� Ȱ��ȭ�ϴ� �Լ�
HRESULT CreateMediaSinkActivate(IMFStreamDescriptor *pSourceSD, HWND hVideoWindow, IMFActivate **ppActivate)
{
	IMFMediaTypeHandler *pHandler = NULL;
	IMFActivate *pActivate = NULL;

	//��Ʈ������ �̵�� Ÿ���� ������ �ڵ鷯 ����
	HRESULT hr = pSourceSD->GetMediaTypeHandler(&pHandler);
	if (FAILED(hr))
	{
		goto done;
	}

	//major type(���������, Ȥ�� ��������)�� Ȯ���Ѵ�
	GUID guidMajorType;
	hr = pHandler->GetMajorType(&guidMajorType);
	if (FAILED(hr))
	{
		goto done;
	}

	//�̵�� ������ ���� ������ Ȱ��ȭ�� �����Ѵ�

	if (MFMediaType_Audio == guidMajorType) //�̵� ������� ���
	{
		//����� ������ ����
		hr = MFCreateAudioRendererActivate(&pActivate);
	}

	//������ ������� ���� ���̹Ƿ� �ּ� ó��

	/*
	else if (MFMediaType_Video == guidMajorType)
	{
		//���� ������ ����
		hr = MFCreateVideoRendererActivate(hVideoWindow, &pActivate);
	}
	*/

	else //�ƴ� ��� (�� �ڵ忡��, ������ ���� �� �� ���� ��츦 ������)
	{
		hr = E_FAIL;
	}

	if (FAILED(hr))
	{
		goto done;
	}

	//Ȱ��ȭ ��ü�� ��ȯ
	*ppActivate = pActivate;
	(*ppActivate)->AddRef();

done:
	SafeRelease(&pHandler);
	SafeRelease(&pActivate);
	return hr;
}

//topology�� source node�� �߰��ϴ� �Լ�
HRESULT AddSourceNode(IMFTopology *pTopology, IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, IMFStreamDescriptor *pSD, IMFTopologyNode **ppNode)
{
	IMFTopologyNode *pNode = NULL;

	//source node�� �����Ѵ�
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	//�Ӽ��� �����Ѵ�. (source, descriptor ��)

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

	//topology�� node�� �߰��Ѵ�.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	//��带 ��ȯ
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(&pNode);
	return hr;
}

//topology�� output node�� �߰��ϴ� �Լ�
HRESULT AddOutputNode(IMFTopology *pTopology, IMFActivate *pActivate, DWORD dwId, IMFTopologyNode **ppNode)
{
	IMFTopologyNode *pNode = NULL;

	//output node�� �����Ѵ�
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	//�̵�� �������� �����Ѵ�
	hr = pNode->SetObject(pActivate);
	if (FAILED(hr))
	{
		goto done;
	}

	//�̵�� ������ �������� media sink�� �ĺ���(id)�� �����Ѵ�
	hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId);
	if (FAILED(hr))
	{
		goto done;
	}

	//node�� �������� �� shutdown�� ���θ� ���� -> shutdown�� �ϵ��� ������

	hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE);
	if (FAILED(hr))
	{
		goto done;
	}

	//topology�� node�� �߰��Ѵ�.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	//��带 ��ȯ
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(&pNode);
	return hr;
}