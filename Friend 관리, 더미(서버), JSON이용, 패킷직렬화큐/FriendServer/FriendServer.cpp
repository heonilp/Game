// FriendServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//JSON�� ������ OFF ���� / ������ ON�Ҷ� �ʱ��Ҷ� �ҷ��鸲
//������ F3�� ������ ���� ���� �� ����

#include "stdafx.h"
#include "PacketSZ.h"
#include "FriendServer.h"


#pragma comment(lib,"WS2_32.lib")

#define FD_SETSIZE 1000
__int64 errnum = 1000;
__int64 g_number = 1;
SOCKET g_sock;

list <Account*> g_clist;
list <Frined*> g_flist;
list <FriendRequest*> g_frlist;
list <Client*> g_list;


int g_inumber;
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
	InetPton(AF_INET, INADDR_ANY, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(9000);

	reval = bind(g_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (reval == SOCKET_ERROR) err_quit(L"bind()");

	reval = listen(g_sock, SOMAXCONN);
	if (reval == SOCKET_ERROR) err_quit(L"listen()");

	u_long on = TRUE;
	reval = ioctlsocket(g_sock, FIONBIO, &on);
	if (reval == SOCKET_ERROR) err_display(L"ioctlsocket()");

	JsonLoad();
	printf("[ F3 ] Ű�� �����ø� ������ ���� �� ����\n");

}


void Network()
{
	static int lnput=1;
	SOCKET client_sock;
	FD_SET rset;

	timeval time;
	int addrlen;
	auto iterPos1 = g_list.begin();
	SOCKADDR_IN clinetaddr;
	int val = 0;
	int	retval = 1;
	Client* ptr = NULL;
	//Account* ptr = NULL;
	time.tv_sec = 0;
	time.tv_usec = 0;

	while (retval != 0)
	{
		
		ServerOFF();
		FD_ZERO(&rset);

		FD_SET(g_sock, &rset);


		iterPos1 = g_list.begin();
		if (g_list.size() > 0)
		{
			for (; iterPos1 != g_list.end(); iterPos1++)
			{
				ptr = *iterPos1;
				FD_SET(ptr->sock, &rset);
			}
		}

		val = select(NULL, &rset, NULL, NULL, &time);

		//if (val == SOCKET_ERROR)
		//	err_display(L"select()");


		//��������
		if (FD_ISSET(g_sock, &rset))
		{
			addrlen = sizeof(clinetaddr);

			client_sock = accept(g_sock, (SOCKADDR*)&clinetaddr, &addrlen);
			if (client_sock == INVALID_SOCKET)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
					err_display(L"accept()");
			}
			else
			{
				printf("[%d], [TCP ����]Ŭ���̾�Ʈ ����: IP �ּ� = %s, ��Ʈ ��ȣ = %d\n", lnput,inet_ntoa(clinetaddr.sin_addr), ntohs(clinetaddr.sin_port));
				//iterPos = Clientlist.begin();
				lnput++;
				ClinetAdd(client_sock);
			}
		}


		auto iterPos = g_list.begin();
		//  Ŭ����� �ޱ�
		iterPos = g_list.begin();

		if (g_list.size() > 0)
		{
			for (; iterPos != g_list.end(); iterPos++)
			{
				ptr = *iterPos;

				if (FD_ISSET(ptr->sock, &rset))
				{
					//������
					RecvProc(ptr, ptr->sock);
					break;
				}

			}
		}

	}

}



void ClinetAdd(SOCKET sock)
{


	Client *ptr = new Client;

	ptr->sock = sock;
	g_list.push_back(ptr);

	return;
}






void SendProc(Client* aptr,SOCKET sock, int Type)
{

	PacketSZ Packet;

	Packet << Type;

	switch (Type)
	{
	case Friend_Login_SCR:
		FreindLogin(aptr, sock);
		break;
	case Friend_Recom_SC:
		FriendRecom(aptr, sock);
		break;
	case FriendList_SC:
		FriendList(aptr, sock);
		break;
	case FriendList_Request_SC:
		FriendReList(aptr, sock);
		break;
	case FriendList_Reply_SC:
		FriendRReList(aptr, sock);
		break;
	case FriendRemove_SC:
		FriendRemoveS(aptr, sock);;
		break;
	case FriendRequest_SC:
		FriendRequestS(aptr, sock);
		break;
	case FriendCancel_SC:
		FriendCancelS(aptr, sock);
		break;
	case FriendDeny_SC:
		FriendDenyS(aptr, sock);
		break;
	case FriendAgree_SC:
		FriendAgreeS(aptr, sock);
		break;
	case Friend_OldLogin_SC:
		FreindOldLogin(aptr, sock);
		break;
	case DUMY_TEST:
		DumyS(aptr, sock);
		break;
	}


}


