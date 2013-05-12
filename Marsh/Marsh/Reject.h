#ifndef REJECT_H
#define REJECT 1

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;

class Reject:public Mob{
	public:
		Reject(int, int, int, int, Sprite*, std::vector<std::pair<int,int>>*);
		~Reject(void);
		virtual void set_ranges(void);
		virtual int choose_attack(void);

};

#endif