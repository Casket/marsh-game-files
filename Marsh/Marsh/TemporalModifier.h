#pragma once

#include "Main.h"

class TemporalModifier: public Attack{
int delay;
public:
	TemporalModifier(Sprite* img, AttackStatistics stats, int update_rate);
	~TemporalModifier(void);
	virtual void update(void);
	virtual Attack* clone(int x, int  y, Direction dir);
};