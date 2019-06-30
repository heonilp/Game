#ifndef __FC_H__
#define __FC_H__

#define Friend_Login_CS 1//로그인
#define Friend_Login_CSQ 2//로그인 받기
#define Friend_Recom_CS 3 //추천 친구
#define FriendList_CS 4 //- 친구목록
#define FriendList_Request_CS 5//- 친구목록_요청보낸거
#define FriendList_Reply_CS 6//- 친구목록_요청받은거
#define FriendRemove_CS 7//- 친구관계 끊기
#define FriendRequest_CS 8//- 친구 요청하기
#define FriendCancel_CS 9// - 친구 요청 보낸거 취소
#define FriendDeny_CS 10 //- 친구 요청 거부
#define FriendAgree_CS 11//- 친구 요청 수락
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

//네트워크 함수
void err_quit(WCHAR *msg);
void err_display(WCHAR *msg);
void Network();
void NetworkInit();
void SendProc(int Type);
void RecvProc();
void DisConnect();
//로그인 함수
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