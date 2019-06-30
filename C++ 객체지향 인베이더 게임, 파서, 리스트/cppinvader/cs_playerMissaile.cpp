#include "stdafx.h"
#include "Global.h"
#include "cs_playerMissaile.h"



void cs_playMissaile::Draw()
{
	//true�� �̻����� �׷���
	if (GetFlag() == true)
	{
		Sprite_Draw(this->GetX(), this->GetY(), '^');
	}
}



void cs_playMissaile::Move(void)
{
	
	//�̻����� Y��ǥ�� �޾ƿ�
	int MissileY = this->GetY();

	//�̻��� ���� �� ���� ��ü �����Ѵ�.
	//�÷��̾ 1	// ���Ͱ� 2 // �̻����� 3

	//��ǥ�� �����ϸ� ������
	if (this->GetY() < 2)
	{
		this->SetFlag(false);

	}
	//�̻����� �ൿó�� 

	//Y��ǥ ���� �ö�
	if (this->GetFlag() == true)
	{
		MissileY--;
		this->SetY(MissileY);
	}

	if (this->GetFlag() == false)
	{
		//�ڽ� this�� ����
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
		if (g_List.GetAt()->GetType() == 2)	//�÷��̾ 1	// ���Ͱ� 2 // �̻����� 3
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

