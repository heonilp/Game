#include "stdafx.h"
#include "Global.h"
#include "cs_Player.h"

extern void MenuLose(void);

void cs_Player::Draw()
{
	if (this->GetHP() > 0)
	{
		this->SetFlag(true);
		Sprite_Draw(this->GetX(), this->GetY(), '#');
	}
}






bool cs_Player::Action()
{
	//HP가 0이면 이어하기
	//LOSE 메뉴 출력
	if (this->GetHP() <= 0)
	{
		this->SetFlag(false);
		MenuLose();
	}


	return true;
}







