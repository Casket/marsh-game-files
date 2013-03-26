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

Sprite* Drawable::get_image(void){
	return this->image;
}

int Drawable::get_x_pos(void){
	return this->x_pos;
}

int Drawable::get_y_pos(void){
	return this->y_pos;
}

int Drawable::get_reference_x(void){
	return this->x_pos + this->reference_horizontal;
}

int Drawable::get_reference_y(void){
	return this->y_pos + this->reference_vertical;
}

void Drawable::set_reference_horizontal(int x){
	this->reference_horizontal = x;
}

void Drawable::set_reference_vertical(int y){
	this->reference_vertical = y;
}

World* Drawable::get_world(void){
	return this->my_world;
}

void Drawable::set_x_pos(int x){
	this->x_pos = x;
}
void Drawable::set_y_pos(int y){
	this->y_pos = y;
}

