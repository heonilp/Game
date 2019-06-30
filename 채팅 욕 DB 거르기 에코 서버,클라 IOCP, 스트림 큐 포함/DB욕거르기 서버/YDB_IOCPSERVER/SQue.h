
#ifndef __SQUE_H__
#define __SQUE_H__
#define TOTALSIZE 128 //Test�Ϸ��� �۰���
#define EMPTYSIZE 4
#define df_SIZE TOTALSIZE-EMPTYSIZE
//-------------------------------------------
//��Ʈ�� ť ����
//1. char(�Ǵ� BYTE) Buffer[128]; OR �����Ҵ�
//2. Read(����Ʈ)
//3. Write(����)
//-------------------------------------------

#define INPUT 0
#define OUTPUT 1
#define EMPTYSIZE 4
class SQue
{
private:
	char *m_buffer; //���ۻ�����
	int m_write; //����
	int m_read; //����Ʈ
	int m_Totalsize; //�ѻ�����

	CRITICAL_SECTION m_cs;


public:
	SQue(int size = TOTALSIZE);
	virtual ~SQue();
	//������ 1 ť���ο� ���� ����� ����
	//-Recv, Send �� ť�� ����ȭ ��ü�� �ش� Ŭ���� �ȿ� �ִ°� ����
	//�̰� �̹��� �ʹݿ� ����������.
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
