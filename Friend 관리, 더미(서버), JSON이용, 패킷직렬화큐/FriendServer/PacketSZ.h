#ifndef __PACKETSZ_H__
#define __PACKETSZ_H__

#define BUFFERSIZE 1024


class PacketSZ
{
private:
	char *m_buffer;
	int	m_write;//리어
	int	m_read;	//프론트
	int	m_bufferSize;//사이즈1024

public:
	PacketSZ(int iSize = BUFFERSIZE);
	PacketSZ(const PacketSZ& deepCopy);
	PacketSZ* operator=(const PacketSZ& a);
	~PacketSZ();
	//--------------------------------------------------------	

	//Put

	//정수
	int	operator<<(short shData);
	int	operator<<(int iData);
	int	operator<<(__int64 Data);
	int	operator<<(DWORD Data);;
	//버퍼
	int	Put(char *pChData, int iSize);

	//문자
	int	operator<<(char *pChData);
	int	operator<<(BYTE bData);
	int	operator<<(WCHAR *pData);


	//---------------------------------------------------------
	//Get


	//정수
	int	operator>>(short &shData);
	int	operator>>(int &iData);
	int	operator>>(__int64 &Data);
	int	operator>>(DWORD Data);
	//버퍼
	//문자

	int	Get(char *pChData, int iSize);
	int	Get(WCHAR *wchpData, int iSize);

	//---------------------------------------------------------
	//빈공간 체크
	int	emptyck();

	////write 사이즈 체크
	int	sizeck();

	//문자열 길이 확인
	int	lensize(char *pChData);
	int lensize(WCHAR *pData);

	int	size(WCHAR *pData);
	void EmptyBuffer();
	//모든 버퍼
	char* Totalbuffer();
};

#endif