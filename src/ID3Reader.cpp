#include "ID3Reader.h"
#include "GenreTable.h"
#include <atlstr.h> 

using namespace std;

ID3Reader::ID3Reader()
{
	if (m_inFile) m_inFile.close(); //���� ��Ʈ���� �����ִٸ� �ݴ´�.
	majorVersion = 0;
	minorVersion = 0;
	revision = 0;
	tagSize = 0;
	track = 0;
	genre = -1;
	frames.MakeEmpty();
}

ID3Reader::ID3Reader(const wstring& filepath)
{
	if (m_inFile) m_inFile.close();
	majorVersion = 0;
	minorVersion = 0;
	revision = 0;
	track = 0;
	genre = -1;
	frames.MakeEmpty();
	read(filepath);
}

ID3Reader::~ID3Reader()
{
	if (m_inFile) m_inFile.close();
	frames.MakeEmpty();
}

int ID3Reader::getFrameCase(const std::string& id)
{
	if (!strcmp(id.c_str(),"USLT")) return 1; //unsynchronized lyrics ó��
	if (!strcmp(id.c_str(), "SYLT")) return 2; //synchronized lyrics ó��
	return 0;
}

bool ID3Reader::canRead(char* id)
{
	for (int i = 0; i < 4; i++)
	{
		if (id[i] < 20 || id[i] > 90) return false;
	}

	return true;
}

