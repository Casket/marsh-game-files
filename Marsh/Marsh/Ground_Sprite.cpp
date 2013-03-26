#include "Main.h"
#include "Ground_Sprite.h"

Ground_Sprite::Ground_Sprite(char* file_name, int tile_x, int tile_y){
	this->x = tile_x*TILE_WIDTH;
	this->y = tile_y*TILE_HEIGHT;
	this->frame = tile_x;
	this->total_frames = 13;
	this->sprite_sheet = load_bitmap(file_name, NULL);
	this->frames[0] = create_sub_bitmap(this->sprite_sheet, tile_x*TILE_WIDTH, 0,
		TILE_WIDTH, TILE_HEIGHT);
	this->frames[1] = create_sub_bitmap(this->sprite_sheet, tile_x*TILE_WIDTH, 0,
		TILE_WIDTH, TILE_HEIGHT);
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

	return this->frames[this->frame];
	//try to use an instance variable for the bitmap so it can be destroyed when the bitmap is free
}

void Ground_Sprite::change_frame(int frame){
	this->frame = frame;
}
