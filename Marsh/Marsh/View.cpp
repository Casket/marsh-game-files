#include <omp.h>
#include "Main.h"
class QuestGiver;
class Portal;


using namespace std;
using namespace Marsh;

bool should_save(World* check_world);
int pick_cast_color(Attack* attack);

Marsh::View::View(Player* hero){
	this->playa = hero;
	this->world_buffer = create_bitmap(VISIBLE_W, VISIBLE_H);
	clear_bitmap(this->world_buffer);
	this->loaded_worlds = new std::map<WorldName, World*>();
	this->current_world = NULL;
	this->ui_image = load_bitmap("Resources//MarshUI5.bmp", NULL);
	this->ui_buffer = create_bitmap(UI_WIDTH, UI_HEIGHT);
	blit(this->ui_image, this->ui_buffer, 0, 0, 0, 0, this->ui_image->w, this->ui_image->h);
	this->resource_bars[0] = create_sub_bitmap(this->ui_image, HEALTH_BAR_X_POS, 
		HEALTH_BAR_Y_POS, RESOURCE_BAR_WIDTH + BAR_PAD, RESOURCE_BAR_HEIGHT + BAR_PAD);
	this->resource_bars[1] = create_sub_bitmap(this->ui_image, CAST_BAR_X_POS, 
		CAST_BAR_Y_POS, CAST_BAR_WIDTH + BAR_PAD, CAST_BAR_HEIGHT + BAR_PAD);
	this->clear_console = create_sub_bitmap(this->ui_image, CONSOLE_X_POS, CONSOLE_Y_POS, CONSOLE_WIDTH, CONSOLE_HEIGHT);
	this->in_use_console = create_bitmap(CONSOLE_WIDTH, CONSOLE_HEIGHT);
	blit(this->clear_console, this->in_use_console, 0, 0, 0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT);
	this->behind_bars = load_bitmap("Resources//MarshUI5_background.bmp", NULL);
	hero->set_consoles(this->clear_console, this->in_use_console);
	this->behind_exp = load_bitmap("Resources//MarshUI5_experience.bmp", NULL);
	this->displayed_images = new std::vector<std::string>();
	this->spell_icon_coords = new std::vector<std::pair<int, int>>();
	this->populate_spell_locs();
	this->populate_image_names();

	this->draw_updated_loadout(Player_Accessor::get_player()->attack_loadout);
}

Marsh::View::~View(void){
	if (this->playa != NULL)
		delete this->playa;

	std::map<WorldName, World*>::iterator start = this->loaded_worlds->begin();
	std::map<WorldName, World*>::iterator finish = this->loaded_worlds->end();
	for ( ; start != finish; start++){
		delete (*start).second;
	}

	if (this->current_world != NULL)
		delete this->current_world;
	this->current_world = NULL;

	delete this->loaded_worlds;

	if (this->world_buffer != NULL)
		destroy_bitmap(this->world_buffer);
	if (this->ui_buffer != NULL)
		destroy_bitmap(this->ui_buffer);
	destroy_bitmap(this->ui_image);
	destroy_bitmap(this->resource_bars[0]);
	destroy_bitmap(this->resource_bars[1]);
	destroy_bitmap(this->clear_console);
	destroy_bitmap(this->in_use_console);
	destroy_bitmap(this->behind_bars);
}