bool ID3Reader::read(const wstring& filepath)
{
	majorVersion = 0;
	minorVersion = 0;

	if (filepath.size() < 4) return 0; //�ùٸ��� ���� ����� ��� 0 ��ȯ

	if (wcscmp(filepath.substr(filepath.size()-4, 4).c_str(), L".mp3")) return 0; //mp3 ������ �ƴϸ� 0 ��ȯ

	m_inFile.open(filepath, ios::in | ios::binary);

	if (!m_inFile) return 0; //���� ���⿡ �����ϸ� 0 ��ȯ

	m_inFile.seekg(0, ios::end);
	int fileSize = m_inFile.tellg(); //���� ũ��
	m_inFile.seekg(0, ios::beg);

	char tag[3] = { 0 };
	m_inFile.read(tag, 3);
	if (memcmp(tag, "ID3", 3)) //file���� data�� �о�� ��� strcmp�� ����� �۵����� �ʴ´�. (���� buffer � ������ �ִ� ������ ����). �޸� �� �Լ� memcmp ���.
	{
		//ID3v2�� �ƴ� ���
		m_inFile.seekg(-128, ios::end); //������ ������ 128 byte �������� �´�.
		m_inFile.read(tag, 3);
		if (memcmp(tag, "TAG", 3))
		{
			m_inFile.close();
			return 0; //ID3 v1�� �ƴ� ���, �±׸� ���� �������Ƿ� 0 ��ȯ
		}

		majorVersion = 1; //ID3v1

		m_inFile.read(title, 30); //Ÿ��Ʋ
		m_inFile.read(artist, 30); //��Ƽ��Ʈ
		m_inFile.read(album, 30); //�ٹ�
		m_inFile.read(year, 4); //����
		m_inFile.read(comment, 28); //�ڸ�Ʈ
		unsigned char zeroByte = 0;
		m_inFile.read((char*)&zeroByte, 1);
		if (!zeroByte) m_inFile.read(comment + 28, 2); //zero-byte�� 0�� �ƴ� ���, ID3v1. �������� ��� �ڸ�Ʈ�� ����.
		else m_inFile.read(&track, 1); //zero-byte�� 0�� ���, ID3v1.1�� �� ����. (Ʈ�� �ѹ�). ID3v1��� �ϴ���, zeroByte�� 0�̸� �� �����̹Ƿ� �� ����Ʈ�� �ڸ�Ʈ�� �� �� ����.

		m_inFile.read(&genre, 1); //�帣
	}
	else
	{
		majorVersion = 2; //ID3v2
		m_inFile.read(&minorVersion, 1); //���� ���� �б� 
		m_inFile.read(&revision, 1); //revision ���� �б�

		if (!(minorVersion == 3 || minorVersion == 4))
		{
			m_inFile.close();
			return 0; //2.3�̳� 2.4�� �ƴ� ���, ȣȯ���� �����Ƿ� 0 ��ȯ. (2.1�� 2.2�� ���� structure ������ ã�� ����)
		}

		//��� ������ �о�´�.

		char flag = 0; //mp3 ������ ���� header flag
		m_inFile.read(&flag, 1); //flag �б�. v2.3/2.4������ �� 8��Ʈ �� ���� 3��Ʈ�� flag�� ����ϰ� �ִ�.

		char syncSize[4] = { 0 };
		m_inFile.read(syncSize, 4); //����� ũ�⸦ �о�´�. ũ��� sync�Ǿ� �ִ�.
		tagSize = decodeSync(syncSize); //sync�� byte�� decode�Ͽ� int�� �����Ѵ�.

		bool flag_c = flag >>= 5 & 1; //���� 3��° ��Ʈ. experimental indicator. tag�� ���� �������� ������ ����Ǿ��� �� 1�̴�.
		bool flag_b = flag >>= 1 & 1; //���� 2��° ��Ʈ. extended header. Ȯ��� ��� ���ǰ� ���� ��� 1�̴�.
		bool flag_a = flag >>= 1; //���� 1��° ��Ʈ. unsynchronisation. ������ software�� ȣȯ���� ���� unsync�� �����ߴٸ� 1�̴�.

		//���⼭ ����� ���� �� �ʿ��� ��Ʈ�� flag_b�̴�.

		if (flag_b) //extended header�� �����ϸ�
		{
			m_inFile.read(syncSize, 4); //extended header�� ũ�⸦ �о�´�. �� ũ�� ���� sync�Ǿ� �ִ�.
			m_inFile.seekg(decodeSync(syncSize),ios::cur); //extended header���� tag�� ���õ� ������ ���Ե��� �����Ƿ� skip
		}

		if (fileSize < tagSize + 10) //����� ũ�Ⱑ 10����Ʈ�̴�.
		{
			//���� ���� ũ�Ⱑ �о���� �±� ũ�� + ��� ũ�⺸�� ũ��, �о�� �� ����.
			m_inFile.close();
			throw id3_error("Invalid tag size");
			return 0;
		}

		//�±� ������ �о�´�.
		int offset = 0;
		while (offset < tagSize)
		{
			char ID[4] = { 0 }; //property�� �����ϱ� ���� frame ID.
			m_inFile.read(ID, 4); //frame ID�� �о�´�.

			if (!canRead(ID)) //�о�� ID�� Reader���� �������� �ʴ´ٸ�
			{
				break; //�����Ѵ�.
			}
			
			char frameSize[4];
			m_inFile.read(frameSize, 4); //property�� ũ�⸦ �о�´�. sync�Ǿ� �ִ�.

			int realSize;
			if (minorVersion == 4) realSize = decodeSync(frameSize); //2.4������ frameSize�� sync�Ǿ� �ִ�.
			else realSize = (frameSize[0] << 24) + (frameSize[1] << 16) + (frameSize[2] << 8) + frameSize[3]; //�ƴ� ���� decode���� ����
			char* data = new char[realSize];

			m_inFile.seekg(2, ios::cur); //frame flag 2���� skip
			//�� flag�� frame�� ��ȿ���� ���� �� skip����, Ȥ�� frame�� compressed �Ǵ� encrypted����, group information���� ���� ��Ÿ����. 
			//�� class������ �� flag�� ���� ó���� �����ϰ�, tag�� �о���� ������ �ڵ常 �ۼ��� ���̴�.
			//�̿� ���Ҿ� ���������� �� flag�� set�� sample data�� ã�� ���߰�, �׽�Ʈ�� �� ���� �� ȯ���� ������ �����Ѵ�.

			m_inFile.read(data, realSize); //data�� �о�´�.

			char encoder = *data; //���ڵ��� ��Ʈ�� Ÿ��

			string id = ""; //key�� �� ID�� �����ϴ� string
			wstring frame = L""; //value�� �� frame data�� �����ϴ� wstring. ���ڵ� Ÿ���� �޶��� �� �����Ƿ� wstring�� �����Ѵ�.

			//char*���� string���� �Ҵ� �����ڷ� �ű� �� �ڿ� �߰��� index�� �þ ���ڿ� �νĿ��� ������ �߻���.
			for (int i = 0; i < 4; i++) id += ID[i]; //ID�� 4���ڸ� string�� �ű��.

			int Case = getFrameCase(id);
			int ignore = 4;
			char* descriptor;

			if (Case >= 1)
			{
				//unsynchronized lyrics. ���� �ڵ尡 3byte, descriptor�� \0\0�� ���� ������ ������ ũ�⸦ �����Ѵ�.
				if (Case == 2) //synchronized lyrics. unsynchronized���� time stamp�� content ����� 2����Ʈ �� �ִ�.
					ignore += 2;
				descriptor = data + ignore;
				while (memcmp(descriptor, "\0\0", 2))
				{
					descriptor++;
					ignore++; //content descriptor�� ���������� �޸� �ּҸ� �ǳʶڴ�.
				}
				ignore += 2; //descriptor �ǳʶ�

				if (encoder == 0)
				{
					for (int i = ignore; i < realSize; i++) frame += data[i];
				}
				else if (encoder == 1)
				{
					if (data[ignore] == -1 && data[ignore + 1] == -2) //little endian
						for (int i = ignore + 2; i < realSize; i += 2) frame += *(uint16_t*)(data + i); //���ڵ� Ÿ��, BOM�� �����ϰ� 3��° ���ں��� �ű��.
					else if (data[ignore] == -2 && data[ignore + 1] == -1) // big endian
						for (int i = realSize - 2; i > ignore + 2; i -= 2) frame += *(uint16_t *)(data + i); //big endian�� ��� �ݴ�� �о�� �ϹǷ� ���������� 3��° ���ڱ��� �ű��.
				}
				else if (encoder == 3)
				{
					for (int i = ignore; i < realSize; i++) frame += *(uint8_t*)(data + i); //UTF8�� ��� BOM�� ���ų� ��� �����Ƿ� 1��° ���ں��� �ű��.
				}
			}
			else
			{
				if (encoder == 0)
				{
					//ISO-8859-1
					for (int i = 1; i < realSize; i++) frame += data[i]; //0��° ���� (���ڵ� Ÿ��)�� �����ϰ� �������� frame�� �ű��.
				}
				else if (encoder == 1 || encoder == 2)
				{
					if (data[1] == -1 && data[2] == -2) //little endian
						for (int i = 3; i < realSize; i += 2) frame += *(uint16_t*)(data + i); //���ڵ� Ÿ��, BOM�� �����ϰ� 3��° ���ں��� �ű��.
					else if (data[1] == -2 && data[2] == -1) // big endian
						for (int i = realSize - 2; i > 2; i -= 2) frame += *(uint16_t *)(data + i); //big endian�� ��� �ݴ�� �о�� �ϹǷ� ���������� 3��° ���ڱ��� �ű��.
				}
				else if (encoder == 3)
				{
					for (int i = 1; i < realSize; i += 2) frame += *(uint8_t*)(data + i); //UTF8�� ��� BOM�� ���ų� ��� �����Ƿ� 1��° ���ں��� �ű��.
				}
			}
			

			ID3Frame id3frame(id, frame);
			frames.Add(id3frame); //frame �߰�
			delete data; //�޸� ����

			offset += realSize + 10; //frame + id size (= 4) + frame size (= 4) + flag size (= 2)
		}
	}

	m_inFile.close();
	return 1;
}

