#ifndef __FS_H__
#define __FS_H__
#include "SQue.h"


#define Friend_Login_SC 1 //���̵� ��ġ�°��� �ִ� Ȯ��
#define Friend_Login_SCR 2 //����
#define Friend_Recom_SC 3 //��õ ģ��
#define FriendList_SC 4 //- ģ�����
#define FriendList_Request_SC 5//- ģ�����_��û������
#define FriendList_Reply_SC 6//- ģ�����_��û������
#define FriendRemove_SC 7//- ģ������ ����
#define FriendRequest_SC 8//- ģ�� ��û�ϱ�
#define FriendCancel_SC 9// - ģ�� ��û ������ ���
#define FriendDeny_SC 10 //- ģ�� ��û �ź�
#define FriendAgree_SC 11//- ģ�� ��û ����
#define Friend_OldLogin_SC 12 //���� �α��� �߰�
#define DUMY_TEST 13
class PacketSZ;



enum Request
{
	OK = 1,
	OVERLAP,
	ERR,
	DUMYCK,
};


struct Account
{
	__int64 AccountNo;
	WCHAR ID[20];
};


struct Client
{
	__int64 AccountNo;
	SOCKET sock;
	SQue RecvQ;
	SQue SendQ;
};




struct Frined
{
	__int64 FromAccountNo;
	__int64	ToAccountNo;
	time_t Time;
};



struct FriendRequest
{
	__int64 FromAccountNo;
	__int64 ToAccountNo;
	time_t Time;
};


//��Ʈ��ũ �⺻ �Լ�(����Ʈ)
void err_quit(WCHAR *msg);
void err_display(WCHAR *msg);
void Network();
void NetworkInit();
void DisConnect(SOCKET sock);
//Ŭ���̾�Ʈ �߰�
void ClinetAdd(SOCKET sock);

//�۽� ����
void SendProc(Client* aptr, SOCKET sock, int Type);
void RecvProc(Client* aptr, SOCKET sock);
//�α��� �Լ�

void FreindLogin(Client* aptr, SOCKET sock);

void FreindIDcpy(Client* aptr, SOCKET sock, PacketSZ *Packet);
//ȸ���� ��� ������ //(��õģ��)
void FriendRecom(Client* aptr, SOCKET sock);

//send�Լ�

void FSendData(Client* aptr, SOCKET sock, BYTE *buf, int bufsize);
void FriendRecomRecv(Client* aptr, SOCKET sock, PacketSZ *Packet);

void FriendList(Client* aptr, SOCKET sock);
void FriendListRecv(Client* aptr, SOCKET sock, PacketSZ *Packet);


void FriendRequestR(Client* aptr, SOCKET sock, PacketSZ *Packet);
void FriendRequestS(Client* aptr, SOCKET sock);


void FriendReListRecv(Client* aptr, SOCKET sock, PacketSZ *Packet);
void FriendReList(Client* aptr, SOCKET sock);


void FriendReListReply(Client* aptr, SOCKET sock, PacketSZ *Packet);
void FriendRReList(Client* aptr, SOCKET sock);

void FriendCancelR(Client* aptr, SOCKET sock, PacketSZ *Packet);
void FriendCancelS(Client* aptr, SOCKET sock);


void FriendRemoveR(Client* aptr, SOCKET sock, PacketSZ *Packet);
void FriendRemoveS(Client* aptr, SOCKET sock);

void FriendDenyR(Client* aptr, SOCKET sock, PacketSZ *Packet);
void FriendDenyS(Client* aptr, SOCKET sock);

void FriendAgreeR(Client* aptr, SOCKET sock, PacketSZ *Packet);
void FriendAgreeS(Client* aptr, SOCKET sock);

void FreindOldLogin(Client* aptr, SOCKET sock);
void FreindOldIDcpy(Client* aptr, SOCKET sock, PacketSZ *Packet);

void DumyR(Client* aptr, SOCKET sock, PacketSZ *Packet);
void DumyS(Client* aptr, SOCKET sock);


//JSON �κ�
void ServerOFF();
void JsonSave();
void JsonLoad();
bool UTF8toUTF16(const char *szText, WCHAR *szBuff, int iBuffLen);
#endif