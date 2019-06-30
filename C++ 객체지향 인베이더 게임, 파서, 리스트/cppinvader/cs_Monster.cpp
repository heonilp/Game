#include "stdafx.h"
#include "Global.h"
#include "cs_Monster.h"
#include "cs_MonsterMissaile.h"

void cs_Monster::Draw()
{
	//몬스터 체력마다 그리기
	if (this->GetHP() == 1)
	{
		Sprite_Draw(this->GetX(), this->GetY(), '@');
	}

	if (this->GetHP() == 2)
	{
		Sprite_Draw(this->GetX(), this->GetY(), '$');

	}

	if (this->GetHP() == 3)
	{
		Sprite_Draw(this->GetX(), this->GetY(), '%');

	}

	if (this->GetHP() == 4)
	{
		Sprite_Draw(this->GetX(), this->GetY(), '*');

	}
}


bool cs_Monster::Action()
{
	//몬스터 미사일
	MosterMissaile();
	
	//몬스터 이동
	static int sw;
	int x = this->GetX();

	if (sw == 0){
		x++;
		this->SetX(x);

	}
	else if (sw == 1){
		x--;
		this->SetX(x);
	}

	if (sw == 0){
		if (this->GetX() >= 60) {
			x = x - 2;
			this->SetX(x);
			sw = 1;
		}
	}
	else if (sw == 1){
		if ((this->GetX() <= 10)){
			sw = 0;
		}
	}

	if (this->GetHP() == 0)
	{	//몬스터 false되면 삭제됨
		g_List.RemoveAt();
		delete this;
		
	}

	return true;
}




void  cs_Monster::MosterMissaile()
{

	int itemp;
	int missaliepencent;
	//20마리중에 한마리만 나가게함 생성자에 번호 부여
	itemp = rand() % 20 + 1;

	cs_MonsterMissaile *MonsterMissaile = new cs_MonsterMissaile(this->GetX(), this->GetY(), 4, true, 1);

	//1/10 확률로 미사일이 나감
	missaliepencent = rand() % 10 + 1;

	if (missaliepencent == 1){
		if ((this->Randomtemp == itemp) || (this->Randomtemp == 1))
		{
			//확률로 통해서 미사일 객체를 넣어줌
			g_List.InsertNext(MonsterMissaile);
		}
	}


}

