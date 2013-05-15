#pragma once

#include "Main.h"

class TargetSeeker : public Attack{

public:
	TargetSeeker(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats, iDrawable* target);
	~TargetSeeker(void);

	virtual void update_position(void);
	Attack* clone(int, int, Direction);
};