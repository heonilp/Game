
#ifndef __SQUE_H__
#define __SQUE_H__

#define EMPTYSIZE 4
//-------------------------------------------
//��Ʈ�� ť ����
//1. char(�Ǵ� BYTE) Buffer[128]; OR �����Ҵ�
//2. Read(����Ʈ)
//3. Write(����)
//-------------------------------------------

class SQue
{
private:
	char *m_buffer; //���ۻ�����
	int m_write; //����
	int m_read; //����Ʈ
	int m_Totalsize; //�ѻ�����

public:
	SQue(int size=10000);
	virtual ~SQue();

	int Put(char* pData, int iSize);
	int Get(char* pData, int iSize);
	int Peek(char* pData, int iSize);
	int Putsize(int iSize);
	int GetPeeksize(int iSize);
	void Remove(int iSize);
	void EmptyBuffer();
	int GetEmptySize();
};




#endif


