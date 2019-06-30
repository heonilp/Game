#ifndef __CS_PLAYERMISSAILE_H__
#define __CS_PLAYERMISSAILE_H__

class cs_playMissaile : public cs_BaseObject
{
private:

public:

	cs_playMissaile(int iX, int iY, int iType, bool bFlag,int iHP)
		:cs_BaseObject(iX, iY, iType, bFlag, iHP)
	{
	
	}

	~cs_playMissaile(){}
	virtual void Move(void);
	virtual void Draw(void);
	virtual bool Action(void);


};


#endif