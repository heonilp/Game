#include "stdafx.h"
#include "SQue.h"


SQue::SQue(int size) : m_write(0), m_read(0)
{
	InitializeCriticalSection(&m_cs);

	m_buffer = new char[size];
	m_Totalsize = size;

	memset(m_buffer, NULL, m_Totalsize);

}



SQue::~SQue()
{
	DeleteCriticalSection(&m_cs);

	memset(m_buffer, NULL, m_Totalsize);
	//�ı��� : �����Ҵ� ����
	delete[]m_buffer;
}

void SQue::Lock()
{
	EnterCriticalSection(&m_cs);
}



void SQue::UnLock()
{
	LeaveCriticalSection(&m_cs);
}


int SQue::Put(char* pData, int iSize)
{

	//Lock();

	//������ ���� // ��? : Put,Get, Peek�� ������� ���� ����� ������ 0�̵Ǵ� �� Ȯ���Ϸ���
	int sizen = iSize;

	int iReturntemp = 0; //������ ������ �ϳ� �д�. 

	int putemtpysize = (m_Totalsize + m_read - EMPTYSIZE - m_write) % m_Totalsize;
	//1. ������� ���� ������� ���� ���

	//���� ����� 0�̰ų� ���� ����� 0�϶� 
	if ((sizen <= 0) || (putemtpysize == 0))
	{
		//UnLock();
		return 0;
	}
	//���� ������ ���ؼ� �ֱ�

	int psize = Putsize(sizen);

	int leftsize = m_Totalsize - m_write;

	if (psize  > leftsize)
	{
		memcpy(m_buffer + m_write, pData, leftsize);
		memcpy(m_buffer, pData + leftsize, psize - leftsize);
		m_write = (psize - leftsize) % m_Totalsize;
	}
	//�����͸� ������ ��ŭ m_buffer�� ����
	else
	{
		memcpy(&m_buffer[m_write], pData, psize);
		m_write = (m_write + psize) % m_Totalsize;
	}
	sizen = sizen - psize;


	//Put�� ������� ���� ����� ������ sizen 0
	//�������̸� iSize ����
	iReturntemp = iSize - sizen;

	//UnLock();
	return iReturntemp;
}


int SQue::Get(char* pData, int iSize)
{

	//Lock();
	int sizen = iSize;
	int iReturntemp = 0;
	int Getenablesize = (m_Totalsize + m_write - m_read) % m_Totalsize;

	//���� ����� 0�̰ų� ���� ����� 0�϶� 
	if ((sizen <= 0) || (Getenablesize == 0))
	{
		//UnLock();
		return 0;
	}
	//���� ������ ���ؼ� �ֱ�

	int gsize = GetPeeksize(sizen);

	int LSize = m_Totalsize - m_read; //�ѻ��뷮���� ���� ���� ����.
	//�Ѿ����
	if (gsize > LSize)
	{
		memcpy(pData, m_buffer + m_read, LSize);
		memcpy(pData + LSize, m_buffer, gsize - LSize);
		m_read = (gsize - LSize) % m_Totalsize;
	}
	//�����͸� ������ ��ŭ pData�� ����
	else
	{
		memcpy(pData, &m_buffer[m_read], gsize);
		//read�� ����
		m_read = (m_read + gsize) % m_Totalsize;
	}
	//Get�� ������� ���� ����� ������ 0
	sizen = sizen - gsize;

	//Get�� ������� ���� ����� ������ sizen 0
	//�������̸� iSize ����
	iReturntemp = iSize - sizen;


	//UnLock();
	return iReturntemp;
}


//CAysStreamDQ
//FlipBuffer�ʿ��� ���۰� ������ִ��� Ȯ���Ѵ�.
int SQue::Peek(char* pData, int iSize)
{

	//Lock();

	int iReturntemp = 0;
	int sizen = iSize;

	int peekemsize = (m_Totalsize + m_write - m_read) % m_Totalsize;

	//���� ����� 0�̰ų� ���� ����� 0�϶� 
	if ((sizen == 0) || (peekemsize == 0))
	{

		//UnLock();
		return 0;
	}
	//���� ������ ���ؼ� �ֱ�
	int iPeeksize = GetPeeksize(sizen);
	//�����͸� ������ ��ŭ pData�� ����

	//�Ѿ����
	int LSize = m_Totalsize - m_read; //�ѻ��뷮���� ���� ���� ����.
	//�Ѿ����
	if (iPeeksize > LSize)
	{
		memcpy(pData, m_buffer + m_read, LSize);
		memcpy(pData + LSize, m_buffer, iPeeksize - LSize);
	}

	else
	{
		memcpy(pData, &m_buffer[m_read], iPeeksize);
		//Peek�� ������� ���� ����� ������ 0
	}
	sizen = sizen - iPeeksize;

	//Peek�� ������� ���� ����� ������ sizen 0
	//�������̸� iSize ����
	iReturntemp = iSize - sizen;

	//UnLock();
	return iReturntemp;
}




void SQue::Remove(int iSize)
{
	//���常ŭ �̵�
	int emptySize = min(iSize, (m_Totalsize + m_write - m_read) % m_Totalsize);

	m_read = ((m_read + emptySize) % m_Totalsize);
	//80 60 40 20  0 �ݺ�
}


void SQue::Removew(int iSize)
{
	m_write = ((m_write - iSize) % m_Totalsize);
	//80 60 40 20  0 �ݺ�
}



void SQue::SRemove(int iSize)
{
	m_read = (m_read + iSize) % m_Totalsize;
}



int SQue::Putsize(int iSize)
{
	int putemtpysize = (m_Totalsize + m_read - EMPTYSIZE - m_write) % m_Totalsize;

	int iPutsize = min(iSize, putemtpysize);

	return iPutsize;
}



int SQue::GetPeeksize(int iSize)
{
	int Getpeekemtpysize = (m_Totalsize + m_write - m_read) % m_Totalsize;

	int iGetPeeksize = min(iSize, Getpeekemtpysize);

	return iGetPeeksize;
}


void SQue::Remove()
{
	m_read = 0;
	m_write = 0;
	//memset(m_buffer, NULL, m_Totalsize);
}


int SQue::NotBrokenGetSize()
{
	return (m_Totalsize - m_write - EMPTYSIZE + m_read) % m_Totalsize;

}


int SQue::SendNotBrokenGetSize()
{
	if (m_read <= m_write)
	{
		return m_write - m_read;
	}

	else
	{
		return m_Totalsize - m_read;

	}

}


int SQue::NNotBrokenGetSize()
{
	if (m_read <= m_write)
	{
		return m_write - m_read;
	}

	else
	{
		return (m_Totalsize - m_write - EMPTYSIZE + m_read) % m_Totalsize;

	}

}



int SQue::GetUseSize()
{
	int GetGetsize = (m_Totalsize + m_write - m_read) % m_Totalsize;

	return GetGetsize;
}


int SQue::PutPutsize()
{
	return m_write - m_read;
}

char* SQue::GetBuff()
{
	//���� ������ ��ŭ �����;��Ѵ�.
	return m_buffer;
	//return m_buffer;
}



char* SQue::GetReadBuff()
{
	//if (m_read >= m_Totalsize - df_SIZE)
	//{
	//	m_read = 0;
	//}
	//���� �� �ڿ� ���� 
	return m_buffer + m_read;
}


char* SQue::GetWBuff()
{

	return m_buffer + m_write;
}


void SQue::deletesize(int iSize)
{

	m_read = m_read - iSize;

}