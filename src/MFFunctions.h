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

//Media Foundation�� ����ϱ� ���� ���� �Լ����� ������ ���

#define IDD_MFPLAYBACK_DIALOG           102
#define IDM_EXIT                        105
#define IDC_MFPLAYBACK                  109
#define IDD_OPENURL                     129
#define IDC_EDIT_URL                    1000
#define ID_FILE_OPENFILE                32771
#define ID_FILE_OPENURL                 32772
#define IDC_STATIC                      -1

/**
*	@brief	�̵�� �ҽ��� �����ϴ� �Լ�
*	@param	sURL	���
*	@param	ppSource	������ �ҽ��� ������ ������
*	@return	�Լ� ���� ���
*/
HRESULT CreateMediaSource(PCWSTR sURL, IMFMediaSource **ppSource);

/**
*	@brief	���� ��� topology�� �����ϴ� �Լ�
*	@param	pSource	�̵�� �ҽ�
*	@param	pPD	presentation descriptor (�����/���� ���� ���� �о���� ����)
*	@param	hVideoWnd	�̵� ����� ������
*	@param	ppTopology	������ topology�� ������ ������
*	@return	�Լ� ���� ���
*/
HRESULT CreatePlaybackTopology(IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, HWND hVideoWnd, IMFTopology **ppTopology);

/**
*	@brief	�κ� topology�� �����ϴ� �Լ�
*	@param	pTopology	topology
*	@param	pSource	�̵�� �ҽ�
*	@param	pPD	presentaion descriptor
*	@param	iStream	��Ʈ�� ����
*	@param	hVideoWnd	�̵� ����� ������
*	@return	�Լ� ���� ���
*/
HRESULT AddBranchToPartialTopology(IMFTopology *pTopology, IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, DWORD iStream, HWND hVideoWnd);

/**
*	@brief	�̵�� ��ũ�� �����ϴ� �Լ�
*	@param	pSourceSD	��Ʈ���� ���� ������ ������ �ִ� ����
*	@param	hVideoWindow	�̵� ����� ������
*	@param	ppActivate	�̵�� �������� ������ ������
*	@return	�Լ� ���� ���
*/
HRESULT CreateMediaSinkActivate(IMFStreamDescriptor *pSourceSD, HWND hVideoWindow, IMFActivate **ppActivate);

/**
*	@brief	�̵�� �ҽ� ��带 �߰��ϴ� �Լ�
*	@param	pTopology	topology
*	@param	pSource	�̵�� �ҽ�
*	@param	pPD	presentaion descriptor
*	@param	pSD	��Ʈ���� ���� ������ ������ �ִ� ����
*	@param	ppNode	������ ��带 ������ ������
*	@return	�Լ� ���� ���
*/
HRESULT AddSourceNode(IMFTopology *pTopology, IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, IMFStreamDescriptor *pSD, IMFTopologyNode **ppNode);

/**
*	@brief	�̵�� ��� ��带 �߰��ϴ� �Լ�
*	@param	pTopology	topology
*	@param	pActivate	�̵�� ������
*	@param	dwId	�̵�� ��ũ �ĺ���
*	@param	ppNode	������ ��带 ������ ������
*	@return	�Լ� ���� ���
*/
HRESULT AddOutputNode(IMFTopology *pTopology, IMFActivate *pActivate, DWORD dwId, IMFTopologyNode **ppNode);

#define WM_APP_PLAYER_EVENT  WM_APP + 1 //application�� player�� �����ϱ� ���� �߰��� �̺�Ʈ

/*
*	@brief	T �����͸� �����ϰ� �����ϴ� �Լ�
*	@param	ppT	������ ������
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
*	@brief	�̵��� �߻��� �̺�Ʈ�κ��� T Ÿ���� ��ü�� �������� �Լ�
*	@param	pEvent	�߻��� �̺�Ʈ
*	@param	ppObject	������ ��ü�� �������� ������
*	@return	�Լ� ���� ���
*/
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