void RecvProc(Client* aptr, SOCKET sock)
{
	PacketSZ Packet;
	int reval = 0;
	int Type = 0;

	BYTE revbuf[BUFFERSIZE];
	BYTE buf[BUFFERSIZE];

	memset(revbuf, NULL, BUFFERSIZE);
	memset(buf, NULL, BUFFERSIZE);

	reval = recv(sock, (char*)revbuf, sizeof(revbuf), 0);

	if (reval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			closesocket(sock);
			DisConnect(sock);
			return;
		}
	}

	
	reval = aptr->RecvQ.Put((char*)revbuf, sizeof(revbuf));


	reval = aptr->RecvQ.Get((char*)buf, sizeof(buf));


	Packet.Put((char*)buf, reval);


	Packet >> Type;


	switch (Type)
	{
	case Friend_Login_SC:
		FreindIDcpy(aptr, sock, &Packet);
		break;
	case Friend_Recom_SC:
		FriendRecomRecv(aptr, sock, &Packet);
		break;
	case FriendList_SC:
		FriendListRecv(aptr, sock, &Packet);
		break;
	case FriendList_Request_SC:
		FriendReListRecv(aptr, sock, &Packet);
		break;
	case FriendList_Reply_SC:
		FriendReListReply(aptr, sock, &Packet);
		break;
	case FriendRemove_SC:
		FriendRemoveR(aptr, sock, &Packet);
		break;

	case FriendRequest_SC:
		FriendRequestR(aptr, sock, &Packet);
		break;
	case FriendCancel_SC:
		FriendCancelR(aptr, sock, &Packet);
		break;

	case FriendDeny_SC:
		FriendDenyR(aptr, sock, &Packet);
		break;

	case FriendAgree_SC:
		FriendAgreeR(aptr, sock, &Packet);
		break;

	case Friend_OldLogin_SC:
		FreindOldIDcpy(aptr, sock, &Packet);
		break;
	case DUMY_TEST:
		DumyR(aptr, sock, &Packet);
		break;
	default:
		break;
	}
	
}

//�޴°�
void FreindOldIDcpy(Client* aptr, SOCKET sock, PacketSZ *Packet)
{
	WCHAR account[20];
	memset(account, NULL, 40);
	g_inumber = 0;//�ʱ�ȭ
	Packet->Get((char*)account, 20);
	
	auto iterPos = g_clist.begin();
	Account* ptr = NULL;


	iterPos = g_clist.begin();
	for (; iterPos != g_clist.end(); iterPos++)
	{
		ptr = *iterPos;

		if ((wcscmp(ptr->ID, account) == 0)) //�ߺ��г�
		{
			g_inumber = OK;
			g_number = ptr->AccountNo;
			aptr->AccountNo = ptr->AccountNo;
			SendProc(aptr, sock, Friend_OldLogin_SC);
			return;
		}
	}

	g_inumber = ERR;
	aptr->AccountNo = errnum;
	errnum++;
	SendProc(aptr, sock, Friend_OldLogin_SC);
	return;

}


//�����°�
void FreindOldLogin(Client* aptr, SOCKET sock)
{
	int Type = Friend_OldLogin_SC;
	PacketSZ sendPacket;

	auto iterPos = g_clist.begin();
	auto citerPos = g_list.begin();
	Client *ctr = NULL;
	int Anum = 0;

	Account* ptr = NULL;

	sendPacket << Type;

	switch (g_inumber)
	{
	case OK:

		iterPos = g_clist.begin();
		for (; iterPos != g_clist.end(); iterPos++)
		{
			ptr = *iterPos;
			if (g_number == ptr->AccountNo)
			{
				sendPacket << g_inumber;
				sendPacket << ptr->AccountNo;
				sendPacket.Put((char*)ptr->ID, 20);
				wprintf(L"[ %s ]", ptr->ID);
				printf("���� �������ϼ̽��ϴ�.\n");
				FSendData(aptr, sock, (BYTE*)sendPacket.Totalbuffer(), sendPacket.sizeck());
				return;
			}
		}
		break;

	case ERR:
		sendPacket << g_inumber;

		FSendData(aptr, sock, (BYTE*)sendPacket.Totalbuffer(), sendPacket.sizeck());
		break;
	case DUMYCK:
		sendPacket << g_inumber;

		FSendData(aptr, sock, (BYTE*)sendPacket.Totalbuffer(), sendPacket.sizeck());
		break;
	}


}



