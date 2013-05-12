#include "Main.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <winalleg.h>
#include "LevelUp.h"


using namespace std;

#define MODE GFX_AUTODETECT_WINDOWED
#define COLOR_DEPTH 32
#define SCREENW 1400
#define SCREENH 1000
#define FRAME_RATE_DELAY 20
#define INTRO_GAME 0
#define OVER_GAME 1
#define IN_GAME 2
#define FINISH_GAME 3
#define SAVE_GAME 4
#define LOAD_GAME 5
#define INVENTORY_GAME 6
#define LEVEL_UP_GAME 7

Player* Player_Accessor::hero;
Marsh::View* v;
WorldName world_name;

volatile int ticks, framerate;
volatile bool rested;
volatile int world_time_counter = 0;
volatile int world_time_delay = 1;

void load_inventory(string stream);
void load_spells(string stream);
void timer_framerate_counter(void);
void timer_game_rester(void);
void set_up_game(void);
void update_world(World*);
void draw_world_to_buffer(World*);
void start_game(void);
void end_game(void);
void show_intro(void);
void save_game(void);
void load_game(void);
void show_inv(void);
void show_screen(Equipment*);
void show_level_up(void);
World* generate_world(void); // TODO add world identifying thingymahinger
Marsh::View* create_view(Player*);

// gamestate
int game_state = INTRO_GAME;
BITMAP* buffer;
FONT *font1;
FONT *font2;
FONT *font3;
SAMPLE *theme;
BITMAP *inv_screen_bitmap;
int mute = 0;

int main(void)
{
	//initialize everything
	set_up_game();
	font1 = load_font("font1.pcx",NULL,NULL);
	font2 = load_font("font2.pcx",NULL,NULL);
	font3 = load_font("font3.pcx",NULL,NULL);
	world_name = test_map;
	theme = load_wav("Resources//Music//main_theme.wav");
	if (!theme) allegro_message("error theme wav");
	else play_sample(theme,255,128,1000,1);

	buffer = create_bitmap(SCREENW,SCREENH);

	while(game_state != FINISH_GAME) {
		if (game_state == INTRO_GAME) show_intro();
		if (game_state == LEVEL_UP_GAME) show_level_up();
	}

	destroy_bitmap(buffer);
	destroy_sample(theme);
	allegro_exit();
	return 0;
}
END_OF_MAIN()

Marsh::View* create_view(Player* hero){
	Marsh::View* v = new Marsh::View(hero);
	v->load_world(main_world16);
	return v;
}


void timer_frame_counter(void) {
	framerate = ticks;
	ticks = 0;
}

void timer_game_rester(void) {
	rested = true;
}

void set_up_game(void) {
	allegro_init();
	install_keyboard();
	install_timer();
	install_mouse();
	install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);

	set_color_depth(COLOR_DEPTH);
	set_gfx_mode(MODE, SCREENW, SCREENH, 0,0);
	set_display_switch_mode(SWITCH_PAUSE);
	set_alpha_blender();

	srand(time(NULL));

	LOCK_VARIABLE(framerate);
	LOCK_VARIABLE(ticks);
	LOCK_VARIABLE(rested);
	LOCK_FUNCTION(timer_frame_counter);
	LOCK_FUNCTION(timer_game_rester);
	install_int(timer_frame_counter, 1000);
	install_int(timer_game_rester, FRAME_RATE_DELAY);

	// default data for inventory - TEST DATA
	Player_Sprite* img = new Player_Sprite("Resources//player//player_sheet.bmp", S, 5, 2, 16, 2*16);
	Player_Accessor::create_player(300, 256, img, 28, 14, 0, 18);
	Player*	hero = Player_Accessor::get_player();
	Equipment* equip = new Equipment();
	Equipment* equip1 = new Equipment();
	Equipment* equip2 = new Equipment(); 
	equip1->name = "Cloth Armor";
	equip1->type = Tunic;
	equip1->vitality = 5;
	equip1->description = "+5 Vit";
	equip1->item_id = 0;
	equip1->equipped = false;
	equip1->equipable = true;
	equip1->number_held = 1;
	hero->add_to_inventory(equip1);
	equip2->name = "Long Sword";
	equip2->type = Dagger;
	equip2->description = "+5 Wp";
	equip2->willpower = 5;
	equip2->item_id = 1;
	equip2->equipped = false;
	equip2->equipable = true;
	equip2->number_held = 1;
	hero->add_to_inventory(equip2);
	equip->name = "None";
	equip->description = "Filler test";
	equip->item_id = -1;
	equip->number_held = -1;
	int i = 2;
	while (i<MAX_HELD_ITEMS) {
		hero->add_to_inventory(equip);
		i += 1;
	}
}

