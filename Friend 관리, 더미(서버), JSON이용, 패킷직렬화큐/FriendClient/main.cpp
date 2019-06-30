#include "stdafx.h"
#include "FriendClient.h"
#include "../FriendServer/PacketSZ.h"
#include "../FriendServer/SQue.h"
#pragma comment(lib,"WS2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	NetworkInit();


	while (1)
	{
		Network();
		KeyFone();
	}

	return 0;
}


