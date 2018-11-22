#include "FileDialog.h"

//���� ���ڿ��� �ִ� ����
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
	if (defaultPath.empty()) return DIALOG_SUCCESS; //�� ���ڿ��� ��쿣 �⺻ ��θ� �������� ���� ������ �Ǵ�

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

	//���̾�α��� ������ ����
	dialog->SetFolder(folder);

	free(path);
	folder->Release();
	return DIALOG_SUCCESS;
}

//��Ʈ�� ����, ���� ������ ã�� �� ���� ���͸� ����� �ʹٸ� jpg,png;mp3,wav ������ �߰��ؾ� ��.
DialogResult FileDialog::setFilterToDialog(IFileDialog* dialog, const std::wstring& filter)
{
	if (filter.empty()) return DIALOG_CANCEL; //�� ���ڿ��� ��� cancel ��ȯ. (������ �ƴ�)
	//openFileDialog������ �� ���ڿ��� �ƴ��� �����Ѵ�.

	unsigned int filterCount = 1; //������ ����
	//�� ���ڿ��� �ƴϹǷ� ������ ������ 1 �̻��̴�.

	const wchar_t *p_filterList; //���� ����Ʈ

	for (p_filterList = filter.c_str(); *p_filterList; p_filterList++)
	{
		//filter�� �� ���ڿ��� üũ�Ѵ�. ����Ű�� ���ڿ��� NULL ���ڿ��� ��� �����Ѵ�.
		if (*p_filterList == L';') filterCount++; //���� ������ ;���� �ϹǷ�, üũ�Ͽ� ++�Ѵ�.
	}

	if (!filterCount) //������ 0�̸� ����
	{
		throw filedialog_error("error occured parsing filters");
		return DIALOG_ERROR;
	}

	COMDLG_FILTERSPEC *specList = (COMDLG_FILTERSPEC*)allocate(sizeof(COMDLG_FILTERSPEC) * filterCount);

	if (!specList) return DIALOG_ERROR; //�޸� �Ҵ翡 �����ϸ� ����

	for (size_t i = 0; i < filterCount; i++)
	{
		specList[i].pszName = NULL;
		specList[i].pszSpec = NULL;
	} //���� ����Ʈ �ʱ�ȭ

	size_t specIndex = 0; //�� ���Ϳ� ���� �ε��� ����
	p_filterList = filter.c_str(); //���͸� �Ľ��ϱ� ���� �Ҵ�

	std::wstring type = L""; //Ÿ���� �Ľ��Ͽ� ������ ���ڿ�
	std::wstring spec = L""; //���̾�α� ���Ϳ� �߰��� ���ڿ�
	unsigned int maxLen = MAX_FILTER_LEN; //���Ϳ� �� �� �ִ� �ִ� ����

	while (1)
	{
		if (isFilterSegment(*p_filterList)) //���� ���ڿ��̸� (,�� �ƴ϶� ;�̳� \0�̴��� Ȯ���ڴ� �߰��ؾ� �Ѵ�)
		{
			//spec�� �߰�

			assert(maxLen > type.size() + 3); //Ȯ���� ���ڿ��� �߰��� ������ �־�� �Ѵ�.

			if (spec.size() > 0) //spec�� ���Ͱ� �����ϸ�
			{
				spec += L';'; //���� ���ڿ��� �߰�
				maxLen++; //���� ���ڿ��� �ִ� ���̿��� ���ܵǾ�� �ϹǷ�, �ִ� ���� ���� 1 �����ش�.
			}

			std::wstring wild = L"*." + type; //���ϸ��� �����ϵ��� ���ϵ� ī�带 �տ� �߰��Ѵ�.
			assert(wild.size() == type.size() + 2); //*.�� �پ����Ƿ� ���̴� 2 �����߾�� �Ѵ�.

			spec += wild;

			//spec �߰� ����
			type = L""; //Ȯ���� �ʱ�ȭ
		}
		else //���� ���ڿ��� �ƴ� ��� Ȯ���ڿ� ���ڸ� �߰�
			type += *p_filterList;

		if (*p_filterList == ';' || *p_filterList == '\0') //�ش� ���͸� �� ���� ���
		{          
			specList[specIndex].pszName = L""; //���� ������ OS���� �ڵ����� �����ϰԲ� ����ش�.
			specList[specIndex].pszSpec = spec.c_str();

			spec = L""; //spec �ʱ�ȭ
			if (specIndex == filterCount) break; //���͸� ��� �߰��ϸ� ������.
		}

		p_filterList++; //���� ���ڷ�
	}

	dialog->SetFileTypes(filterCount, specList);

	for (size_t i = 0; i < filterCount; i++) free((void*)specList[i].pszSpec);
	free(specList); //�޸� ����

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

	IFileDialog* pFileDialog; //���̾�α׸� ���� COM object

	if (!SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileDialog))))
	{
		throw filedialog_error("CoCreateInstance for CLSID_FileOpenDialog failed.");
		pFileDialog->Release();
		return DIALOG_ERROR;
	}

	//default path ����
	if (setDefaultPath(pFileDialog) != DIALOG_SUCCESS)
	{
		pFileDialog->Release();
		return DIALOG_ERROR;
	}

	//���̾�α��� �ɼ� ��������
	DWORD dwOptions = 0;
	if (!SUCCEEDED(pFileDialog->GetOptions(&dwOptions)))
	{
		throw filedialog_error("failed to get options from dialog");
		pFileDialog->Release();
		return DIALOG_ERROR;
	}

	//���� ���� �ɼ��� �߰�

	if (!SUCCEEDED(pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS)))
	{
		throw filedialog_error("failed to add pickFolder option to the dialog");
		pFileDialog->Release();
		return DIALOG_ERROR;
	}

	//���̾�α� ���� (ȭ�鿡 ����)
	result = pFileDialog->Show(NULL);
	//result�� ������� �ൿ�� ���޵ȴ�. (����, ��� ��)

	if (result == HRESULT_FROM_WIN32(ERROR_CANCELLED)) //����� ���
	{
		pFileDialog->Release();
		return DIALOG_CANCEL;
	}
	else if (!SUCCEEDED(result)) //���̾�α� ��ȣ�ۿ��� ������ ���
	{
		throw filedialog_error("error occured showing the dialog");
		pFileDialog->Release();
		return DIALOG_ERROR;
	}

	//������ �׸� ��������

	IShellItem* pShellItem;
	if (!SUCCEEDED(pFileDialog->GetResult(&pShellItem)))
	{
		throw filedialog_error("error occured getting result from the dialog");
		pFileDialog->Release();
		pShellItem->Release();
		return DIALOG_ERROR;
	}

	//�׸񿡼� ��� ��������
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
	DialogResult dialogResult = DIALOG_ERROR; //goto���� ����ϱ� ������ �ʿ��� ���������� ���⼭ ��� �����Ѵ�. (return�� ���)
	bool setFilter = false; //���� ���� ������ ���� ����

	HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE); //COM ����

	IFileOpenDialog *fileOpenDialog(NULL);

	if (!SUCCEEDED(result))
	{
		throw filedialog_error("failed to initialized COM object");
		goto end;
	}

	// ���̾�α� ����
	result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&fileOpenDialog));

	if (!SUCCEEDED(result))
	{
		throw filedialog_error("failed to create the dialog.");
		goto end;
	}

	// ���� ���� ����
	if (filter.empty()) setFilter = setFilterToDialog(fileOpenDialog, L"*.*"); //�� ���ڿ��� ��� ���ϵ� ���ڿ��� �ڵ� ����
	else setFilter = setFilterToDialog(fileOpenDialog, filter);

	if (setFilter != DIALOG_SUCCESS)
	{
		throw filedialog_error("failed to set the file filter");
		goto end;
	}

	// �⺻ ��� ����
	if (setDefaultPath(fileOpenDialog) != DIALOG_SUCCESS)
		goto end;

	// ���̾�α� ����
	result = fileOpenDialog->Show(NULL);

	if (SUCCEEDED(result))
	{
		// ���� ��� ��������
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

	HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE); //COM ����

	if (!SUCCEEDED(result))
	{
		throw filedialog_error("failed to initialized COM object");
		return dialogResult;
	}

	IFileSaveDialog *fileSaveDialog(NULL);

	result = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&fileSaveDialog)); //���̾�α� ����

	if (!SUCCEEDED(result))
	{
		throw filedialog_error("failed to create the dialog.");
		goto end;
	}

	// ���� ���� ����
	if (filter.empty()) setFilter = setFilterToDialog(fileSaveDialog, L"*.*"); //�� ���ڿ��� ��� ���ϵ� ���ڿ��� �ڵ� ����
	else setFilter = setFilterToDialog(fileSaveDialog, filter);

	if (setFilter != DIALOG_SUCCESS)
	{
		throw filedialog_error("failed to set the file filter");
		goto end;
	}

	// �⺻ ��� ����
	if (setDefaultPath(fileSaveDialog) != DIALOG_SUCCESS)
		goto end;

	// ���̾�α� ����
	result = fileSaveDialog->Show(NULL);
	if (SUCCEEDED(result))
	{
		// ���� ��� ��������
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