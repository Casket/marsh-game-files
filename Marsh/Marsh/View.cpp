#include "Main.h"

using namespace std;

void View::load_world(char* filename){
	this->current_world->load_world;
}

void View::update(void){
	
}

void View::draw_active_world(void){
	int tile_wide = this->current_world->get_tiles_wide;
	int tile_high = this->current_world->get_tiles_high;
	Tile*** tiles = this->current_world->tile_map;
	draw_interface(this->playa);
	draw_sprites(this->buffer, tiles, tile_wide, tile_high);

void draw_interface(Player* hero){
	draw_status(hero, this->buffer);
	draw_dialogs(this->buffer);
}

void draw_sprites(BITMAP* buffer, Tile*** tile_map, int tile_wide, int tile_high){
	for (int i=0; i<tile_wide; i++){
		for (int j=0; j<tile_high; j++){
			
		}
	}
}

void View::save_game(void){
	fstream fout;
	fout.put("default", ios::out);
	// do stuff
	fout.close;
}