
#ifndef __SQUE_H__
#define __SQUE_H__

#define EMPTYSIZE 4
//-------------------------------------------
//스트림 큐 변수
//1. char(또는 BYTE) Buffer[128]; OR 동적할당
//2. Read(프론트)
//3. Write(리어)
//-------------------------------------------

class SQue
{
private:
	char *m_buffer; //버퍼사이즈
	int m_write; //리어
	int m_read; //프론트
	int m_Totalsize; //총사이즈

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


