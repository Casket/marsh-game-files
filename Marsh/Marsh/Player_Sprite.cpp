#include "Main.h"

using namespace std;


Player_Sprite::Player_Sprite(std::string file_name, Direction cur_dir, int ani_delay, int rows, int cols, int total_frames)
:Sprite(file_name, cur_dir, ani_delay, rows, cols, total_frames)
{
	this->wearing_mask = false;
	this->flicker = false;
	if(this->frame_intial){
		intial_blank_frame();
		this->frame_intial = false;
	}

	this->image_frames = new BITMAP*[total_frames];
	for (int i=0; i < rows; i++){
		for (int j=0; j < cols; j++){
			this->image_frames[j+cols*i] = create_sub_bitmap(this->sprite_sheet,
				j*SPRITE_WIDTH, i*32, SPRITE_WIDTH, SPRITE_HEIGHT);
			
		}
	}
}


Player_Sprite::~Player_Sprite(void) {
	for (int i=0; i < this->total_frames; i++){
		destroy_bitmap(this->image_frames[i]);
	}
	delete [] this->image_frames;
	destroy_bitmap(this->blank_frame);

}
void Player_Sprite::intial_blank_frame(void){
	this->blank_frame = create_bitmap(1,1);
	clear_to_color(this->blank_frame, makecol(255, 0,255));
}
BITMAP* Player_Sprite::get_current_frame(void) {
	if (this->facing == None)
		return this->sprite_sheet;
	
	if(this->animation_frame == FLICKER_FRAME_NUMBER){
		this->animation_counter = this->animation_delay;
		this->update();	
		if (this->flicker){
			this->flicker = !this->flicker;
			return this->blank_frame;
		}
	}

	this->flicker = true;

	int y = this->wearing_mask ? 1 : 0;

	return this->image_frames[this->animation_frame + y*this->sheet_cols];
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
	switch(this->facing){
		case N:
			this->animation_frame = NORTH_START;
			break;
		case E:
			this->animation_frame = EAST_START;
			break;
		case S:
			this->animation_frame = SOUTH_START;
			break;
		case W:
			this->animation_frame = WEST_START;
			break;
		case NE:
			this->animation_frame = NORTH_EAST_START;
			break;
		case NW:
			this->animation_frame = NORTH_WEST_START;
			break;
		case SE:
			this->animation_frame = SOUTH_EAST_START;
			break;
		case SW:
			this->animation_frame = SOUTH_WEST_START;
			break;
	}
}

void Player_Sprite::check_casting(){

}

Sprite* Player_Sprite::clone(Direction dir){
	// TODO do this
	Player_Sprite* ps = new Player_Sprite(this->file, this->facing, this->animation_delay, this->sheet_rows, this->sheet_cols, this->total_frames);
	return ps;
}