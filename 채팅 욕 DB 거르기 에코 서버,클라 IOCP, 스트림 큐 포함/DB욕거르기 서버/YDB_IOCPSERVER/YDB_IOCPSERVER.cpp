// YDB_IOCPSERVER.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "SQue.h"
#include "YDB_IOCPSERVER.h"

using namespace std;
#pragma comment(lib, "ws2_32.lib") 
#define df_YDBSIZE 2500
list <st_CLIENTINFO*> g_list;

HANDLE g_hSendThread;
HANDLE g_hWorkerThread[2];
HANDLE g_hAccpetThread;
HANDLE g_hExitThreadEvent;


WCHAR *YDB[df_YDBSIZE];
int g_offset;
HANDLE g_hcp;
SOCKET g_listen_sock;
CRITICAL_SECTION	g_cs;
#define	CLock()		EnterCriticalSection(&g_cs)
#define CUnlock()	LeaveCriticalSection(&g_cs)


// ���� ��� �Լ�
void err_quit(WCHAR *msg)
{
	LPWSTR IpMsgBuf = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), IpMsgBuf, 0, NULL);
	MessageBox(NULL, IpMsgBuf, msg, MB_OK);
	LocalFree(IpMsgBuf);
	return;
}

// ���� ��� �Լ�
void err_display(WCHAR *msg)
{
	LPWSTR IpMsgBuf = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), IpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, IpMsgBuf);
	LocalFree(IpMsgBuf);
}



void InfoInit(void)
{
	
	WCHAR * token;
	setlocale(LC_ALL, "");
	
	InitializeCriticalSection(&g_cs);

	g_hExitThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//�� �ؽ�Ʈ ������ �� DB �迭�� �ִ� ����
	WCHAR *delimiter = L"\n";
	FILE *file = NULL;
	//�����ڵ� ���� �־ �迭�� �ֱ�
	errno_t err =_wfopen_s(&file, L"ydb.txt", L"r,ccs= UNICODE");
	if (err == 0)
	{
		wprintf(L"The file 'YDB' was opened\n");
	}
	else
	{
		wprintf(L"The file 'YDB' was not opened\n");
	}
	fseek(file, 0, SEEK_END);
	int size = ftell(file);


	wprintf(L"�� DB ���� �ε� ��...");

	WCHAR *ptr=NULL;
	WCHAR *bufw = new WCHAR[size];

	wmemset(bufw,NULL,size);

	rewind(file);

	
	fread(bufw, size, 1, file);

	//file ����
	fclose(file);


	for (token = wcstok(bufw, delimiter); token != NULL; token = wcstok(NULL, delimiter))
	{
		//�迭���ٰ� �������ʳֱ�
		YDB[g_offset] = token;
		//wprintf(L"%s\n", YDB[offset]);
		g_offset++;
	}

	wprintf(L"�� DB ���� �ε� �Ϸ�\n");

	

	//delete[]bufw;
}



