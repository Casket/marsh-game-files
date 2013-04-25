#ifndef PORTAL_H
#define PORTAL_H 1

#include "Main.h"

class Portal : public iDrawable{
	
public:
	WorldName portal_to;
	bool activated;
	int target_x_pos;
	int target_y_pos;

public:
	Portal(int x, int y, Sprite* img, WorldName togo, int targ_x, int targ_y);
	~Portal(void);
	virtual void update(void);
	virtual void deal_with_attack(Attack* attack);
	virtual Portal* fetch_me_as_portal(void);


};

#endif