void show_intro(void) {
	BITMAP *title_screen_bitmap = create_bitmap(SCREENW,SCREENH);
	if (!title_screen_bitmap)
		exit(1);
	int menu_sel = 0;
	int max_sel = 3;
	if (game_state == IN_GAME) max_sel = 4;
	while (game_state == INTRO_GAME || game_state == IN_GAME) {
		if (keypressed()) {
			int k = readkey();
			switch(k >> 8) {
				case KEY_ESC: break;
				case KEY_UP: menu_sel--; if (menu_sel < 0) menu_sel = 0; break;
				case KEY_DOWN: menu_sel++; if (menu_sel > max_sel) menu_sel = max_sel; break;
				case KEY_ENTER:
					switch (menu_sel) {
				case 0: game_state=IN_GAME; start_game(); break; // new game
				case 1: game_state=IN_GAME; load_game(); break; // load game
				case 2: game_state=FINISH_GAME; break; // exit game 
				case 3: game_state=IN_GAME; save_game(); break; // save game
				case 4: game_state=IN_GAME; goto exit_loop;
					} break;
				case KEY_M: {
					if (mute==0) {
						mute=1;
						stop_sample(theme);
					}
					else {
						mute=0;
						play_sample(theme,255,128,1000,1);
					}
							}
			}
			clear_keybuf();
		}

		// drawing
		blit(title_screen_bitmap, buffer, 0, 0, 0, 0, SCREENW, SCREENH);

		textprintf_ex(buffer, font2, 50, 20, makecol(255,051,102), -1, "Marsh");
		if (menu_sel == 0) { // new game
			textprintf_ex(buffer, font1, 50,  130, makecol(0,255,255), -1, "New Game");
		} else {
			textprintf_ex(buffer, font1, 50,  130, makecol(255,255,255), -1, "NEW GAME");
		}
		if (menu_sel == 1) { // load game
			textprintf_ex(buffer, font1, 50,  200, makecol(0,255,255), -1, "Load Game");
		} else {
			textprintf_ex(buffer, font1, 50,  200, makecol(255,255,255), -1, "LOAD GAME");
		}
		if (menu_sel == 2) { // exit game
			textprintf_ex(buffer, font1, 50,  270, makecol(0,255,255), -1, "Exit Game");
		} else {
			textprintf_ex(buffer, font1, 50,  270, makecol(255,255,255), -1, "EXIT GAME");
		} 
		if (game_state == IN_GAME) {
			if (menu_sel == 3) { 
				textprintf_ex(buffer, font1, 50,  340, makecol(0,255,255), -1, "Save Game");
			} else {
				textprintf_ex(buffer, font1, 50,  340, makecol(255,255,255), -1, "SAVE GAME");
			} 

			if (menu_sel == 4) {
				textprintf_ex(buffer, font1, 50,  420, makecol(255,0,51), -1, "Return");
			} else {
				textprintf_ex(buffer, font1, 50,  420, makecol(255,255,0), -1, "RETURN");
			} 
		}
		if (mute==0) textprintf_ex(buffer, font, 370, 100, makecol(204,255,204), -1, "Sound On! (M to mute)");
		else textprintf_ex(buffer, font, 370, 100, makecol(204,255,51), -1, "SOUND OFF! (M to unmute)");

		// draw to screen
		blit(buffer, screen, 0,0, 0,0, SCREENW, SCREENH);
	}
exit_loop: ;

	destroy_bitmap(title_screen_bitmap);
	return;
}

