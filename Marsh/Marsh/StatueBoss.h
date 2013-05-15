#pragma once

#include "Main.h"

class StatueBoss : public Mob{
public:
	StatueBoss(int x, int y, Sprite* img);
	~StatueBoss(void);

	virtual void populate_spells(void);
	virtual int choose_attack(void);

};