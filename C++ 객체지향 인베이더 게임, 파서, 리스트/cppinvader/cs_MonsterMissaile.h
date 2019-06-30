#ifndef __CS_MONSTERMISSAILE_H__
#define __CS_MONSTERMISSAILE_H__


class cs_MonsterMissaile : public cs_BaseObject
{
private:
	int Angle;
	
public:

	cs_MonsterMissaile(int iX, int iY, int iType, bool bFlag, int iHP)
		:cs_BaseObject(iX, iY, iType, bFlag, iHP)
	{
		Angle = 90;
	}
	~cs_MonsterMissaile()
	{
	
	}
	void Shot(void);
	virtual void Move(void);
	virtual void Draw(void);
	virtual bool Action(void);
	double SinX(double g, int iAngle);
};


#endif