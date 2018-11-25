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

///Media Foundation을 사용하기 위한 전역 함수들을 정의한 헤더

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

const UINT WM_APP_PLAYER_EVENT = WM_APP + 1; //application과 player가 소통하기 위해 추가한 이벤트

// T 포인터를 안전하게 해제하는 함수
template <class T>
void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

// 미디어에서 발생한 이벤트로부터 T 타입의 객체를 가져오는 함수
template <class T>
HRESULT GetEventObject(IMFMediaEvent *pEvent, T **ppObject)
{
	*ppObject = NULL; //기본값은 NULL

	PROPVARIANT var;
	HRESULT hr = pEvent->GetValue(&var); //데이터를 가져온다

	if (SUCCEEDED(hr))
	{
		if (var.vt == VT_UNKNOWN) //변수의 타입이 IUnknown인 경우 (어떤 인터페이스인 경우)
		{
			hr = var.punkVal->QueryInterface(ppObject); //해당 타입으로 캐스팅 시도
		}
		else
		{
			hr = MF_E_INVALIDTYPE; //아닌 경우는 유효하지 않은 타입
		}
		PropVariantClear(&var); //메모리 해제
	}
	return hr;
}
#endif