#include "Main.h"

Drawable::Drawable(int x, int y, int vel, int vel_d, Sprite *img)
{
	this->x_pos = x;
	this->y_pos = y;
	this->velocity = vel;
	this->velocity_delay = vel_d;
	this->image = img;
}

Drawable::~Drawable(void) {

}

void update(void){

}

void deal_with_attack(Attack* attack){

}

void set_sprite(Sprite* spr){
	
}