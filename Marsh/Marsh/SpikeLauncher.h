#pragma once

#include "Main.h"

using namespace std;

class SpikeLauncher: public Attack{
int spawn_delay;
int spawn_counter;
int spawn_num;
Attack* attack_spawn;


public:
	SpikeLauncher(int x, int y, Sprite* img, Attack* to_launch, AttackStatistics stats, int delay, int spawn_num);
	~SpikeLauncher(void);

	virtual Attack* clone(int, int, Direction);
	virtual void update(void);
	virtual void start_death_sequence(void);


};