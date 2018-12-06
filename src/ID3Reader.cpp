#include "ID3Reader.h"
#include "GenreTable.h"
#include <atlstr.h> 

using namespace std;

ID3Reader::ID3Reader()
{
	if (m_inFile) m_inFile.close(); //만약 스트림이 열려있다면 닫는다.
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
	if (!strcmp(id.c_str(),"USLT")) return 1; //unsynchronized lyrics 처리
	if (!strcmp(id.c_str(), "SYLT")) return 2; //synchronized lyrics 처리
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

	if (filepath.size() < 4) return 0; //올바르지 않은 경로인 경우 0 반환

	if (wcscmp(filepath.substr(filepath.size()-4, 4).c_str(), L".mp3")) return 0; //mp3 파일이 아니면 0 반환

	m_inFile.open(filepath, ios::in | ios::binary);

	if (!m_inFile) return 0; //파일 열기에 실패하면 0 반환

	m_inFile.seekg(0, ios::end);
	int fileSize = m_inFile.tellg(); //파일 크기
	m_inFile.seekg(0, ios::beg);

	char tag[3] = { 0 };
	m_inFile.read(tag, 3);
	if (memcmp(tag, "ID3", 3)) //file에서 data를 읽어올 경우 strcmp가 제대로 작동하지 않는다. (이후 buffer 등에 문제가 있는 것으로 추정). 메모리 비교 함수 memcmp 사용.
	{
		//ID3v2가 아닌 경우
		m_inFile.seekg(-128, ios::end); //파일의 끝에서 128 byte 앞쪽으로 온다.
		m_inFile.read(tag, 3);
		if (memcmp(tag, "TAG", 3))
		{
			m_inFile.close();
			return 0; //ID3 v1도 아닌 경우, 태그를 읽지 못했으므로 0 반환
		}

		majorVersion = 1; //ID3v1

		m_inFile.read(title, 30); //타이틀
		m_inFile.read(artist, 30); //아티스트
		m_inFile.read(album, 30); //앨범
		m_inFile.read(year, 4); //연도
		m_inFile.read(comment, 28); //코멘트
		unsigned char zeroByte = 0;
		m_inFile.read((char*)&zeroByte, 1);
		if (!zeroByte) m_inFile.read(comment + 28, 2); //zero-byte가 0이 아닌 경우, ID3v1. 나머지를 모두 코멘트에 읽음.
		else m_inFile.read(&track, 1); //zero-byte가 0인 경우, ID3v1.1일 수 있음. (트랙 넘버). ID3v1라고 하더라도, zeroByte가 0이면 널 문자이므로 이 바이트는 코멘트가 될 수 없음.

		m_inFile.read(&genre, 1); //장르
	}
	else
	{
		majorVersion = 2; //ID3v2
		m_inFile.read(&minorVersion, 1); //하위 버전 읽기 
		m_inFile.read(&revision, 1); //revision 버전 읽기

		if (!(minorVersion == 3 || minorVersion == 4))
		{
			m_inFile.close();
			return 0; //2.3이나 2.4가 아닌 경우, 호환되지 않으므로 0 반환. (2.1과 2.2는 관련 structure 정보를 찾지 못함)
		}

		//헤더 정보를 읽어온다.

		char flag = 0; //mp3 정보에 대한 header flag
		m_inFile.read(&flag, 1); //flag 읽기. v2.3/2.4에서는 총 8비트 중 상위 3비트만 flag로 사용하고 있다.

		char syncSize[4] = { 0 };
		m_inFile.read(syncSize, 4); //헤더의 크기를 읽어온다. 크기는 sync되어 있다.
		tagSize = decodeSync(syncSize); //sync된 byte를 decode하여 int로 저장한다.

		bool flag_c = flag >>= 5 & 1; //상위 3번째 비트. experimental indicator. tag에 관해 실험적인 내용이 적용되었을 때 1이다.
		bool flag_b = flag >>= 1 & 1; //상위 2번째 비트. extended header. 확장된 헤더 정의가 있을 경우 1이다.
		bool flag_a = flag >>= 1; //상위 1번째 비트. unsynchronisation. 구버전 software와 호환성을 위해 unsync를 적용했다면 1이다.

		//여기서 헤더를 읽을 때 필요한 비트는 flag_b이다.

		if (flag_b) //extended header가 존재하면
		{
			m_inFile.read(syncSize, 4); //extended header의 크기를 읽어온다. 이 크기 역시 sync되어 있다.
			m_inFile.seekg(decodeSync(syncSize),ios::cur); //extended header에는 tag와 관련된 정보가 포함되지 않으므로 skip
		}

		if (fileSize < tagSize + 10) //헤더의 크기가 10바이트이다.
		{
			//만약 파일 크기가 읽어야할 태그 크기 + 헤더 크기보다 크면, 읽어올 수 없다.
			m_inFile.close();
			throw id3_error("Invalid tag size");
			return 0;
		}

		//태그 정보를 읽어온다.
		int offset = 0;
		while (offset < tagSize)
		{
			char ID[4] = { 0 }; //property를 구분하기 위한 frame ID.
			m_inFile.read(ID, 4); //frame ID를 읽어온다.

			if (!canRead(ID)) //읽어온 ID가 Reader에서 지원하지 않는다면
			{
				break; //종료한다.
			}
			
			char frameSize[4];
			m_inFile.read(frameSize, 4); //property의 크기를 읽어온다. sync되어 있다.

			int realSize;
			if (minorVersion == 4) realSize = decodeSync(frameSize); //2.4에서는 frameSize가 sync되어 있다.
			else realSize = (frameSize[0] << 24) + (frameSize[1] << 16) + (frameSize[2] << 8) + frameSize[3]; //아닌 경우는 decode하지 않음
			char* data = new char[realSize];

			m_inFile.seekg(2, ios::cur); //frame flag 2개는 skip
			//이 flag는 frame이 유효하지 않을 때 skip할지, 혹은 frame이 compressed 또는 encrypted인지, group information인지 등을 나타낸다. 
			//이 class에서는 이 flag에 따른 처리를 무시하고, tag를 읽어오는 간단한 코드만 작성할 것이다.
			//이와 더불어 실질적으로 이 flag가 set된 sample data를 찾지 못했고, 테스트할 수 없는 등 환경적 이유로 생략한다.

			m_inFile.read(data, realSize); //data에 읽어온다.

			char encoder = *data; //인코딩된 스트링 타입

			string id = ""; //key가 될 ID를 저장하는 string
			wstring frame = L""; //value가 될 frame data를 저장하는 wstring. 인코딩 타입이 달라질 수 있으므로 wstring에 저장한다.

			//char*에서 string으로 할당 연산자로 옮길 시 뒤에 추가로 index가 늘어나 문자열 인식에서 오류가 발생함.
			for (int i = 0; i < 4; i++) id += ID[i]; //ID의 4글자를 string에 옮긴다.

			int Case = getFrameCase(id);
			int ignore = 4;
			char* descriptor;

			if (Case >= 1)
			{
				//unsynchronized lyrics. 국가 코드가 3byte, descriptor가 \0\0가 나올 때까지 임의의 크기를 차지한다.
				if (Case == 2) //synchronized lyrics. unsynchronized보다 time stamp와 content 헤더가 2바이트 더 있다.
					ignore += 2;
				descriptor = data + ignore;
				while (memcmp(descriptor, "\0\0", 2))
				{
					descriptor++;
					ignore++; //content descriptor가 끝날때까지 메모리 주소를 건너뛴다.
				}
				ignore += 2; //descriptor 건너뜀

				if (encoder == 0)
				{
					for (int i = ignore; i < realSize; i++) frame += data[i];
				}
				else if (encoder == 1)
				{
					if (data[ignore] == -1 && data[ignore + 1] == -2) //little endian
						for (int i = ignore + 2; i < realSize; i += 2) frame += *(uint16_t*)(data + i); //인코딩 타입, BOM을 제외하고 3번째 문자부터 옮긴다.
					else if (data[ignore] == -2 && data[ignore + 1] == -1) // big endian
						for (int i = realSize - 2; i > ignore + 2; i -= 2) frame += *(uint16_t *)(data + i); //big endian일 경우 반대로 읽어야 하므로 마지막부터 3번째 문자까지 옮긴다.
				}
				else if (encoder == 3)
				{
					for (int i = ignore; i < realSize; i++) frame += *(uint8_t*)(data + i); //UTF8의 경우 BOM이 없거나 상관 없으므로 1번째 문자부터 옮긴다.
				}
			}
			else
			{
				if (encoder == 0)
				{
					//ISO-8859-1
					for (int i = 1; i < realSize; i++) frame += data[i]; //0번째 문자 (인코딩 타입)을 제외하고 나머지를 frame에 옮긴다.
				}
				else if (encoder == 1 || encoder == 2)
				{
					if (data[1] == -1 && data[2] == -2) //little endian
						for (int i = 3; i < realSize; i += 2) frame += *(uint16_t*)(data + i); //인코딩 타입, BOM을 제외하고 3번째 문자부터 옮긴다.
					else if (data[1] == -2 && data[2] == -1) // big endian
						for (int i = realSize - 2; i > 2; i -= 2) frame += *(uint16_t *)(data + i); //big endian일 경우 반대로 읽어야 하므로 마지막부터 3번째 문자까지 옮긴다.
				}
				else if (encoder == 3)
				{
					for (int i = 1; i < realSize; i += 2) frame += *(uint8_t*)(data + i); //UTF8의 경우 BOM이 없거나 상관 없으므로 1번째 문자부터 옮긴다.
				}
			}
			

			ID3Frame id3frame(id, frame);
			frames.Add(id3frame); //frame 추가
			delete data; //메모리 해제

			offset += realSize + 10; //frame + id size (= 4) + frame size (= 4) + flag size (= 2)
		}
	}

	m_inFile.close();
	return 1;
}

