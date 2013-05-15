#pragma once

#include "Main.h"
#include "Vampire.h"
#include "VampireClone.h"

class VampireClone;

class VampireBoss: public Vampire{
	Vampire* first_clone;
	Vampire* second_clone;
public:
	VampireBoss(int, int, int, int, Sprite*, std::vector<std::pair<int, int>>*);
	~VampireBoss(void);

	virtual int choose_attack(void);
	virtual void populate_spells(void);
	void spawn_clones(void);
	void alert_clone_death(VampireClone* clone);
};