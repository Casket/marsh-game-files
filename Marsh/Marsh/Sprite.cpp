#include "Sprite.h"

using namespace std;

Sprite::Sprite(std::string file_name, Direction cur_dir, int ani_delay, int rows, int cols, int total_frames){
	this->animation_counter = 0;
	this->animation_delay = ani_delay;
	this->animation_frame = 0;
	this->facing = cur_dir;
	this->sheet_cols = cols;
	this->sheet_rows = rows;
	this->total_frames = total_frames;

	this->lighted = true;
	this->current_state = Normal;
	this->file = file_name;
	this->wearing_mask = false;
	this->is_translucent = false;

	this->sprite_sheet = load_bitmap(file_name.c_str(), NULL);
	if (this->sprite_sheet == NULL){
		allegro_message(file_name.c_str());
		exit(0);
	}
}

Sprite::~Sprite(void) {
	if (this->sprite_sheet)
		destroy_bitmap(this->sprite_sheet);
}

void Sprite::load_sprite_sheet(std::string name) {
	this->sprite_sheet = load_bitmap(name.c_str(), NULL);
	
}

Direction Sprite::get_facing(void) {
	return this->facing;
}

void Sprite::set_facing(Direction d) {
	this->facing = d;
}

Sprite* Sprite::clone(Direction dir){
	return NULL;
}

void Sprite::force_direction_update(Direction dir){
	this->facing = dir;
	switch (dir){
		case N:
			this->animation_frame = NORTH_START;
			break;
		case S:
			this->animation_frame = SOUTH_START;
			break;
		case E:
			this->animation_frame = EAST_START;
			break;
		case W:
			this->animation_frame = WEST_START;
			break;
		default:
			break;
	}
}