void ID3Reader::close()
{
	//어떤 이유로 인해 외부에서 수동적으로 file stream을 종료할 때를 위한 함수
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
	if (majorVersion == 2) return getFrame("TIT2"); //title에 해당하는 frame 이름이 TIT2

	wstring output; //v1인 경우, char array에서 wstring으로 변환하여 반환
	for (int i = 0; i < 30; i++) output += title[i];
	return output;
}

wstring ID3Reader::getArtist()
{
	if (majorVersion == 2) return getFrame("TPE1"); //lead artist에 해당하는 frame 이름이 TPE1

	wstring output; //v1인 경우, char array에서 wstring으로 변환하여 반환
	for (int i = 0; i < 30; i++) output += artist[i];
	return output;
}

wstring ID3Reader::getAlbum()
{
	if (majorVersion == 2) return getFrame("TALB"); //album에 해당하는 frame 이름이 TALB

	wstring output; //v1인 경우, char array에서 wstring으로 변환하여 반환
	for (int i = 0; i < 30; i++) output += album[i];
	return output;
}

wstring ID3Reader::getYear()
{
	if (majorVersion == 2)
	{
		if (minorVersion == 3) return getFrame("TYER"); //날짜를 나타내는 frame은 recording time, release time 등 많지만 여기선 year(TYER)를 사용하기로 함
		else if (minorVersion == 4)
		{
			//v2.4에서는 TYER 및 기존의 날짜 frame이 모두 deprecate됨.
			wstring recordTime = getFrame("TDRC"); //TYER 대신 대체되어 새로 생긴 frame (Record Time)
			if (recordTime.empty()) return getFrame("TDRL"); //TDRC가 없다면 TDRL 반환. (Release Time)
			return recordTime;
		}
	}
	wstring output; //v1인 경우, char array에서 wstring으로 변환하여 반환
	for (int i = 0; i < 4; i++) output += year[i];
	return output;
}

