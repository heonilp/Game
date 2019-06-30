#ifndef __FC_H__
#define __FC_H__

#define Friend_Login_CS 1//�α���
#define Friend_Login_CSQ 2//�α��� �ޱ�
#define Friend_Recom_CS 3 //��õ ģ��
#define FriendList_CS 4 //- ģ�����
#define FriendList_Request_CS 5//- ģ�����_��û������
#define FriendList_Reply_CS 6//- ģ�����_��û������
#define FriendRemove_CS 7//- ģ������ ����
#define FriendRequest_CS 8//- ģ�� ��û�ϱ�
#define FriendCancel_CS 9// - ģ�� ��û ������ ���
#define FriendDeny_CS 10 //- ģ�� ��û �ź�
#define FriendAgree_CS 11//- ģ�� ��û ����
#define Friend_OldLogin_CS 12


enum FRIENDMENU
{
	Friend_Recomlist=0,
	Friend_list,
	Friend_listRequest,
	Friend_Reply,
	Friend_Remove,
	FriendRequest,
	FriendCancel,
	FriendDeny,
	FriendAgree,
	MyDisconnet,

};


class PacketSZ;

struct Account
{
	__int64 AccountNo;
	WCHAR ID[20];
	SOCKET sock;
};



struct Frined
{
	__int64 FromAccountNo;
	__int64	ToAccountNo;
	DWORD Time;
};



struct FriendRequest
{
	__int64 FromAccountNo;
	__int64 ToAccountNo;
	DWORD Time;
};


enum Request
{
	OK = 1,
	OVERLAP,
	ERR,
};

//��Ʈ��ũ �Լ�
void err_quit(WCHAR *msg);
void err_display(WCHAR *msg);
void Network();
void NetworkInit();
void SendProc(int Type);
void RecvProc();
void DisConnect();
//�α��� �Լ�
void LoginIDput();
void LoginSend(int Type);

void LoginCreate(PacketSZ *packet);
void FSendData(BYTE *buf, int bufsize);

void FriendOldLogin(PacketSZ *packet);

void FriendMg();


void FriendSend(int Type);
void RecomPrint(PacketSZ *Packet);
void ListPrint(PacketSZ *Packet);


void FriendRequestS(int Type);
void FriendRequestR(PacketSZ *Packet);
void OrderPrint(PacketSZ *Packet);
void ReplyPrint(PacketSZ *Packet);

void KeyFone();
void FriendAgreeS(int Type);
void FriendAgreeR(PacketSZ* Packet);


void FriendCancelS(int Type);
void FriendDenyS(int Type);
void FriendRemove(int Type);


void FriendCancelR(PacketSZ* Packet);
void FriendDenyR(PacketSZ* Packet);
void FriendRemoveR(PacketSZ* Packet);

#endif  //#__FC_H__