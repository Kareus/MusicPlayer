#include "FileDialog.h"

//필터 문자열의 최대 길이
#define MAX_FILTER_LEN 256

void FileDialog::free(void* ptr)
{
	try
	{
		delete ptr;
	}
	catch (std::bad_alloc& e)
	{
		throw filedialog_error("bad allocation occured while deleting memory");
	}
}

void* FileDialog::allocate(size_t bytes)
{
	try
	{
		void* allocated = malloc(bytes);
		return allocated;
	}
	catch (std::bad_alloc& e)
	{
		throw filedialog_error("bad allocation occured allocationg memory : " + std::to_string(bytes) + " Bytes");
		return nullptr;
	}
}

bool FileDialog::isFilterSegment(wchar_t ch)
{
	return ch == L',' || ch == L';' || ch == 0;
}

DialogResult FileDialog::setDefaultPath(IFileDialog* dialog)
{
	if (defaultPath.empty()) return DIALOG_SUCCESS; //빈 문자열인 경우엔 기본 경로를 설정하지 않은 것으로 판단

	wchar_t* path = (wchar_t*)defaultPath.c_str();
	IShellItem *folder;
	HRESULT result = SHCreateItemFromParsingName(path, NULL, IID_PPV_ARGS(&folder));

	if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND) || result == HRESULT_FROM_WIN32(ERROR_INVALID_DRIVE))
		free(path);
	else if (!SUCCEEDED(result))
	{
		throw filedialog_error("error occured creating ShellItem for dialog default path");
		free(path);
		return DIALOG_ERROR;
	}

	//다이얼로그의 폴더로 설정
	dialog->SetFolder(folder);

	free(path);
	folder->Release();
	return DIALOG_SUCCESS;
}

//비트맵 파일, 사운드 파일을 찾는 두 개의 필터를 만들고 싶다면 jpg,png;mp3,wav 식으로 추가해야 함.
DialogResult FileDialog::setFilterToDialog(IFileDialog* dialog, const std::wstring& filter)
{
	if (filter.empty()) return DIALOG_CANCEL; //빈 문자열인 경우 cancel 반환. (에러는 아님)
	//openFileDialog에서는 빈 문자열이 아님을 보장한다.

	unsigned int filterCount = 1; //필터의 개수
	//빈 문자열이 아니므로 개수는 무조건 1 이상이다.

	const wchar_t *p_filterList; //필터 리스트

	for (p_filterList = filter.c_str(); *p_filterList; p_filterList++)
	{
		//filter를 한 문자열씩 체크한다. 가리키는 문자열이 NULL 문자열인 경우 종료한다.
		if (*p_filterList == L';') filterCount++; //필터 구분은 ;으로 하므로, 체크하여 ++한다.
	}

	if (!filterCount) //개수가 0이면 에러
	{
		throw filedialog_error("error occured parsing filters");
		return DIALOG_ERROR;
	}

	COMDLG_FILTERSPEC *specList = (COMDLG_FILTERSPEC*)allocate(sizeof(COMDLG_FILTERSPEC) * filterCount);

	if (!specList) return DIALOG_ERROR; //메모리 할당에 실패하면 에러

	for (size_t i = 0; i < filterCount; i++)
	{
		specList[i].pszName = NULL;
		specList[i].pszSpec = NULL;
	} //필터 리스트 초기화

	size_t specIndex = 0; //각 필터에 관한 인덱스 변수
	p_filterList = filter.c_str(); //필터를 파싱하기 위해 할당

	std::wstring type = L""; //타입을 파싱하여 저장할 문자열
	std::wstring spec = L""; //다이얼로그 필터에 추가할 문자열
	unsigned int maxLen = MAX_FILTER_LEN; //필터에 들어갈 수 있는 최대 길이

	while (1)
	{
		if (isFilterSegment(*p_filterList)) //구분 문자열이면 (,이 아니라 ;이나 \0이더라도 확장자는 추가해야 한다)
		{
			//spec에 추가

			assert(maxLen > type.size() + 3); //확장자 문자열이 추가될 공간이 있어야 한다.

			if (spec.size() > 0) //spec에 필터가 존재하면
			{
				spec += L';'; //구분 문자열을 추가
				maxLen++; //구분 문자열은 최대 길이에서 제외되어야 하므로, 최대 길이 또한 1 더해준다.
			}

			std::wstring wild = L"*." + type; //파일명은 무시하도록 와일드 카드를 앞에 추가한다.
			assert(wild.size() == type.size() + 2); //*.이 붙었으므로 길이는 2 증가했어야 한다.

			spec += wild;

			//spec 추가 종료
			type = L""; //확장자 초기화
		}
		else //필터 문자열이 아닌 경우 확장자에 문자를 추가
			type += *p_filterList;

		if (*p_filterList == ';' || *p_filterList == '\0') //해당 필터를 다 읽은 경우
		{          
			specList[specIndex].pszName = L""; //필터 설명은 OS에서 자동으로 선택하게끔 비워준다.
			specList[specIndex].pszSpec = spec.c_str();

			spec = L""; //spec 초기화
			if (specIndex == filterCount) break; //필터를 모두 추가하면 끝낸다.
		}

		p_filterList++; //다음 문자로
	}

	dialog->SetFileTypes(filterCount, specList);

	for (size_t i = 0; i < filterCount; i++) free((void*)specList[i].pszSpec);
	free(specList); //메모리 해제

	return DIALOG_SUCCESS;
}

