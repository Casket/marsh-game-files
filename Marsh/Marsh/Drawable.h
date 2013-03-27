#ifndef DRAWABLE_H
#define DRAWABLE_H 1

#include "Main.h"

using namespace std;

class World;

class Drawable : public iDrawable{		
	
	public:
		Drawable(int x, int y, int vel, int vel_d, Sprite* img);
		~Drawable(void);
		virtual void update(void);
		virtual void deal_with_attack(Attack* attack);	
		virtual void check_collisions(void);

};


#endif