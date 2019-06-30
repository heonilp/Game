#ifndef __CS_MONSTER_H__
#define __CS_MONSTER_H__


class cs_Monster : public cs_BaseObject
{
private:
	//생성자 마다 번호를 부여함
	int Randomtemp;
public:
	
	cs_Monster(int iX, int iY, int iType, bool bFlag, int iHP, int iRandomtemp)
		:cs_BaseObject(iX, iY, iType, bFlag, iHP), Randomtemp(iRandomtemp)
	{}

	~cs_Monster()
	{}

	virtual void Draw(void);
	virtual void Move(void){}
	virtual bool Action(void);
	void MosterMissaile(void);
};

#endif