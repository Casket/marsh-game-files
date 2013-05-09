#include "Main.h"
#include "Ground_Sprite.h"

using namespace std;

Ground_Sprite::Ground_Sprite(std::string file_name, int tile_x, int tile_y)
:Sprite(file_name, W, 0,0,0,0)
{
	this->x = tile_x*TILE_WIDTH;
	this->y = tile_y*TILE_HEIGHT;
	this->frame = tile_x;
	this->total_frames = 13;

	this->image = create_bitmap(TILE_WIDTH, TILE_HEIGHT);
	if (!this->image){
		allegro_message("Bad news bears: a ground tile failed to get a sprite.", allegro_error);
		exit(-1);
	}
	blit(this->sprite_sheet, this->image, tile_x*TILE_WIDTH, 0, 0, 0, TILE_WIDTH, TILE_HEIGHT);

	/*
	for(int i = 0; i < 13; i++){
		this->frames[i] = create_sub_bitmap(this->sprite_sheet, i*TILE_WIDTH, 0,
			TILE_WIDTH, TILE_HEIGHT);
	}
	*/

	destroy_bitmap(this->sprite_sheet);
	
}

Ground_Sprite::~Ground_Sprite(void) {
	destroy_bitmap(this->image);
	delete [] this->frames;
}

void Ground_Sprite::load_sprite_sheet(std::string name) {
	this->sprite_sheet = load_bitmap(name.c_str(), NULL);
}

BITMAP* Ground_Sprite::get_current_frame(void) {
	int y = 0;
	//return this->sprite_sheet;

	//return this->frames[this->frame];
	return this->image;
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