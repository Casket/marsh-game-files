#include "Main.h"

iDrawable::iDrawable(int x, int y, int vel, int vel_d, Sprite* img){
	this->x_pos = x;
	this->y_pos = y;
	this->velocity = vel;
	this->velocity_delay = vel_d;
	this->image = img;
}

iDrawable::~iDrawable(void){
	if (this->image != NULL)
		delete this->image;
}


void iDrawable::set_sprite(Sprite* spr){
	this->image = spr;
}

void iDrawable::set_world(World* world){
	this->my_world = world;
}

Sprite* iDrawable::get_image(void){
	return this->image;
}

int iDrawable::get_x_pos(void){
	return this->x_pos;
}

int iDrawable::get_y_pos(void){
	return this->y_pos;
}

int iDrawable::get_reference_x(void){
	return this->x_pos + this->reference_horizontal;
}

int iDrawable::get_reference_y(void){
	return this->y_pos + this->reference_vertical;
}

void iDrawable::set_boundary_value(int bounding_width, int bounding_height, int reference_horizontal, int reference_vertical){
	this->bounding_width = bounding_width;
	this->bounding_height = bounding_height;
	this->reference_horizontal = reference_horizontal;
	this->reference_vertical = reference_vertical;
}

int iDrawable::get_bounding_width(void){
	return this->bounding_width; 
}

int iDrawable::get_bounding_height(void){
	return this->bounding_height;
}

World* iDrawable::get_world(void){
	return this->my_world;
}

void iDrawable::set_x_pos(int x){
	this->x_pos = x;
}
void iDrawable::set_y_pos(int y){
	this->y_pos = y;
}