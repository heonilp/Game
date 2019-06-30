#include "stdafx.h"
#include "Global.h"
#include "cs_playerMissaile.h"



void cs_playMissaile::Draw()
{
	//true면 미사일을 그려줌
	if (GetFlag() == true)
	{
		Sprite_Draw(this->GetX(), this->GetY(), '^');
	}
}



void cs_playMissaile::Move(void)
{
	
	//미사일의 Y좌표를 받아옴
	int MissileY = this->GetY();

	//미사일 누를 때 마다 객체 생성한다.
	//플레이어가 1	// 몬스터가 2 // 미사일이 3

	//좌표가 도달하면 없어짐
	if (this->GetY() < 2)
	{
		this->SetFlag(false);

	}
	//미사일의 행동처리 

	//Y좌표 위로 올라감
	if (this->GetFlag() == true)
	{
		MissileY--;
		this->SetY(MissileY);
	}

	if (this->GetFlag() == false)
	{
		//자신 this를 지움
		g_List.RemoveAt();
		delete this;
	}

	

}





bool cs_playMissaile::Action()
{
	extern int monster;
	extern void Menuwin(void);

	if (monster == 0)
	{
		Menuwin();
	}
	g_List.MoveHead();
	while (g_List.MoveNext()==true)
	{
		if (g_List.GetAt()->GetType() == 2)	//플레이어가 1	// 몬스터가 2 // 미사일이 3
		{
			int MonsterHP = g_List.GetAt()->GetHP();
			if ((this->GetY() == g_List.GetAt()->GetY())&&(this->GetX() == g_List.GetAt()->GetX()))
			{
				//kill++;
				this->SetFlag(false);
				g_List.GetAt()->SetFlag(false);
				MonsterHP--;
				g_List.GetAt()->SetHP(MonsterHP);
				monster--;
			}
		}
	}

	g_List.MoveHead();
	while (g_List.MoveNext() == true)
	{
		if (g_List.GetAt() == this)
		{
			return true;
		}
	}
	


	return false;
}

