// FriendServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
//#include "SQue.h"
#include "FriendServer.h"


#pragma comment(lib,"WS2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	NetworkInit();


	while (1)
	{
		Network();
		ServerOFF();
	}

	return 0;
}



