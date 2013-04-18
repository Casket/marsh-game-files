#include "Main.h"


using namespace std;

int pick_cast_color(Attack* attack);
std::pair<int, std::string> substr_word_boundaries(std::string str, int pos, int max_len);

View::View(Player* hero){
	this->playa = hero;
	this->world_buffer = create_bitmap(VISIBLE_W, VISIBLE_H);
	clear_bitmap(this->world_buffer);
	for (int i=0; i < MAX_NUMBER_WORLDS; i++)
		this->loaded_worlds[i] = NULL;
	this->current_world = NULL;
	this->ui_image = load_bitmap("Resources//MarshUI4.bmp", NULL);
	this->ui_buffer = create_bitmap(UI_WIDTH, UI_HEIGHT);
	blit(this->ui_image, this->ui_buffer, 0, 0, 0, 0, this->ui_image->w, this->ui_image->h);
	this->resource_bars[0] = create_sub_bitmap(this->ui_image, HEALTH_BAR_X_POS, 
		HEALTH_BAR_Y_POS, RESOURCE_BAR_WIDTH + BAR_PAD, RESOURCE_BAR_HEIGHT + BAR_PAD);
	this->resource_bars[1] = create_sub_bitmap(this->ui_image, CAST_BAR_X_POS, 
		CAST_BAR_Y_POS, CAST_BAR_WIDTH + BAR_PAD, CAST_BAR_HEIGHT + BAR_PAD);
	this->clear_console = create_sub_bitmap(this->ui_image, CONSOLE_X_POS, CONSOLE_Y_POS, CONSOLE_WIDTH, CONSOLE_HEIGHT);
	this->in_use_console = create_bitmap(CONSOLE_WIDTH, CONSOLE_HEIGHT);

}

View::~View(void){
	if (this->playa != NULL)
		delete this->playa;
	for (int i=0; i < MAX_NUMBER_WORLDS; i++){
		if (this->loaded_worlds[i] != NULL)
			delete this->loaded_worlds[i];
	}
	if (this->world_buffer != NULL)
		destroy_bitmap(this->world_buffer);


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

	QuestReward r;
	r.gold = 1;

	QuestDescription des;
	des.text = (char*)malloc(sizeof(char)*10);

	KillObjective* objective;
	objective = new KillObjective(EntityType::Monster, 2);

	Quest* quest = new Quest(des, objective);
	quest->add_reward(r);

	objective->register_objective(quest);
	this->playa->quest_manager->flush_queues();


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
	rambo_sheep->set_my_type(EntityType::Monster);
	rambo_sheep->set_stats(103, 0, 0, 0, 0);
	rambo_sheep->set_world(this->current_world);
	this->current_world->insert_entity(rambo_sheep);
	rambo_sheep->set_boundary_value(30, 30, 2, 2);

	Combat* rambo_sheep2 = new Combat(1000,500, 0,0, new Solid_Sprite("Resources//drawable_images//sheep.bmp"));
	rambo_sheep2->set_my_type(EntityType::Monster);
	rambo_sheep2->set_stats(99, 0, 0, 0, 0);
	rambo_sheep2->set_world(this->current_world);
	this->current_world->insert_entity(rambo_sheep2);
	rambo_sheep2->set_boundary_value(30, 30, 2, 2);

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
	//clear_bitmap(this->world_buffer);

	int tile_wide = this->current_world->get_tiles_wide();
	int tile_high = this->current_world->get_tiles_high();
	Tile*** tiles = this->current_world->get_tile_map();

	draw_sprites(this->world_buffer, tiles, tile_wide, tile_high);
	draw_drawables(this->world_buffer, this->current_world->get_visible_entities());


	//masked_blit(this->playa->get_image()->get_current_frame(), this->world_buffer,
	//	0,0, SCREEN_W/2, SCREEN_H/2, 32, 30);
	draw_interface(this->playa);
	draw_to_screen();

	this->current_world->remove_destroyed();

}

void View::draw_to_screen(void){
	acquire_screen();
	blit(this->world_buffer, screen, 0,0,0,0, VISIBLE_W, VISIBLE_H);
	blit(this->ui_buffer, screen, 0, 0, 0, SCREEN_H - UI_HEIGHT, UI_WIDTH, UI_HEIGHT);
	release_screen();
}

