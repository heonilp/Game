#include "stdafx.h"
#include "Global.h"
#include "cs_MonsterMissaile.h"


void delay(clock_t n);

void cs_MonsterMissaile::Draw(void)
{
	if (GetFlag() == true)
	{
		Sprite_Draw(this->GetX(), this->GetY(), '|');
	}
}


void cs_MonsterMissaile::Move(void)
{
	//미사일의 행동처리 
	int mMissileY = this->GetY();

	//좌표가 도달하면 없어짐
	if (this->GetY() > 20)
	{
		this->SetFlag(false);
	}
	
	//Y좌표 위로 올라감
	
	this->Shot();

	//펄스면 삭제하고 지움
	if (this->GetFlag() == false)
	{
		g_List.RemoveAt();
		delete this;
	}

}

void cs_MonsterMissaile::Shot(void)
{
	int mMissileY = this->GetY();
	int mMissileX = this->GetX();


	if (this->GetFlag() == true)
	{
		//몬스터미사일이 아래로내려감
		mMissileY++;
		//각도
		Angle += 30;
		this->SetX((int)SinX(mMissileX, Angle));
		this->SetY(mMissileY);
	}
}



bool cs_MonsterMissaile::Action(void)
{
	g_List.MoveHead();
	while (g_List.MoveNext() == true)
	{
		if (g_List.GetAt()->GetType() == 1)	//플레이어가 1	// 몬스터가 2 // 미사일이 3 //몬스터 미사일이 4
		{
			int HP = g_List.GetAt()->GetHP();
			if ((this->GetY() == g_List.GetAt()->GetY()) && (this->GetX() == g_List.GetAt()->GetX()))
			{
				this->SetFlag(false);
				g_List.GetAt()->SetFlag(false);
				HP--;
				g_List.GetAt()->SetHP(HP);
				
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




double cs_MonsterMissaile::SinX(double g, int iAngle)
{
	double radian;
	radian = iAngle * 3.14f / 180;
	g = g + ((sin(radian))*2);
	return g;

}



