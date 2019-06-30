#include "stdafx.h"
#include "Console.h"

HANDLE hConsole;

char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

void cs_Initial(void){

	CONSOLE_CURSOR_INFO stConsoleCursor;
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);

}

void cs_MoveCursor(int iPosX, int iPosY){

	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;

	SetConsoleCursorPosition(hConsole, stCoord);


}

void cs_ClearScreen(void){

	int iCountX, iCountY;

	for (iCountY = 0; iCountY < dfSCREEN_HEIGHT; iCountY++){
		for (iCountX = 0; iCountX < dfSCREEN_WIDTH; iCountX++){

			cs_MoveCursor(iCountX, iCountY);
			printf(" ");
		}
	}

}

void Buffer_Flip(void){
	// [24] [80]
	int i;
	for (i = 0; i < dfSCREEN_HEIGHT; i++){
		printf("%s", szScreenBuffer[i]);
		cs_MoveCursor(0, i);
	}
}



//버퍼에다가 스페이스로 지움
void Buffer_Clear(void){

	int iCountX, iCountY;
	for (iCountY = 0; iCountY < dfSCREEN_HEIGHT; iCountY++){

		for (iCountX = 0; iCountX < dfSCREEN_WIDTH - 1; iCountX++){
			szScreenBuffer[iCountY][iCountX] = ' ';
		}
		//널을 넣어야함
		szScreenBuffer[iCountY][iCountX] = 0;
	}
}


//좌표에다가 문자 넣음
//오버로딩 문자열, 문자
void Sprite_Draw(int iPosX, int iPosY, char *sprite){
	//cs_MoveCursor(iPosX, iPosX);
	int x;
	x = iPosX;
	char buf[BUFFER];
	memcpy(buf, sprite, BUFFER);
	for (int i = 0; i < BUFFER; i++)
	{
		szScreenBuffer[iPosY][iPosX] = buf[i];
		iPosX++;
	}

}

//좌표에다가 문자 넣음
void Sprite_Draw(int iPosX, int iPosY, char sprite)
{
	szScreenBuffer[iPosY][iPosX] = sprite;
}