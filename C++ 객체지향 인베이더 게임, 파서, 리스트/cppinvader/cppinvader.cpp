// cppinvader.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "Global.h"
#include "cs_Monster.h"
#include "cs_Player.h"
#include "cs_playerMissaile.h"
#include "cs_MonsterMissaile.h"
#include "Parser.h"



//////////////전역함수/////////////
extern void Delay();
void MenuLose(void);
void Menuwin(void);
void KeyProcess(void);
void Draw(void);
void Gamelnit(void);
void Move(void);
void GamePlay(void);
void Logo(void);
void Tilte(void);
void Menu(void);
bool Action(void);
void Ending(void);
void Shot(void);
///////////////////////////////////

//////////////전역변수/////////////
CLinkedlist <cs_BaseObject> g_List;
int monster;
int g_GameState;
int itemp;
///////////////////////////////////


int _tmain(int argc, _TCHAR* argv[])
{
	srand(unsigned int(time(NULL)));
	/*
	//메인안이 분기문으로
	*/
	cs_Initial();

	while (1)
	{
		//<4. stage 용>
		switch (g_GameState)
		{
		case 0:
			Logo();
			break;
		case 1:
			Tilte();
			break;
		case 2:
			Menu();
			break;
		case 3:
			GamePlay();
			break;
		case 4:
			GamePlay();
			break;
		}


	}


	return 0;
}


void GamePlay(void)
{
	Gamelnit();

	while (1){
		Buffer_Clear(); // 버퍼지우기();
		KeyProcess(); // 키보드();
		Action();
		Move();
		Shot();
		Draw();
		Buffer_Flip(); //Filp();
		Sleep(30);
	}
}






void Shot(void)
{
	g_List.MoveHead();
	while (g_List.MoveNext() == true)
	{
		if (g_List.GetAt()->GetType() == 4)
		    g_List.GetAt()->Shot();
	}
}

void Logo(void)
{
	int g_iLogoY = 20;

	while (1){
		Buffer_Clear(); // 버퍼지우기();
		Sprite_Draw(30, g_iLogoY, "Hello INVADER");
		Sleep(60);
		g_iLogoY--;
		Buffer_Flip(); //Filp();
		if (g_iLogoY < 10){
			g_GameState = 1;
			break;
		}
	}
}

void Tilte(void)
{
	int g_Tilte = 20;
	while (1){
		Buffer_Clear(); // 버퍼지우기();
		Sprite_Draw(30, g_Tilte, "INVADER GAME");
		Sleep(100);
		g_Tilte--;
		Buffer_Flip(); //Filp();
		if (g_Tilte < 10){
			g_GameState = 2;
			break;
		}
	}
}

void Menu(void)
{
	while (1){
		Buffer_Clear(); // 버퍼지우기();
		Sprite_Draw(27, 8, "<  INVADER GAME  >");
		Sprite_Draw(28, 10, "GAME START : F1");
		Sprite_Draw(29, 12, "GAME END ESC");
		Buffer_Flip(); //Filp();

		if (GetAsyncKeyState(VK_F1))
		{
			g_GameState = 3;
			break;
		}

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			exit(1);
			break;
		}
	}
	
}


void Ending(void)
{
	while (1){
		Buffer_Clear(); // 버퍼지우기();
		Sprite_Draw(27, 8, "< INVADER ENDING  >");
		Sprite_Draw(27, 8, " PLAYER WIN  ");
		Sprite_Draw(27, 10, "GAME END ESC");
		Buffer_Flip(); //Filp();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			exit(1);
			break;
		}
	}

}


void Menuwin(void)
{
	while (1){
		Buffer_Clear(); // 버퍼지우기();
		Sprite_Draw(27, 8, "<  PLAYER WIN  >");
		Sprite_Draw(27, 9,  "1.NEXT STAGE : F2");
		Sprite_Draw(27, 10, "2.GAME END ESC");
		Buffer_Flip(); //Filp();

		if (GetAsyncKeyState(VK_F2))
		{
			g_GameState = 4;
			itemp = 0;
			Gamelnit();
			break;
		}

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			exit(1);
			break;
		}
	}

}


void MenuLose(void)
{

	while (1){
		Buffer_Clear(); // 버퍼지우기();
		Sprite_Draw(27, 8, "<  PLAYER LOSE  >");
		Sprite_Draw(27, 9, "1.이어서 하기 : F3");
		Sprite_Draw(27, 10,"2.GAME END ESC");
		Buffer_Flip(); //Filp();

		if (GetAsyncKeyState(VK_F3))
		{
			g_List.MoveHead();
			while (g_List.MoveNext() == true)
			{
				if (g_List.GetAt()->GetType() == 1)	//플레이어가 1	// 몬스터가 2 // 미사일이 3 //몬스터 미사일이 4
				{
					g_List.GetAt()->SetHP(10);
				}
			}
			g_GameState = 3;
			break;
		}


		if (GetAsyncKeyState(VK_ESCAPE))
		{
			exit(1);
			break;
		}
	}

}





