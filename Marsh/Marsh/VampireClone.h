#pragma once

#include "Main.h"
#include "VampireClone.h"
#include "Vampire.h"
#include "VampireBoss.h"

class VampireBoss;

class VampireClone: public Vampire{
	VampireBoss* owner;

public:
	VampireClone(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int, int>>* ways, VampireBoss* boss);
	~VampireClone(void);

	virtual void upon_death(void);

};