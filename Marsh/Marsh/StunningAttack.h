#pragma once

#include "Main.h"

using namespace std;

class StunningAttack : public Attack{
public:
	Combat* caught_target;
	int caught_timer;
	int caught_duration;


public:
	StunningAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats);
	~StunningAttack(void);
	virtual void update(void);
	virtual bool detect_collisions(void);
	virtual void start_death_sequence(void);
	virtual Attack* clone(int, int, Direction);
	void entrap_my_target(void);
	virtual iDrawable* get_above_target(void);


};