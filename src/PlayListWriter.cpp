#include "PlayListWriter.h"
#include "MusicType.h"

WPLWriter::WPLWriter()
{
	if (m_outFile) m_outFile.close(); //만약 스트림이 열려있다면 닫는다.
	resetHead();
	resetBody();
	title = L"";
	extFlag = 0;
}

WPLWriter::WPLWriter(const wstring& filepath)
{
	if (m_outFile) m_outFile.close();
	resetHead();
	resetBody();
	title = L"";
	extFlag = 0;
	open(filepath); //초기화한 후, filepath를 인자로 받아 스트림을 연다.
}

WPLWriter::~WPLWriter()
{
	if (m_outFile) m_outFile.close(); //만약 스트림이 열려있다면 닫는다.
}

bool WPLWriter::open(const wstring& filepath)
{
	int ext = filepath.find_last_of(L'.'); //파일 확장자 위치
	const wchar_t* extension = filepath.substr(ext + 1).c_str(); //파일 확장자

	if (!_wcsicmp(extension, L"wpl")) extFlag = 1; //wpl 파일과 대소문자 구분 없이 일치하면 flag = 1
	if (!_wcsicmp(extension, L"zpl")) extFlag = 2; //zpl 파일과 대소문자 구분 없이 일치하면 flag = 2
	
	if (!extFlag) return 0; //유효한 확장자가 아니면 0 반환

	m_outFile.open(filepath, ios::out); //쓰기 모드로 파일 스트림을 연다.

	if (!m_outFile) return 0; //파일을 여는 데 실패하면 0 반환

	int lastDir = (int)filepath.find_last_of(L'\\'); //타이틀을 자동으로 설정하기 위해 디렉토리를 구분하는 마지막 문자를 찾는다.
	if (lastDir < 0) lastDir = (int)filepath.find_last_of(L'/'); //아닌 경우 다른 디렉토리 구분 문자로 찾는다.
	if (lastDir < 0) return 0; //아닌 경우 에러. 0 반환.

	m_outFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>)); //한글 등 글자가 깨지지 않도록 locale 설정
	///cvt의 동적 메모리는 stream이 해제되면서 같이 해제되는 것으로 보임.

	title = filepath.substr(lastDir + 1); //위에서 구한 인덱스를 이용해 그 뒤의 파일명만을 가져온다.
	
	title = title.substr(0, title.find_last_of(L'.')); //파일 확장자를 제거한다.
	return 1;
}

void WPLWriter::setTitle(const wstring& title)
{
	this->title = title;
}

wstring WPLWriter::getTitle()
{
	return title;
}

void WPLWriter::close()
{
	if (!m_outFile) return; //스트림이 유효하지 않다면 종료

	if (extFlag == 1) m_outFile << L"<?wpl version = 1.0?>" << endl; //WPL 파일 작성 시작
	else if (extFlag == 2) m_outFile << L"<?zpl version = 2.0?>" << endl; //ZPL 파일 작성 시작

	m_outFile << L"<smil>" << endl;

	m_outFile << L"    <head>" << endl; //헤드 부분에 메타데이터 및 타이틀 작성
	m_outFile << head;
	m_outFile << L"        <title>" << title << L"</title>" << endl;
	m_outFile << L"    </head>" << endl; //헤드 작성 종료

	m_outFile << L"    <body>" << endl; //바디 부분에 음악 목록 작성
	m_outFile << L"        <seq>" << endl;
	m_outFile << body;
	m_outFile << L"        </seq>" << endl; //음악 목록 작성 종료
	m_outFile << L"    </body>" << endl; //바디 작성 종료
	m_outFile << L"</smil>" << endl; //파일 작성 종료

	m_outFile.close();
}

void WPLWriter::addFile(const wstring& mediaPath)
{
	body += L"            <media src=\"" + mediaPath + L"\"/>\n";
}

void WPLWriter::addMeta(const wstring& name, const wstring& data)
{
	head += L"<meta name = \"" + name + L"\" content = \"" + data + L"\" />\n";
}

void WPLWriter::resetHead()
{
	head = L"        <meta name=\"Generator\" content=\"MusicPlayer Application by Seongju Kim\"/>\n"; //메타데이터
}

void WPLWriter::resetBody()
{
	body = L"";
}

M3U8Writer::M3U8Writer()
{
	if (m_outFile) m_outFile.close(); //만약 스트림이 열려있다면 닫는다.
	title = L"";
	resetBody();
}

M3U8Writer::M3U8Writer(const wstring& filepath)
{
	if (m_outFile) m_outFile.close(); //만약 스트림이 열려있다면 닫는다.
	title = L"";
	resetBody();
	open(filepath);
}

M3U8Writer::~M3U8Writer()
{
	if (m_outFile) m_outFile.close(); //만약 스트림이 열려있다면 닫는다.
}

bool M3U8Writer::open(const wstring& filepath)
{
	int ext = filepath.find_last_of(L'.'); //파일 확장자 위치
	wstring extension = filepath.substr(ext + 1); //파일 확장자
	if (_wcsicmp(extension.c_str(), L"m3u8")) return 0; //m3u8 파일이 아니면 0 반환

	m_outFile.open(filepath, ios::out); //쓰기 모드로 파일 스트림을 연다.

	if (!m_outFile) return 0; //파일을 여는 데 실패하면 0 반환

	int lastDir = (int)filepath.find_last_of(L'\\'); //타이틀을 자동으로 설정하기 위해 디렉토리를 구분하는 마지막 문자를 찾는다.
	if (lastDir < 0) lastDir = (int)filepath.find_last_of(L'/'); //아닌 경우 다른 디렉토리 구분 문자로 찾는다.
	if (lastDir < 0) return 0; //아닌 경우 에러. 0 반환.

	m_outFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>)); //한글 등 글자가 깨지지 않도록 locale 설정

	title = filepath.substr(lastDir + 1); //위에서 구한 인덱스를 이용해 그 뒤의 파일명만을 가져온다.
	title = title.substr(0, title.find_last_of(L'.')); //파일 확장자를 제거한다.
	return 1;
}

void M3U8Writer::resetBody()
{
	body = L"";
}

void M3U8Writer::close()
{
	if (!m_outFile) return; //스트림이 유효하지 않다면 종료
	m_outFile << L"#EXTM3U" << endl; //헤더

	m_outFile << body << endl;
}

void M3U8Writer::addMedia(unsigned int length, const wstring& name, const wstring& mediapath)
{
	body += L"#EXTINF:";
	body += std::to_wstring(length);
	body += L',';
	body += name + L'\n';
	body += mediapath + L'\n';
}


void M3U8Writer::addMedia(const MusicType& music)
{
//	addMedia(music.GetLength(), music.GetArtist() + ' - ' + music.GetName(), music.GetPath());
}


void M3U8Writer::setTitle(const wstring& title)
{
	this->title = title;
}

wstring M3U8Writer::getTitle()
{
	return title;
}