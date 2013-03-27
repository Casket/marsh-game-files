#include "Main.h"
#include "Ground_Sprite.h"

using namespace std;

Ground_Sprite::Ground_Sprite(char* file_name, int tile_x, int tile_y)
:Sprite(file_name, W, 0,0,0,0)
{
	this->x = tile_x*TILE_WIDTH;
	this->y = tile_y*TILE_HEIGHT;
	this->frame = tile_x;
	this->total_frames = 13;
	this->sprite_sheet = load_bitmap(file_name, NULL);
	
	for(int i = 0; i < 13; i++){
		this->frames[i] = create_sub_bitmap(this->sprite_sheet, i*TILE_WIDTH, 0,
			TILE_WIDTH, TILE_HEIGHT);
	}
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

void Ground_Sprite::update(){
	// do nothing
}

void Ground_Sprite::casting_update(){
	// do nothing
}

void Ground_Sprite::check_casting(){
	// MOAR NOTHING
}

Sprite* Ground_Sprite::clone(void){
	// TODO implement this
	return NULL;
}