#ifndef __PACKETSZ_H__
#define __PACKETSZ_H__

#define BUFFERSIZE 1024


class PacketSZ
{
private:
	char *m_buffer;
	int	m_write;//����
	int	m_read;	//����Ʈ
	int	m_bufferSize;//������1024

public:
	PacketSZ(int iSize = BUFFERSIZE);
	PacketSZ(const PacketSZ& deepCopy);
	PacketSZ* operator=(const PacketSZ& a);
	~PacketSZ();
	//--------------------------------------------------------	

	//Put

	//����
	int	operator<<(short shData);
	int	operator<<(int iData);
	int	operator<<(__int64 Data);
	int	operator<<(DWORD Data);;
	//����
	int	Put(char *pChData, int iSize);

	//����
	int	operator<<(char *pChData);
	int	operator<<(BYTE bData);
	int	operator<<(WCHAR *pData);


	//---------------------------------------------------------
	//Get


	//����
	int	operator>>(short &shData);
	int	operator>>(int &iData);
	int	operator>>(__int64 &Data);
	int	operator>>(DWORD Data);
	//����
	//����

	int	Get(char *pChData, int iSize);
	int	Get(WCHAR *wchpData, int iSize);

	//---------------------------------------------------------
	//����� üũ
	int	emptyck();

	////write ������ üũ
	int	sizeck();

	//���ڿ� ���� Ȯ��
	int	lensize(char *pChData);
	int lensize(WCHAR *pData);

	int	size(WCHAR *pData);
	void EmptyBuffer();
	//��� ����
	char* Totalbuffer();
};

#endif