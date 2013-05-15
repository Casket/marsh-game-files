#ifndef WORLDPORTAL_H
#define WORLDPORTAL_H 1

#include "Main.h"
#include "Portal.h"


class WorldPortal : public Portal{
	
public:
	WorldPortal(int x, int y, Sprite* img, WorldName togo);
	~WorldPortal(void);
	virtual void update(void);
};

#endif