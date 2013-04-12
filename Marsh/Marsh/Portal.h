#ifndef PORTAL_H
#define PORTAL_H 1

#include "Main.h"

class Portal : public iDrawable{
	
public:
	WorldName portal_to;

public:
	Portal(int x, int y, Sprite* img, WorldName togo);
	~Portal(void);
	virtual void update(void);
	virtual void deal_with_attack(Attack* attack);


};

#endif