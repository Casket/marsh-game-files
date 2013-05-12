#ifndef MARSHMONSTER_H
#define MARSHMONSTER 1

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;

class Marsh_Monster:public Mob{
	public:
		Marsh_Monster(int, int, int, int, Sprite*, std::vector<std::pair<int,int>>*);
		~Marsh_Monster(void);
		virtual void check_collisions(void);
		
};

#endif