void ID3Reader::close()
{
	//� ������ ���� �ܺο��� ���������� file stream�� ������ ���� ���� �Լ�
	m_inFile.close();
}

int ID3Reader::getMajorVersion()
{
	return (int)majorVersion;
}

int ID3Reader::getMinorVersion()
{
	return (int)minorVersion;
}

int ID3Reader::getRevision()
{
	return (int)revision;
}

wstring ID3Reader::getTitle()
{
	if (majorVersion == 2) return getFrame("TIT2"); //title�� �ش��ϴ� frame �̸��� TIT2

	wstring output; //v1�� ���, char array���� wstring���� ��ȯ�Ͽ� ��ȯ
	for (int i = 0; i < 30; i++) output += title[i];
	return output;
}

wstring ID3Reader::getArtist()
{
	if (majorVersion == 2) return getFrame("TPE1"); //lead artist�� �ش��ϴ� frame �̸��� TPE1

	wstring output; //v1�� ���, char array���� wstring���� ��ȯ�Ͽ� ��ȯ
	for (int i = 0; i < 30; i++) output += artist[i];
	return output;
}

wstring ID3Reader::getAlbum()
{
	if (majorVersion == 2) return getFrame("TALB"); //album�� �ش��ϴ� frame �̸��� TALB

	wstring output; //v1�� ���, char array���� wstring���� ��ȯ�Ͽ� ��ȯ
	for (int i = 0; i < 30; i++) output += album[i];
	return output;
}

