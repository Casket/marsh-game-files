#pragma once

#include "Main.h"

using namespace std;

class AttackCharge : public iDrawable{
	int duration;
	int living_timer;

public:
	AttackCharge(int x, int y, Sprite* img, int longevity);
	~AttackCharge(void);
	virtual void update(void);
	virtual void deal_with_attack(Attack* attack);
};