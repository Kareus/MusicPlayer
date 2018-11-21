#include "PlayListWriter.h"
#include "MusicType.h"

WPLWriter::WPLWriter()
{
	if (m_outFile) m_outFile.close(); //���� ��Ʈ���� �����ִٸ� �ݴ´�.
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
	open(filepath); //�ʱ�ȭ�� ��, filepath�� ���ڷ� �޾� ��Ʈ���� ����.
}

WPLWriter::~WPLWriter()
{
	if (m_outFile) m_outFile.close(); //���� ��Ʈ���� �����ִٸ� �ݴ´�.
}

bool WPLWriter::open(const wstring& filepath)
{
	int ext = filepath.find_last_of(L'.'); //���� Ȯ���� ��ġ
	const wchar_t* extension = filepath.substr(ext + 1).c_str(); //���� Ȯ����

	if (!_wcsicmp(extension, L"wpl")) extFlag = 1; //wpl ���ϰ� ��ҹ��� ���� ���� ��ġ�ϸ� flag = 1
	if (!_wcsicmp(extension, L"zpl")) extFlag = 2; //zpl ���ϰ� ��ҹ��� ���� ���� ��ġ�ϸ� flag = 2
	
	if (!extFlag) return 0; //��ȿ�� Ȯ���ڰ� �ƴϸ� 0 ��ȯ

	m_outFile.open(filepath, ios::out); //���� ���� ���� ��Ʈ���� ����.

	if (!m_outFile) return 0; //������ ���� �� �����ϸ� 0 ��ȯ

	int lastDir = (int)filepath.find_last_of(L'\\'); //Ÿ��Ʋ�� �ڵ����� �����ϱ� ���� ���丮�� �����ϴ� ������ ���ڸ� ã�´�.
	if (lastDir < 0) lastDir = (int)filepath.find_last_of(L'/'); //�ƴ� ��� �ٸ� ���丮 ���� ���ڷ� ã�´�.
	if (lastDir < 0) return 0; //�ƴ� ��� ����. 0 ��ȯ.

	m_outFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>)); //�ѱ� �� ���ڰ� ������ �ʵ��� locale ����
	///cvt�� ���� �޸𸮴� stream�� �����Ǹ鼭 ���� �����Ǵ� ������ ����.

	title = filepath.substr(lastDir + 1); //������ ���� �ε����� �̿��� �� ���� ���ϸ��� �����´�.
	
	title = title.substr(0, title.find_last_of(L'.')); //���� Ȯ���ڸ� �����Ѵ�.
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
	if (!m_outFile) return; //��Ʈ���� ��ȿ���� �ʴٸ� ����

	if (extFlag == 1) m_outFile << L"<?wpl version = 1.0?>" << endl; //WPL ���� �ۼ� ����
	else if (extFlag == 2) m_outFile << L"<?zpl version = 2.0?>" << endl; //ZPL ���� �ۼ� ����

	m_outFile << L"<smil>" << endl;

	m_outFile << L"    <head>" << endl; //��� �κп� ��Ÿ������ �� Ÿ��Ʋ �ۼ�
	m_outFile << head;
	m_outFile << L"        <title>" << title << L"</title>" << endl;
	m_outFile << L"    </head>" << endl; //��� �ۼ� ����

	m_outFile << L"    <body>" << endl; //�ٵ� �κп� ���� ��� �ۼ�
	m_outFile << L"        <seq>" << endl;
	m_outFile << body;
	m_outFile << L"        </seq>" << endl; //���� ��� �ۼ� ����
	m_outFile << L"    </body>" << endl; //�ٵ� �ۼ� ����
	m_outFile << L"</smil>" << endl; //���� �ۼ� ����

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
	head = L"        <meta name=\"Generator\" content=\"MusicPlayer Application by Seongju Kim\"/>\n"; //��Ÿ������
}

void WPLWriter::resetBody()
{
	body = L"";
}

M3U8Writer::M3U8Writer()
{
	if (m_outFile) m_outFile.close(); //���� ��Ʈ���� �����ִٸ� �ݴ´�.
	title = L"";
	resetBody();
}

M3U8Writer::M3U8Writer(const wstring& filepath)
{
	if (m_outFile) m_outFile.close(); //���� ��Ʈ���� �����ִٸ� �ݴ´�.
	title = L"";
	resetBody();
	open(filepath);
}

M3U8Writer::~M3U8Writer()
{
	if (m_outFile) m_outFile.close(); //���� ��Ʈ���� �����ִٸ� �ݴ´�.
}

bool M3U8Writer::open(const wstring& filepath)
{
	int ext = filepath.find_last_of(L'.'); //���� Ȯ���� ��ġ
	wstring extension = filepath.substr(ext + 1); //���� Ȯ����
	if (_wcsicmp(extension.c_str(), L"m3u8")) return 0; //m3u8 ������ �ƴϸ� 0 ��ȯ

	m_outFile.open(filepath, ios::out); //���� ���� ���� ��Ʈ���� ����.

	if (!m_outFile) return 0; //������ ���� �� �����ϸ� 0 ��ȯ

	int lastDir = (int)filepath.find_last_of(L'\\'); //Ÿ��Ʋ�� �ڵ����� �����ϱ� ���� ���丮�� �����ϴ� ������ ���ڸ� ã�´�.
	if (lastDir < 0) lastDir = (int)filepath.find_last_of(L'/'); //�ƴ� ��� �ٸ� ���丮 ���� ���ڷ� ã�´�.
	if (lastDir < 0) return 0; //�ƴ� ��� ����. 0 ��ȯ.

	m_outFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>)); //�ѱ� �� ���ڰ� ������ �ʵ��� locale ����

	title = filepath.substr(lastDir + 1); //������ ���� �ε����� �̿��� �� ���� ���ϸ��� �����´�.
	title = title.substr(0, title.find_last_of(L'.')); //���� Ȯ���ڸ� �����Ѵ�.
	return 1;
}

void M3U8Writer::resetBody()
{
	body = L"";
}

void M3U8Writer::close()
{
	if (!m_outFile) return; //��Ʈ���� ��ȿ���� �ʴٸ� ����
	m_outFile << L"#EXTM3U" << endl; //���

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