//�޴°�
void FreindIDcpy(Client* aptr, SOCKET sock, PacketSZ *Packet)
{

	WCHAR account[20];
	WCHAR dumyac[20];
	memset(account, NULL, 40);
	memset(dumyac, NULL, 40);
	g_inumber = 0;//�ʱ�ȭ
	Packet->Get((char*)account, 20);

	//���� �����̶��
	if (wcscmp(account, dumyac) == 0)
	{
		g_inumber = DUMYCK;
		//Account *pDumy = new Account;
		//g_clist.push_back(pDumy);
		
		return SendProc(aptr, sock, Friend_Login_SCR);
	}



	auto iterPos = g_clist.begin();
	auto iterPos2 = g_clist.begin();
	Account* ptr = NULL;

	
	//���� �ٽ�¥����...
	iterPos = g_clist.begin();
	for (; iterPos != g_clist.end(); iterPos++)
	{
		ptr = *iterPos;

		if (g_number == ptr->AccountNo) //�ߺ���ȣ
		{
			g_number++;
		}
	}



	//���� �ٽ�¥����...
	iterPos = g_clist.begin();
	for (; iterPos != g_clist.end(); iterPos++)
	{
		ptr = *iterPos;

		if ((wcscmp(ptr->ID, account) == 0) ) //�ߺ��г�
		{
			g_inumber = OVERLAP;
			aptr->AccountNo = errnum;
			errnum++;
			SendProc(aptr, sock, Friend_Login_SCR);
			return;
		}
	}


	g_inumber = OK;

	ptr = new Account;
	ptr->AccountNo = g_number;
	memset(ptr->ID, NULL, 40);
	memcpy(ptr->ID, account, wcslen(account) * 2);
	g_clist.push_back(ptr);

	SendProc(aptr, sock, Friend_Login_SCR);
	return;




}

//�����°�

void FreindLogin(Client* aptr, SOCKET sock)
{
	int Type = Friend_Login_SCR;
	PacketSZ sendPacket;

	auto iterPos = g_clist.begin();
	auto citerPos = g_list.begin();
	Client *ctr = NULL;
	int Anum = 0;

	Account* ptr = NULL;
	
	sendPacket << Type;

	switch (g_inumber)
	{
	case OK:

		iterPos=g_clist.begin();
		for (; iterPos != g_clist.end(); iterPos++)
		{
			ptr = *iterPos;
			if (g_number == ptr->AccountNo)
			{
				aptr->AccountNo = g_number;
				sendPacket << g_inumber;
				sendPacket << ptr->AccountNo;
				sendPacket.Put((char*)ptr->ID,20);
				g_number++;
				wprintf(L"[ %s ]", ptr->ID);
				printf("���� �����ϼ̽��ϴ�.\n");
				FSendData(aptr, sock, (BYTE*)sendPacket.Totalbuffer(), sendPacket.sizeck());
				return;
			}
		}
		break;

	case OVERLAP:
		sendPacket << g_inumber;

		FSendData(aptr, sock, (BYTE*)sendPacket.Totalbuffer(), sendPacket.sizeck());
		break;

	case DUMYCK:
		sendPacket << g_inumber;
		sendPacket << g_number;
		g_number++;
		FSendData(aptr, sock, (BYTE*)sendPacket.Totalbuffer(), sendPacket.sizeck());
		break;
	}
}


void FriendRecom(Client* aptr, SOCKET sock)
{
	PacketSZ Packet;

	int Type = Friend_Recom_SC;
	
	auto iterPos = g_clist.begin();
	Account* ptr = NULL;


	auto citerPos = g_list.begin();
	Client *ctr = NULL;

	iterPos=g_clist.begin();
	for (; iterPos != g_clist.end(); iterPos++)
	{
		ptr = *iterPos;

		if (ptr->AccountNo != aptr->AccountNo)
		{

			Packet << Type;
			Packet << ptr->AccountNo;
			Packet.Put((char*)ptr->ID, 20);
			FSendData(aptr, sock, (BYTE*)Packet.Totalbuffer(), Packet.sizeck());
			Packet.EmptyBuffer();
			//���⼭ ������ �ɾ �ٺ����ش�. //�ʹ� ���� 2���ۿ� ������
			//Sleep(10);
		}
	}


	
}



void FSendData(Client* aptr, SOCKET sock, BYTE *buf, int bufsize)
{

	BYTE getbuf[BUFFERSIZE];
	int reval = 0;
	memset(getbuf, NULL, BUFFERSIZE);

	aptr->SendQ.Put((char*)buf, bufsize);

	aptr->SendQ.Get((char*)getbuf, bufsize);

	reval = send(sock, (char*)getbuf, bufsize, 0);


	if (reval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			closesocket(sock);
			DisConnect(sock);
			return;
		}
	}
	
}



void FriendRecomRecv(Client* aptr, SOCKET sock, PacketSZ *Packet)
{

	return SendProc(aptr, sock, Friend_Recom_SC);
}



void FriendListRecv(Client* aptr, SOCKET sock, PacketSZ *Packet)
{
	return SendProc(aptr, sock, FriendList_SC);
}



