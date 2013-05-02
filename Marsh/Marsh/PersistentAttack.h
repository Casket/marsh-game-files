#pragma once

#include "Main.h"

using namespace std;

class PersistentAttack : public Attack {
	
public:
	PersistentAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats);
	~PersistentAttack(void);
	virtual Attack* clone(int, int, Direction);
	virtual void start_death_sequence(void);
	virtual bool detect_collisions(void);


};