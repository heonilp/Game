#include "stdafx.h"
#include "cLinkedlist.h"
#include "cs_BaseObject.h"
#include "console.h"

extern CLinkedlist <cs_BaseObject> g_List;

int cs_BaseObject::GetX()
{
	return X;

}
int cs_BaseObject::GetY()
{
	return Y;
}

void cs_BaseObject::SetX(int iX)
{
	X = iX;
}
void cs_BaseObject::SetY(int iY)
{
	Y = iY;

}


int cs_BaseObject::GetType(void)
{
	return Type;

}
void cs_BaseObject::SetType(int iType)
{
	Type = iType;
}


bool cs_BaseObject::GetFlag()
{
	return Flag;

}
void cs_BaseObject::SetFlag(bool iFlag)

{
	Flag = iFlag;
}



int cs_BaseObject::GetHP(void){

	return HP;
}

void cs_BaseObject::SetHP(int iHP)
{
	HP = iHP;

}