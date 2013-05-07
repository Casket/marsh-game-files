#ifndef TOWNGUARD_H
#define TOWNGUARD 1

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;

class Town_Guard:public Mob{
	public:
		Town_Guard(int, int, int, int, Sprite*, std::vector<std::pair<int,int>>*);
		~Town_Guard(void);
};

#endif