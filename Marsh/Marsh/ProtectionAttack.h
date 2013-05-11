#pragma once

#include "Main.h"

using namespace std;

enum WardType { ReflectAttack, ShieldAttack};

class ProtectionAttack : public Attack{
WardType ward;
public:
	ProtectionAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats, WardType type);
	~ProtectionAttack(void);
	
	virtual void deal_with_attack(Attack* attack);
	virtual void update(void);
	virtual Attack* clone(int, int, Direction);
	virtual void start_death_sequence(void);

};