void DisConnect(SOCKET sock)
{
	auto iterPos = g_clist.begin();
	Account* ptr = NULL;

	auto citerPos = g_list.begin();
	Client *ctr = NULL;
	int Anum = 0;

	citerPos = g_list.begin();
	for (; citerPos != g_list.end(); citerPos++)
	{
		ctr = *citerPos;
		if (sock == ctr->sock)
		{
			printf("Socket :[ %d ] out\n", ctr->sock);
			Anum = ctr->AccountNo;
			delete ctr;
			g_list.erase(citerPos);
			break;
		}
	}


	iterPos = g_clist.begin();
	for (; iterPos != g_clist.end(); iterPos++)
	{
		ptr = *iterPos;

		if (ptr->AccountNo == Anum)
		{
			//ptr->AccountNo = 0;
			wprintf(L"[ %s ] ", ptr->ID);
			printf("���� �����̽��ϴ�.\n");
			break;
		}
	}


}


void FriendList(Client* aptr, SOCKET sock)
{
	int Type = FriendList_SC;
	auto iterPosc = g_clist.begin();
	auto iterPos = g_flist.begin();
	Frined* ptr = NULL;
	PacketSZ Packet;
	Account* bptr = NULL;

	
	//Friend ����Ʈ���� From aptr�� ã�� to�ѹ��� account ����Ʈ���� ã�Ƽ�
	//WCHAR�� ����


	iterPos = g_flist.begin();
	for (; iterPos != g_flist.end(); iterPos++)
	{

		ptr = *iterPos;

		//������� ���̵� ã��
		if (aptr->AccountNo == ptr->ToAccountNo)
		{
			iterPosc = g_clist.begin();
			for (; iterPosc != g_clist.end(); iterPosc++)
			{
				bptr = *iterPosc;
				//����
				if (ptr->FromAccountNo == bptr->AccountNo)
				{
					Packet << Type;
					Packet.Put((char*)bptr->ID, 20);
					//Packet << bptr->ID;
					FSendData(aptr, sock, (BYTE*)Packet.Totalbuffer(), Packet.sizeck());
					Packet.EmptyBuffer();
					//Sleep(10);
					break;

				}
		
			}

		}		
	}
}


void FriendRequestR(Client* aptr, SOCKET sock, PacketSZ *Packet)
{

	//���� �ð� ���
	struct tm TM;

	FriendRequest *fptr = new FriendRequest;
	WCHAR account[20];
	memset(account, NULL, 40);

	Packet->Get((char*)account,20);

	auto iterPos = g_clist.begin();
	Account* ptr = NULL;

	g_clist.begin();
	for (; iterPos != g_clist.end(); iterPos++)
	{
		ptr = *iterPos;
		if (wcscmp(account,ptr->ID) == 0)
		{
			fptr->ToAccountNo = ptr->AccountNo;
			break;
		}
	}


	fptr->FromAccountNo = aptr->AccountNo;


	
	PacketSZ cPacket;

	g_inumber = 0;//�ʱ�ȭ

	//��ȣ�� ������
	if (fptr->FromAccountNo == fptr->ToAccountNo)
	{
		g_inumber = ERR;
		goto A;
	}


	time(&fptr->Time);
	localtime_s(&TM, &fptr->Time);

	g_frlist.push_back(fptr);
	g_inumber = OK;


	A:
	return SendProc(aptr, sock, FriendRequest_SC);

}


void FriendRequestS(Client* aptr, SOCKET sock)
{
	PacketSZ Packet;

	int Type = FriendRequest_SC;

	Packet << Type;
	Packet << g_inumber;

	FSendData(aptr, sock, (BYTE*)Packet.Totalbuffer(), Packet.sizeck());

}


void FriendReListRecv(Client* aptr, SOCKET sock, PacketSZ *Packet)
{
	//��û ������ �����ֱ�
	return SendProc(aptr, sock, FriendList_Request_SC);

}

void FriendReListReply(Client* aptr, SOCKET sock, PacketSZ *Packet)
{

	return SendProc(aptr, sock, FriendList_Reply_SC);

}

//��û�Ѱ� ����
void FriendRReList(Client* aptr, SOCKET sock)
{
	int Type = FriendList_Reply_SC;

	auto iterPos = g_frlist.begin();
	FriendRequest* ptr = NULL;

	auto iterPosc = g_clist.begin();
	Account* bptr = NULL;

	iterPos = g_frlist.begin();
	for (; iterPos != g_frlist.end(); iterPos++)
	{
		PacketSZ Packet;

		ptr = *iterPos;
		Packet << Type;

		if (aptr->AccountNo == ptr->FromAccountNo)
		{

			iterPosc = g_clist.begin();
			for (; iterPosc != g_clist.end(); iterPosc++)
			{
				bptr = *iterPosc;
				if (bptr->AccountNo == ptr->ToAccountNo)//&&bptr->sock != sock)
				{
					Packet.Put((char*)bptr->ID, 20);
					FSendData(aptr, sock, (BYTE*)Packet.Totalbuffer(), Packet.sizeck());
					Packet.EmptyBuffer();
					//Sleep(10);
					break;
				}
			}
		}
	}

}


