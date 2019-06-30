
#ifndef __SQUE_H__
#define __SQUE_H__
#define TOTALSIZE 128 //Test하려고 작게함
#define EMPTYSIZE 4
#define df_SIZE TOTALSIZE-EMPTYSIZE
//-------------------------------------------
//스트림 큐 변수
//1. char(또는 BYTE) Buffer[128]; OR 동적할당
//2. Read(프론트)
//3. Write(리어)
//-------------------------------------------

#define INPUT 0
#define OUTPUT 1
#define EMPTYSIZE 4
class SQue
{
private:
	char *m_buffer; //버퍼사이즈
	int m_write; //리어
	int m_read; //프론트
	int m_Totalsize; //총사이즈

	CRITICAL_SECTION m_cs;


public:
	SQue(int size = TOTALSIZE);
	virtual ~SQue();
	//문제점 1 큐내부에 락과 언락을 넣음
	//-Recv, Send 용 큐의 동기화 객체는 해당 클래스 안에 넣는게 좋음
	//이거 이번달 초반에 과제였었음.
	void Lock();
	void UnLock();

	int Put(char* pData, int iSize);
	int Get(char* pData, int iSize);
	int Peek(char* pData, int iSize);


	int Putsize(int iSize);
	int GetPeeksize(int iSize);
	void Remove(int iSize);
	void Remove();
	int NotBrokenGetSize();
	int GetUseSize();
	//int GetEmptySize();
	//int GetGetSize();
	int PutPutsize();
	char* GetBuff();
	char* GetReadBuff();
	void deletesize(int iSize);
	int SendNotBrokenGetSize();
	char* GetWBuff();
	void SRemove(int iSize);
	void Removew(int iSize);
	int NNotBrokenGetSize();
};





#endif