void start_game(void) {
	game_state = IN_GAME;
	Player*	hero = Player_Accessor::get_player();
	Marsh::View *our_viewer= create_view(hero);
	hero->set_my_type(Hero);

	while(game_state == IN_GAME) {
		if (key[KEY_ESC]) {
			show_intro();
		} 
		if (keyrel(KEY_I)) {
			show_inv();			
		}
		if (keyrel(KEY_L)) {
			show_level_up();
		}
		if (!rested) {
			rest(4);
			continue;
		}
		rested = false;
		ticks++;
		if (++world_time_counter >= world_time_delay){
			world_time_counter = 0;
			our_viewer->update();
		}
		our_viewer->update();
		hero->update();

		textprintf_centre_ex(screen,font,100,20,makecol(255,255,255),-1,"FRAMERATE %d", framerate);		
		textprintf_centre_ex(screen,font,100,30,makecol(255,255,255),-1,"SIZE %d ", sizeof(Combat));
		clear_keybuf();
	}

	//delete hero;
	delete our_viewer;
}

void load_inventory(string stream) {
	std::vector<int> item_ids;
	std::stringstream ss(stream);
	int i;
	while (ss >> i)
	{
		item_ids.push_back(i);
		if (ss.peek() == ',') ss.ignore();
	}
	//std::vector<Equipment*>* inventory;
}

void load_spells(string stream) {
	std::vector<int> spell_ids;
	std::stringstream ss(stream);
	int i;
	while (ss >> i)
	{
		spell_ids.push_back(i);
		if (ss.peek() == ',') ss.ignore();
	}
	i = 0;
	AttackDB* db = new AttackDB();
	std::vector<int>::iterator end = spell_ids.end();
	for (std::vector<int>::iterator iter = spell_ids.begin(); iter != end; ++iter){
		Player_Accessor::get_player()->attack_loadout[i] = db->fetch_attack(spell_ids[i])->clone(0,0,W);
		++i;
	}
	delete db;
}

void load_game(void) {
	int x_pos, y_pos, height, width;
	int level, experience, notoriety, stats, mana, max_mana, health, max_health, gold;
	int world;
	Player_Sprite* img = new Player_Sprite("Resources//player//player_sheet.bmp", S, 5, 2, 16, 32);

	ifstream file1("Save1.marsh");
	string line;
	string beg, end;
	if (file1.is_open()) {
		while (file1.good()) {
			getline(file1,line);
			istringstream iss(line);
			iss >> beg;
			iss >> end;
			if (beg.compare("X-pos") == 0) stringstream(end) >> x_pos;
			else if (beg.compare("Y-pos") == 0) stringstream(end) >> y_pos;
			else if (beg.compare("Height") == 0) stringstream(end) >> height;
			else if (beg.compare("Width") == 0) stringstream(end) >> width;
			else if (beg.compare("Level") == 0) stringstream(end) >> level;
			else if (beg.compare("Experience") == 0) stringstream(end) >> experience;
			else if (beg.compare("Notoriety") == 0) stringstream(end) >> notoriety;
			else if (beg.compare("Stats") == 0) stringstream(end) >> stats;
			else if (beg.compare("Mana") == 0) stringstream(end) >> mana;
			else if (beg.compare("Max-mana") == 0) stringstream(end) >> max_mana;
			else if (beg.compare("Health") == 0) stringstream(end) >> health;
			else if (beg.compare("Max-health") == 0) stringstream(end) >> max_health;
			else if (beg.compare("Gold") == 0) stringstream(end) >> gold;
			else if (beg.compare("World") == 0) stringstream(end) >> world;
			else if (beg.compare("Inventory") == 0) load_inventory(end);
			else if (beg.compare("Spell-loadout") == 0) load_spells(end);
		}
	}
	world_name = static_cast<WorldName>(world);
	Player_Accessor::create_player(x_pos, y_pos, img, width, height, 0, 18);
	start_game();
}