void Marsh::View::populate_spell_locs(void){
	this->spell_icon_coords->push_back(std::pair<int, int>(378, 106));
	this->spell_icon_coords->push_back(std::pair<int, int>(475, 106));
	this->spell_icon_coords->push_back(std::pair<int, int>(575, 106));
	this->spell_icon_coords->push_back(std::pair<int, int>(675, 106));
	this->spell_icon_coords->push_back(std::pair<int, int>(773, 106));
	this->spell_icon_coords->push_back(std::pair<int, int>(872, 106));
	this->spell_icon_coords->push_back(std::pair<int, int>(970, 106));
	this->spell_icon_coords->push_back(std::pair<int, int>(1070, 106));
	this->spell_icon_coords->push_back(std::pair<int, int>(1168, 106));
	this->spell_icon_coords->push_back(std::pair<int, int>(1268, 106));
}
void Marsh::View::populate_image_names(void){
	this->displayed_images->push_back("shadow_ball");
	this->displayed_images->push_back("shadow_ball");
	this->displayed_images->push_back("Energy_Wave");
	this->displayed_images->push_back("shadow_nova");
	this->displayed_images->push_back("Energy_Needle");
	this->displayed_images->push_back("Energy_Needle");
	this->displayed_images->push_back("Shadow_Spike");
	this->displayed_images->push_back("death_beam");
	this->displayed_images->push_back("Drain");
	this->displayed_images->push_back("bind");
	this->displayed_images->push_back("mass_slow");
	this->displayed_images->push_back("shadow_dash");
	this->displayed_images->push_back("teleport");
	this->displayed_images->push_back("shield");
	this->displayed_images->push_back("deflect");
}

void Marsh::View::draw_updated_loadout(Attack* loadout[]){
	for (int i=0; i < MAX_ATTACKS; i++){
		if (loadout[i] == NULL)
			continue;
		int id = loadout[i]->spell_id;
		if (id < 0 || id >= 15){
			int bad_code = 1;
			continue;
		}
		std::string file("Resources//Attack_Images//");
		file.append(this->displayed_images->at(id));
		file.append(".bmp");
		std::pair<int, int> coord = this->spell_icon_coords->at(i);
		BITMAP* image = load_bitmap(file.c_str(), NULL);
		masked_blit(image, this->ui_buffer, 0, 0, coord.first, coord.second, image->w, image->h);
		destroy_bitmap(image);

		textprintf_right_ex(this->ui_buffer, font3, coord.first+SPELL_ICON_SIZE, coord.second, makecol(255, 255, 255), -1, "%d", i+1);
	}
}

void Marsh::View::load_world(WorldName world){
	if (this->current_world != NULL){
		put_world_in_loaded(this->current_world);
	}

	std::map<WorldName, World*>::iterator pos = this->loaded_worlds->find(world);
	std::map<WorldName, World*>::iterator last = this->loaded_worlds->end();
	std::map<WorldName, World*>::iterator first = this->loaded_worlds->begin();

	if (pos != this->loaded_worlds->end()){
		// we have it ready to go
		this->current_world = (*pos).second;
		// now remove it from the map
		this->loaded_worlds->erase(pos);
	} else {
		// the world needs loaded
		this->current_world = new World(world);
		this->current_world->set_player(this->playa);
		this->current_world->insert_entity(this->playa);
	}
	this->playa->set_world(this->current_world);
	this->insert_testing_entities();
}

static bool should_save(World* check_world){
	switch(check_world->my_name){
	case main_world11:
		return check_world->current_mission == Player_Accessor::get_player()->current_mission;
	case main_world13:
		return check_world->current_mission == Player_Accessor::get_player()->current_mission;
	case main_world16:
		return check_world->current_mission == Player_Accessor::get_player()->current_mission;
	case main_world19:
		return check_world->current_mission == Player_Accessor::get_player()->current_mission;
	default:
		return false;

	}
}

void Marsh::View::put_world_in_loaded(World* world){
	std::list<std::map<WorldName, World*>::iterator> bad_worlds;
	//start by ditching worlds that don't need cached
	std::map<WorldName, World*>::iterator end = this->loaded_worlds->end();
	for (std::map<WorldName, World*>::iterator iter = this->loaded_worlds->begin(); iter != end; ++iter){
		// check if a world should be saved
		if (should_save((*iter).second))
			continue;
		else {
			//throw that hoe out
			bad_worlds.push_back(iter);
			delete (*iter).second;
			(*iter).second = NULL;
		}
	}

	std::list<std::map<WorldName, World*>::iterator>::iterator ls_end = bad_worlds.end();
	for (std::list<std::map<WorldName, World*>::iterator>::iterator iter = bad_worlds.begin(); iter != ls_end; ++iter){
		this->loaded_worlds->erase((*iter));
	}

	this->loaded_worlds->insert(std::pair<WorldName, World*>(world->my_name, world));
}