DialogResult FileDialog::openDirectoryDialog(std::wstring& resultPath)
{
	HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (!SUCCEEDED(result))
	{
		throw filedialog_error("CoInitializeEx failed.");
		return DIALOG_ERROR;
	}

	IFileDialog* pFileDialog; //다이얼로그를 위한 COM object

	if (!SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileDialog))))
	{
		throw filedialog_error("CoCreateInstance for CLSID_FileOpenDialog failed.");
		pFileDialog->Release();
		return DIALOG_ERROR;
	}

	//default path 설정
	if (setDefaultPath(pFileDialog) != DIALOG_SUCCESS)
	{
		pFileDialog->Release();
		return DIALOG_ERROR;
	}

	//다이얼로그의 옵션 가져오기
	DWORD dwOptions = 0;
	if (!SUCCEEDED(pFileDialog->GetOptions(&dwOptions)))
	{
		throw filedialog_error("failed to get options from dialog");
		pFileDialog->Release();
		return DIALOG_ERROR;
	}

	//폴더 선택 옵션을 추가

	if (!SUCCEEDED(pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS)))
	{
		throw filedialog_error("failed to add pickFolder option to the dialog");
		pFileDialog->Release();
		return DIALOG_ERROR;
	}

	//다이얼로그 열기 (화면에 띄운다)
	result = pFileDialog->Show(NULL);
	//result에 사용자의 행동이 전달된다. (선택, 취소 등)

	if (result == HRESULT_FROM_WIN32(ERROR_CANCELLED)) //취소한 경우
	{
		pFileDialog->Release();
		return DIALOG_CANCEL;
	}
	else if (!SUCCEEDED(result)) //다이얼로그 상호작용이 실패한 경우
	{
		throw filedialog_error("error occured showing the dialog");
		pFileDialog->Release();
		return DIALOG_ERROR;
	}

	//선택한 항목 가져오기

	IShellItem* pShellItem;
	if (!SUCCEEDED(pFileDialog->GetResult(&pShellItem)))
	{
		throw filedialog_error("error occured getting result from the dialog");
		pFileDialog->Release();
		pShellItem->Release();
		return DIALOG_ERROR;
	}

	//항목에서 경로 가져오기
	wchar_t *path = NULL;

	if (!SUCCEEDED(pShellItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &path)))
	{
		throw filedialog_error("failed to get the path from the dialog");
		pFileDialog->Release();
		pShellItem->Release();
		return DIALOG_ERROR;
	}

	resultPath = path;
	CoTaskMemFree(path);

	pFileDialog->Release();
	pShellItem->Release();
	return DIALOG_SUCCESS;
}

