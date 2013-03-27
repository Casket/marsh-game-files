#include "Solid_Sprite.h"

Solid_Sprite::Solid_Sprite(char* file_name)
:Sprite(file_name, W, 0,0,0,0)
{
	
}

Solid_Sprite::~Solid_Sprite(void) {

}


BITMAP* Solid_Sprite::get_current_frame(void) {
	int y = 0;
	if (!this->lighted)
		y = 32;
	return this->sprite_sheet;

	//BITMAP* pic = create_sub_bitmap(this->sprite_sheet,
		//this->animation_frame*SPRITE_WIDTH, y, SPRITE_WIDTH, SPRITE_HEIGHT);
	//return pic;
}

void Solid_Sprite::update() {
	//if (++this->animation_counter <= this->animation_delay)
	// not ready for a frame update
	//int state_multiplier;
	//switch(this->current_state) {
	//case Normal:
		//state_multiplier = 1;
		//break;
		// TODO implement this when we have states and stuffs
	//}
	/*
	this->src_x = p_x_pos - this->start_x;
	this->src_y = p_y_pos - this->start_y;
	this->dst_x = 0;
	this->dst_y = 0;
	this->buff_scrl = buffer;
	*/ 
	//int current_frame = this->animation_frame;
	//Direction current_direction = this->facing;

	//this->animation_frame = current_frame;
	//this->animation_counter = 0; // reset the delay counter
}

void Solid_Sprite::casting_update(void){

}

void Solid_Sprite::check_casting(void){

}

Sprite* Solid_Sprite::clone(void){
	// TODO write this
	return NULL;
}