void Marsh::View::insert_testing_entities(void){
	/*
	if(this->current_world->my_name == main_world){
		std::vector<std::pair<int, int>>* ways = new std::vector<std::pair<int,int>>();
		std::pair<int, int> test = std::make_pair(120, 1000);
		ways->insert(ways->end(), test);
		std::pair<int, int> test2 = std::make_pair(120, 800);
		ways->insert(ways->end(), test2);
		std::pair<int, int> test3 = std::make_pair(45, 800);
		ways->insert(ways->end(), test3);

		ItemBestower* shop = new ItemBestower(700, 700, 0, 0, NULL);
		shop->bestow_all_items = false;

		ItemBestower* farmer_bob = new ItemBestower(500, 400, 0, 0, new Player_Sprite("Resources//AI_characters//nice_folk.bmp", S, 5, 1, 16, 16));
		farmer_bob->set_world(this->current_world);
		farmer_bob->append_dialogue("Good work tiny adventurer.");
		farmer_bob->can_speak = true;
		farmer_bob->bestow_all_items = true;
		this->current_world->insert_entity(farmer_bob);
		farmer_bob->set_boundary_value(30, 30, 0, 0);
		Equipment* manapot = get_new_equipment();
		manapot->intelligence = 100;
		manapot->number_held = 1;
		manapot->description = "+100 Mana [Enter to drink]";
		manapot->item_id = 17;
		manapot->type = Consumable;
		manapot->stackable = true;
		manapot->name = "Weak Mana Potion";

		farmer_bob->append_inventory(manapot, 20);

		Equipment* healthpot = get_new_equipment();
		healthpot->vitality = 300;
		healthpot->number_held = 1;
		healthpot->description = "+300 Health [Enter to drink]";
		healthpot->name = "Super Powerful Healing Potion";
		healthpot->item_id = 19;
		healthpot->type = Consumable;
		healthpot->stackable = true;

		farmer_bob->append_inventory(healthpot, 15);

		Equipment* companion = get_new_equipment();
		companion->description = "A fluffy kitty who will smite your enemies with cuteness.";
		companion->name = "Mr. Biggles Worth";
		companion->number_held = 1;
		companion->stackable = false;
		companion->item_id = 197;
		farmer_bob->append_inventory(companion, 1000000);

		/*
		Mob* g = new Town_Guard(116,800,0,0,new Player_Sprite("Resources//Misc//guard.bmp", S, 5, 1, 16, 16),ways); 
		g->set_boundary_value(32,18,0,14);
		g->set_world(this->current_world);
		g->set_stats(1000, 1000, 1000, 1000, 1000);
		this->current_world->insert_entity(g);

		Mob* g1 = new Vampire(188,800,0,0,new Player_Sprite("Resources//people//boy_one.bmp", S, 5, 1, 16, 16),ways); 
		g1->set_boundary_value(32,18,0,14);
		g1->set_world(this->current_world);
		g1->set_stats(1000, 1000, 1000, 1000, 1000);
		this->current_world->insert_entity(g1);
	
		Mob* g2 = new VampireBoss(152,800,0,0,new Player_Sprite("Resources//Misc//guard.bmp", S, 5, 1, 16, 16),ways); 
		g2->set_boundary_value(32,18,0,14);
		g2->set_world(this->current_world);
		g2->set_stats(10, 10, 10, 10, 10);
		this->current_world->insert_entity(g2);
	}
	*/
	/*	QuestDescription kill_chickens;
	Drawable* test_transp = new Drawable(200, 200, 0, 0, new Solid_Sprite("Resources//Misc//Special14.tga"));
	test_transp->set_world(this->current_world);
	test_transp->get_image()->is_translucent = true;
	this->current_world->insert_entity(test_transp);

	kill_chickens.text = "Chickens are up in here eating our food.  Kill one chicken and claim your rewards.";
	QuestReward lootz;
	lootz.gold = 1;
	KillObjective* obj = new KillObjective(EntityType::Chicken, 1);
	Quest* farm_quest = new Quest(kill_chickens, obj);
	farm_quest->add_reward(lootz);

	RetrieveObjective* item_obj = new RetrieveObjective(10, 1);
	QuestDescription item;
	item.text = "You need to get some items for me because I like stuff.";
	Quest* quest2 = new Quest(item, item_obj);
	quest2->add_reward(lootz);

	OptionPresenter* farmer_bob = new OptionPresenter(400, 400, 0, 0, new Player_Sprite("Resources//people//nice_folk.bmp", S, 5, 1, 16, 16));
	farmer_bob->set_world(this->current_world);
	farmer_bob->append_quest(farm_quest);
	farmer_bob->append_quest(quest2);
	farmer_bob->append_pre_dialogue("Howdy, how would you like to help me out?");
	farmer_bob->append_pre_dialogue("Would you like to [1] kill chickens or [2] retrieve fancy items?");
	farmer_bob->append_post_dialogue("Thanks for helping out.");
	farmer_bob->can_speak = true;
	this->current_world->insert_entity(farmer_bob);
	farmer_bob->set_boundary_value(30, 30, 0, 0);

	/*std::vector<std::pair<int, Direction>>* ways = new std::vector<std::pair<int,Direction>>();
	std::pair<int, Direction> test = std::make_pair(-1, N);
	ways->insert(ways->end(), test);
	std::pair<int, Direction> test2 = std::make_pair(30, S);
	ways->insert(ways->end(), test2);
	std::pair<int, Direction> test5 = std::make_pair(-1,N);
	ways->insert(ways->end(), test5);


	Mob* g = new Mob(300,450,0,0,new Player_Sprite("Resources//Misc//guard.bmp", S, 5, 1, 16, 16),ways); 
	g->set_world(this->current_world);
	this->current_world->insert_entity(g);

	Combat* talker = new Combat(400,500, 0,0, new Solid_Sprite("Resources//drawable_images//barrel.bmp"));
	talker->set_my_type(EntityType::Speaker);
	talker->set_world(this->current_world);
	talker->can_speak = true;
	talker->append_dialogue("I used to be an adventurer like you, until I took an arrow to the knee");
	talker->append_dialogue("How does a rogue beat a paladin?  He pickpockets his hearth stone.");
	talker->append_dialogue("I wondered why the baseball was getting bigger.  Then it hit me.");
	talker->append_dialogue("I used to have a fear of hurdles, but I got over it.");
	talker->append_dialogue("I see you have some graph paper.  You must be plotting something.");
	this->current_world->insert_entity(talker);
	talker->set_boundary_value(40, 40, 0, 0);

	QuestReward r;
	r.gold = 1;

	QuestDescription des;
	des.text = "Go kill two killer rambo sheep";

	KillObjective* objective;
	objective = new KillObjective(EntityType::Monster, 2);

	Quest* quest = new Quest(des, objective);
	quest->add_reward(r);

	quest->begin_quest();

	des.text = "Go talk to the barrel";

	InteractObjective* inter_objective;
	inter_objective = new InteractObjective(EntityType::Speaker, 1);

	Quest* quest2 = new Quest(des, inter_objective);
	quest2->add_reward(r);

	quest2->begin_quest();


	this->playa->quest_manager->flush_queues();


	Drawable* d = new Drawable(550, 10, 0,0, new Solid_Sprite("Resources//drawable_images//tree_pine.bmp"));
	d->set_boundary_value(0, 0, 0, 0);
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

	Combat* rambo_sheep = new Combat(800,500, 0,0, new Solid_Sprite("Resources//drawable_images//sheep.bmp", 0, 0, 30, 30));
	rambo_sheep->set_my_type(EntityType::Monster);
	rambo_sheep->set_stats(103, 0, 0, 0, 0);
	rambo_sheep->set_world(this->current_world);
	this->current_world->insert_entity(rambo_sheep);
	rambo_sheep->set_boundary_value(30, 30, 2, 2);

	Combat* rambo_sheep2 = new Combat(1000, 800, 0,0, new Solid_Sprite("Resources//drawable_images//sheep.bmp", 33, 0, 30, 30));
	rambo_sheep2->set_my_type(EntityType::Monster);
	rambo_sheep2->set_stats(99, 0, 0, 0, 0);
	rambo_sheep2->set_world(this->current_world);
	this->current_world->insert_entity(rambo_sheep2);
	rambo_sheep2->set_boundary_value(30, 30, 2, 2);

	Combat* chicken = new Combat(500, 1000, 0, 0, new Solid_Sprite("Resources//Misc//chicken.bmp", 0, 0, 30, 30));
	chicken->set_my_type(EntityType::Chicken);
	chicken->set_stats(99, 0, 0, 0, 0);
	chicken->set_world(this->current_world);
	this->current_world->insert_entity(chicken);
	chicken->set_boundary_value(30, 30, 2, 2);

	Attack* att = new Attack(800, 800, 5, 5, new Player_Sprite("magic//fireball.bmp", W, 0,0,0,0), 0,0,0,0,0,0);
	att->set_boundary_value(28, 28, 4, 4);
	this->current_world->insert_entity(att);
	att->set_world(this->current_world);

	Portal* port = new Portal(1000, 1000, new Solid_Sprite("Resources//drawable_images//sheep.bmp"), small_map);
	port->set_boundary_value(100, 100, 0, 0);
	port->set_world(this->current_world);
	this->current_world->insert_entity(port);
	*/
}