void NetWorkConnect(void)
{

	InfoInit();

	int retval = 0;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// ����� �Ϸ� ��Ʈ ����
	g_hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (g_hcp == NULL) return;


	// socket()
	g_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_listen_sock == INVALID_SOCKET) err_quit(L"socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(g_listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit(L"bind()");


	// listen()
	retval = listen(g_listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit(L"listen()");
}



DWORD _stdcall AcceptThread(LPVOID arg)
{
	DWORD dwError;
	int retval = 0;

	while (1)
	{
		//���� ó���� �ϱ����ؼ� WaitForSingleObject�� ��
		dwError = WaitForSingleObject(g_hExitThreadEvent, 10);

		if (dwError != WAIT_TIMEOUT)
			break;

		// accept()
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		SOCKET client_sock = accept(g_listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			//err_display(L"accept()");
			continue;
		}
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// ���ϰ� ����� �Ϸ� ��Ʈ ����
		HANDLE hResult = CreateIoCompletionPort((HANDLE)client_sock, g_hcp, (DWORD)client_sock, 0);
		//if (hResult == NULL) return -1;

		// ���� ���� ����ü �Ҵ�
		st_CLIENTINFO *ptr = new st_CLIENTINFO;
		if (ptr == NULL)
		{
			printf("[����] �޸𸮰� �����մϴ�!\n");
			break;
		}


		ZeroMemory(&(ptr->RecvOverlapped), sizeof(ptr->RecvOverlapped));
		ZeroMemory(&(ptr->SendOverlapped), sizeof(ptr->SendOverlapped));


		//�����Ҵ� �ʱ�ȭ�� �Ѵ�.

		WSABUF wsabuf[2];
		ptr->sock = client_sock;
		ptr->Sendflag = false;
		ptr->IOcount = 0;
		ptr->Login = true;




		CLock();
		g_list.push_back(ptr);
		CUnlock();


		// �񵿱� ����� ����
		DWORD recvbytes = 0;
		DWORD flags = 0;


		//Recv��Ͻ� ++


		//ptr->RecvQue.Lock();

		wsabuf[0].buf = ptr->RecvQue.GetWBuff();
		wsabuf[0].len = ptr->RecvQue.NotBrokenGetSize();  //���⼭ ť������� ����


		if (ptr->RecvQue.NotBrokenGetSize() < ptr->RecvQue.PutPutsize())
		{

			wsabuf[1].len = ptr->RecvQue.PutPutsize() - ptr->RecvQue.NotBrokenGetSize();
			wsabuf[1].buf = ptr->RecvQue.GetBuff();

			InterlockedIncrement(&(ptr->IOcount));

		}

		else
		{
			wsabuf[1].buf = NULL;
			wsabuf[1].len = 0;

			InterlockedIncrement(&(ptr->IOcount));
		}

		//pclient->RecvQue.UnLock();

		retval = WSARecv(ptr->sock, wsabuf, 2, &recvbytes, &flags, &(ptr->RecvOverlapped), NULL);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				InterlockedDecrement(&(ptr->IOcount));
				err_display(L"WSARecv()");
				//����ó��
			}
		}

	

	}


	return 0;


}


DWORD _stdcall WorkerThread(LPVOID arg)
{
	DWORD dwError = 0;
	while (1)
	{
		int retval = 0;
		DWORD dwError = 0;
		DWORD cbTransferred = 0;
		SOCKET client_sock = 0;
		st_CLIENTINFO *ptr = NULL;

		LPOVERLAPPED lpoverlapped;


		retval = GetQueuedCompletionStatus(g_hcp, &cbTransferred, (LPDWORD)&client_sock, &lpoverlapped, INFINITE);

		//Ű�� �������� �Ѱ� �޾ұ� ������ �������� ����Ʈ�� ���Ƽ� ������ ã��
		ptr = FindClient(client_sock);


		if (retval == FALSE)
		{
			//���⼭ �� wprintf�� �ٲ�����
			dwError = WSAGetLastError();
			wprintf(L"[���� ]dwError == %d IOcount [%d]\n  ", dwError, ptr->IOcount);

			if (NULL != ptr)
			{

				if (ERROR_NETNAME_DELETED == dwError)
				{
					//
					//sevDisconnet(ptr);
					wprintf(L"ERROR_NETNAME_DELETED? %d \n", dwError);
					//continue�� ������ ���������� IOcount�� ���ᰡ �����ʰ� 
					//���⼭ ���ᰡ �Ǿ���� ,������ ������ IOcount�� �������� ����
					//continue;
				}
				ptr->Login = false;
				//sevDisconnet(ptr);
				//continue;
			}

		}


		// Ŭ���̾�Ʈ ���� ���
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);

		//���� 0�̳� NULL�̸� ��Ŀ�����带 �����Ѵ�. //�̺κ� ������ ���ϵǱ� ������ �����尡 ����Ǵ°� ...
		if (cbTransferred == 0 && lpoverlapped == NULL && ptr == NULL)
		{
			break;
		}

		//���۷��� 0�ε� IOcount�� 0���� ũ�� --�� �Ѵ� �׸��� ���
		if (ptr->IOcount > 0 && cbTransferred == 0)
		{
			InterlockedDecrement(&(ptr->IOcount));
			wprintf(L"IOcount :%d  \n", ptr->IOcount);
		}

		// �񵿱� ����� ��� Ȯ��
		if (cbTransferred == 0)
		{
			if (ptr->IOcount > 0)
			{
				continue;
				//���� X
			}
			else if (ptr->IOcount <= 0)
			{
				//���� O
				closesocket(ptr->sock);
				printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
				YDisconnet(ptr);
				//delete ptr;
				continue;
			}

		}

		else //���⼭GetQueuedCompletionStatus�� ���� lpoverlapped�� ������ üũ�ϴ� �κ��̴�.
		{
			if (&ptr->SendOverlapped == lpoverlapped)
			{
				ptr->Sendflag = false;
				ProcSend(ptr, cbTransferred);
			}
			else if (&ptr->RecvOverlapped == lpoverlapped)
			{
				RecvProc(ptr, cbTransferred);
			}
		}



	}

	PostQueuedCompletionStatus(g_hcp, 0, 0, NULL);
	// ���� ����
	WSACleanup();

	return 0;


}