void save_game(void) {
	Player* hero = Player_Accessor::get_player();
	ofstream file1("Save1.marsh");

	// create player
	file1 << "X-pos " << hero->get_x_pos() << endl;
	file1 << "Y-pos " << hero->get_y_pos() << endl;
	file1 << "Height " << hero->get_bounding_height() << endl;
	file1 << "Width " << hero->get_bounding_width() << endl;

	// player extras
	file1 << "Level " << hero->level << endl;
	file1 << "Experience " << hero->current_experience << endl;
	file1 << "Notoriety " << hero->notoriety << endl;
	file1 << "Stats " << hero->statPoints << endl; 
	file1 << "Mana " << hero->get_current_mana() << endl;
	file1 << "Max-mana " << hero->get_max_mana() << endl;
	file1 << "Health " << hero->get_current_health() << endl;
	file1 << "Max-health " << hero->get_max_health() << endl;
	file1 << "Gold " << hero->gold << endl;

	// world
	int world = v->current_world->my_name;
	file1 << "World " << world << endl;
	
	// player inventory
	std::string items;
	std::stringstream line;
	std::vector<Equipment*> inventory = *hero->get_inventory();
	std::vector<Equipment*>::iterator end = inventory.end();
	for (std::vector<Equipment*>::iterator iter = inventory.begin(); iter != end; ++iter){
		line << (*iter)->item_id;
		if (iter < (end-1)) line << ",";
	}
	items = line.str();
	file1 << "Inventory " << items << endl;

	// player spell loadout
	Attack* loadout = *hero->attack_loadout;
	int i=0;
	for (i; i<MAX_ATTACKS; i++) {
		line << loadout[i].spell_id;
		if (i < (MAX_ATTACKS-1)) line << ",";
	}
	items.clear();
	items = line.str();
	file1 << "Spell-loadout " << items << endl;

	file1.close();
	cout << "Game saved to Save1.marsh!";

	show_intro();
}

void show_inv(void) { // show inventory items in a list as well as quanitty (click each item to view what they do)
	inv_screen_bitmap = create_bitmap(SCREENW,SCREENH);
	Player* hero = Player_Accessor::get_player();
	int menu_sel = 0;
	std::vector<Equipment*> inventory = *hero->get_inventory();
	int max_sel = MAX_HELD_ITEMS;
	clear_keybuf();

	while (game_state == INVENTORY_GAME || game_state == IN_GAME) {
		if(keyrel(KEY_I))
			goto exit_loop;
		if (keypressed()) {
			int k = readkey();
			switch(k >> 8) {
				case KEY_ESC: break;
				case KEY_UP: menu_sel--; if (menu_sel < 0) menu_sel = 0; break;
				case KEY_DOWN: menu_sel++; if (menu_sel > max_sel) menu_sel = max_sel; break;
				case KEY_ENTER:
					switch (menu_sel) {
				case 0: show_screen(inventory[0]); break; 
				case 1: show_screen(inventory[1]); break; 
				case 2: show_screen(inventory[2]); break; 
				case 3: show_screen(inventory[3]); break; 
				case 4: show_screen(inventory[4]); break; 
				case 5: show_screen(inventory[5]); break;
				case 6: show_screen(inventory[6]); break;
				case 7: show_screen(inventory[7]); break;
				case 8: show_screen(inventory[8]); break;
				case 9: show_screen(inventory[9]); break;
				case 10: show_screen(inventory[10]); break;
				case 11: game_state=IN_GAME; goto exit_loop;
					} break;
			}
			clear_keybuf();
		}

		// drawing
		blit(inv_screen_bitmap, buffer, 0, 0, 0, 0, SCREENW, SCREENH);

		textprintf_ex(buffer, font2, 50, 20, makecol(255,051,102), -1, "Character");
		textprintf_ex(buffer, font3, 50, 160, makecol(56,235,181), -1, "Inventory");
		textprintf_ex(buffer, font3, 350, 160, makecol(56,235,181), -1, "Equipment");
		textprintf_ex(buffer, font3, 650, 160, makecol(56,235,181), -1, "Stats");
		textprintf_ex(buffer, font, 350, 260, makecol(236,221,9), -1, "Armor");
		textprintf_ex(buffer, font, 350, 280, makecol(236,221,9), -1, "Weapon");
		textprintf_ex(buffer, font, 350, 300, makecol(236,221,9), -1, "Helmet");
		textprintf_ex(buffer, font, 350, 320, makecol(236,221,9), -1, "Boots");
		textprintf_ex(buffer, font, 350, 340, makecol(236,221,9), -1, "Jewelry");
		textprintf_ex(buffer, font, 650, 260, makecol(236,221,9), -1, "Vitality");
		textprintf_ex(buffer, font, 650, 280, makecol(236,221,9), -1, "Willpower");
		textprintf_ex(buffer, font, 650, 300, makecol(236,221,9), -1, "Intelligence");
		textprintf_ex(buffer, font, 650, 320, makecol(236,221,9), -1, "Focus");
		textprintf_ex(buffer, font, 710, 300, makecol(236,145,9), -1, "0");
		textprintf_ex(buffer, font, 710, 320, makecol(236,145,9), -1, "0");

		int count = 0;
		int start_x = 50;
		int start_y = 260;
		while (count < inventory.size()) {
			if (inventory[count]->equipable == true) {
				if (inventory[count]->equipped == true) {
					if (inventory[count]->item_id == 0) {
						textprintf_ex(buffer, font, 410, 260, makecol(236,145,9), -1, "%s (%s)", inventory[count]->name, inventory[count]->description);
						textprintf_ex(buffer, font, 710, 260, makecol(236,145,9), -1, "%d", hero->vitality);
					} else {
						textprintf_ex(buffer, font, 410, 280, makecol(236,145,9), -1, "%s (%s)", inventory[count]->name, inventory[count]->description);
						textprintf_ex(buffer, font, 710, 280, makecol(236,145,9), -1, "%d", hero->willpower);
					}
				} else {
					if (inventory[count]->item_id == 0) {
						textprintf_ex(buffer, font, 410, 260, makecol(236,145,9), -1, "");
						textprintf_ex(buffer, font, 710, 260, makecol(236,145,9), -1, "0");
					} else {
						textprintf_ex(buffer, font, 410, 280, makecol(236,145,9), -1, "");
						textprintf_ex(buffer, font, 710, 280, makecol(236,145,9), -1, "0");
					}
				}
			}
			if (menu_sel != count) { 
				textprintf_ex(buffer, font, start_x,  start_y, makecol(0,255,255), -1, "(%d) %s", inventory[count]->number_held, inventory[count]->name);
			} else {	
				if (inventory[count]->equipable == true) {
					if (inventory[count]->equipped == false) {
						textprintf_ex(buffer, font, start_x,  start_y, makecol(255,255,255), -1, "%s [Enter to equip]", inventory[count]->description);
					} else {
						textprintf_ex(buffer, font, start_x,  start_y, makecol(255,255,255), -1, "%s [Enter to unequip]", inventory[count]->description);
					}
				} else {
					textprintf_ex(buffer, font, start_x,  start_y, makecol(255,255,255), -1, "%s", inventory[count]->description);
				}
			}
			count+=1;
			start_y+=20;
		}
		if (menu_sel == 11) {
			textprintf_ex(buffer, font1, start_x,  start_y+19*MAX_HELD_ITEMS, makecol(255,0,51), -1, "Return");
		} else {
			textprintf_ex(buffer, font1, start_x,  start_y+19*MAX_HELD_ITEMS, makecol(255,255,0), -1, "RETURN");
		} 

		// draw to screen
		blit(buffer, screen, 0,0, 0,0, SCREENW, SCREENH);
	}
exit_loop: ;

	destroy_bitmap(inv_screen_bitmap);
	return;
}