int ID3Reader::getYearAsInteger()
{
	int n = 0;

	if (majorVersion == 1)
	{
		char* temp = year; //각 char를 얻기 위한 임시 포인터 (해제 시 dangling pointer 발생)

		if (*temp == 0) return 0; //year 정보가 없으면 0 반환

		for (int i = 0; i < 4; i++) n = n * 10 + *(++temp) - 48;
		//0~9의 ascii code = 48~57. 이전 값 * 10을 하고, 각 자릿수의 숫자를 더한다.
		//ex) 2018 : 2 -> 2 * 10 + 0 -> 20 * 10 + 1 -> 201 * 10 + 8 = 2018
	}
	else
	{
		wstring y = getYear();
		if (y.empty()) return 0; //year 정보가 없으면 0 반환

		for (int i = 0; i < y.size(); i++) n = n * 10 + y.at(i) - 48;
	}

	return n;
}

wstring ID3Reader::getComment()
{
	if (majorVersion == 2) return getFrame("COMM"); //comment에 해당하는 frame 이름이 COMM

	wstring output; //v1인 경우, char array에서 wstring으로 변환하여 반환
	for (int i = 0; i < 30; i++) output += comment[i];
	return output;
}

int ID3Reader::getTrackNum()
{
	if (majorVersion == 2) return (int)getFrame("TRCK")[0]; //track number에 해당하는 frame 이름이 TRCK

	return track; //v1인 경우, char array에서 wstring으로 변환하여 반환
}

