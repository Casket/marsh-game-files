#pragma once 

#include "Main.h"

class HealthDrainAttack: public Attack{
public:
	HealthDrainAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats);
	~HealthDrainAttack(void);

	virtual void attack_target(iDrawable* target);
	virtual void start_death_sequence(void);
	virtual Attack* clone(int, int, Direction);

};
