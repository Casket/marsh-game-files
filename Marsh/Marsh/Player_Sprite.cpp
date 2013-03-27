#include "Main.h"

using namespace std;


Player_Sprite::Player_Sprite(char* file_name, Direction cur_dir, int ani_delay, int rows, int cols, int total_frames)
:Sprite(file_name, cur_dir, ani_delay, rows, cols, total_frames)
{
	// do nothing for now
}

Player_Sprite::~Player_Sprite(void) {

}

BITMAP* Player_Sprite::get_current_frame(void) {
	int y = 0;
	if (!this->lighted)
		y = 32;
	
	if (this->facing == None)
		return this->sprite_sheet;

	BITMAP* pic = create_sub_bitmap(this->sprite_sheet,
		this->animation_frame*SPRITE_WIDTH, y, SPRITE_WIDTH, SPRITE_HEIGHT);
	return pic;
}

void Player_Sprite::update(void) {
	if (++this->animation_counter <= this->animation_delay)
		return;	// not ready for a frame update

	int state_multiplier;
	switch(this->current_state) {
	case Normal:
		state_multiplier = 1;
		break;
		// TODO implement this when we have states and stuffs
	}
	
	int current_frame = this->animation_frame;
	Direction current_direction = this->facing;

	switch(current_direction) {
	case N:
		current_frame++;
		if (current_frame > state_multiplier*NORTH_END)
			current_frame = state_multiplier*NORTH_START;
		if (current_frame < state_multiplier*NORTH_START)
			current_frame = state_multiplier*NORTH_START;
		// last case is a bit of a fail safe here
		//  only invoked if direction is not reflected in the current frame
		break;
	case S:
		current_frame++;
		if (current_frame > state_multiplier*SOUTH_END)
			current_frame = state_multiplier*SOUTH_START;
		if (current_frame < state_multiplier*SOUTH_START)
			current_frame = state_multiplier*SOUTH_START;
		break;
	case W:
		current_frame++;
		if (current_frame > state_multiplier*WEST_END)
			current_frame = state_multiplier*WEST_START;
		if (current_frame < state_multiplier*WEST_START)
			current_frame = state_multiplier*WEST_START;
		break;
	case E:
		current_frame++;
		if (current_frame > state_multiplier*EAST_END)
			current_frame = state_multiplier*EAST_START;
		if (current_frame < state_multiplier*EAST_START)
			current_frame = state_multiplier*EAST_START;
		break;
		
	default:
		// means that they are facing in a non-cardinal direction
		// probably should't update
		break;
	}

	this->animation_frame = current_frame;
	this->animation_counter = 0; // reset the delay counter
}

void Player_Sprite::casting_update(){

}

void Player_Sprite::check_casting(){

}

Sprite* Player_Sprite::clone(void){
	// TODO do this
	return NULL;
}