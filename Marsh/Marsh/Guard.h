#ifndef GUARD_H
#define GUARD_H 1

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;

class Guard: public Combat{
	
	public:
		Guard(int x, int y, int vel, int vel_d, Sprite* img);
		~Guard(void);
		virtual void update(void);
		virtual void deal_with_attack(Attack* attack);

	private:
	//

};

#endif