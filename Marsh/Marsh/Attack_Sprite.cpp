#include "Main.h"

using namespace std;

Attack_Sprite::Attack_Sprite(char* file_name, Direction cur_dir, int ani_delay, int rows, int cols, int total_frames, int width, int height)
							 :Sprite(file_name, cur_dir, ani_delay, rows, cols, total_frames)
{
	this->width = width;
	this->height = height;
	this->image_frames = new BITMAP*[this->total_frames];

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

	if(++this->animation_frame >= total_frames){
		this->animation_frame = 0;
	}

	this->animation_counter = 0;
}

void Attack_Sprite::casting_update(void){

}

void Attack_Sprite::check_casting(void){

}

Sprite* Attack_Sprite::clone(Direction dir){
	Attack_Sprite* as = new Attack_Sprite(this->file, dir, this->animation_delay, this->sheet_rows, this->sheet_cols, this->total_frames, this->width, this->height);
	return as;

}
