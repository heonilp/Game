// FriendClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "FriendClient.h"
#include "../FriendServer/PacketSZ.h"
#include "../FriendServer/SQue.h"


SQue SendQ(10000);
SQue RecvQ(10000);
SOCKET g_sock;

list <Account*> g_clist;


void err_quit(WCHAR *msg)
{
	LPWSTR IpMsgBuf = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), IpMsgBuf, 0, NULL);
	MessageBox(NULL, IpMsgBuf, msg, MB_OK);
	LocalFree(IpMsgBuf);
	return;
}


void err_display(WCHAR *msg)
{
	LPWSTR IpMsgBuf = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), IpMsgBuf, 0, NULL);
	wprintf(L"[%s] %s", msg, IpMsgBuf);
	LocalFree(IpMsgBuf);
}


void NetworkInit()
{
	int reval = 0;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sock == INVALID_SOCKET) err_quit(L"socket()");
	SOCKADDR_IN serveraddr;

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &serveraddr.sin_addr);
	serveraddr.sin_port = htons(9000);

	if (connect(g_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
		err_display(L"connect");

	//로그인
	LoginIDput();
}


void Network()
{
	Account* ptr=NULL;
	int retval = 1;
	FD_SET rset;
//	FD_SET wset;

	timeval time;
	
	//auto iterPos = BaseList.begin
	time.tv_sec = 0;
	time.tv_usec = 0;


	while (retval != 0)
	{
		FD_ZERO(&rset);
		FD_SET(g_sock, &rset);

		// select
		retval = select(NULL, &rset, NULL, NULL, &time);
		if (retval == SOCKET_ERROR)
			err_display(L"select()");

		if (FD_ISSET(g_sock, &rset))
		{
			RecvProc();
		}
			
	}



}


void SendProc(int Type)
{

	switch (Type)
	{
	case Friend_Login_CS:
		LoginSend(Type);
		break;
	case Friend_Recom_CS:
		FriendSend(Type);
		break;
	case FriendList_CS:
		FriendSend(Type);
		break;
	case FriendList_Request_CS:
		FriendSend(Type);
		break;
	case FriendList_Reply_CS:
		FriendSend(Type);
		break;
	case FriendRemove_CS:
		FriendRemove(Type);
		break;
	case FriendRequest_CS:
		FriendRequestS(Type);
		break;
	case FriendCancel_CS:
		FriendCancelS(Type);
		break;
	case FriendDeny_CS:
		FriendDenyS(Type);
		break;
	case FriendAgree_CS:
		FriendAgreeS(Type);
		break;
	case Friend_OldLogin_CS:
		LoginSend(Type);
		break;

	default:
		break;
	}


}


void RecvProc()
{
	
	PacketSZ Packet;
	int reval = 0;
	int Type = 0;

	BYTE revbuf[BUFFERSIZE];
	BYTE buf[BUFFERSIZE];

	memset(revbuf, NULL, BUFFERSIZE);
	memset(buf, NULL, BUFFERSIZE);

	reval = recv(g_sock, (char*)revbuf, sizeof(revbuf), 0);

	if (reval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return;
		}
	}

	if (reval > 0)
	{
		reval = RecvQ.Put((char*)revbuf, sizeof(revbuf));
	}


	reval = RecvQ.Get((char*)buf, sizeof(buf));


	Packet.Put((char*)buf, sizeof(buf));


	Packet >> Type;

	switch (Type)
	{
	case Friend_Login_CSQ:
		LoginCreate(&Packet);
		break;
	case Friend_Recom_CS:
		RecomPrint(&Packet);
		break;
	case FriendList_CS:
		ListPrint(&Packet);
		break;
	case FriendList_Request_CS:
		OrderPrint(&Packet);
		break;
	case FriendList_Reply_CS:
		ReplyPrint(&Packet);
		break;
	case FriendRemove_CS:
		FriendRemoveR(&Packet);
		break;

	case FriendRequest_CS:
		FriendRequestR(&Packet);
		break;
	case FriendCancel_CS:
		FriendCancelR(&Packet);
		break;

	case FriendDeny_CS:
		FriendDenyR(&Packet);
		break;

	case FriendAgree_CS:
		FriendAgreeR(&Packet);
		break;
	
	case Friend_OldLogin_CS:
		FriendOldLogin(&Packet);
		break;

	default:
		break;
	}
}