wstring ID3Reader::getGenre()
{
	if (majorVersion == 2)
	{
		wstring content = getFrame("TCON"); //v2에서는 content type에 (genre)형태로 저장되어 있다. ex) TCON = (Electronic)...
		if (content.empty()) return L""; //content type이 없으면 빈 문자열 반환

		int count = 0;
		int index1 = -1;
		int index2 = -1;
		for (int i = 0; i < content.size() && count < 2; i++)
		{
			if (content.at(i) == '(' && index1 < 0)
			{
				index1 = i; //처음 나온 (의 위치
				count++;
			}

			if (content.at(i) == ')' && index2 < 0)
			{
				index2 = i; //처음 나온 )의 위치
				count++;
			}
		}

		if (count < 2 || index1 < 0 || index2 < 0) return L""; //content에서 유효한 genre를 찾지 못한 경우 빈 문자열 반환
		
		int genreNum = 0;

		for (int j = index1 + 1; j < index2; j++) genreNum = genreNum * 10 + content.at(j) - 48; //getYearAsIntenger과 마찬가지로, 장르 번호 추출

		if (genreNum == -1 || genreNum > ID3GenreTable::MAXGENRE) return L""; //해당하는 장르가 없을 경우 빈 문자열 반환

		return ID3GenreTable::genreTable[genreNum];
	}

	if (genre == -1 || genre > ID3GenreTable::MAXGENRE) return L""; //해당하는 장르가 없을 경우 빈 문자열 반환

	return ID3GenreTable::genreTable[genre]; //lookup table에서 해당 장르명을 찾아서 반환
}

int ID3Reader::decodeSync(char* byte)
{
	//pointer는 pass by value로 복사되어 온 variable이므로 원래 pointer는 그대로 남는다.
	//id3 tag에서 sync되어 있다는 것은, 모든 바이트가 각각 가장 큰 비트 (7th bit)가 0으로 인코딩되어, 이를 무시하면 데이터가 총 28bit로 표현되어 있다는 것을 의미한다.
	//따라서, 원래 byte->int로 변환한다면 각 자리수가 8, 16, 24bit만큼 shift되어야 하지만, 7th bit를 무시해야 하므로 7, 14, 21bit만큼 shift되어야 한다.
	//각 바이트의 7th bit가 무조건 0이므로, 최대값은 2^7-1 = 127 = 0x7F이다.

	int n = int(((unsigned char)(*(byte++) & 0x7F) << 21)); //각 바이트를 0x7F와 마스킹한 후, 각 자리수 만큼 left shift한다.
	n |= (unsigned char)(*(byte++) & 0x7F) << 14; //그리고 계산된 바이트를 or하여 decode된 int 값을 구한다.
	n |= (unsigned char)(*(byte++) & 0x7F) << 7;
	n |= (unsigned char)(*byte & 0x7F);
	return n;
}

wstring ID3Reader::getFrame(const string& frameName)
{
	ID3Frame frame(frameName);
	if (frames.Get(frame)) return frame.getContent(); //frameName에 해당하는 값이 존재하면 그 값을 반환
	return L""; //없다면 빈 문자열 반환
}