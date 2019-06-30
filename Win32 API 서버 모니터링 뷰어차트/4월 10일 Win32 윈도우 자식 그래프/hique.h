#ifndef __CPPQUE_H__
#define __CPPQUE_H__

template <class T>
class CQue
{

public:
	T* queue;
	int write;
	int read;
	int size;

	CQue() : size(130), write(0), read(0)
	{
		queue = new T[size];
	}

	~CQue()
	{
		delete[] queue;
	}

	bool Put(T data);
	bool Get(T *point);
	int TailGet();
};


template <class T>
bool CQue<T>::Put(T data)
{
	if (((write + 1) % size) == read)
	{
		return false;
	}
	queue[write] = data;
	write = (write + 1) % size;
	return true;
}


template <class T>
bool CQue<T>::Get(T* point)
{
	if (write == read) return false;

	if (point != NULL)
	{
		*point = queue[read];
	}

	read = (read + 1) % size;
	return true;
}


template <class T>
int CQue<T>::TailGet()
{
	if (write == read) return false;

	T Data = queue[read];

	read = (read + 1) % size;

	return Data;
}


#endif