void Gamelnit(void)
{
	Parser parser;
	cs_Monster *Monster;
	cs_Player *Player;
	int iMonsterX = 0;
	int iHP,iX,iY;

	int iMonsterLOW1;
	int iMonsterLOW2;
	int iMonsterLOW3;

	int iMonsterHpLev1;
	int iMonsterHpLev2;
	int iMonsterHpLev3;
	int iMonsterHpLev4;

	parser.Parser_LoadFile("invader.txt");

	if (g_GameState == 3)
	{
		parser.Parser_ProvideArea("STAGE1");
		parser.Parser_ProvideArea("PLAYER");
		parser.Parser_GetValue("HP", &iHP);
		parser.Parser_GetValue("X", &iX);
		parser.Parser_GetValue("Y", &iY);
		Player = new cs_Player(iX, iY, 1, true, iHP);
		g_List.InsertHead(Player);
		
	}

	if (g_GameState == 4)
	{
		parser.Parser_ProvideArea("STAGE2");
	}


	parser.Parser_ProvideArea("MONSTER");
	parser.Parser_GetValue("MONSTERLOW1", &iMonsterLOW1);
	parser.Parser_GetValue("MONSTERLOW2", &iMonsterLOW2);
	parser.Parser_GetValue("MONSTERLOW3", &iMonsterLOW3);

	parser.Parser_GetValue("LEV1", &iMonsterHpLev1);
	parser.Parser_GetValue("LEV2", &iMonsterHpLev2);
	parser.Parser_GetValue("LEV3", &iMonsterHpLev3);
	parser.Parser_GetValue("LEV4", &iMonsterHpLev4);



	
	//몬스터 5마리만 생성하고 싶을 때 
	//(40,10) ,(42,10) ,(44,10) ,(46,10), (48,10) 좌표에 할당


	

	for (iMonsterX = 30; iMonsterX < 30 + (2 *iMonsterLOW1); iMonsterX++)
	{
		Monster = new cs_Monster(iMonsterX, 10, 2, true, iMonsterHpLev1,itemp);
		g_List.InsertNext(Monster);
		iMonsterX = iMonsterX + 2;
		monster = monster + iMonsterHpLev1;
		itemp++;
	}

	for (iMonsterX = 30; iMonsterX < 30 + (2 * iMonsterLOW1); iMonsterX++)
	{
		Monster = new cs_Monster(iMonsterX + 8, 7, 2, true, iMonsterHpLev2, itemp);
		g_List.InsertNext(Monster);
		iMonsterX = iMonsterX + 2;
		monster = monster + iMonsterHpLev2;
		itemp++;
	}

	for (iMonsterX = 30; iMonsterX < 30 + (2 * iMonsterLOW1); iMonsterX++)
	{
		Monster = new cs_Monster(iMonsterX - 10, 4, 2, true, iMonsterHpLev3, itemp);
		g_List.InsertNext(Monster);
		iMonsterX = iMonsterX + 2;
		monster = monster + iMonsterHpLev3;
		itemp++;
	}

	Monster = new cs_Monster(40, 1, 2, true, iMonsterHpLev4, 1);
	g_List.InsertNext(Monster);
	monster = monster + iMonsterHpLev4;
}




void Draw(void)
{
	g_List.MoveHead();
	while (g_List.MoveNext() == true)
	{
		g_List.GetAt()->Draw();
	}
}




void Move(void)
{
	g_List.MoveHead();
	while (g_List.MoveNext() == true)
	{
		g_List.GetAt()->Move();
	}

}




bool Action()
{
	g_List.MoveHead();
	while (g_List.MoveNext() == true)
	{
		g_List.GetAt()->Action();
	}
	
	return true;
}








void KeyProcess(void)
{
	while (g_List.MoveNext() == true)
	{
		if (g_List.GetAt()->GetType() == 1)
		{
			goto KEYPROCESS;

		}
	}

KEYPROCESS:

	int x = g_List.GetAt()->GetX();
	printf("[ PLAYER HP : %d ] ", g_List.GetAt()->GetHP());
	printf("[ 몬스터 체력 : %d ]", monster);
	//플레이어의 좌우 변화

	if (GetAsyncKeyState(VK_LEFT))
	{
		if (g_List.GetAt()->GetX() < 10)
		{
			x++;
		}
		x--;
		g_List.GetAt()->SetX(x);

	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		if (g_List.GetAt()->GetX() > 65)
		{
			x--;
		}
		x++;
		g_List.GetAt()->SetX(x);

	}


	if (GetAsyncKeyState(VK_SPACE))
	{
		//미사일 동적할당
		cs_playMissaile *Missaile = new cs_playMissaile(g_List.GetAt()->GetX(), g_List.GetAt()->GetY(), 3, true, 1);
		g_List.InsertHead(Missaile);
	}

}



// 객체 지향 ObjectManeger가 리스트처럼 관리한다.
//1. 직접 키를 누를 때 생성하던가
// Action(); Draw();
//총알을 누를 때 객체를 생성해야한다.
//2. ObjectManeger한테 시키던가
/*
List.MoveHead();
while(List.MoveNext())
{
pObject=List.GetAt();
pObect->PostMsg(CHECK,this->X,this->Y);
객체간 메시지 ->객체마다 있음
충돌처리
}

*/





