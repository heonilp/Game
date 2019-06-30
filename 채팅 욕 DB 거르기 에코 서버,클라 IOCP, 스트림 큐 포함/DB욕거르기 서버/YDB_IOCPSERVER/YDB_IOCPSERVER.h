
#ifndef __YDB_IOCPSERVER_H__
#define __YDB_IOCPSERVER_H__

#define BUFSIZE 1024

struct st_CLIENTINFO
{
	SOCKET sock;
	OVERLAPPED RecvOverlapped;
	OVERLAPPED SendOverlapped;
	LONG IOcount;
	bool Sendflag;
	SQue SendQue;
	SQue RecvQue;
	bool Login;
};

//오류 출력함수
void err_quit(WCHAR *msg);
void err_display(WCHAR *msg);

//초기화 함수
void InfoInit(void);
void NetWorkConnect(void);

//스레드
DWORD _stdcall SendThread(LPVOID arg);
DWORD _stdcall WorkerThread(LPVOID arg);
DWORD _stdcall AcceptThread(LPVOID arg);


void RecvProc(st_CLIENTINFO *pclient, DWORD dbTransferred);
void ProcSend(st_CLIENTINFO *pclient, DWORD dbTransferred);


st_CLIENTINFO* FindClient(SOCKET sock);
void Release(void);
void YDisconnet(st_CLIENTINFO *pclient);
char* YDB_change(char* str, char* source, char* newstr);

#endif //__YDB_IOCPSERVER_H__