//ģ�� ��û ���� ��Ϻ���
void FriendReList(Client* aptr, SOCKET sock)
{
	int Type = FriendList_Request_SC;

	auto iterPos = g_frlist.begin();
	FriendRequest* ptr = NULL;

	auto iterPosc = g_clist.begin();	
	Account* bptr = NULL;

	iterPos = g_frlist.begin();
	for (; iterPos != g_frlist.end(); iterPos++)
	{
		PacketSZ Packet;

		ptr = *iterPos;
		Packet << Type;

		if (aptr->AccountNo == ptr->ToAccountNo)
		{

			iterPosc = g_clist.begin();
			for (; iterPosc != g_clist.end(); iterPosc++)
			{
				bptr = *iterPosc;
				if (bptr->AccountNo == ptr->FromAccountNo)//&&bptr->sock != sock)
				{
					Packet.Put((char*)bptr->ID, 20);
					FSendData(aptr, sock, (BYTE*)Packet.Totalbuffer(), Packet.sizeck());
					Packet.EmptyBuffer();
					//Sleep(10);
					break;
				}
			}
		}

		
	}
}



void FriendCancelR(Client* aptr, SOCKET sock, PacketSZ *Packet)
{

	//ģ�� ��û ��� ����

	g_inumber = 0;
	//���̵� ������ case 1
	//Frined�� ã�Ƽ� ���̵�� sock�� ���̵� ã�Ƽ� FriendRequest ����Ʈ���� �����

	//���̵� ������ case 2
	//������ �ȵǰ� 2�� �����־ Ŭ�󿡼� ��µǾ���Ѵ�.
	WCHAR account[20];
	memset(account, NULL, 40);

	Packet->Get((char*)account, 20);
	
	auto iterPos = g_frlist.begin();
	auto iterPosc = g_clist.begin();
	auto iterPosfind = g_frlist.begin();
	FriendRequest* findptr = NULL;
	FriendRequest* ptr = NULL;
	Account* bptr = NULL;
	__int64 number = -1;

	



	iterPosc = g_clist.begin();
	for (; iterPosc != g_clist.end(); iterPosc++)
	{
		bptr = *iterPosc;
		//���̵� ������ ��ȣ ����
		if (wcscmp(bptr->ID, account) == 0)
		{
			number = bptr->AccountNo;
			break;
		}
	}

	//FQ���� �ֳ�������
	for (; iterPosfind != g_frlist.end(); iterPosfind++)
	{
		findptr = *iterPosfind;

		if (number == findptr->ToAccountNo)
		{
			g_inumber = OK;
			break;
		}
	}


	//�ȿ� ������.. //�ڱ��ڽ� // �׸��� ģ�� ��û�� �� �ȿ� ������
	if ((g_inumber == 0) || (number == -1))
	{
		g_inumber = ERR;
		goto A;

	}


	for (; iterPos != g_frlist.end(); iterPos++)
	{
		ptr = *iterPos;
		//�ڱ� �ѹ�                                    //��û�� �ѹ�                     
		if (ptr->FromAccountNo == aptr->AccountNo &&  ptr->ToAccountNo == number)
		{
			
			delete(ptr);
			g_frlist.erase(iterPos);
			break;
		}
	
	}

	
	A:
	return SendProc(aptr, sock, FriendCancel_SC);

}