DWORD _stdcall SendThread(LPVOID arg)
{

	
	DWORD dwError;
	int iResult = 0;
	auto itor = g_list.begin();
	st_CLIENTINFO *ptr = NULL;

	DWORD sendbytes = 0;
	DWORD sflags = 0;
	WSABUF Sendbuf[2];

	while (1)
	{


		dwError = WaitForSingleObject(g_hExitThreadEvent, 10);

		if (dwError != WAIT_TIMEOUT)
			return (DWORD)g_hcp;

		

		list<st_CLIENTINFO *>::iterator itor = g_list.begin();

		CLock();


		itor = g_list.begin();
		for (; itor != g_list.end(); itor++)
		{
			ptr = *itor;

			if (ptr->SendQue.NotBrokenGetSize() < df_SIZE)
			{
				if (ptr->Sendflag == false)
				{
					ptr->SendQue.Lock();

					Sendbuf[0].len = ptr->SendQue.SendNotBrokenGetSize();
					Sendbuf[0].buf = ptr->SendQue.GetReadBuff();


					if (ptr->SendQue.SendNotBrokenGetSize() < ptr->SendQue.GetUseSize())
					{
						Sendbuf[1].len = ptr->SendQue.GetUseSize() - ptr->SendQue.SendNotBrokenGetSize();   //64
						Sendbuf[1].buf = ptr->SendQue.GetBuff();


						InterlockedIncrement(&(ptr->IOcount));
						//�ѹ��� 80

					}

					else
					{
						InterlockedIncrement(&(ptr->IOcount));
						Sendbuf[1].len = 0;
						Sendbuf[1].buf = NULL;

					}

					iResult = WSASend(ptr->sock, Sendbuf, 2, &sendbytes,
						sflags, &(ptr->SendOverlapped), NULL);

					ptr->SendQue.UnLock();

					ptr->Sendflag = true;


					if (iResult == SOCKET_ERROR)
					{
						if (WSAGetLastError() != ERROR_IO_PENDING)
						{
							//����ó��
							InterlockedDecrement(&(ptr->IOcount));
							//err_display(L"WSASend()");
						}
					}
				}
			}

		}
		CUnlock();

	}

	WSACleanup();
	return 0;


}



