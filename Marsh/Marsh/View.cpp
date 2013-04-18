#include "Main.h"


using namespace std;

View::View(Player* hero){
	this->playa = hero;
	this->buffer = create_bitmap(SCREEN_W + PAD, SCREEN_H + PAD);
	clear_bitmap(this->buffer);
	for (int i=0; i < MAX_NUMBER_WORLDS; i++)
		this->loaded_worlds[i] = NULL;
	this->current_world = NULL;
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


	//TODO FIX THIS CAUSE ITS BAD
	delete this->current_world;
	this->current_world = NULL;
}


void View::load_world(char* filename){
	if (this->current_world != NULL){
		put_world_in_loaded(this->current_world);
	}

	this->current_world = new World(45, 34);
	this->current_world->load_world(filename);
	this->current_world->set_player(this->playa);
	this->current_world->insert_entity(this->playa);
	this->playa->set_world(this->current_world);
	
	std::vector<std::pair<int, Direction>>* ways = new std::vector<std::pair<int,Direction>>();
	std::pair<int, Direction> test = std::make_pair(-1, N);
	ways->insert(ways->end(), test);
	std::pair<int, Direction> test2 = std::make_pair(30, S);
	ways->insert(ways->end(), test2);
	//std::pair<int, Direction> test3 = std::make_pair(3, W);
	//ways->insert(ways->end(), test3);
	//std::pair<int, Direction> test4 = std::make_pair(30, E);
	//ways->insert(ways->end(), test4);
	std::pair<int, Direction> test5 = std::make_pair(-1,N);
	ways->insert(ways->end(), test5);
	

	Town_Guard* g = new Town_Guard(300,450,0,0,new Player_Sprite("Resources//Misc//chicken.bmp", S, 5, 1, 16, 16),ways); 
	g->set_world(this->current_world);
	this->current_world->insert_entity(g);

	Drawable* d = new Drawable(550, 10, 0,0, new Solid_Sprite("Resources//drawable_images//tree_pine.bmp"));
	d->set_boundary_value(35, 30, 60, 123);
	this->current_world->insert_entity(d);

	Drawable* d1 = new Drawable(650, 149, 0,0, new Solid_Sprite("Resources//drawable_images//tree_pine.bmp"));
	d1->set_boundary_value(35, 30, 60, 123);
	this->current_world->insert_entity(d1);

	Drawable* d2 = new Drawable(780, 300, 0,0, new Solid_Sprite("Resources//drawable_images//tree_pine.bmp"));
	d2->set_boundary_value(35, 30, 60, 123);
	this->current_world->insert_entity(d2);

	Drawable* d3 = new Drawable(800, 593, 0,0, new Solid_Sprite("Resources//drawable_images//tree_pine.bmp"));
	d3->set_boundary_value(35, 30, 60, 123);
	this->current_world->insert_entity(d3);

	Drawable* d4 = new Drawable(1000, 450, 0,0, new Solid_Sprite("Resources//drawable_images//tree_pine.bmp"));
	d4->set_boundary_value(35, 30, 60, 123);
	this->current_world->insert_entity(d4);

	Drawable* d5 = new Drawable(950, 438, 0,0, new Solid_Sprite("Resources//drawable_images//tree_pine.bmp"));
	d5->set_boundary_value(35, 30, 60, 123);
	this->current_world->insert_entity(d5);

	Drawable* d6 = new Drawable(850, 600, 0,0, new Solid_Sprite("Resources//drawable_images//tree_pine.bmp"));
	d6->set_boundary_value(35, 30, 60, 123);
	this->current_world->insert_entity(d6);

	Drawable* d7 = new Drawable(850, 834, 0,0, new Solid_Sprite("Resources//drawable_images//tree_pine.bmp"));
	d7->set_boundary_value(35, 30, 60, 123);
	this->current_world->insert_entity(d7);

	Combat* rambo_sheep = new Combat(800,500, 0,0, new Solid_Sprite("Resources//drawable_images//sheep.bmp"));
	rambo_sheep->set_stats(103, 0, 0, 0, 0);
	rambo_sheep->set_world(this->current_world);
	this->current_world->insert_entity(rambo_sheep);
	rambo_sheep->set_boundary_value(30, 30, 2, 2);

	/*Attack* att = new Attack(800, 800, 5, 5, new Player_Sprite("magic//fireball.bmp", W, 0,0,0,0), 0,0,0,0,0,0);
	att->set_boundary_value(28, 28, 4, 4);
	this->current_world->insert_entity(att);
	att->set_world(this->current_world);*/

	Portal* port = new Portal(1000, 1000, new Solid_Sprite("Resources//drawable_images//sheep.bmp"), main_world);
	port->set_boundary_value(100, 100, 0, 0);
	port->set_world(this->current_world);
	this->current_world->insert_entity(port);

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
	std::list<iDrawable*>* actives = this->current_world->get_active_entities();
	
	std::list<iDrawable*>::iterator iter;
	for (iter = actives->begin(); iter != actives->end(); iter++){
		(*iter)->update();
	}

}