void FriendRemoveR(Client* aptr, SOCKET sock, PacketSZ *Packet)
{

	g_inumber = 0;
	//ģ�� ��û ��� ����
	WCHAR account[20];
	memset(account, NULL, 40);

	Packet->Get((char*)account, 20);

	//���̵� ������ case 1
	//Frined�� ã�Ƽ� ���̵�� sock�� ���̵� ã�Ƽ� FriendRequest ����Ʈ���� �����
	//B-A, A-B�Ѵ� �����

	//���̵� ������ case 2
	//������ �ȵǰ� 2�� �����־ Ŭ�󿡼� ��µǾ���Ѵ�.

	//���� �� ������ �����ϸ� itor ������ �� ->�ذ�
	Frined* findptr = NULL;
	Frined* eptr = NULL;
	Frined* ptr = NULL;
	Account* bptr = NULL;
	__int64 number = -1;

	auto iterPos = g_flist.begin();
	auto iterPosa = g_flist.begin();
	auto iterPosc = g_clist.begin();
	auto iterPosfind = g_flist.begin();

	iterPosc = g_clist.begin();
	for (; iterPosc != g_clist.end(); iterPosc++)
	{
		bptr = *iterPosc;
		//���̵� ������ ��ȣ ����
		if (wcscmp(bptr->ID, account) == 0)
		{
			number = bptr->AccountNo;
			break;
		}
	}
	//FQ���� �ֳ�������

	iterPosfind = g_flist.begin();
	for (; iterPosfind != g_flist.end(); iterPosfind++)
	{
		findptr = *iterPosfind;

		if (number == findptr->FromAccountNo)
		{
			g_inumber = 0;
			break;
		}
	}


	iterPosfind = g_flist.begin();
	for (; iterPosfind != g_flist.end(); iterPosfind++)
	{
		findptr = *iterPosfind;

		if (number == findptr->ToAccountNo)
		{
			g_inumber = OK;
			break;
		}
	}


	//�ȿ� ������.. //�ڱ��ڽ� // �׸��� ģ�� ��û�� �� �ȿ� ������
	if ((g_inumber != OK) || (number == -1))
	{
		g_inumber = ERR;
		goto RM;

	}


	//�ι� ����
	iterPos = g_flist.begin();
	for (; iterPos != g_flist.end(); iterPos++)
	{
		ptr = *iterPos;
	                   
		if (ptr->FromAccountNo == number &&  ptr->ToAccountNo == aptr->AccountNo)
		{
			delete ptr;
			g_flist.erase(iterPos);
			break;
		}
	}


	iterPosa = g_flist.begin();
	for (; iterPosa != g_flist.end(); iterPosa++)
	{
		eptr = *iterPosa;
                    
		if (eptr->FromAccountNo == aptr->AccountNo &&  eptr->ToAccountNo == number)
		{
			delete eptr;
			g_flist.erase(iterPosa);
			break;
		}

	}

RM:

	return SendProc(aptr, sock, FriendRemove_SC);
}

void FriendDenyR(Client* aptr, SOCKET sock, PacketSZ *Packet)
{
	g_inumber = 0;
	//ģ�� �ź� ����

	//���̵� ������ case 1
	//Frined�� ã�Ƽ� ���̵�� sock�� ���̵� ã�Ƽ� FriendRequest ����Ʈ���� �����
	
	//���̵� ������ case 2
	//������ �ȵǰ� 2�� �����־ Ŭ�󿡼� ��µǾ���Ѵ�.

	WCHAR account[20];
	memset(account, NULL, 40);

	Packet->Get((char*)account, 20);

	auto iterPos = g_frlist.begin();
	auto iterPosc = g_clist.begin();

	FriendRequest* ptr = NULL;
	Account* bptr = NULL;
	__int64 number = -1;
	auto iterPosfind = g_frlist.begin();
	FriendRequest* findptr = NULL;

	iterPosc = g_clist.begin();
	for (; iterPosc != g_clist.end(); iterPosc++)
	{
		bptr = *iterPosc;
		//���̵� ������ ��ȣ ����
		if (wcscmp(bptr->ID, account) == 0)
		{
			number = bptr->AccountNo;
			break;
		}
	}


	//FQ���� �ֳ�������
	for (; iterPosfind != g_frlist.end(); iterPosfind++)
	{
		findptr = *iterPosfind;

		if (number == findptr->FromAccountNo)
		{
			g_inumber = OK;
			break;
		}
	}





	//�ȿ� ������.. //�ڱ��ڽ� // �׸��� ģ�� ��û�� �� �ȿ� ������
	if ((g_inumber == 0) || (number == -1))
	{
		g_inumber = ERR;
		goto D;

	}



	for (; iterPos != g_frlist.end(); iterPos++)
	{
		ptr = *iterPos;
		//��û�� �ѹ�                                    //�ڱ� �ѹ�                     
		if (ptr->FromAccountNo == number &&  ptr->ToAccountNo == aptr->AccountNo)
		{
			delete(ptr);
			g_frlist.erase(iterPos);
			break;
		}

	}


D:

	return SendProc(aptr, sock, FriendDeny_SC);

}