void RecvProc(st_CLIENTINFO *pclient, DWORD dbTransferred)
{
	//�����Ҵ�
	char *buf = new char[BUFSIZE];

	WSABUF wsabuf[2];

	int iResult;
	//�Ƶ� //ũ�� 

	char sizeckptr[BUFSIZE];
	short shsize = 0;
	memset(sizeckptr, NULL, BUFSIZE);
	memset(buf, 0, BUFSIZE);

	pclient->RecvQue.Lock();
	
	wsabuf[0].buf = pclient->RecvQue.GetBuff();
	wsabuf[0].len = pclient->RecvQue.NotBrokenGetSize();

	pclient->RecvQue.Put(pclient->RecvQue.GetBuff(), dbTransferred);

	
	pclient->RecvQue.Get((char*)buf, dbTransferred+2);


	pclient->RecvQue.UnLock();


	shsize = buf[0]; //������ �տ� 2����Ʈ
	
	memcpy(sizeckptr,buf + 2, shsize);


	

	//Recv �Ϸ�� �� ���
	wprintf(L"[Recv]ID = %d SIZE : %d\n", pclient->sock, dbTransferred);
	//ProRecv �Ϸ� �Ǿ����� IOcount ����
	InterlockedDecrement(&(pclient->IOcount));


	WCHAR wptr[BUFSIZE];
	wmemset(wptr, NULL, BUFSIZE);

	char cptr[BUFSIZE];


	

	char *sptr = NULL;
	//char->wchar��ȯ�ؼ� ����
	mbstowcs(wptr, (char*)sizeckptr, wcslen((WCHAR*)sizeckptr) * 2);


	//buf�� ���� �ִ� �� Ȯ��
	for (int ioffset = 0; ioffset < g_offset; ioffset++)
	{
		//���ڿ� �˻�
		if (wcsstr(wptr, YDB[ioffset]) != 0)
		{
			wprintf(L"ID : %d Send ������ �� ����\n", pclient->sock);
			
			memset(cptr, NULL, BUFSIZE);
			wcstombs((char*)sizeckptr, wptr, wcslen(wptr));
			wcstombs((char*)cptr, YDB[ioffset], wcslen(YDB[ioffset])*2);
			

			if (sptr == NULL)
			{
				sptr = YDB_change((char*)sizeckptr, cptr, "����������");
			}
			else
			{
				sptr = YDB_change((char*)sptr, cptr, "����������");
			}
			
		}

	}


	if (sptr == NULL)
	{
		short shsenbufsize = strlen(sizeckptr);
		pclient->SendQue.Lock();
		pclient->SendQue.Put((char*)&shsenbufsize, sizeof(short));
		pclient->SendQue.Put((char*)sizeckptr, strlen(sizeckptr));
		pclient->SendQue.UnLock();
	}


	else if (sptr != NULL)
	{

		short shsenbufsize = strlen(sptr);
		
		pclient->SendQue.Lock();
		pclient->SendQue.Put((char*)&shsenbufsize, sizeof(short));
		pclient->SendQue.Put((char*)sptr, strlen((char*)sptr));
		pclient->SendQue.UnLock();
	}

	//WSARecv ���
	//ZeroMemory(&pclient->rbuf, sizeof(pclient->rbuf));

	DWORD rflags = 0;


	DWORD recvbytes = 0;


	if (pclient->RecvQue.NotBrokenGetSize() < pclient->RecvQue.PutPutsize())
	{

		wsabuf[1].len = pclient->RecvQue.PutPutsize() - pclient->RecvQue.NotBrokenGetSize();
		wsabuf[1].buf = pclient->RecvQue.GetWBuff();

		InterlockedIncrement(&(pclient->IOcount));

	}

	else
	{
		wsabuf[1].buf = NULL;
		wsabuf[1].len = 0;

		InterlockedIncrement(&(pclient->IOcount));
	}

	//pclient->RecvQue.UnLock();

	iResult = WSARecv(pclient->sock, wsabuf, 2, &recvbytes, &rflags, &(pclient->RecvOverlapped), NULL);

	if (iResult == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			InterlockedDecrement(&(pclient->IOcount));
			err_display(L"WSARecv()");
			//����ó��
		}
	}

	//WSARecv ��ϵǱ����� ����

	//����
	delete []buf;
	free(sptr);

}



void ProcSend(st_CLIENTINFO *pclient, DWORD dbTransferred)
{
	
	pclient->SendQue.Lock();
	//pclient->SendQue.Remove();
	pclient->SendQue.Remove(dbTransferred);
	pclient->SendQue.UnLock();

	wprintf(L"[Send] SIZE : %d\n", dbTransferred);

	InterlockedDecrement(&(pclient->IOcount));



}



int _tmain(int argc, _TCHAR* argv[])
{

	NetWorkConnect();

	//������ ����

	DWORD ThreadId;
	g_hAccpetThread = CreateThread(NULL, 0, AcceptThread, g_hcp, 0, &ThreadId);

	g_hSendThread = CreateThread(NULL, 0, SendThread, g_hcp, 0, &ThreadId);

	//��Ŀ ������ 2�� , g_hWorkerThread �迭
	g_hWorkerThread[0] = CreateThread(NULL, 0, WorkerThread, g_hcp, 0, &ThreadId);
	g_hWorkerThread[1] = CreateThread(NULL, 0, WorkerThread, g_hcp, 0, &ThreadId);


	//�������� �ߴ� ����� ���� , Ű�� ������ ����
	WCHAR ControlKey;

	while (1)
	{
		ControlKey = _getwch(); //���Լ�
		if (ControlKey == L'q' || ControlKey == L'Q')
		{
			//------------------------------------------------
			// ����ó��
			//------------------------------------------------
			SetEvent(g_hExitThreadEvent); //Ȱ��ȭ�� �ϰ� ��������
			break;
		}

	}
	Release();


	HANDLE hThread[] = { g_hSendThread, g_hAccpetThread, g_hWorkerThread[0], g_hWorkerThread[1] };
	WaitForMultipleObjects(4, hThread, FALSE, INFINITE);

	DWORD ExitCode;

	wprintf(L"\n\n--- THREAD CHECK LOG -----------------------------\n\n");


	GetExitCodeThread(g_hSendThread, &ExitCode);
	if (ExitCode != 0)
		wprintf(L"error - Send Thread not exit\n");

	GetExitCodeThread(g_hAccpetThread, &ExitCode);
	if (ExitCode != 0)
		wprintf(L"error - Accept Thread not exit\n");

	GetExitCodeThread(g_hWorkerThread[0], &ExitCode);
	if (ExitCode != 0)
		wprintf(L"error - Worker Thread1 not exit\n");

	GetExitCodeThread(g_hWorkerThread[1], &ExitCode);
	if (ExitCode != 0)
		wprintf(L"error - Worker Thread2 not exit\n");


	return 0;
}




