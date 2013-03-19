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
	delete this->image;
}

void Drawable::update(void){
	//Not needed as these objects don't move in this level
	return;
}

void Drawable::deal_with_attack(Attack* attack){
	//cause attack to start its dieing proces

}

void Drawable::set_sprite(Sprite* spr){
	this->image = spr;
}

void Drawable::set_world(World* world){
	this->my_world = world;
}