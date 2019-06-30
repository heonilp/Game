#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__


template<class DATA>
class CLinkedlist
{
protected:
	struct NODE
	{
		NODE *pPrev;
		NODE *pNext;
		DATA *pData;
	};

	NODE *m_pHead;
	NODE *m_pTail;
	NODE *m_pFreeNode;
	int m_iListCount;
	//UNIT m_iListCount;


public:
	CLinkedlist();
	~CLinkedlist();

	void InsertHead(DATA *pAddData);
	void InsertTail(DATA *pAddData);
	bool InsertNext(DATA *pAddData);
	bool InsertPrev(DATA *pAddData);

	bool MoveHead(void);
	bool MoveTail(void);
	bool MoveNext(void);
	bool MovePrev(void);
	bool MoveNum(int iNum);


	DATA *GetHead(void);
	DATA *GetTail(void);
	DATA *GetAt(void);
	DATA *GetNext(void);
	DATA *GetPrev(void);
	DATA *GetNum(int iNum);

	DATA *RemoveAt(void);
	DATA *RemoveNext(void);
	DATA *RemovePrev(void);

	bool RemoveAll(void);

	int GetConut(void);


protected:
	void Initial(void);
	void AfreshList(void);
};




template <class DATA>
inline CLinkedlist<DATA>::CLinkedlist()
{

	Initial();
	AfreshList();

}



template <class DATA>
inline CLinkedlist<DATA>::~CLinkedlist()
{
	RemoveAll();

	if (m_pHead != NULL)
		delete m_pHead;

	if (m_pTail != NULL)
		delete m_pTail;

	Initial();

}

//초기화 담당
template <class DATA>
inline void CLinkedlist<DATA>::Initial(void)
{
	m_pHead = NULL;
	m_pTail = NULL;
	m_pFreeNode = NULL;
	m_iListCount = 0;
}

template <class DATA>
inline void CLinkedlist<DATA>::AfreshList(void)
{
	if (m_iListCount != 0) RemoveAll();


	m_pHead = new NODE;
	m_pTail = new NODE;

	m_pHead->pPrev = m_pHead;
	m_pHead->pNext = m_pTail;
	m_pTail->pPrev = m_pHead;
	m_pTail->pNext = m_pTail;

	m_pFreeNode = m_pHead;
}

template <class DATA>
inline void CLinkedlist<DATA>::InsertHead(DATA *pAddData)
{
	NODE *pNode;

	pNode = new NODE;

	pNode->pData = pAddData;
	pNode->pPrev = m_pHead;
	pNode->pNext = m_pHead->pNext;
	m_pHead->pNext = pNode;
	pNode->pNext->pPrev = pNode;

	m_iListCount++;
}


template <class DATA>
inline void CLinkedlist<DATA>::InsertTail(DATA *pAddData)
{
	NODE *pNode;

	pNode = new NODE;

	pNode->pData = pAddData;
	pNode->pPrev = m_pTail->pPrev;
	pNode->pNext = m_pTail;
	pNode->pPrev->pNext = pNode;
	m_pTail->pPrev = pNode;

	m_iListCount++;
}

template <class DATA>
inline bool CLinkedlist<DATA>::InsertNext(DATA *pAddData)
{
	if (m_pFreeNode == NULL || m_pFreeNode == m_pTail)	return false;
	

	NODE *pNode;

	pNode = new NODE;

	pNode->pData = pAddData;
	pNode->pPrev = m_pFreeNode;
	pNode->pNext = m_pFreeNode->pNext;
	m_pFreeNode->pNext = pNode;
	pNode->pNext->pPrev = pNode;

	m_iListCount++;

	return true;

}

template <class DATA>
inline bool CLinkedlist<DATA>::InsertPrev(DATA *pAddData)
{
	if (m_pFreeNode == NULL || m_pFreeNode == m_pTail)
	{
		return false;
	}

	NODE *pNode;

	pNode = new NODE;

	pNode->pData = pAddData;
	pNode->pPrev = m_pFreeNode->pPrev;
	pNode->pNext = m_pFreeNode;
	pNode->pPrev->pNext = pNode;
	m_pFreeNode->pPrev = pNode;


	m_iListCount++;

	return true;

}


template <class DATA>
inline bool CLinkedlist<DATA>::MoveHead(void)
{
	m_pFreeNode = m_pHead;
	return true;
}


