#pragma once

#include "Main.h"

class StationaryAttack: public Attack{
public:
	StationaryAttack(int x, int y, Sprite* img, AttackStatistics stats);
	~StationaryAttack(void);
	virtual Attack* clone(int, int, Direction);
	virtual void update_position(void);
	virtual void start_death_sequence(void);

};