#include "Sprite_Solid.h"

Sprite_Solid::Sprite_Solid(char* file_name, int focus_x, int focus_y, int start_x, int start_y){
	this->focus_x = focus_x;
	this->focus_y = focus_y;
	this->start_x = start_x;
	this->start_y = start_y;
	//this->player_y = player_y;
	this->lighted = true;

	this->sprite_sheet = load_bitmap(file_name, NULL);
}

Sprite_Solid::~Sprite_Solid(void) {
	destroy_bitmap(this->sprite_sheet);
}

void Sprite_Solid::load_sprite_sheet(char* name) {
	this->sprite_sheet = load_bitmap(name, NULL);
}

void Sprite_Solid::set_lighted(bool light) {
	this->lighted = light;
}

BITMAP* Sprite_Solid::get_current_frame(void) {
	int y = 0;
	if (!this->lighted)
		y = 32;
	return this->sprite_sheet;

	//BITMAP* pic = create_sub_bitmap(this->sprite_sheet,
		//this->animation_frame*SPRITE_WIDTH, y, SPRITE_WIDTH, SPRITE_HEIGHT);
	//return pic;
}

void Sprite_Solid::update(BITMAP* buffer, BITMAP* scroll, int p_x_pos, int p_y_pos) {
	//if (++this->animation_counter <= this->animation_delay)
	// not ready for a frame update
	//int state_multiplier;
	//switch(this->current_state) {
	//case Normal:
		//state_multiplier = 1;
		//break;
		// TODO implement this when we have states and stuffs
	//}
	this->src_x = p_x_pos - this->start_x;
	this->src_y = p_y_pos - this->start_y;
	this->dst_x = 0;
	this->dst_y = 0;
	this->buff_scrl = buffer;
	//int current_frame = this->animation_frame;
	//Direction current_direction = this->facing;

	//this->animation_frame = current_frame;
	//this->animation_counter = 0; // reset the delay counter



}