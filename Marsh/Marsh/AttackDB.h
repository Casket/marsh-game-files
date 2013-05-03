#pragma once

#include "Main.h"

using namespace std;

#define LONG_RANGE VISIBLE_W / 2
#define MID_RANGE VISIBLE_W / 4
#define SHORT_RANGE VISIBLE_W / 8

#define SHADOW_BALL 0
#define SHADOW_BALL_PLUS 1
#define SHADOW_WAVE 2
#define SHADOW_NOVA 3

#define SHADOW_NEEDLE 4
#define SHADOW_NEEDLE_PLUS 5
#define SHADOW_SPIKES 6
#define DEATH_BEAM 7

#define DRAIN_HEALTH 8
#define BIND_IN_SHADOWS 9
#define MASS_SLOW 10
#define SHADOW_DASH 11
#define TELEPORT 12
#define SHADOW_STEPS 13
#define SHIELD 14
#define DEFLECT 15

class AttackDB {
	std::map<int, Attack*>* attacks_by_id;

public:
	AttackDB(void);
	~AttackDB(void);
	Attack* fetch_attack(int attack_id);

private:
	void populate_penetration_spells(void);
	void populate_utility_spells(void);
	void populate_damage_spells(void);
};