void FriendAgreeR(Client* aptr, SOCKET sock, PacketSZ *Packet)
{
	g_inumber = 0;
	//ģ�� ���� 
	//Ŭ������ �޾Ƽ� Account�� ã�Ƽ� ���̵�� sock�� ���̵� ã�Ƽ� FriendRequest ����Ʈ���� �����
	//Frined������ 2�� A-B, B-A ��ȣ �־��ֱ�

	//�����ٰ� �ʿ���
	WCHAR account[20];
	memset(account, NULL, 40);

	Packet->Get((char*)account, 20);

	auto iterPos = g_frlist.begin();
	auto iterPosc = g_clist.begin();
	auto iterPosfind = g_frlist.begin();
	FriendRequest* findptr = NULL;

	FriendRequest* ptr = NULL;
	Account* bptr = NULL;
	__int64 number = -1;
	struct tm TM;
	


	iterPosc = g_clist.begin();
	for (; iterPosc != g_clist.end(); iterPosc++)
	{
		bptr = *iterPosc;
		//���̵� ������ ��ȣ ����
		if (wcscmp(bptr->ID, account) == 0)
		{
			number = bptr->AccountNo;
			break;
		}
	}

	//FQ���� �ֳ�������
	for (; iterPosfind != g_frlist.end(); iterPosfind++)
	{
		findptr = *iterPosfind;

		if (number == findptr->FromAccountNo)
		{
			g_inumber = OK;
			break;
		}
	}



	//�ȿ� ������.. //�ڱ��ڽ� // �׸��� ģ�� ��û�� �� �ȿ� ������
	if ((g_inumber == 0) || (number == -1))
	{
		g_inumber = ERR;
		goto AG;

	}



	for (; iterPos != g_frlist.end(); iterPos++)
	{
		ptr = *iterPos;
		//��û�� �ѹ�                                    //�ڱ� �ѹ�                          
		if (ptr->FromAccountNo == number &&  ptr->ToAccountNo == aptr->AccountNo)
		{
			delete(ptr);
			g_frlist.erase(iterPos);
			break;
		}

	}

	//A-B,B-A �ξ���
	Frined *friptrone = new Frined;

	friptrone->FromAccountNo = number;

	friptrone->ToAccountNo = aptr->AccountNo;


	time(&friptrone->Time);
	localtime_s(&TM, &friptrone->Time);

	g_flist.push_back(friptrone);


	Frined *friptrtwo = new Frined;

	friptrtwo->FromAccountNo = aptr->AccountNo;

	friptrtwo->ToAccountNo = number;

	time(&friptrtwo->Time);
	localtime_s(&TM, &friptrtwo->Time);
	g_flist.push_back(friptrtwo);



AG:

	return SendProc(aptr, sock, FriendAgree_SC);

}


void FriendRemoveS(Client* aptr, SOCKET sock)
{
	PacketSZ Packet;

	int Type = FriendRemove_SC;

	Packet << Type;
	Packet << g_inumber;

	FSendData(aptr, sock, (BYTE*)Packet.Totalbuffer(), Packet.sizeck());

}


void FriendDenyS(Client* aptr, SOCKET sock)
{
	PacketSZ Packet;

	int Type = FriendDeny_SC;

	Packet << Type;
	Packet << g_inumber;

	FSendData(aptr, sock, (BYTE*)Packet.Totalbuffer(), Packet.sizeck());

}

void FriendAgreeS(Client* aptr, SOCKET sock)
{
	PacketSZ Packet;

	int Type = FriendAgree_SC;

	Packet << Type;
	Packet << g_inumber;

	FSendData(aptr, sock, (BYTE*)Packet.Totalbuffer(), Packet.sizeck());

}


//�̰�üũ
void FriendCancelS(Client* aptr, SOCKET sock)
{
	PacketSZ Packet;

	int Type = FriendCancel_SC;

	Packet << Type;
	Packet << g_inumber;

	FSendData(aptr, sock, (BYTE*)Packet.Totalbuffer(), Packet.sizeck());

}


void ServerOFF()
{
	//Sleep(1000);
	if (GetAsyncKeyState(VK_F3))
	{
		JsonSave();
	}
}

//JON ����
void JsonSave()
{
	FILE *pFiles = NULL;

	StringBuffer StringJSON;
	Writer<StringBuffer, UTF16<>> writer(StringJSON);

	auto iterPos = g_clist.begin();
	auto iterPosq = g_flist.begin();
	auto iterPosf = g_frlist.begin();

	Account* ptr = NULL;
	Frined* fptr = NULL;
	FriendRequest* frptr = NULL;

	writer.StartObject(); //{
	writer.String(L"Account");

	writer.StartArray(); //[

	//����Ʈ ���� <-�̰� �迭ó�� [] �ȿ�
	iterPos = g_clist.begin();
	for (; iterPos != g_clist.end(); iterPos++)
	{

		ptr = *iterPos;

		writer.StartObject();
		writer.String(L"AccountNo");
		writer.Uint64(ptr->AccountNo);

		writer.String(L"ID");
		writer.String(ptr->ID);
		writer.EndObject();

	}
	writer.EndArray(); //]


	writer.String(L"Friend");
	writer.StartArray(); //[
	iterPosq = g_flist.begin();
	for (; iterPosq != g_flist.end(); iterPosq++)
	{

		fptr = *iterPosq;

		writer.StartObject();
		writer.String(L"FromAccountNo");
		writer.Uint64(fptr->FromAccountNo);
		writer.String(L"ToAccountNo");
		writer.Uint64(fptr->ToAccountNo);
		writer.EndObject();

	}
	writer.EndArray(); //]


	writer.String(L"FriendRequest");
	writer.StartArray(); //[
	iterPosf = g_frlist.begin();
	for (; iterPosf != g_frlist.end(); iterPosf++)
	{

		frptr = *iterPosf;

		writer.StartObject();
		writer.String(L"FromAccountNo");
		writer.Uint64(frptr->FromAccountNo);
		writer.String(L"ToAccountNo");
		writer.Uint64(frptr->ToAccountNo);
		writer.EndObject();

	}
	writer.EndArray(); //]




	writer.EndObject();  //}
	//���Ͽ� ����
	_wfopen_s(&pFiles, L"DB_HI_JSON.txt", L"wb");
	const char* pJson = StringJSON.GetString();
	fprintf_s(pFiles, pJson);
	fclose(pFiles);

	printf("[ ���� �� ���� ] �Ǿ����ϴ�.\n");
	//Sleep(1000);
	exit(1);
	

}


