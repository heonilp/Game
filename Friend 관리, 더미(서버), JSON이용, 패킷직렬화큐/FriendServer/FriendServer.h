#ifndef __FS_H__
#define __FS_H__
#include "SQue.h"


#define Friend_Login_SC 1 //아이디 겹치는것이 있는 확인
#define Friend_Login_SCR 2 //응답
#define Friend_Recom_SC 3 //추천 친구
#define FriendList_SC 4 //- 친구목록
#define FriendList_Request_SC 5//- 친구목록_요청보낸거
#define FriendList_Reply_SC 6//- 친구목록_요청받은거
#define FriendRemove_SC 7//- 친구관계 끊기
#define FriendRequest_SC 8//- 친구 요청하기
#define FriendCancel_SC 9// - 친구 요청 보낸거 취소
#define FriendDeny_SC 10 //- 친구 요청 거부
#define FriendAgree_SC 11//- 친구 요청 수락
#define Friend_OldLogin_SC 12 //기존 로그인 추가
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


//네트워크 기본 함수(셀렉트)
void err_quit(WCHAR *msg);
void err_display(WCHAR *msg);
void Network();
void NetworkInit();
void DisConnect(SOCKET sock);
//클라이언트 추가
void ClinetAdd(SOCKET sock);

//송신 수신
void SendProc(Client* aptr, SOCKET sock, int Type);
void RecvProc(Client* aptr, SOCKET sock);
//로그인 함수

void FreindLogin(Client* aptr, SOCKET sock);

void FreindIDcpy(Client* aptr, SOCKET sock, PacketSZ *Packet);
//회원들 모두 보내기 //(추천친구)
void FriendRecom(Client* aptr, SOCKET sock);

//send함수

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


//JSON 부분
void ServerOFF();
void JsonSave();
void JsonLoad();
bool UTF8toUTF16(const char *szText, WCHAR *szBuff, int iBuffLen);
#endif