template <class DATA>
inline bool CLinkedlist<DATA>::MoveTail(void)
{
	m_pFreeNode = m_pTail;
	return true;
}



template <class DATA>
inline bool CLinkedlist<DATA>::MoveNext(void)
{
	if (m_pFreeNode == m_pTail || m_pFreeNode->pNext == m_pTail) return false;

	m_pFreeNode = m_pFreeNode->pNext;

	return true;
}

template <class DATA>
inline bool CLinkedlist<DATA>::MovePrev(void)
{
	if (m_pFreeNode == m_pHead || m_pFreeNode->pPrev == m_pHead) return false;
	
	m_pFreeNode = m_pFreeNode->pPrev;

	return true;
}

template <class DATA>
inline bool CLinkedlist<DATA>::MoveNum(int iNum)
{
	if (!MoveHead())
		return false;
	if (!MoveTail())
		return false;

	for (int iCount = 0; iCount < iNum; iCount++)
	{
		if (!MoveNext())
			return false;
	}
	return true;
}



template <class DATA>
inline DATA *CLinkedlist < DATA >::GetHead(void)
{
	DATA *pData = NULL;
	MoveHead();
	pData = GetNext();
	if (pData == NULL) goto ABORT;

	return pData;

ABORT:
	return NULL;
}



template <class DATA>
inline DATA *CLinkedlist < DATA >::GetTail(void)
{
	DATA *pData = NULL;
	MoveTail();
	pData = GetPrev();
	if (pData == NULL) goto ABORT;

	return pData;

ABORT:
	return NULL;
}

template <class DATA>
inline DATA *CLinkedlist<DATA>::GetAt(void)
{
	if (m_pFreeNode == NULL || m_pFreeNode == m_pHead || m_pFreeNode == m_pTail)
		goto ABORT;

	return m_pFreeNode->pData;

ABORT:
	return NULL;
}


template <class DATA>
inline DATA *CLinkedlist<DATA>::GetNext(void)
{
	DATA *pData = NULL;

	if (!MoveNext()) goto ABORT;

	pData = GetAt();

	if (pData == NULL) goto ABORT;

	return pData;

ABORT:
	return NULL;
}


template <class DATA>
inline DATA *CLinkedlist<DATA>::GetPrev(void)
{
	DATA *pData = NULL;

	if (!MovePrev()) goto ABORT;

	pData = GetAt();

	if (pData == NULL) goto ABORT;

	return pData;

ABORT:
	return NULL;
}


template <class DATA>
inline DATA *CLinkedlist<DATA>::GetNum(int iNum)
{
	DATA *pData = NULL;
	if (!MoveNum(iNum)) goto ABORT;

	pData = GetAt();

	if (pData == NULL) goto ABORT;

	return pData;

ABORT:
	return NULL;
}

template <class DATA>
inline DATA *CLinkedlist<DATA>::RemoveAt(void)
{
	if (m_pFreeNode == NULL || m_pFreeNode == m_pHead || m_pFreeNode == m_pTail)
		return NULL;

	NODE *pNode = m_pFreeNode->pNext;
	DATA *pReturnNode = NULL;

	pNode->pPrev = m_pFreeNode->pPrev;
	m_pFreeNode->pPrev->pNext = pNode;

	pReturnNode = m_pFreeNode->pData;
	delete m_pFreeNode;

	m_pFreeNode = pNode;

	m_iListCount--;

	return pReturnNode;

}



template <class DATA>
inline DATA *CLinkedlist<DATA>::RemoveNext(void)
{
	DATA *pReturnNode = NULL;

	if (!MoveNext()) return NULL;
	pReturnNode = RemoveAt();

	return pReturnNode;
}



template <class DATA>
inline DATA *CLinkedlist<DATA>::RemovePrev(void)
{
	DATA *pReturnNode = NULL;

	if (!MovePrev()) return NULL;
	pReturnNode = RemoveAt();

	return pReturnNode;
}

template <class DATA>
inline bool CLinkedlist<DATA>::RemoveAll(void)
{
	int iCount = m_iListCount;
	if (iCount == 0) return false;

	MoveHead();
	MoveNext();

	while (iCount--)
	{
		RemoveAt();
	}

	MoveHead();

	return true;
}


template <class DATA>
inline int CLinkedlist<DATA>::GetConut(void)
{
	return m_iListCount;
}



#endif