// YDB_IOCPSERVER.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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


// 오류 출력 함수
void err_quit(WCHAR *msg)
{
	LPWSTR IpMsgBuf = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), IpMsgBuf, 0, NULL);
	MessageBox(NULL, IpMsgBuf, msg, MB_OK);
	LocalFree(IpMsgBuf);
	return;
}

// 오류 출력 함수
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

	//욕 텍스트 파일을 욕 DB 배열에 넣는 로직
	WCHAR *delimiter = L"\n";
	FILE *file = NULL;
	//유니코드 파일 넣어서 배열에 넣기
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


	wprintf(L"욕 DB 파일 로딩 중...");

	WCHAR *ptr=NULL;
	WCHAR *bufw = new WCHAR[size];

	wmemset(bufw,NULL,size);

	rewind(file);

	
	fread(bufw, size, 1, file);

	//file 닫음
	fclose(file);


	for (token = wcstok(bufw, delimiter); token != NULL; token = wcstok(NULL, delimiter))
	{
		//배열에다가 차례차례넣기
		YDB[g_offset] = token;
		//wprintf(L"%s\n", YDB[offset]);
		g_offset++;
	}

	wprintf(L"욕 DB 파일 로딩 완료\n");

	

	//delete[]bufw;
}



void NetWorkConnect(void)
{

	InfoInit();

	int retval = 0;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// 입출력 완료 포트 생성
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
		//종료 처리를 하기위해서 WaitForSingleObject을 씀
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
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 소켓과 입출력 완료 포트 연결
		HANDLE hResult = CreateIoCompletionPort((HANDLE)client_sock, g_hcp, (DWORD)client_sock, 0);
		//if (hResult == NULL) return -1;

		// 소켓 정보 구조체 할당
		st_CLIENTINFO *ptr = new st_CLIENTINFO;
		if (ptr == NULL)
		{
			printf("[오류] 메모리가 부족합니다!\n");
			break;
		}


		ZeroMemory(&(ptr->RecvOverlapped), sizeof(ptr->RecvOverlapped));
		ZeroMemory(&(ptr->SendOverlapped), sizeof(ptr->SendOverlapped));


		//동적할당 초기화를 한다.

		WSABUF wsabuf[2];
		ptr->sock = client_sock;
		ptr->Sendflag = false;
		ptr->IOcount = 0;
		ptr->Login = true;




		CLock();
		g_list.push_back(ptr);
		CUnlock();


		// 비동기 입출력 시작
		DWORD recvbytes = 0;
		DWORD flags = 0;


		//Recv등록시 ++


		//ptr->RecvQue.Lock();

		wsabuf[0].buf = ptr->RecvQue.GetWBuff();
		wsabuf[0].len = ptr->RecvQue.NotBrokenGetSize();  //여기서 큐사이즈로 잡음


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
				//에러처리
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

		//키를 소켓으로 넘겨 받았기 때문에 소켓으로 리스트를 돌아서 정보를 찾음
		ptr = FindClient(client_sock);


		if (retval == FALSE)
		{
			//여기서 다 wprintf로 바꿔줬음
			dwError = WSAGetLastError();
			wprintf(L"[종료 ]dwError == %d IOcount [%d]\n  ", dwError, ptr->IOcount);

			if (NULL != ptr)
			{

				if (ERROR_NETNAME_DELETED == dwError)
				{
					//
					//sevDisconnet(ptr);
					wprintf(L"ERROR_NETNAME_DELETED? %d \n", dwError);
					//continue로 끊으면 정상적으로 IOcount로 종료가 되지않고 
					//여기서 종료가 되어버림 ,밑으로 내려서 IOcount로 정상적인 종료
					//continue;
				}
				ptr->Login = false;
				//sevDisconnet(ptr);
				//continue;
			}

		}


		// 클라이언트 정보 얻기
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);

		//전부 0이나 NULL이면 워커스레드를 종료한다. //이부분 때문에 리턴되기 때문에 스레드가 종료되는것 ...
		if (cbTransferred == 0 && lpoverlapped == NULL && ptr == NULL)
		{
			break;
		}

		//전송량이 0인데 IOcount는 0보다 크면 --를 한다 그리고 출력
		if (ptr->IOcount > 0 && cbTransferred == 0)
		{
			InterlockedDecrement(&(ptr->IOcount));
			wprintf(L"IOcount :%d  \n", ptr->IOcount);
		}

		// 비동기 입출력 결과 확인
		if (cbTransferred == 0)
		{
			if (ptr->IOcount > 0)
			{
				continue;
				//해제 X
			}
			else if (ptr->IOcount <= 0)
			{
				//해제 O
				closesocket(ptr->sock);
				printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
				YDisconnet(ptr);
				//delete ptr;
				continue;
			}

		}

		else //여기서GetQueuedCompletionStatus로 받은 lpoverlapped가 같은지 체크하는 부분이다.
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
	// 윈속 종료
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
						//총버퍼 80

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
							//에러처리
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
	//동적할당
	char *buf = new char[BUFSIZE];

	WSABUF wsabuf[2];

	int iResult;
	//아디 //크기 

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


	shsize = buf[0]; //사이즈 앞에 2바이트
	
	memcpy(sizeckptr,buf + 2, shsize);


	

	//Recv 완료된 것 출력
	wprintf(L"[Recv]ID = %d SIZE : %d\n", pclient->sock, dbTransferred);
	//ProRecv 완료 되었으면 IOcount 감소
	InterlockedDecrement(&(pclient->IOcount));


	WCHAR wptr[BUFSIZE];
	wmemset(wptr, NULL, BUFSIZE);

	char cptr[BUFSIZE];


	

	char *sptr = NULL;
	//char->wchar변환해서 복사
	mbstowcs(wptr, (char*)sizeckptr, wcslen((WCHAR*)sizeckptr) * 2);


	//buf에 욕이 있는 지 확인
	for (int ioffset = 0; ioffset < g_offset; ioffset++)
	{
		//문자열 검색
		if (wcsstr(wptr, YDB[ioffset]) != 0)
		{
			wprintf(L"ID : %d Send 데이터 욕 감지\n", pclient->sock);
			
			memset(cptr, NULL, BUFSIZE);
			wcstombs((char*)sizeckptr, wptr, wcslen(wptr));
			wcstombs((char*)cptr, YDB[ioffset], wcslen(YDB[ioffset])*2);
			

			if (sptr == NULL)
			{
				sptr = YDB_change((char*)sizeckptr, cptr, "욕하지마셈");
			}
			else
			{
				sptr = YDB_change((char*)sptr, cptr, "욕하지마셈");
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

	//WSARecv 등록
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
			//에러처리
		}
	}

	//WSARecv 등록되기전에 증가

	//해제
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

	//스레드 선언

	DWORD ThreadId;
	g_hAccpetThread = CreateThread(NULL, 0, AcceptThread, g_hcp, 0, &ThreadId);

	g_hSendThread = CreateThread(NULL, 0, SendThread, g_hcp, 0, &ThreadId);

	//워커 스레드 2개 , g_hWorkerThread 배열
	g_hWorkerThread[0] = CreateThread(NULL, 0, WorkerThread, g_hcp, 0, &ThreadId);
	g_hWorkerThread[1] = CreateThread(NULL, 0, WorkerThread, g_hcp, 0, &ThreadId);


	//지난번에 했던 디버그 응용 , 키로 스레드 종료
	WCHAR ControlKey;

	while (1)
	{
		ControlKey = _getwch(); //블럭함수
		if (ControlKey == L'q' || ControlKey == L'Q')
		{
			//------------------------------------------------
			// 종료처리
			//------------------------------------------------
			SetEvent(g_hExitThreadEvent); //활성화를 하고 빠져나감
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
	//리스트 전체 동적 할당해제 
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

//바꿔주는 함수
char* YDB_change(char* str, char* source, char* newstr)
{
	//각 포인터 선언
	char* preStr = NULL;
	char* nowStr = NULL;
	char* result = NULL;
	char* Pret = NULL;


	//길이들 선언
	int ilenstr = 0;
	int ilensource = 0;
	int ilennewstr = 0;
	int ipos = 0;

	//현재 포인터와 과거포인터에 str, 받은 문자열 넣음
	preStr = str;
	nowStr = str;

	//각각 길이를 계산함
	ilenstr = strlen(str);
	ilensource = strlen(source);
	ilennewstr = strlen(newstr);

	//사이즈 만큼 할당해줌
	Pret = (char*)calloc(sizeof(char), BUFSIZE);
	if (Pret == NULL)
	{
		return NULL;
	}

	//결과값에 할당한 Pret을 포인터 값을 넣음
	result = Pret;

	//값이 널이 아닐때까지 while문을 돔
	while (*nowStr != NULL)
	{
		//이동한 것 돔
		for (ipos = 0; (*(nowStr + ipos) == source[ipos]) && ipos < ilensource; ipos++);
		
		//만약 위치값과 같으면
		if (ipos == ilensource)
		{

			//뺀만큼 result에 복사
			memcpy(result, preStr, nowStr - preStr);

			result = result+ (nowStr - preStr);
			//복사
			memcpy(result, newstr, ilennewstr);

			//포인터 만큼 이동한 만큼 
			result = result + ilennewstr;
			preStr = nowStr + ilensource;
		
			nowStr = nowStr+ ipos;
		}
		else
		{
			nowStr++;
		}

	}
	
	// nowStr - preStr 값을result으로 복사해줌 
	memcpy(result, preStr, nowStr - preStr);

   //복사한것을 재할당해서 리턴해줌
	Pret = (char*)realloc(Pret, strlen(Pret) + 1);

	return Pret;
}