void FriendOldLogin(PacketSZ *packet)
{

	WCHAR account[20];
	memset(account, NULL, 40);
	int icK = 0;
	__int64 Accountnum = 0;

	Account *ptr = new Account;

	*packet >> icK;

	switch (icK)
	{
	case OK:
		*packet >> Accountnum;
		packet->Get((char*)account, 20);
		ptr->sock = g_sock;
		ptr->AccountNo = Accountnum;
		memcpy(ptr->ID, account, 40);

		g_clist.push_back(ptr);

		//system("cls");
		printf("#===== 친구 관리 HEON =====#\n");
		wprintf_s(L"[ %s ]", ptr->ID);
		printf("님 아이디가 재로그인 되었습니다 \n");
		printf("1초 후 메뉴로 이동 합니다.");
		printf("\n");
		//여기서 함수
		return FriendMg();

	case ERR:
		//system("cls");
		printf("아이디가 오류 입니다.\n 종료합니다.\n");
		Sleep(50);
		DisConnect();
		break;
	default:
		break;
	}
}



void LoginIDput()
{
	int input = 0;
	system("cls");
	printf("\n");
	printf("#===== 친구 관리 HEON =====#\n");
	printf("1. 아이디생성 및 로그인 \n");
	printf("2. 기존 회원 로그인 \n");
	printf("3. 접속 종료 \n");
	printf("#==========================#\n");
	printf("입력 > ");
	scanf("%d", &input);
	fflush(stdin);

	switch (input)
	{
	case 1:
		SendProc(Friend_Login_CS);
		break;
	case 2:
		SendProc(Friend_OldLogin_CS);
		break;
	case 3:
		DisConnect();
		break;
	default:
		break;
	break;
	}

}


void LoginSend(int Type)
{
	BYTE sendBuf[BUFFERSIZE];
	WCHAR account[20];

	memset(sendBuf, NULL, BUFFERSIZE);
	memset(account, NULL, 40);

	system("cls");
	printf("#===== 친구 관리 HEON =====#\n");
	printf("같은 아이디가 있을 경우 종료>\n ");
	printf("아이디를 입력해주세요 : ");
	wscanf(L"%s", account);
	fflush(stdin);
	PacketSZ packet;

	packet << Type;

	//packet << account;

	packet.Put((char*)account, wcslen(account) * 2);

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());


}

void LoginCreate(PacketSZ *packet)
{
	WCHAR account[20];
	memset(account, NULL, 40);
	int icK = 0;
	__int64 Accountnum = 0;
	
	Account *ptr = new Account;
	
	*packet >> icK;

	switch (icK)
	{
	case OK:
		*packet >> Accountnum;
		packet->Get((char*)account, 20);
		ptr->sock = g_sock;
		ptr->AccountNo = Accountnum;
		memcpy(ptr->ID, account, 40);
		
		g_clist.push_back(ptr);

		//system("cls");
		printf("#===== 친구 관리 HEON =====#\n");
		wprintf_s(L"[ %s ]", ptr->ID);
		printf("님 아이디가 생성되었습니다\n");
		printf("1초 후 메뉴로 이동 합니다.");
		printf("\n");
		//여기서 함수
		return FriendMg();

	case OVERLAP:
		//system("cls");
		printf("아이디가 오류 입니다.\n 종료합니다.\n");
		DisConnect();
		break;
	default:
		break;
	}




}