void draw_status(Player* hero, BITMAP* buffer){
	int max_health = 31; //hero->get_max_health();
	int cur_health = 15; //hero->get_health();
	double px_per_h =  (double) RESOURCE_BAR_WIDTH / (double) max_health;  //total_pixs / total_health

	rectfill(buffer, HEALTH_BAR_X_POS, HEALTH_BAR_Y_POS, HEALTH_BAR_X_POS + px_per_h*cur_health, HEALTH_BAR_Y_POS + RESOURCE_BAR_HEIGHT, HEALTH_COLOR);

	int max_mana = 20; //hero->get_max_mana();
	int cur_mana = 20; //hero->get_mana();
	double px_per_m = (double) RESOURCE_BAR_WIDTH / (double)max_mana;
	rectfill(buffer, MANA_BAR_X_POS, MANA_BAR_Y_POS, MANA_BAR_X_POS + px_per_m*cur_mana, MANA_BAR_Y_POS + RESOURCE_BAR_HEIGHT, MANA_COLOR);

	if (hero->casted_spell != NULL){
		int charge_time = hero->casted_spell->get_charge_time();
		int time = hero->casting_timer;
		double px_per_c = (double) CAST_BAR_WIDTH / (double)charge_time;

		rectfill(buffer, CAST_BAR_X_POS, CAST_BAR_Y_POS, CAST_BAR_X_POS + px_per_c*time, CAST_BAR_Y_POS + CAST_BAR_HEIGHT, pick_cast_color(hero->casted_spell));
	}
}

int pick_cast_color(Attack* attack){
	return makecol(255, 255, 255);
}

void draw_dialogs(BITMAP* buffer){

}

void View::print_to_console(std::string str){
	blit(this->clear_console, this->in_use_console, 0, 0, 0, 0, 
		this->clear_console->w, this->clear_console->h);

	int max_line_len = 35;
	int line_height = 10;
	int x = 17;
	int y = 18;
	int len = str.length();
	int i = 0;
	while (i < len){
		std::pair<int, std::string> cur_line = substr_word_boundaries(str, i, max_line_len);
		textprintf_ex(this->in_use_console,font,x,y,
			makecol(255,255,255),-1,cur_line.second.c_str());
		i = cur_line.first;
		y += line_height;
		if (y > CONSOLE_HEIGHT - line_height)
			break; // bad news, it won't all fit... you suck
	}
}

std::pair<int, std::string> substr_word_boundaries(std::string str, int pos, int max_len){
	int len = str.length();
	bool saw_space = false;
	int last_word_ending_pos = pos;

	int i=0;
	while (i < max_len){
		if (i > len)
			break;
		if ((str.at(pos+i) == ' ')  && !saw_space){
			last_word_ending_pos = pos+i;
			saw_space = true;
		} else if (str.at(pos+i) != ' ')
			saw_space = false;
		i++;
	}

	// should now have a pos to stop the substring
	int sub_len = last_word_ending_pos - pos;
	std::string ret_str = str.substr(pos, sub_len);

	// now get a forward index, skipping spaces
	while (str.at(last_word_ending_pos) == ' ')
		last_word_ending_pos++;

	return std::pair<int, std::string>(last_word_ending_pos, ret_str);
}

void View::draw_interface(Player* hero){
	blit(this->resource_bars[0], this->ui_buffer, 0, 0, HEALTH_BAR_X_POS, HEALTH_BAR_Y_POS, RESOURCE_BAR_WIDTH + BAR_PAD, RESOURCE_BAR_HEIGHT + BAR_PAD);
	blit(this->resource_bars[0], this->ui_buffer, 0, 0, MANA_BAR_X_POS, MANA_BAR_Y_POS, RESOURCE_BAR_WIDTH + BAR_PAD, RESOURCE_BAR_HEIGHT + BAR_PAD);
	blit(this->resource_bars[1], this->ui_buffer, 0, 0, CAST_BAR_X_POS, CAST_BAR_Y_POS, CAST_BAR_WIDTH + BAR_PAD, CAST_BAR_HEIGHT + BAR_PAD);
	draw_status(hero, this->ui_buffer);
	//draw_dialogs(this->buffer);
	blit(this->in_use_console, this->ui_buffer, 0, 0, CONSOLE_X_POS, CONSOLE_Y_POS, CONSOLE_WIDTH, CONSOLE_HEIGHT);
}

void View::draw_sprites(BITMAP* buffer, Tile*** tile_map, int tile_wide, int tile_high){
	int left_x = this->playa->get_x_pos() - buffer->w/2;
	int start_j = left_x / 32;
	if (start_j < 0)
		start_j = 0;
	int end_j = (left_x + buffer->w) / 32;
	if (++end_j > tile_wide)
		end_j = tile_wide;

	int top_y = this->playa->get_y_pos() - buffer->h/2;
	int start_i = top_y / 32;
	if (start_i < 0)
		start_i = 0;
	int end_i = (top_y + buffer->h) / 32;
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
		}
	}
}

void View::draw_drawables(BITMAP* buffer, std::list<iDrawable*> *sprites){
	int xshift = this->playa->get_x_pos() - buffer->w/2;
	int yshift = this->playa->get_y_pos() - buffer->h/2;

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