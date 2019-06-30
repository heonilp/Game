#ifndef __CS_PLAYER_H__
#define __CS_PLAYER_H__

class cs_Player : public cs_BaseObject
{

private:

public:
	cs_Player(int iX, int iY,int iType, bool bFlag ,int iHP) 
		:cs_BaseObject(iX, iY, iType, bFlag,iHP)
	{
	}


	~cs_Player()
	{

	}
	virtual void Draw(void);
	virtual void Move(void){}
	virtual bool Action(void);

};



#endif