
#include "stdafx.h"

#include "SQue.h"

SQue::SQue(int size) : m_write(0), m_read(0)
{
	m_buffer = new char[size];
	m_Totalsize = size;

}



SQue::~SQue()
{
	memset(m_buffer, NULL, m_Totalsize);
	//파괴자 : 동적할당 해제
	delete[]m_buffer;
}



int SQue::Put(char* pData, int iSize)
{
	//사이즈 저장 // 왜? : Put,Get, Peek한 사이즈와 넣은 사이즈가 같으면 0이되는 걸 확인하려고
	int sizen = iSize;

	int iReturntemp = 0; //리턴할 변수를 하나 둔다. 

	int putemtpysize = (m_Totalsize + m_read - EMPTYSIZE - m_write) % m_Totalsize;
	//1. 빈공간이 넣을 사이즈보다 작은 경우

	//들어온 사이즈가 0이거나 남은 사이즈가 0일때 
	if ((sizen <= 0) || (putemtpysize == 0)) return 0;
	//넣은 사이즈 구해서 넣기

	int psize = Putsize(sizen);

	int leftsize = m_Totalsize - m_write;

	if (psize  > leftsize)
	{
		memcpy(m_buffer + m_write, pData, leftsize);
		memcpy(m_buffer, pData + leftsize, psize - leftsize);
		m_write = (psize - leftsize) % m_Totalsize;
	}
	//데이터를 사이즈 만큼 m_buffer에 넣음
	else
	{
		memcpy(&m_buffer[m_write], pData, psize);
		m_write = (m_write + psize) % m_Totalsize;
	}
	sizen = sizen - psize;


	//Put한 사이즈와 넣은 사이즈가 같으면 sizen 0
	//정상적이면 iSize 리턴
	iReturntemp = iSize - sizen;

	return iReturntemp;
}


int SQue::Get(char* pData, int iSize)
{
	int sizen = iSize;
	int iReturntemp = 0;
	int Getenablesize = (m_Totalsize + m_write - m_read) % m_Totalsize;

	//넣을 사이즈가 0이거나 남은 사이즈가 0일때 
	if ((sizen <= 0) || (Getenablesize == 0)) return 0;
	//넣은 사이즈 구해서 넣기

	int gsize = GetPeeksize(sizen);

	int LSize = m_Totalsize - m_read; //총사용용량에서 빼는 양을 뺀다.
	//넘어갔을떄
	if (gsize > LSize)
	{
		memcpy(pData, m_buffer + m_read, LSize);
		memcpy(pData + LSize, m_buffer, gsize - LSize);
		m_read = (gsize - LSize) % m_Totalsize;
	}
	//데이터를 사이즈 만큼 pData에 넣음
	else
	{
		memcpy(pData, &m_buffer[m_read], gsize);
		//read를 구함
		m_read = (m_read + gsize) % m_Totalsize;
	}
	//Get한 사이즈와 넣은 사이즈가 같으면 0
	sizen = sizen - gsize;

	//Get한 사이즈와 넣은 사이즈가 같으면 sizen 0
	//정상적이면 iSize 리턴
	iReturntemp = iSize - sizen;

	return iReturntemp;
}


//CAysStreamDQ
//FlipBuffer쪽에서 버퍼가 비어져있는지 확인한다.
int SQue::Peek(char* pData, int iSize)
{
	int iReturntemp = 0;
	int sizen = iSize;

	int peekemsize = (m_Totalsize + m_write - m_read) % m_Totalsize;

	//넣을 사이즈가 0이거나 남은 사이즈가 0일때 
	if ((sizen == 0) || (peekemsize == 0)) return 0;
	//넣은 사이즈 구해서 넣기
	int iPeeksize = GetPeeksize(sizen);
	//데이터를 사이즈 만큼 pData에 넣음

	//넘어갔을떄
	int LSize = m_Totalsize - m_read; //총사용용량에서 빼는 양을 뺀다.
	//넘어갔을떄
	if (iPeeksize > LSize)
	{
		memcpy(pData, m_buffer + m_read, LSize);
		memcpy(pData + LSize, m_buffer, iPeeksize - LSize);
	}

	else
	{
		memcpy(pData, &m_buffer[m_read], iPeeksize);
		//Peek한 사이즈와 넣은 사이즈가 같으면 0
	}
	sizen = sizen - iPeeksize;

	//Peek한 사이즈와 넣은 사이즈가 같으면 sizen 0
	//정상적이면 iSize 리턴
	iReturntemp = iSize - sizen;

	return iReturntemp;
}




void SQue::Remove(int iSize)
{
	//리드만큼 이동
	int emptySize = min(iSize, (m_Totalsize + m_write - m_read) % m_Totalsize);

	m_read = (m_read + emptySize) % m_Totalsize;

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


void SQue::EmptyBuffer()
{
	m_read = 0;
	m_write = 0;
}


int SQue::GetEmptySize()
{
	return (m_Totalsize - m_write - EMPTYSIZE + m_read) % m_Totalsize;

}