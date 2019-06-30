// FriendServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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