void Marsh::View::update(void){
	std::list<iDrawable*>* actives = this->current_world->get_active_entities();
	std::list<iDrawable*>::iterator end = actives->end();
	std::list<iDrawable*>::iterator iter;
	for (iter = actives->begin(); iter != end; iter++){
		if ((*iter)->my_type == Hero)
			continue;
		(*iter)->update();
		if ((*iter)->my_type == StarGate){
			Portal* gateway = (*iter)->fetch_me_as_portal();
			if (gateway->activated){
				Player_Accessor::get_player()->x_pos = gateway->target_x_pos;
				Player_Accessor::get_player()->y_pos = gateway->target_y_pos;
				this->load_world(gateway->portal_to);
			}
		}
	}
	this->current_world->remove_destroyed();
}

void Marsh::View::draw_active_world(void){
	clear_bitmap(this->world_buffer);

	int tile_wide = this->current_world->get_tiles_wide();
	int tile_high = this->current_world->get_tiles_high();
	Tile*** tiles = this->current_world->get_tile_map();

	draw_sprites(this->world_buffer, tiles, tile_wide, tile_high);
	draw_drawables(this->world_buffer, this->current_world->get_visible_entities());
	draw_interface(this->playa);
	draw_to_screen();


}

void Marsh::View::draw_to_screen(void){
	acquire_screen();
	blit(this->world_buffer, screen, 0,0,0,0, VISIBLE_W, VISIBLE_H);
	blit(this->ui_buffer, screen, 0, 0, 0, SCREEN_H - UI_HEIGHT, UI_WIDTH, UI_HEIGHT);
	release_screen();
}

