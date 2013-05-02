#include "Main.h"

using namespace std;

Attack_Sprite::Attack_Sprite(std::string file_name, Direction cur_dir, int ani_delay, int rows, int cols, int total_frames, int width, int height)
:Sprite(file_name, cur_dir, ani_delay, rows, cols, total_frames)
{
	this->width = width;
	this->height = height;
	this->image_frames = new BITMAP*[this->total_frames];
	this->frames_to_charge = 0;
	this->frames_to_move = this->total_frames;
	this->frames_to_impact = 0;

	int rotation_angle = 0;
	switch(cur_dir){
		case NW:
			rotation_angle = 32;
			break;
		case N:
			rotation_angle = 64;
			break;
		case NE:
			rotation_angle = 96;
			break;
		case E:
			rotation_angle = 128;
			break;
		case SE:
			rotation_angle = 160;
			break;
		case S:
			rotation_angle = 192;
			break;
		case SW:
			rotation_angle = 224;
			break;
		default:
			rotation_angle = 0;
			break;
	}

	for (int i = 0; i < total_frames; i++){
		this->image_frames[i] = create_bitmap(width, height);
		BITMAP* sub_image = create_sub_bitmap(this->sprite_sheet, i*width, 0, width, height);
		clear_to_color(this->image_frames[i],makecol(255, 0, 255));
		rotate_sprite(this->image_frames[i], sub_image, 0, 0, itofix(rotation_angle));
		destroy_bitmap(sub_image);

	}
	this->current_state = AttackMoving;
	// use moving as the default, because charging is not possible for now

}

Attack_Sprite::~Attack_Sprite(void){
	for(int i = 0; i < this->total_frames; i++){
		destroy_bitmap(this->image_frames[i]);	
	}
	delete this->image_frames;
}

BITMAP* Attack_Sprite::get_current_frame(void){
	return this->image_frames[this->animation_frame];
}

void Attack_Sprite::update(void){
	if(++this->animation_counter <= this->animation_delay){
		return;
	}
	this->animation_counter = 0;
	this->animation_frame++;
	switch (this->current_state){
		case AttackCharging:
			if (this->animation_frame >= this->frames_to_charge)
				this->animation_frame = 0;
			break;

		case AttackMoving:
			if (this->animation_frame >= this->frames_to_move + this->frames_to_charge)
				this->animation_frame = this->frames_to_charge; // move over past the charging
			break;

		case AttackImpacting:
			if (this->animation_frame >= this->frames_to_move + this->frames_to_charge + this->frames_to_impact)
				this->animation_frame = this->frames_to_charge + this->frames_to_move; // shift again
			break;
		default:
			break;
	}
}

void Attack_Sprite::set_state_frame_counts(int charging, int moving, int impacting){
	this->frames_to_charge = charging;
	this->frames_to_move = moving;
	this->frames_to_impact = impacting;
}

void Attack_Sprite::casting_update(void){
	// update means it needs to move to the next state forward
	switch (this->current_state){
		case AttackCharging:
			this->current_state = AttackMoving;
			this->animation_frame = this->frames_to_charge;
			break;

		case AttackMoving:
			this->current_state = AttackImpacting;
			this->animation_frame = this->frames_to_charge+this->frames_to_move;
			break;

		case AttackImpacting:
			this->current_state = AttackImpacting;
			// means it's going to die soon or something probably, I don't really know
			break;
		default:
			break;
	}
}

void Attack_Sprite::check_casting(void){

}

Sprite* Attack_Sprite::clone(Direction dir){
	Attack_Sprite* as = new Attack_Sprite(this->file, dir, this->animation_delay, this->sheet_rows, this->sheet_cols, this->total_frames, this->width, this->height);
	as->set_state_frame_counts(this->frames_to_charge, this->frames_to_move, this->frames_to_impact);
	as->is_translucent = this->is_translucent;
	return as;

}