st_CLIENTINFO* FindClient(SOCKET sock)
{
	
	st_CLIENTINFO *ptr = NULL;
	list<st_CLIENTINFO *>::iterator itor = g_list.begin();

	CLock();
	itor = g_list.begin();
	for (; itor != g_list.end(); itor++)
	{

		ptr = *itor;
		if (ptr->sock == sock)
		{
			CUnlock();
			return ptr;
		}
	}
	CUnlock();

	return NULL;
}




void Release(void)
{
	//����Ʈ ��ü ���� �Ҵ����� 
	CLock();
	list<st_CLIENTINFO *>::iterator Iter = g_list.begin();
	while (Iter != g_list.end())
	{

		delete *Iter;
		Iter = g_list.erase(Iter);
	}
	CUnlock();


	DeleteCriticalSection(&g_cs);

}


void YDisconnet(st_CLIENTINFO *pclient)
{

	
	st_CLIENTINFO *ptr = NULL;
	list<st_CLIENTINFO *>::iterator itor = g_list.begin();

	CLock();
	itor = g_list.begin();
	for (; itor != g_list.end();)
	{

		ptr = *itor;

		if (pclient == ptr)
		{
			itor = g_list.erase(itor);
			delete ptr;
		}
		else
		{
			itor++;
		}

	}
	CUnlock();
}

//�ٲ��ִ� �Լ�
char* YDB_change(char* str, char* source, char* newstr)
{
	//�� ������ ����
	char* preStr = NULL;
	char* nowStr = NULL;
	char* result = NULL;
	char* Pret = NULL;


	//���̵� ����
	int ilenstr = 0;
	int ilensource = 0;
	int ilennewstr = 0;
	int ipos = 0;

	//���� �����Ϳ� ���������Ϳ� str, ���� ���ڿ� ����
	preStr = str;
	nowStr = str;

	//���� ���̸� �����
	ilenstr = strlen(str);
	ilensource = strlen(source);
	ilennewstr = strlen(newstr);

	//������ ��ŭ �Ҵ�����
	Pret = (char*)calloc(sizeof(char), BUFSIZE);
	if (Pret == NULL)
	{
		return NULL;
	}

	//������� �Ҵ��� Pret�� ������ ���� ����
	result = Pret;

	//���� ���� �ƴҶ����� while���� ��
	while (*nowStr != NULL)
	{
		//�̵��� �� ��
		for (ipos = 0; (*(nowStr + ipos) == source[ipos]) && ipos < ilensource; ipos++);
		
		//���� ��ġ���� ������
		if (ipos == ilensource)
		{

			//����ŭ result�� ����
			memcpy(result, preStr, nowStr - preStr);

			result = result+ (nowStr - preStr);
			//����
			memcpy(result, newstr, ilennewstr);

			//������ ��ŭ �̵��� ��ŭ 
			result = result + ilennewstr;
			preStr = nowStr + ilensource;
		
			nowStr = nowStr+ ipos;
		}
		else
		{
			nowStr++;
		}

	}
	
	// nowStr - preStr ����result���� �������� 
	memcpy(result, preStr, nowStr - preStr);

   //�����Ѱ��� ���Ҵ��ؼ� ��������
	Pret = (char*)realloc(Pret, strlen(Pret) + 1);

	return Pret;
}