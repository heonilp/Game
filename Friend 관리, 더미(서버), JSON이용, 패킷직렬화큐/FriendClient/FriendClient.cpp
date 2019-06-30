// FriendClient.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

	//�α���
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
		printf("#===== ģ�� ���� HEON =====#\n");
		wprintf_s(L"[ %s ]", ptr->ID);
		printf("�� ���̵� ��α��� �Ǿ����ϴ� \n");
		printf("1�� �� �޴��� �̵� �մϴ�.");
		printf("\n");
		//���⼭ �Լ�
		return FriendMg();

	case ERR:
		//system("cls");
		printf("���̵� ���� �Դϴ�.\n �����մϴ�.\n");
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
	printf("#===== ģ�� ���� HEON =====#\n");
	printf("1. ���̵���� �� �α��� \n");
	printf("2. ���� ȸ�� �α��� \n");
	printf("3. ���� ���� \n");
	printf("#==========================#\n");
	printf("�Է� > ");
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
	printf("#===== ģ�� ���� HEON =====#\n");
	printf("���� ���̵� ���� ��� ����>\n ");
	printf("���̵� �Է����ּ��� : ");
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
		printf("#===== ģ�� ���� HEON =====#\n");
		wprintf_s(L"[ %s ]", ptr->ID);
		printf("�� ���̵� �����Ǿ����ϴ�\n");
		printf("1�� �� �޴��� �̵� �մϴ�.");
		printf("\n");
		//���⼭ �Լ�
		return FriendMg();

	case OVERLAP:
		//system("cls");
		printf("���̵� ���� �Դϴ�.\n �����մϴ�.\n");
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

	printf("#===== ģ�� ���� HEON =====#\n");
	printf("0. ��õ ģ�� ���  \n");
	printf("1. ģ�� ��� \n");
	printf("2. ģ�� ���� ��� \n");
	printf("3. ģ�� ��û ���  \n");
	printf("4. ģ�� ���� ����\n");
	printf("5. ģ�� ��û �ϱ� \n");
	printf("6. ģ�� ��û ���\n");
	printf("7. ģ�� ��û �ź� \n");
	printf("8. ģ�� ��û ���� \n");
	printf("9. ���� \n"); 
	printf("#==========================#\n");
	printf("[����] F1�� ������ �޴��� �������Ϳ�.\n");
	printf("�Է� >");
	
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
	//��û�� ���� �̻��ϰ� ���� ->�ذ�
	//��û ������ ���� ����� ����->�ذ�
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


	printf("���� �� ���̵� �����.\n");
	printf("�Է� >");
	wscanf(L"%s", account);
	fflush(stdin);

	packet.Put((char*)account, wcslen(account) * 2); //���� ���̵� ����

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());


}

void FriendAgreeR(PacketSZ* Packet)
{
	//���⼭ ���� �߰�
	int num;
	*Packet >> num;
	//���⼭ ���� �߰�
	if (num == OK)
	{
		printf(" [ģ��] �� �Ǿ����ϴ�.\n");
	}
	else if (num == ERR)
	{
		printf(" [ģ��]�� ���� �ʽ��ϴ�.\n");
	}

}

//ģ�� ��û ���  
void FriendCancelS(int Type)
{
	WCHAR account[20];

	memset(account, NULL, 40);

	BYTE sendBuf[BUFFERSIZE];

	memset(sendBuf, NULL, BUFFERSIZE);

	PacketSZ packet;

	packet << Type;


	printf("��û ��� �� ���̵� �����.\n");
	printf("�Է� >");
	wscanf(L"%s", account);
	fflush(stdin);

	packet.Put((char*)account, wcslen(account) * 2); //���� ���̵� ����

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());
}


//ģ�� ��û �ź�
void FriendDenyS(int Type)
{
	WCHAR account[20];

	memset(account, NULL, 40);

	BYTE sendBuf[BUFFERSIZE];

	memset(sendBuf, NULL, BUFFERSIZE);

	PacketSZ packet;

	packet << Type;


	printf("��û �ź� �� ���̵� �����.\n");
	printf("�Է� >");
	wscanf(L"%s", account);
	fflush(stdin);

	packet.Put((char*)account, wcslen(account) * 2); //���� ���̵� ����

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());


}


//ģ�� ����
void FriendRemove(int Type)
{
	WCHAR account[20];

	memset(account, NULL, 40);

	BYTE sendBuf[BUFFERSIZE];

	memset(sendBuf, NULL, BUFFERSIZE);

	PacketSZ packet;

	packet << Type;


	printf("ģ�� ���� �� ���̵� �����.\n");
	printf("�Է� >");
	wscanf(L"%s", account);
	fflush(stdin);

	packet.Put((char*)account, wcslen(account) * 2); //���� ���̵� ����

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());

}

void FriendCancelR(PacketSZ* Packet)
{
	//���⼭ ���� �߰�
	int num;
	*Packet >> num;
	//���⼭ ���� �߰�
	if (num == OK)
	{
		printf(" [ģ�� ���] �� �Ǿ����ϴ�.\n");
	}
	else if (num == ERR)
	{
		printf(" [ģ�� ���]�� ���� �ʽ��ϴ�.\n");
	}
}
void FriendDenyR(PacketSZ* Packet)
{
	int num;
	*Packet >> num;
	//���⼭ ���� �߰�
	if (num == OK)
		printf(" [ģ�� �ź�] �� �Ǿ����ϴ�.\n");

	else if (num == ERR)
		printf(" [ģ�� �ź�]�� ���� �ʽ��ϴ�.\n");
}
void FriendRemoveR(PacketSZ* Packet)
{
	//���⼭ ���� �߰�
	int num;
	*Packet >> num;
	//���� �߰�
	if (num == OK)
		printf(" [ģ�� ����] �� �Ǿ����ϴ�.\n");

	else if (num == ERR)
		printf(" [ģ�� ����]�� ���� �ʽ��ϴ�.\n");
}


void FriendRequestS(int Type)
{
	WCHAR account[20];

	memset(account, NULL, 40);

	BYTE sendBuf[BUFFERSIZE];

	memset(sendBuf, NULL, BUFFERSIZE);

	PacketSZ packet;

	packet << Type;


	printf("ģ�� ��û�� ���̵� �����.\n");
	printf("�Է� >");
	wscanf(L"%s", account);
	fflush(stdin);


	packet.Put((char*)account, wcslen(account) * 2); //���� ���̵� ����

	FSendData((BYTE*)packet.Totalbuffer(), packet.sizeck());

}


void FriendRequestR(PacketSZ *Packet)
{
	int num;
	*Packet >> num;
	//���� �߰�
	if (num == OK)
		printf(" [ģ�� ��û] �� ���½��ϴ�.\n");

	else if (num == ERR)
		printf(" [ģ�� ��û]�� ���� �ʽ��ϴ�.\n");

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