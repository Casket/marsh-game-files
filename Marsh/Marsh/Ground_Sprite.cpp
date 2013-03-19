#include "Ground_Sprite.h"

Ground_Sprite::Ground_Sprite(char* file_name, int tile_x, int tile_y){
	this->x = tile_x*TILE_WIDTH;
	this->y = tile_y*TILE_HEIGHT;
	this->frame = 0;
	this->total_frames = 13;
	this->sprite_sheet = load_bitmap(file_name, NULL);
}

Ground_Sprite::~Ground_Sprite(void) {
	destroy_bitmap(this->sprite_sheet);
}

void Ground_Sprite::load_sprite_sheet(char* name) {
	this->sprite_sheet = load_bitmap(name, NULL);
}

BITMAP* Ground_Sprite::get_current_frame(void) {
	int y = 0;
	//return this->sprite_sheet;

	BITMAP* pic = create_sub_bitmap(this->sprite_sheet,
		this->frame*TILE_WIDTH, y, TILE_WIDTH, TILE_HEIGHT);
	return pic;
}

void Ground_Sprite::change_frame(int frame){
	this->frame = frame;
}