DialogResult FileDialog::openFileDialog(std::wstring& resultPath, const std::wstring& filter)
{
	DialogResult dialogResult = DIALOG_ERROR; //goto문을 사용하기 때문에 필요한 지역변수는 여기서 모두 선언한다. (return할 결과)
	bool setFilter = false; //파일 필터 설정에 관한 변수

	HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE); //COM 생성

	IFileOpenDialog *fileOpenDialog(NULL);

	if (!SUCCEEDED(result))
	{
		throw filedialog_error("failed to initialized COM object");
		goto end;
	}

	// 다이얼로그 생성
	result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&fileOpenDialog));

	if (!SUCCEEDED(result))
	{
		throw filedialog_error("failed to create the dialog.");
		goto end;
	}

	// 파일 필터 설정
	if (filter.empty()) setFilter = setFilterToDialog(fileOpenDialog, L"*.*"); //빈 문자열일 경우 와일드 문자열로 자동 설정
	else setFilter = setFilterToDialog(fileOpenDialog, filter);

	if (setFilter != DIALOG_SUCCESS)
	{
		throw filedialog_error("failed to set the file filter");
		goto end;
	}

	// 기본 경로 설정
	if (setDefaultPath(fileOpenDialog) != DIALOG_SUCCESS)
		goto end;

	// 다이얼로그 열기
	result = fileOpenDialog->Show(NULL);

	if (SUCCEEDED(result))
	{
		// 파일 경로 가져오기
		IShellItem *shellItem(NULL);
		result = fileOpenDialog->GetResult(&shellItem);

		if (!SUCCEEDED(result))
		{
			throw filedialog_error("error occured getting result from the dialog");
			goto end;
		}

		wchar_t *filePath = NULL;
		result = shellItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

		if (!SUCCEEDED(result))
		{
			throw filedialog_error("failed to get the path from the dialog");
			goto end;
		}

		resultPath = filePath;
		CoTaskMemFree(filePath);

		dialogResult = DIALOG_SUCCESS;
		shellItem->Release();
	}
	else if (result == HRESULT_FROM_WIN32(ERROR_CANCELLED))
		dialogResult = DIALOG_CANCEL;
	else
	{
		throw filedialog_error("error occured showing the dialog");
		dialogResult = DIALOG_ERROR;
	}

end:
	CoUninitialize();
	fileOpenDialog->Release();

	return dialogResult;
}

DialogResult FileDialog::saveFileDialog(std::wstring& resultPath, const std::wstring& filter)
{
	DialogResult dialogResult = DIALOG_ERROR;
	bool setFilter = false;

	HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE); //COM 생성

	if (!SUCCEEDED(result))
	{
		throw filedialog_error("failed to initialized COM object");
		return dialogResult;
	}

	IFileSaveDialog *fileSaveDialog(NULL);

	result = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&fileSaveDialog)); //다이얼로그 생성

	if (!SUCCEEDED(result))
	{
		throw filedialog_error("failed to create the dialog.");
		goto end;
	}

	// 파일 필터 설정
	if (filter.empty()) setFilter = setFilterToDialog(fileSaveDialog, L"*.*"); //빈 문자열일 경우 와일드 문자열로 자동 설정
	else setFilter = setFilterToDialog(fileSaveDialog, filter);

	if (setFilter != DIALOG_SUCCESS)
	{
		throw filedialog_error("failed to set the file filter");
		goto end;
	}

	// 기본 경로 설정
	if (setDefaultPath(fileSaveDialog) != DIALOG_SUCCESS)
		goto end;

	// 다이얼로그 열기
	result = fileSaveDialog->Show(NULL);
	if (SUCCEEDED(result))
	{
		// 파일 경로 가져오기
		IShellItem *shellItem;
		result = fileSaveDialog->GetResult(&shellItem);

		if (!SUCCEEDED(result))
		{
			throw filedialog_error("error occured getting result from the dialog");
			goto end;
		}

		wchar_t *filePath = NULL;
		result = shellItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

		if (!SUCCEEDED(result))
		{
			throw filedialog_error("failed to get the path from the dialog");
			goto end;
		}

		resultPath = filePath;
		CoTaskMemFree(filePath);

		dialogResult = DIALOG_SUCCESS;
		shellItem->Release();
	}
	else if (result == HRESULT_FROM_WIN32(ERROR_CANCELLED))
		dialogResult = DIALOG_CANCEL;
	else
	{
		throw filedialog_error("error occured showing the dialog");
		dialogResult = DIALOG_ERROR;
	}

end:
	CoUninitialize();
	fileSaveDialog->Release();

	return dialogResult;
}