void JsonLoad()
{

	int bufsize = 0;


	Account *acout = NULL;
	Frined *fri = NULL;
	FriendRequest* friQ = NULL;
	FILE *pFile=NULL;
	
	

	_wfopen_s(&pFile, L"DB_HI_JSON.txt", L"rb");

	fseek(pFile, 0, SEEK_END);
	bufsize = ftell(pFile);
	rewind(pFile);

	if (bufsize == 0)
	{
		return;
	}

	char *pJSon = new char[bufsize];



	fread(pJSon, bufsize, 1, pFile);

	pJSon[bufsize] = NULL;

	WCHAR szID[20];
	memset(szID, NULL, 40);

	__int64 AccountNo=0;
	__int64 FromAccountNo=0;
	__int64 ToAccountNo=0;
	__int64 FromAccountNoQ=0;
	__int64 ToAccountNoQ=0;

	Document Doc;
	Doc.Parse(pJSon);

	Value &AccountArray = Doc["Account"];

	for (SizeType i = 0; i < AccountArray.Size(); i++)
	{
		Value &AccountObject = AccountArray[i];

		acout = new Account;

		AccountNo = AccountObject["AccountNo"].GetInt64();
		//�������� ����� �Լ�
		UTF8toUTF16(AccountObject["ID"].GetString(), szID, 20);
		acout->AccountNo = AccountNo;
		memset(acout->ID, NULL, 40);

		memcpy(acout->ID, szID, wcslen(szID) * 2);

		g_clist.push_back(acout);

	}


	Value &FriendArray = Doc["Friend"];

	for (SizeType i = 0; i < FriendArray.Size(); i++)
	{

		fri = new Frined;

		Value &FriendObject = FriendArray[i];

		FromAccountNo = FriendObject["FromAccountNo"].GetInt64();

		ToAccountNo = FriendObject["ToAccountNo"].GetInt64();

		fri->FromAccountNo = FromAccountNo;
		fri->ToAccountNo = ToAccountNo;

		g_flist.push_back(fri);

	}


	Value &FriendRequestArray = Doc["FriendRequest"];

	for (SizeType i = 0; i < FriendRequestArray.Size(); i++)
	{
		Value &FriendRequestObject = FriendRequestArray[i];


		friQ = new FriendRequest;

		FromAccountNoQ = FriendRequestObject["FromAccountNo"].GetInt64();

		ToAccountNoQ = FriendRequestObject["ToAccountNo"].GetInt64();

		friQ->FromAccountNo = FromAccountNoQ;
		friQ->ToAccountNo = ToAccountNoQ;

		g_frlist.push_back(friQ);
	}
	
	fclose(pFile);

}


bool UTF8toUTF16(const char *szText, WCHAR *szBuff, int iBuffLen)
{
	int iRe = MultiByteToWideChar(CP_UTF8, 0, szText, strlen(szText), szBuff, iBuffLen);

	if (iRe < iBuffLen)
		szBuff[iRe] = L'\0';

	return true;
}



WCHAR g_dumy[20];
DWORD dwStart;
DWORD dwEnd;

//���� �׽�Ʈ�� ���� //�޴°�
void DumyR(Client* aptr, SOCKET sock, PacketSZ *Packet)
{
	dwStart = GetTickCount();

	Packet->Get((char*)g_dumy, 20);
	printf("Server [Packet] : ");
	wprintf(L"%s \n", g_dumy);

	printf("RecvPacketSize : %d byte \n", Packet->sizeck());
	SendProc(aptr, sock, DUMY_TEST);

}

//�����°�
void DumyS(Client* aptr, SOCKET sock)
{


	int Type = DUMY_TEST;

	PacketSZ Packet;

	Packet << Type;

	Packet.Put((char*)g_dumy, 20);

	FSendData(aptr, sock, (BYTE*)Packet.Totalbuffer(), Packet.sizeck());

	dwEnd = GetTickCount();

	double dvtime = dwEnd - dwStart;
	double dtime = dvtime;// / 1000;

	printf("SendPacketSize : %d byte \n", Packet.sizeck());
	printf("Time : %.2f \n\n", dtime);
	memset(g_dumy, NULL, 40);

	//dwStart = 0;
	//dwEnd = 0;
}