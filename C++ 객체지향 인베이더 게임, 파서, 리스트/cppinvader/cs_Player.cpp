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
	//HP�� 0�̸� �̾��ϱ�
	//LOSE �޴� ���
	if (this->GetHP() <= 0)
	{
		this->SetFlag(false);
		MenuLose();
	}


	return true;
}