void View::draw_active_world(void){
	//clear_bitmap(this->buffer);

	int tile_wide = this->current_world->get_tiles_wide();
	int tile_high = this->current_world->get_tiles_high();
	Tile*** tiles = this->current_world->get_tile_map();

	draw_sprites(this->buffer, tiles, tile_wide, tile_high);
	draw_drawables(this->buffer, this->current_world->get_visible_entities());

	
	//masked_blit(this->playa->get_image()->get_current_frame(), this->buffer,
	//	0,0, SCREEN_W/2, SCREEN_H/2, 32, 30);
	//draw_interface(this->playa);
	draw_to_screen();

	this->current_world->remove_destroyed();

}

void View::draw_to_screen(void){
	textprintf_centre_ex(screen,font,100,50,makecol(255,255,255),-1,"World size %d", this->current_world->active_entities->size());		

	int x = this->playa->get_x_pos();
	x -= SCREEN_W/2;
	int y = this->playa->get_y_pos();
	y -= SCREEN_H/2;
	blit(this->buffer, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
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
	int left_x = this->playa->get_x_pos() - SCREEN_W/2;
	int start_j = left_x / 32;
	if (start_j < 0)
		start_j = 0;
	int end_j = (left_x + SCREEN_W) / 32;
	if (++end_j > tile_wide)
		end_j = tile_wide;

	int top_y = this->playa->get_y_pos() - SCREEN_H/2;
	int start_i = top_y / 32;
	if (start_i < 0)
		start_i = 0;
	int end_i = (top_y + SCREEN_H) / 32;
	if (++end_i > tile_high)
		end_i = tile_high;

	for (int i=start_i; i<end_i; i++){
		for (int j=start_j; j<end_j; j++){
			masked_blit(tile_map[i][j]->background_image->get_current_frame(),
				buffer, 0,0, j*TILE_SIZE - left_x, i*TILE_SIZE - top_y, TILE_SIZE, TILE_SIZE);
			/*if (!tile_map[i][j]->can_walk)
				rect(buffer, j*TILE_SIZE - left_x, i*TILE_SIZE - top_y, 
					j*TILE_SIZE - left_x + TILE_SIZE, i*TILE_SIZE - top_y + TILE_SIZE, makecol(255, 255, 255));
			*/ 
			// draw the background image for starters
			//draw_drawables(buffer, tile_map[i][j]->contents);
		}
	}
}

void View::draw_drawables(BITMAP* buffer, std::list<iDrawable*> *sprites){
	int xshift = this->playa->get_x_pos() - SCREEN_W/2;
	int yshift = this->playa->get_y_pos() - SCREEN_H/2;

	std::list<iDrawable*>::iterator iter;
	for (iter = sprites->begin(); iter != sprites->end(); ++iter){
		BITMAP* frame = (*iter)->get_image()->get_current_frame();
		int x = (*iter)->get_x_pos();
		int y = (*iter)->get_y_pos();
		int width = frame->w;
		int height = frame->h;
		masked_blit(frame, buffer, 0,0, x-xshift, y-yshift, 154, 154);

		
		rect(buffer, (*iter)->get_reference_x() - xshift, (*iter)->get_reference_y() - yshift,
			(*iter)->get_reference_x() + (*iter)->get_bounding_width() - xshift,
			(*iter)->get_reference_y() + (*iter)->get_bounding_height() - yshift,
			makecol(255, 255, 255));
			 
			

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