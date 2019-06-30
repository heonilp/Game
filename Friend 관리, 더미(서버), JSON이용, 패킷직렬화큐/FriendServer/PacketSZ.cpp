// PacketSZ.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "PacketSZ.h"


 PacketSZ::PacketSZ(int iSize)
{

	m_write = 0;
	m_read = 0;

	m_buffer = new char[iSize];
	m_bufferSize = iSize;

}


//깊은 복사
PacketSZ::PacketSZ(const PacketSZ& deepCopy)
{
	m_read = deepCopy.m_read;
	m_write = deepCopy.m_write;
	m_bufferSize = deepCopy.m_bufferSize;

	m_buffer = new char[m_bufferSize];
	memcpy(m_buffer, deepCopy.m_buffer, m_bufferSize);
}


PacketSZ* PacketSZ::operator=(const PacketSZ& a)
{
	delete[]m_buffer;

	m_read = a.m_read;
	m_write = a.m_write;
	m_bufferSize = a.m_bufferSize;

	m_buffer = new char[m_bufferSize];
	memcpy(m_buffer, a.m_buffer, m_bufferSize);

	return this;
}


PacketSZ::~PacketSZ()
{
	delete[]m_buffer;

}
//--------------------------------------------------------	

//Put

//정수
int	PacketSZ::operator<<(short shData)
{
	int size = min(emptyck(), sizeof(short));

	
	if (sizeof(short) <= size)
	{
		*(short*)(m_buffer + m_write) = shData;
		m_write = m_write + sizeof(short);
		return sizeof(short);
	}

	return 0;
}


int	PacketSZ::operator<<(int iData)
{

	int size = min(emptyck(), sizeof(int));

	
	if (sizeof(int) <= size)
	{
		*(int*)(m_buffer + m_write) = iData;
		m_write = m_write + sizeof(int);
		return sizeof(int);
	}

	return 0;
}

int	PacketSZ::operator<<(__int64 iData)
{

	int size = min(emptyck(), sizeof(__int64));

	if (sizeof(__int64) <= size)
	{
		*(__int64*)(m_buffer + m_write) = iData;
		m_write = m_write + sizeof(__int64);
		return sizeof(__int64);
	}

	return 0;
}


int	PacketSZ::operator<<(DWORD iData)
{

	int size = min(emptyck(), sizeof(DWORD));

	if (sizeof(DWORD) <= size)
	{
		*(DWORD*)(m_buffer + m_write) = iData;
		m_write = m_write + sizeof(DWORD);
		return sizeof(DWORD);
	}

	return 0;
}



//버퍼
int	PacketSZ::Put(char *pChData, int iSize)
{
	int size = min(emptyck(), iSize);

	memcpy(m_buffer + m_write, pChData, size);

	m_write = m_write + size;

	return size;
}



//문자
int	PacketSZ::operator << (char *pChData)
{
	int size = lensize(pChData);

	if (size != 0)
	{
		memcpy(m_buffer + m_write, pChData, size);
		m_write = m_write + size;

		return size;
	}

	return 0;
}


int	PacketSZ::operator << (BYTE bData)
{
	int size = sizeof(bData);

	if (size != 0)
	{
		memcpy(m_buffer + m_write, &bData, size);
		m_write = m_write + size;

		return size;
	}

	return 0;


}


int	PacketSZ::operator << (WCHAR *pData)
{
	int size = wcslen(pData) * 2;

	if (size != 0)
	{
		memcpy(m_buffer + m_write, (char*)pData, size);
		m_write = m_write + size;

		return size;
	}

	return 0;

}


//---------------------------------------------------------
//Get


//정수
int	PacketSZ::operator >> (short &shData)
{

	int iSize = min(m_bufferSize - emptyck(), sizeof(short));

	if (sizeof(short) <= iSize)
	{
		shData = *(short*)(m_buffer + m_read);
		m_read = m_read + sizeof(short);

		return sizeof(short);
	}

	return 0;

}

int	PacketSZ::operator>>(int &iData)
{
	int iSize = min(m_bufferSize - emptyck(), sizeof(int));

	if (sizeof(int) <= iSize)
	{
		iData = *(int*)(m_buffer + m_read);
		m_read = m_read + sizeof(int);

		return sizeof(int);
	}

	return 0;


}


int	PacketSZ::operator>>(__int64 &iData)
{
	int iSize = min(m_bufferSize - emptyck(), sizeof(__int64));

	if (sizeof(__int64) <= iSize)
	{
		iData = *(__int64*)(m_buffer + m_read);
		m_read = m_read + sizeof(__int64);

		return sizeof(__int64);
	}

	return 0;


}

int	PacketSZ::operator >> (DWORD iData)
{

	int iSize = min(m_bufferSize - emptyck(), sizeof(DWORD));

	if (sizeof(DWORD) <= iSize)
	{
		iData = *(DWORD*)(m_buffer + m_read);
		m_read = m_read + sizeof(DWORD);

		return sizeof(DWORD);
	}

	return 0;


}


//버퍼

//문자

int	PacketSZ::Get(char *pChData, int iSize)
{
	int size = min(m_bufferSize - emptyck(), iSize);


	memcpy(pChData, m_read + m_buffer, size);

	m_read = m_read + size;

	return size;



}
int	PacketSZ::Get(WCHAR *wchpData, int iSize)
{

	int iWcharSize = iSize * 2;

	int size = *((int*)(m_buffer + m_read));

	if (size == iWcharSize)
	{
		m_read += sizeof(int);
		memcpy(wchpData, m_buffer + m_read, size);
		m_read = m_read + size;
		return size;
	}

	return 0;
}

//---------------------------------------------------------
//빈공간 체크
int	PacketSZ::emptyck()
{
	int size = m_bufferSize - (m_write - m_read);

	return size;

}

//write 사이즈 체크
int	PacketSZ::sizeck()
{
	return m_write;
}

//문자열 길이 확인
int	PacketSZ::lensize(char *pChData)
{
	int iLen = strlen(pChData);

	int iSize = min(emptyck(), (int)sizeof(int) + iLen);


	if ((int)sizeof(int) + iLen <= iSize)
	{
		*(int*)(m_buffer + m_write) = iLen;
		m_write = m_write + sizeof(int);


		return iSize - sizeof(int);
	}

	return 0;

}
int PacketSZ::lensize(WCHAR *pData)
{

	int iLen = strlen((char*)(pData));

	int iSize = min(emptyck(), (int)sizeof(int) + iLen);


	if ((int)sizeof(int) + iLen <= iSize)
	{
		*(int*)(m_buffer + m_write) = iLen;
		m_write = m_write + sizeof(int);


		return iSize - sizeof(int);
	}

	return 0;

}
int	PacketSZ::size(WCHAR *pData)
{

	int len = wcslen(pData);
	int iSize = 0;


	for (int i = 0; i < len; ++i)
	{
		if (pData[i] > 255)
		{
			iSize += 2;
		}
		else
		{
			++iSize;
		}
	}

	return iSize;

}


char* PacketSZ::Totalbuffer()
{
	return m_buffer;
}

void PacketSZ::EmptyBuffer()
{
	m_read = 0;
	m_write = 0;
	memset(m_buffer, NULL, BUFFERSIZE);
}