void FSendData(BYTE *buf, int bufsize)
{
	int reval = 0;

	BYTE sendBuf[BUFFERSIZE];

	memset(sendBuf, NULL, BUFFERSIZE);

	SendQ.Put((char*)buf, bufsize);

	SendQ.Get((char*)sendBuf, bufsize);

	reval = send(g_sock, (char*)buf, bufsize, 0);

	if (reval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return;
	}
	
	
}


void FriendMg()
{
	Sleep(1000);

	int in = 0;

	//system("cls");

	printf("#===== 친구 관리 HEON =====#\n");
	printf("0. 추천 친구 목록  \n");
	printf("1. 친구 목록 \n");
	printf("2. 친구 받은 목록 \n");
	printf("3. 친구 요청 목록  \n");
	printf("4. 친구 관계 끊기\n");
	printf("5. 친구 요청 하기 \n");
	printf("6. 친구 요청 취소\n");
	printf("7. 친구 요청 거부 \n");
	printf("8. 친구 요청 수락 \n");
	printf("9. 종료 \n"); 
	printf("#==========================#\n");
	printf("[정보] F1를 누르면 메뉴로 빠져나와요.\n");
	printf("입력 >");
	
	scanf("%d", &in);
	fflush(stdin);

	switch (in)
	{
	case Friend_Recomlist:
		SendProc(Friend_Recom_CS);
		break;
	case Friend_list:
		SendProc(FriendList_CS);
		break;
	case Friend_listRequest:
		SendProc(FriendList_Request_CS);
		break;
	case Friend_Reply:
		SendProc(FriendList_Reply_CS);
		break;
	case Friend_Remove:
		SendProc(FriendRemove_CS);
		break;
	case FriendRequest:
		SendProc(FriendRequest_CS);
		break;
	case FriendCancel:
		SendProc(FriendCancel_CS);
		break;
	case FriendDeny:
		SendProc(FriendDeny_CS);
		break;
	case FriendAgree:
		SendProc(FriendAgree_CS);
		break;
	case MyDisconnet:
		DisConnect();
		break;
	default:
		break;
	}


}



void FriendSend(int Type)
{

	PacketSZ packet;

	packet << Type;

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());

}




void RecomPrint(PacketSZ *Packet)
{

	WCHAR account[20];

	__int64 Accountnum = 0;

	memset(account, NULL, 40);

	*Packet >> Accountnum;

	Packet->Get((char*)account, 20);

	wprintf_s(L"[ %s ]\n", account);
}



void OrderPrint(PacketSZ *Packet)
{

	WCHAR account[20];

	memset(account, NULL, 40);

	Packet->Get((char*)account, 20);

	wprintf_s(L"[ %s ]\n", account);
}



void ReplyPrint(PacketSZ *Packet)
{
	WCHAR account[20];

	memset(account, NULL, 40);

	Packet->Get((char*)account, 20);

	wprintf_s(L"[ %s ]\n", account);


}


void ListPrint(PacketSZ *Packet)
{
	//신청한 쪽이 이상하게 나옴 ->해결
	//요청 수락한 쪽은 제대로 나옴->해결
	WCHAR account[20];

	memset(account, NULL, 40);

	Packet->Get((char*)account, 20);

	wprintf(L"[ %s ]\n", account);


}



void FriendAgreeS(int Type)
{
	WCHAR account[20];

	memset(account, NULL, 40);

	BYTE sendBuf[BUFFERSIZE];

	memset(sendBuf, NULL, BUFFERSIZE);

	PacketSZ packet;

	packet << Type;


	printf("동의 할 아이디를 적어요.\n");
	printf("입력 >");
	wscanf(L"%s", account);
	fflush(stdin);

	packet.Put((char*)account, wcslen(account) * 2); //상대방 아이디 저장

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());


}

