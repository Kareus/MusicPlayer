#include "GlobalFunctions.h"

namespace String
{
	std::wstring StrToWstr(const std::string& str)
	{
		USES_CONVERSION;
		return A2W_CP(str.c_str(), CP_UTF8); //utf8�� ���ڵ��Ͽ� ��ȯ
	}

	std::string WstrToStr(const std::wstring& str)
	{
		USES_CONVERSION;
		return W2A(str.c_str());
	}
}

namespace Stream
{
	void IgnoreJunk(std::istream& in)
	{
		std::string temp;
		getline(in, temp);
	}
}

namespace Internet
{
	void OpenURL(const std::wstring& path)
	{
		ShellExecute(0, 0, path.c_str(), 0, 0, SW_SHOW);
	}
}