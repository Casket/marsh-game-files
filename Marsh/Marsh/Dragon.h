#ifndef DRAGON_H
#define DRAGON 1

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;

class Dragon:public Mob{
	public:
		Dragon(int, int, int, int, Sprite*, std::vector<std::pair<int,int>>*);
		~Dragon(void);
		virtual void set_ranges(void);
		virtual int choose_attack(void);
};

#endif