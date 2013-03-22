#ifndef DRAWABLE_H
#define DRAWABLE_H 1

#include "Main.h"

using namespace std;

class World;

class Drawable{
	protected:
		Sprite* image;
		int x_pos, y_pos;
		int velocity, velocity_delay;
		World* my_world;
	
	public:
		Drawable(int x, int y, int vel, int vel_d, Sprite* img);
		~Drawable(void);
		void update(void);
		void deal_with_attack(Attack* attack);
		void set_sprite(Sprite* spr);
		void set_world(World* world);
		Sprite* get_image(void);
		int get_x_pos(void);
		int get_y_pos(void);
		World* get_world(void);
		void set_x_pos(int x);
		void set_y_pos(int y);

};


#endif