void draw_status(Player* hero, BITMAP* buffer){
	int max_health = hero->get_max_health();
	int cur_health = hero->get_current_health();
	double px_per_h =  (double) RESOURCE_BAR_WIDTH / (double) max_health;  //total_pixs / total_health

	rectfill(buffer, HEALTH_BAR_X_POS, HEALTH_BAR_Y_POS, HEALTH_BAR_X_POS + px_per_h*cur_health, HEALTH_BAR_Y_POS + RESOURCE_BAR_HEIGHT, HEALTH_COLOR);

	int max_mana = hero->get_max_mana();
	int cur_mana = hero->get_current_mana();
	double px_per_m = (double) RESOURCE_BAR_WIDTH / (double)max_mana;
	rectfill(buffer, MANA_BAR_X_POS, MANA_BAR_Y_POS, MANA_BAR_X_POS + px_per_m*cur_mana, MANA_BAR_Y_POS + RESOURCE_BAR_HEIGHT, MANA_COLOR);

	if (hero->casted_spell != NULL){
		int charge_time = hero->casted_spell->get_charge_time();
		int time = hero->casting_timer;
		double px_per_c = (double) CAST_BAR_WIDTH / (double)charge_time;

		rectfill(buffer, CAST_BAR_X_POS, CAST_BAR_Y_POS, CAST_BAR_X_POS + px_per_c*time, CAST_BAR_Y_POS + CAST_BAR_HEIGHT, pick_cast_color(hero->casted_spell));
	}

	int max_exp = EXPERIENCE_TO_LEVEL;
	int cur_exp = hero->experience;
	double px_per_e = (double) EXP_BAR_WIDTH / (double)max_exp;
	rectfill(buffer, EXP_BAR_X_POS+5, EXP_BAR_Y_POS-5, EXP_BAR_X_POS + px_per_e*cur_exp+5, EXP_BAR_Y_POS + EXP_BAR_HEIGHT-5, EXP_COLOR);
}