wstring ID3Reader::getYear()
{
	if (majorVersion == 2)
	{
		if (minorVersion == 3) return getFrame("TYER"); //��¥�� ��Ÿ���� frame�� recording time, release time �� ������ ���⼱ year(TYER)�� ����ϱ�� ��
		else if (minorVersion == 4)
		{
			//v2.4������ TYER �� ������ ��¥ frame�� ��� deprecate��.
			wstring recordTime = getFrame("TDRC"); //TYER ��� ��ü�Ǿ� ���� ���� frame (Record Time)
			if (recordTime.empty()) return getFrame("TDRL"); //TDRC�� ���ٸ� TDRL ��ȯ. (Release Time)
			return recordTime;
		}
	}
	wstring output; //v1�� ���, char array���� wstring���� ��ȯ�Ͽ� ��ȯ
	for (int i = 0; i < 4; i++) output += year[i];
	return output;
}

int ID3Reader::getYearAsInteger()
{
	int n = 0;

	if (majorVersion == 1)
	{
		char* temp = year; //�� char�� ��� ���� �ӽ� ������ (���� �� dangling pointer �߻�)

		if (*temp == 0) return 0; //year ������ ������ 0 ��ȯ

		for (int i = 0; i < 4; i++) n = n * 10 + *(++temp) - 48;
		//0~9�� ascii code = 48~57. ���� �� * 10�� �ϰ�, �� �ڸ����� ���ڸ� ���Ѵ�.
		//ex) 2018 : 2 -> 2 * 10 + 0 -> 20 * 10 + 1 -> 201 * 10 + 8 = 2018
	}
	else
	{
		wstring y = getYear();
		if (y.empty()) return 0; //year ������ ������ 0 ��ȯ

		for (int i = 0; i < y.size(); i++) n = n * 10 + y.at(i) - 48;
	}

	return n;
}

wstring ID3Reader::getComment()
{
	if (majorVersion == 2) return getFrame("COMM"); //comment�� �ش��ϴ� frame �̸��� COMM

	wstring output; //v1�� ���, char array���� wstring���� ��ȯ�Ͽ� ��ȯ
	for (int i = 0; i < 30; i++) output += comment[i];
	return output;
}

