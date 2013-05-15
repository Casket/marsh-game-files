#ifndef VAMPIRE_H
#define VAMPIRE 1

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;

class Vampire:public Mob{
	public:
		Vampire(int, int, int, int, Sprite*, std::vector<std::pair<int,int>>*);
		~Vampire(void);
		virtual void set_ranges(void);
		virtual int choose_attack(void);
		virtual void populate_spells(void);
};

#endif