void show_screen(Equipment* equip) {
	if (equip->item_id == -1) {
		return;
	}
	if (equip->equipable == true) {
		if (equip->equipped == false) {
			Player_Accessor::get_player()->equip_item(equip);
		} else {
			Player_Accessor::get_player()->unequip_item(equip);
		}
	}

	if (equip->type == Consumable && equip->number_held > 0){
		Player_Accessor::get_player()->use_consumable(equip);
	}
	//return; // do something with clicked item
}

Equipment* get_new_equipment(void){
	Equipment* item = (Equipment*)malloc(sizeof(Equipment));
	item->name = "";
	item->description = "";
	item->item_id = -1;
	item->vitality = 0;
	item->focus = 0;
	item->intelligence = 0;
	item->willpower = 0;
	item->armor = 0;
	item->equipped = false;
	item->equipable = false;
	item->stackable = false;
	item->number_held = 0;
	item->type = Unitialized;
	return item;
}

Equipment* get_item_clone(Equipment* old_item){
	Equipment* item = (Equipment*)malloc(sizeof(Equipment));
	item->name = old_item->name;
	item->description = old_item->description;
	item->item_id = old_item->item_id;
	item->vitality = old_item->vitality;
	item->focus = old_item->focus;
	item->intelligence = old_item->intelligence;
	item->willpower = old_item->willpower;
	item->armor = old_item->armor;
	item->equipped = old_item->equipped;
	item->equipable = old_item->equipable;
	item->stackable = old_item->stackable;
	item->number_held = old_item->number_held;
	item->type = old_item->type;
	return item;

}