int pick_cast_color(Attack* attack){
	return makecol(205, 198, 193);
}

void draw_dialogs(BITMAP* buffer){

}

void Marsh::View::draw_interface(Player* hero){
	blit(this->behind_bars, this->ui_buffer, 0, 0, BACK_LAYER_X, BACK_LAYER_Y, this->behind_bars->w, this->behind_bars->h);
	blit(this->behind_exp, this->ui_buffer, 0, 0, BACK_EXP_X, BACK_EXP_Y, this->behind_exp->w, this->behind_exp->h);
	draw_status(hero, this->ui_buffer);
	masked_blit(this->resource_bars[0], this->ui_buffer, 0, 0, HEALTH_BAR_X_POS, HEALTH_BAR_Y_POS, RESOURCE_BAR_WIDTH + BAR_PAD, RESOURCE_BAR_HEIGHT + BAR_PAD);
	masked_blit(this->resource_bars[0], this->ui_buffer, 0, 0, MANA_BAR_X_POS, MANA_BAR_Y_POS, RESOURCE_BAR_WIDTH + BAR_PAD, RESOURCE_BAR_HEIGHT + BAR_PAD);
	masked_blit(this->resource_bars[1], this->ui_buffer, 0, 0, CAST_BAR_X_POS, CAST_BAR_Y_POS, CAST_BAR_WIDTH + BAR_PAD, CAST_BAR_HEIGHT + BAR_PAD);
	blit(this->in_use_console, this->ui_buffer, 0, 0, CONSOLE_X_POS, CONSOLE_Y_POS, CONSOLE_WIDTH, CONSOLE_HEIGHT);
}

void Marsh::View::draw_sprites(BITMAP* buffer, Tile*** tile_map, int tile_wide, int tile_high){
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
			int x_test = j*TILE_SIZE - left_x;
			int y_test =  i*TILE_SIZE - top_y;
			BITMAP* testBMP = tile_map[i][j]->background_image->get_current_frame();
			Tile* testTile = tile_map[i][j];
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

void Marsh::View::draw_drawables(BITMAP* buffer, std::list<iDrawable*> *sprites){
	int xshift = this->playa->get_x_pos() - buffer->w/2;
	int yshift = this->playa->get_y_pos() - buffer->h/2;

	std::list<iDrawable*>::iterator iter;
	std::list<iDrawable*>::iterator end = sprites->end();
	for (iter = sprites->begin(); iter != end; ++iter){
		if ((*iter)->get_image() == NULL)
			continue;
		BITMAP* frame = (*iter)->get_image()->get_current_frame();
		int x = (*iter)->get_x_pos();
		int y = (*iter)->get_y_pos();

		int width = frame->w;
		int height = frame->h;

		if ((*iter)->get_image()->is_translucent){
			draw_trans_sprite(buffer, frame, x-xshift, y-yshift);
		} else {
			masked_blit(frame, buffer, 0,0, x-xshift, y-yshift, width, height);
		}
		rect(buffer, (*iter)->get_reference_x() - xshift, (*iter)->get_reference_y() - yshift,
			(*iter)->get_reference_x() + (*iter)->get_bounding_width() - xshift,
			(*iter)->get_reference_y() + (*iter)->get_bounding_height() - yshift,
			makecol(255, 255, 255));
	}
}

void Marsh::View::save_game(void){
	/*
	fstream fout;
	fout.put("default", ios::out);
	// do stuff
	fout.close;
	*/ 
}