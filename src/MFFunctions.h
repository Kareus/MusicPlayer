#pragma once
#ifndef _MF_FUNCTIONS_
#define _MF_FUNCTIONS_

//for COM, thread, etc
#include <new>
#include <Windows.h>
#include <shobjidl.h> 
#include <shlwapi.h>
#include <assert.h>
#include <strsafe.h>
#include <string>

//Media Foundation
#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#include <evr.h>

///Media Foundation�� ����ϱ� ���� ���� �Լ����� ������ ���

#define IDD_MFPLAYBACK_DIALOG           102
#define IDM_EXIT                        105
#define IDC_MFPLAYBACK                  109
#define IDD_OPENURL                     129
#define IDC_EDIT_URL                    1000
#define ID_FILE_OPENFILE                32771
#define ID_FILE_OPENURL                 32772
#define IDC_STATIC                      -1

HRESULT CreateMediaSource(PCWSTR sURL, IMFMediaSource **ppSource);
HRESULT CreatePlaybackTopology(IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, HWND hVideoWnd, IMFTopology **ppTopology);
HRESULT AddBranchToPartialTopology(IMFTopology *pTopology, IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, DWORD iStream, HWND hVideoWnd);
HRESULT CreateMediaSinkActivate(IMFStreamDescriptor *pSourceSD, HWND hVideoWindow, IMFActivate **ppActivate);
HRESULT AddSourceNode(IMFTopology *pTopology, IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, IMFStreamDescriptor *pSD, IMFTopologyNode **ppNode);
HRESULT AddOutputNode(IMFTopology *pTopology, IMFActivate *pActivate, DWORD dwId, IMFTopologyNode **ppNode);

const UINT WM_APP_PLAYER_EVENT = WM_APP + 1; //application�� player�� �����ϱ� ���� �߰��� �̺�Ʈ

// T �����͸� �����ϰ� �����ϴ� �Լ�
template <class T>
void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

// �̵��� �߻��� �̺�Ʈ�κ��� T Ÿ���� ��ü�� �������� �Լ�
template <class T>
HRESULT GetEventObject(IMFMediaEvent *pEvent, T **ppObject)
{
	*ppObject = NULL; //�⺻���� NULL

	PROPVARIANT var;
	HRESULT hr = pEvent->GetValue(&var); //�����͸� �����´�

	if (SUCCEEDED(hr))
	{
		if (var.vt == VT_UNKNOWN) //������ Ÿ���� IUnknown�� ��� (� �������̽��� ���)
		{
			hr = var.punkVal->QueryInterface(ppObject); //�ش� Ÿ������ ĳ���� �õ�
		}
		else
		{
			hr = MF_E_INVALIDTYPE; //�ƴ� ���� ��ȿ���� ���� Ÿ��
		}
		PropVariantClear(&var); //�޸� ����
	}
	return hr;
}
#endif