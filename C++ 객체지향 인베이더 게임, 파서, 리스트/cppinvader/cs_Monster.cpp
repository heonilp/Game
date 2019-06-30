#include "stdafx.h"
#include "Global.h"
#include "cs_Monster.h"
#include "cs_MonsterMissaile.h"

void cs_Monster::Draw()
{
	//���� ü�¸��� �׸���
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
	//���� �̻���
	MosterMissaile();
	
	//���� �̵�
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
	{	//���� false�Ǹ� ������
		g_List.RemoveAt();
		delete this;
		
	}

	return true;
}




void  cs_Monster::MosterMissaile()
{

	int itemp;
	int missaliepencent;
	//20�����߿� �Ѹ����� �������� �����ڿ� ��ȣ �ο�
	itemp = rand() % 20 + 1;

	cs_MonsterMissaile *MonsterMissaile = new cs_MonsterMissaile(this->GetX(), this->GetY(), 4, true, 1);

	//1/10 Ȯ���� �̻����� ����
	missaliepencent = rand() % 10 + 1;

	if (missaliepencent == 1){
		if ((this->Randomtemp == itemp) || (this->Randomtemp == 1))
		{
			//Ȯ���� ���ؼ� �̻��� ��ü�� �־���
			g_List.InsertNext(MonsterMissaile);
		}
	}


}

