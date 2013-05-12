#ifndef TGCAPTAIN_H
#define TGCAPTAIN 1

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;

class GuardCaptain:public Mob{
	public:
		GuardCaptain(int, int, int, int, Sprite*, std::vector<std::pair<int,int>>*);
		~GuardCaptain(void);
		virtual void set_ranges(void);
		virtual int choose_attack(void);
};

#endif