void addStat(int selection){
	Player* hero = Player_Accessor::get_player();
	if(hero->statPoints > 0){
		switch(selection){
			case 0: hero->vitality += STAT_INCREASE; break;
			case 1: hero->willpower += STAT_INCREASE; break;
			case 2: hero->intelligence += STAT_INCREASE; break;
			case 3: hero->focus += STAT_INCREASE; break;
			default: return;
		}
		hero->statPoints--;
	}
}

void show_level_up(void) { // show level up menu
	LevelUp^ menu = gcnew LevelUp(Player_Accessor::get_player());
	menu->StartPosition = FormStartPosition::CenterScreen;
	menu->ShowDialog();
	delete menu;
/*
	inv_screen_bitmap = create_bitmap(SCREENW,SCREENH);
	bool is_done = false;
	volatile bool* done_ref = &is_done;
#pragma omp parallel num_threads(2) shared(done_ref)
	{
		int my_id = omp_get_thread_num();
		if (my_id == 0){
			while(!done_ref){
				// wait for a while
			}
		} else {
			LevelUp^ menu = gcnew LevelUp();
			menu->BringToFront();
			menu->ShowDialog();
			Application::Run(menu);
		}

		//	

	}
	Player* hero = Player_Accessor::get_player();
	int menu_sel = 0;
	int max_sel = 4;
	while (game_state == LEVEL_UP_GAME || game_state == IN_GAME) {
		if (keypressed()) {
			int k = readkey();
			switch(k >> 8) {
				case KEY_ESC: break;
				case KEY_UP: menu_sel--; if (menu_sel < 0) menu_sel = 0; break;
				case KEY_DOWN: menu_sel++; if (menu_sel > max_sel) menu_sel = max_sel; break;
				case KEY_ENTER:
					switch (menu_sel) {
				case 0: addStat(0); break; 
				case 1: addStat(1); break; 
				case 2: addStat(2); break; 
				case 3: addStat(3); break;  
				case 4: game_state=IN_GAME; goto exit_level_loop;
					} break;
			}
			clear_keybuf();
		}

		// drawing
		blit(inv_screen_bitmap, buffer, 0, 0, 0, 0, SCREENW, SCREENH);

		textprintf_ex(buffer, font, 150, 120, makecol(236,221,9), -1, "Available Stat Points:");
		textprintf_ex(buffer, font, 360, 120, makecol(236,145,9), -1, "%d", hero->statPoints);

		textprintf_ex(buffer, font, 150, 260, makecol(255,0,51), -1, "Vitality");
		textprintf_ex(buffer, font, 150, 280, makecol(255,0,51), -1, "Willpower");
		textprintf_ex(buffer, font, 150, 300, makecol(255,0,51), -1, "Intelligence");
		textprintf_ex(buffer, font, 150, 320, makecol(255,0,51), -1, "Focus");
		textprintf_ex(buffer, font, 310, 260, makecol(255,0,51), -1, "%d", hero->vitality);
		textprintf_ex(buffer, font, 310, 280, makecol(255,0,51), -1, "%d", hero->willpower);
		textprintf_ex(buffer, font, 310, 300, makecol(255,0,51), -1, "%d", hero->intelligence);
		textprintf_ex(buffer, font, 310, 320, makecol(255,0,51), -1, "%d", hero->focus);

		int start_x = 50;
		int start_y = 260;

		if(menu_sel == 0){
			textprintf_ex(buffer, font, 150, 260, makecol(255,255,0), -1, "Vitality");
		}
		else if(menu_sel == 1){
			textprintf_ex(buffer, font, 150, 280, makecol(255,255,0), -1, "Willpower");
		}
		else if(menu_sel == 2){
			textprintf_ex(buffer, font, 150, 300, makecol(255,255,0), -1, "Intelligence");
		}
		else if(menu_sel == 3){
			textprintf_ex(buffer, font, 150, 320, makecol(255,255,0), -1, "Focus");
		}
		if (menu_sel == 4) {
			textprintf_ex(buffer, font1, start_x,  start_y+19*MAX_HELD_ITEMS, makecol(255,255,0), -1, "Return");
		} else {
			textprintf_ex(buffer, font1, start_x,  start_y+19*MAX_HELD_ITEMS, makecol(255,0,51), -1, "RETURN");
		} 

		// draw to screen
		blit(buffer, screen, 0,0, 0,0, SCREENW, SCREENH);
	}
exit_level_loop: ;

	destroy_bitmap(inv_screen_bitmap);
	return;*/
}