void FriendAgreeR(PacketSZ* Packet)
{
	//여기서 응답 추가
	int num;
	*Packet >> num;
	//여기서 응답 추가
	if (num == OK)
	{
		printf(" [친구] 가 되었습니다.\n");
	}
	else if (num == ERR)
	{
		printf(" [친구]가 되지 않습니다.\n");
	}

}

//친구 신청 취소  
void FriendCancelS(int Type)
{
	WCHAR account[20];

	memset(account, NULL, 40);

	BYTE sendBuf[BUFFERSIZE];

	memset(sendBuf, NULL, BUFFERSIZE);

	PacketSZ packet;

	packet << Type;


	printf("신청 취소 할 아이디를 적어요.\n");
	printf("입력 >");
	wscanf(L"%s", account);
	fflush(stdin);

	packet.Put((char*)account, wcslen(account) * 2); //상대방 아이디 저장

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());
}


//친구 신청 거부
void FriendDenyS(int Type)
{
	WCHAR account[20];

	memset(account, NULL, 40);

	BYTE sendBuf[BUFFERSIZE];

	memset(sendBuf, NULL, BUFFERSIZE);

	PacketSZ packet;

	packet << Type;


	printf("요청 거부 할 아이디를 적어요.\n");
	printf("입력 >");
	wscanf(L"%s", account);
	fflush(stdin);

	packet.Put((char*)account, wcslen(account) * 2); //상대방 아이디 저장

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());


}


//친구 삭제
void FriendRemove(int Type)
{
	WCHAR account[20];

	memset(account, NULL, 40);

	BYTE sendBuf[BUFFERSIZE];

	memset(sendBuf, NULL, BUFFERSIZE);

	PacketSZ packet;

	packet << Type;


	printf("친구 삭제 할 아이디를 적어요.\n");
	printf("입력 >");
	wscanf(L"%s", account);
	fflush(stdin);

	packet.Put((char*)account, wcslen(account) * 2); //상대방 아이디 저장

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());

}

void FriendCancelR(PacketSZ* Packet)
{
	//여기서 응답 추가
	int num;
	*Packet >> num;
	//여기서 응답 추가
	if (num == OK)
	{
		printf(" [친구 취소] 가 되었습니다.\n");
	}
	else if (num == ERR)
	{
		printf(" [친구 취소]가 되지 않습니다.\n");
	}
}
void FriendDenyR(PacketSZ* Packet)
{
	int num;
	*Packet >> num;
	//여기서 응답 추가
	if (num == OK)
		printf(" [친구 거부] 가 되었습니다.\n");

	else if (num == ERR)
		printf(" [친구 거부]가 되지 않습니다.\n");
}
void FriendRemoveR(PacketSZ* Packet)
{
	//여기서 응답 추가
	int num;
	*Packet >> num;
	//조건 추가
	if (num == OK)
		printf(" [친구 삭제] 가 되었습니다.\n");

	else if (num == ERR)
		printf(" [친구 삭제]가 되지 않습니다.\n");
}


void FriendRequestS(int Type)
{
	WCHAR account[20];

	memset(account, NULL, 40);

	BYTE sendBuf[BUFFERSIZE];

	memset(sendBuf, NULL, BUFFERSIZE);

	PacketSZ packet;

	packet << Type;


	printf("친구 신청할 아이디를 적어요.\n");
	printf("입력 >");
	wscanf(L"%s", account);
	fflush(stdin);


	packet.Put((char*)account, wcslen(account) * 2); //상대방 아이디 저장

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());

}


void FriendRequestR(PacketSZ *Packet)
{
	int num;
	*Packet >> num;
	//조건 추가
	if (num == OK)
		printf(" [친구 요청] 을 보냈습니다.\n");

	else if (num == ERR)
		printf(" [친구 요청]이 되지 않습니다.\n");

}

void KeyFone()
{
	if (GetAsyncKeyState(VK_F1))
	{
		FriendMg();
	}

}

void DisConnect()
{

	exit(1);

}