int ID3Reader::getTrackNum()
{
	if (majorVersion == 2) return (int)getFrame("TRCK")[0]; //track number�� �ش��ϴ� frame �̸��� TRCK

	return track; //v1�� ���, char array���� wstring���� ��ȯ�Ͽ� ��ȯ
}

wstring ID3Reader::getGenre()
{
	if (majorVersion == 2)
	{
		wstring content = getFrame("TCON"); //v2������ content type�� (genre)���·� ����Ǿ� �ִ�. ex) TCON = (Electronic)...
		if (content.empty()) return L""; //content type�� ������ �� ���ڿ� ��ȯ

		int count = 0;
		int index1 = -1;
		int index2 = -1;
		for (int i = 0; i < content.size() && count < 2; i++)
		{
			if (content.at(i) == '(' && index1 < 0)
			{
				index1 = i; //ó�� ���� (�� ��ġ
				count++;
			}

			if (content.at(i) == ')' && index2 < 0)
			{
				index2 = i; //ó�� ���� )�� ��ġ
				count++;
			}
		}

		if (count < 2 || index1 < 0 || index2 < 0) return L""; //content���� ��ȿ�� genre�� ã�� ���� ��� �� ���ڿ� ��ȯ
		
		int genreNum = 0;

		for (int j = index1 + 1; j < index2; j++) genreNum = genreNum * 10 + content.at(j) - 48; //getYearAsIntenger�� ����������, �帣 ��ȣ ����

		if (genreNum == -1 || genreNum > ID3GenreTable::MAXGENRE) return L""; //�ش��ϴ� �帣�� ���� ��� �� ���ڿ� ��ȯ

		return ID3GenreTable::genreTable[genreNum];
	}

	if (genre == -1 || genre > ID3GenreTable::MAXGENRE) return L""; //�ش��ϴ� �帣�� ���� ��� �� ���ڿ� ��ȯ

	return ID3GenreTable::genreTable[genre]; //lookup table���� �ش� �帣���� ã�Ƽ� ��ȯ
}

int ID3Reader::decodeSync(char* byte)
{
	//pointer�� pass by value�� ����Ǿ� �� variable�̹Ƿ� ���� pointer�� �״�� ���´�.
	//id3 tag���� sync�Ǿ� �ִٴ� ����, ��� ����Ʈ�� ���� ���� ū ��Ʈ (7th bit)�� 0���� ���ڵ��Ǿ�, �̸� �����ϸ� �����Ͱ� �� 28bit�� ǥ���Ǿ� �ִٴ� ���� �ǹ��Ѵ�.
	//����, ���� byte->int�� ��ȯ�Ѵٸ� �� �ڸ����� 8, 16, 24bit��ŭ shift�Ǿ�� ������, 7th bit�� �����ؾ� �ϹǷ� 7, 14, 21bit��ŭ shift�Ǿ�� �Ѵ�.
	//�� ����Ʈ�� 7th bit�� ������ 0�̹Ƿ�, �ִ밪�� 2^7-1 = 127 = 0x7F�̴�.

	int n = int(((unsigned char)(*(byte++) & 0x7F) << 21)); //�� ����Ʈ�� 0x7F�� ����ŷ�� ��, �� �ڸ��� ��ŭ left shift�Ѵ�.
	n |= (unsigned char)(*(byte++) & 0x7F) << 14; //�׸��� ���� ����Ʈ�� or�Ͽ� decode�� int ���� ���Ѵ�.
	n |= (unsigned char)(*(byte++) & 0x7F) << 7;
	n |= (unsigned char)(*byte & 0x7F);
	return n;
}

wstring ID3Reader::getFrame(const string& frameName)
{
	ID3Frame frame(frameName);
	if (frames.Get(frame)) return frame.getContent(); //frameName�� �ش��ϴ� ���� �����ϸ� �� ���� ��ȯ
	return L""; //���ٸ� �� ���ڿ� ��ȯ
}