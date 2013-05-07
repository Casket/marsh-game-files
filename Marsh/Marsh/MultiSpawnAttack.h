#pragma once

#include "Main.h"

using namespace std;

class MultiSpawnAttack: public Attack {
	int spawn_delay;
	int delay_counter;
	Attack* attack_spawning;


public:

	MultiSpawnAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats, Attack* spawning, int delay);
	~MultiSpawnAttack(void);
	virtual void update(void);
	virtual void start_death_sequence(void);
	virtual void deal_with_attack(Attack*);
	virtual Attack* clone(int, int, Direction);
	


};