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
	//�̻����� �ൿó�� 
	int mMissileY = this->GetY();

	//��ǥ�� �����ϸ� ������
	if (this->GetY() > 20)
	{
		this->SetFlag(false);
	}
	
	//Y��ǥ ���� �ö�
	
	this->Shot();

	//�޽��� �����ϰ� ����
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
		//���͹̻����� �Ʒ��γ�����
		mMissileY++;
		//����
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
		if (g_List.GetAt()->GetType() == 1)	//�÷��̾ 1	// ���Ͱ� 2 // �̻����� 3 //���� �̻����� 4
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



