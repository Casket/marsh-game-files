#pragma once

#include "Main.h"

class SprintSpell: public Attack{
int velocity_to_receive;
int previous_velocity;

public:
	SprintSpell(int vel, int duration, int charge);
	~SprintSpell(void);
	virtual void update(void);
	virtual Attack* clone(int, int, Direction);

};