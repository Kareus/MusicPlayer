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

//Media Foundation을 사용하기 위한 전역 함수들을 정의한 헤더

#define IDD_MFPLAYBACK_DIALOG           102
#define IDM_EXIT                        105
#define IDC_MFPLAYBACK                  109
#define IDD_OPENURL                     129
#define IDC_EDIT_URL                    1000
#define ID_FILE_OPENFILE                32771
#define ID_FILE_OPENURL                 32772
#define IDC_STATIC                      -1

/**
*	@brief	미디어 소스를 생성하는 함수
*	@param	sURL	경로
*	@param	ppSource	생성한 소스를 저장할 포인터
*	@return	함수 실행 결과
*/
HRESULT CreateMediaSource(PCWSTR sURL, IMFMediaSource **ppSource);

/**
*	@brief	음악 재생 topology를 생성하는 함수
*	@param	pSource	미디어 소스
*	@param	pPD	presentation descriptor (오디오/비디오 정보 등을 읽어오는 변수)
*	@param	hVideoWnd	미디어를 재생할 윈도우
*	@param	ppTopology	생성한 topology를 저장할 포인터
*	@return	함수 실행 결과
*/
HRESULT CreatePlaybackTopology(IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, HWND hVideoWnd, IMFTopology **ppTopology);

/**
*	@brief	부분 topology를 연결하는 함수
*	@param	pTopology	topology
*	@param	pSource	미디어 소스
*	@param	pPD	presentaion descriptor
*	@param	iStream	스트림 정보
*	@param	hVideoWnd	미디어를 재생할 윈도우
*	@return	함수 실행 결과
*/
HRESULT AddBranchToPartialTopology(IMFTopology *pTopology, IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, DWORD iStream, HWND hVideoWnd);

/**
*	@brief	미디어 싱크를 생성하는 함수
*	@param	pSourceSD	스트림에 대한 정보를 가지고 있는 변수
*	@param	hVideoWindow	미디어를 재생할 윈도우
*	@param	ppActivate	미디어 렌더러를 저장할 포인터
*	@return	함수 실행 결과
*/
HRESULT CreateMediaSinkActivate(IMFStreamDescriptor *pSourceSD, HWND hVideoWindow, IMFActivate **ppActivate);

/**
*	@brief	미디어 소스 노드를 추가하는 함수
*	@param	pTopology	topology
*	@param	pSource	미디어 소스
*	@param	pPD	presentaion descriptor
*	@param	pSD	스트림에 대한 정보를 가지고 있는 변수
*	@param	ppNode	생성한 노드를 저장할 포인터
*	@return	함수 실행 결과
*/
HRESULT AddSourceNode(IMFTopology *pTopology, IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, IMFStreamDescriptor *pSD, IMFTopologyNode **ppNode);

/**
*	@brief	미디어 출력 노드를 추가하는 함수
*	@param	pTopology	topology
*	@param	pActivate	미디어 렌더러
*	@param	dwId	미디어 싱크 식별자
*	@param	ppNode	생성한 노드를 저장할 포인터
*	@return	함수 실행 결과
*/
HRESULT AddOutputNode(IMFTopology *pTopology, IMFActivate *pActivate, DWORD dwId, IMFTopologyNode **ppNode);

#define WM_APP_PLAYER_EVENT  WM_APP + 1 //application과 player가 소통하기 위해 추가한 이벤트

/*
*	@brief	T 포인터를 안전하게 해제하는 함수
*	@param	ppT	해제할 포인터
*/
template <class T>
void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

/*
*	@brief	미디어에서 발생한 이벤트로부터 T 타입의 객체를 가져오는 함수
*	@param	pEvent	발생한 이벤트
*	@param	ppObject	저장할 객체의 포인터의 포인터
*	@return	함수 실행 결과
*/
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