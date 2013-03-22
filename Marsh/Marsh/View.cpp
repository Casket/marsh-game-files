#include "Main.h"

using namespace std;

View::View(Player* hero){
	this->playa = hero;
	this->buffer = create_bitmap(3000, 3000);
	for (int i=0; i < MAX_NUMBER_WORLDS; i++)
		this->loaded_worlds[i] = NULL;
}

View::~View(void){
	if (this->playa != NULL)
		delete this->playa;
	for (int i=0; i < MAX_NUMBER_WORLDS; i++){
		if (this->loaded_worlds[i] != NULL)
			delete this->loaded_worlds[i];
	}
	if (this->buffer != NULL)
		destroy_bitmap(this->buffer);
	this->current_world = NULL;
}


void View::load_world(char* filename){
	if (this->current_world != NULL){
		put_world_in_loaded(this->current_world);
	}

	this->current_world = new World(TEST_WORLD_SIZE, TEST_WORLD_SIZE);
	this->current_world->load_world(filename);
}

void View::put_world_in_loaded(World* world){
	for (int i=0; i < MAX_NUMBER_WORLDS; i++) {
		if (this->loaded_worlds[i] == NULL) {
			this->loaded_worlds[i] = world;
		} else {
			if (world->equals(this->loaded_worlds[i]))
				return;
		}
	}
}

void View::update(void){

}

void View::draw_active_world(void){
	clear_bitmap(this->buffer);
	int tile_wide = this->current_world->get_tiles_wide();
	int tile_high = this->current_world->get_tiles_high();
	Tile*** tiles = this->current_world->get_tile_map();
	draw_sprites(this->buffer, tiles, tile_wide, tile_high);
	draw_interface(this->playa);

}

void draw_status(Player* hero, BITMAP* buffer){

}

void draw_dialogs(BITMAP* buffer){

}

void View::draw_interface(Player* hero){
	draw_status(hero, this->buffer);
	draw_dialogs(this->buffer);
}



void View::draw_sprites(BITMAP* buffer, Tile*** tile_map, int tile_wide, int tile_high){
	for (int i=0; i<tile_wide; i++){
		for (int j=0; j<tile_high; j++){
			masked_blit(tile_map[i][j]->background_image->get_current_frame(),
				buffer, 0,0, i*TILE_SIZE, j*TILE_SIZE, TILE_SIZE, TILE_SIZE);
			// draw the background image for starters
			draw_drawables(buffer, tile_map[i][j]->contents);
		}
	}
}

void View::draw_drawables(BITMAP* buffer, std::list<Drawable*> sprites){
	std::list<Drawable*>::iterator iter;
	for (iter = sprites.begin(); iter != sprites.end(); ++iter){
		BITMAP* frame = (*iter)->get_image()->get_current_frame();
		int x = (*iter)->get_x_pos();
		int y = (*iter)->get_y_pos();
		int width = frame->w;
		int height = frame->h;
		masked_blit(frame, buffer, 0,0, x, y, width, height);
	}
}

void View::save_game(void){
	/*
	fstream fout;
	fout.put("default", ios::out);
	// do stuff
	fout.close;
	*/ 
}