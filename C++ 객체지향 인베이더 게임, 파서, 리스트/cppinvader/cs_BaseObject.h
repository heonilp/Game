#ifndef __CS_BASEOBJECT_H__
#define __CS_BASEOBJECT_H__

class cs_BaseObject
{
	
private:
	int X;
	int Y;
	int Type;
	bool Flag;
	int HP;
	
public:
	cs_BaseObject(int iX, int iY, int iType, bool iFlag, int iHP) :X(iX), Y(iY), Type(iType), Flag(iFlag), HP(iHP)
	{}
	int GetHP(void);
	void SetHP(int iHP);
	int GetX(void);
	int GetY(void);
	void SetX(int iX);
	void SetY(int iY);
	int GetType(void);
	void SetType(int iType);
	bool GetFlag(void);
	void SetFlag(bool iFlag);
	virtual void Move(void) = 0;
	virtual void Draw(void) = 0;
	virtual bool Action(void) = 0;
	void Shot(){}

};



#endif