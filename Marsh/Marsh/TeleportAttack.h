#pragma once

#include "Main.h"

class TeleportAttack: public Attack{
int distance;
public:
	TeleportAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats, int distance);
	~TeleportAttack(void);

	virtual void update(void);
	virtual Attack* clone(int, int, Direction);
	bool check_new_coord(int x, int y);

};