#include "Main.h"
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
volatile bool started;

volatile int ticks, framerate;
volatile bool rested;
volatile int world_time_counter = 0;
volatile int world_time_delay = DEFAULT_WORLD_TIME_DELAY;
AttackDB* attackDB;
ItemDB* itemDB;

void new_game(void);
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
void load_game(std::string);
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
	world_name = main_world11;
	theme = load_wav("Resources//Music//main_theme.wav");
	if (!theme) allegro_message("error theme wav");
	else play_sample(theme,255,128,1000,1);

	buffer = create_bitmap(SCREENW,SCREENH);

	while(game_state != FINISH_GAME) {
		if (game_state == INTRO_GAME) show_intro();
		if (game_state == LEVEL_UP_GAME) show_level_up();
		if (game_state == LOAD_GAME) load_game("Save1.Marsh");
	}

	destroy_bitmap(buffer);
	destroy_sample(theme);
	allegro_exit();
	return 0;
}
END_OF_MAIN()

Marsh::View* create_view(Player* hero){
	v = new Marsh::View(hero);
	v->load_world(world_name);
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

	attackDB = new AttackDB();
	itemDB = new ItemDB();

	// default data for inventory - TEST DATA
	Player_Sprite* img = new Player_Sprite("Resources//player//player_sheet.bmp", S, 5, 2, 16, 2*16);
	Player_Accessor::create_player(300, 256, img, 28, 14, 0, 18);
	Player*	hero = Player_Accessor::get_player();
	Equipment* equip = new Equipment();
	//Equipment* equip1 = new Equipment();
	//Equipment* equip2 = new Equipment(); 
	/*equip1->name = "Cloth Armor";
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
	hero->add_to_inventory(equip2);*/
/*	for(int i = 0; i <= 21; i++){

	Equipment* equip = new Equipment(); 
	for(int i = 0; i <= 22; i++){
		equip = itemDB->fetch_item(i);
		equip->number_held = 1;
		hero->add_to_inventory(equip);
		equip = new Equipment();
	}*/
	/*equip->name = "None";
	equip->description = "Filler test";
	equip->item_id = -1;
	equip->number_held = -1;
	int i = 2;
	while (i<MAX_HELD_ITEMS) {
		hero->add_to_inventory(equip);
		i += 1;
	}*/


}

void show_intro(void) {
	BITMAP *title_screen_bitmap = create_bitmap(SCREENW,SCREENH);
	if (!title_screen_bitmap)
		exit(1);
	int menu_sel = 0;
	int max_sel = 2;
	if (game_state == IN_GAME) max_sel = 3;
	while (game_state == INTRO_GAME || game_state == IN_GAME) {
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
				textprintf_ex(buffer, font1, 50,  340, makecol(255,0,51), -1, "Return");
			} else {
				textprintf_ex(buffer, font1, 50,  340, makecol(255,255,0), -1, "RETURN");
			} 
		}
		if (mute==0) textprintf_ex(buffer, font, 370, 100, makecol(204,255,204), -1, "Sound On! (M to mute)");
		else textprintf_ex(buffer, font, 370, 100, makecol(204,255,51), -1, "SOUND OFF! (M to unmute)");

		// draw to screen
		blit(buffer, screen, 0,0, 0,0, SCREENW, SCREENH);
		if (keypressed()) {
			int k = readkey();
			switch(k >> 8) {
				case KEY_ESC: break;
				case KEY_UP: menu_sel--; if (menu_sel < 0) menu_sel = max_sel; break;
				case KEY_DOWN: menu_sel++; if (menu_sel > max_sel) menu_sel = 0; break;
				case KEY_ENTER:
					switch (menu_sel) {
				case 0: game_state=IN_GAME; new_game(); break; // new game
				case 1: game_state=LOAD_GAME; load_game("Save1.Marsh"); break; // load game
				case 2: game_state=FINISH_GAME; break; // exit game 
			//	case 3: game_state=IN_GAME; save_game(); break; // save game
				case 3: game_state=IN_GAME; goto exit_loop;
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
	}
exit_loop: ;

	destroy_bitmap(title_screen_bitmap);
	return;
}

BITMAP* draw_Death(void) {
	BITMAP *death_screen_bitmap = load_bitmap("Resources//DeathScreen.bmp",NULL);
	if (!death_screen_bitmap){
		allegro_message("Failed to load the death screen."); 
		exit(1);	
	}
	blit(death_screen_bitmap, screen, 0,0, 0,0, 1400, 1000);
	return death_screen_bitmap;
}

void restartWithDeathScreen(void){
	BITMAP* deathScreen = draw_Death();
	rest(5000);
	destroy_bitmap(deathScreen);
	game_state = LOAD_GAME;
}

void new_game(void){
	CopyFile("init.Marsh", "Save1.Marsh", false);
	load_game("init.Marsh");
}

void start_game(void) {
	game_state = IN_GAME;
	Player*	hero = Player_Accessor::get_player();
	while(game_state == IN_GAME) {
		if(hero->dead){
			restartWithDeathScreen();
			return;
		}
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
		if(keyrel(KEY_J)){
			hero->experience += 50;
		}
		rested = false;
		ticks++;

		if (++world_time_counter >= world_time_delay){
			world_time_counter = 0;
			v->update();
		}

		hero->update();
		v->draw_active_world();

		if(hero->new_mission){
			save_game();
			hero->new_mission = false;
		}

		textprintf_centre_ex(screen,font,100,20,makecol(255,255,255),-1,"FRAMERATE %d", framerate);		
		textprintf_centre_ex(screen,font,100,30,makecol(255,255,255),-1,"Position %dx%d ", hero->x_pos, hero->y_pos);
		clear_keybuf();

		//if (started) save_game();
	}

	//delete hero;
	delete v;
}

void load_inventory(string stream, string stream1, string stream2) {
	Player* hero = Player_Accessor::get_player();
	std::vector<int> item_ids;
	std::vector<bool> item_equip;
	std::vector<int> item_held;
	std::stringstream ss(stream);
	std::stringstream ss1(stream1);
	std::stringstream ss2(stream2);
	int i;
	while (ss >> i)
	{
		item_ids.push_back(i);
		if (ss.peek() == ',') ss.ignore();
	}
	bool i1;
	while (ss1 >> i1)
	{
		item_equip.push_back(i1);
		if (ss1.peek() == ',') ss1.ignore();
	}
	while (ss2 >> i)
	{
		item_held.push_back(i);
		if (ss2.peek() == ',') ss2.ignore();
	}
	i = 0;
	std::vector<int>::iterator end = item_ids.end();
	Equipment* equip = new Equipment();
	for (std::vector<int>::iterator iter = item_ids.begin(); iter != end; ++iter){
		equip = itemDB->fetch_item(item_ids[i]);
		equip->equipped = item_equip[i];
		equip->number_held = item_held[i];
		hero->add_to_inventory(equip);
		equip = new Equipment();
		++i;
	}
}

void load_spells(string stream) {
	Player* hero = Player_Accessor::get_player();
	std::vector<int> spell_ids;
	std::stringstream ss(stream);
	int i;
	while (ss >> i)
	{
		spell_ids.push_back(i);
		if (ss.peek() == ',') ss.ignore();
	}
	i = 0;
	std::vector<int>::iterator end = spell_ids.end();
	for (std::vector<int>::iterator iter = spell_ids.begin(); iter != end; ++iter){
		hero->attack_loadout[i] = attackDB->fetch_attack(spell_ids[i])->clone(0,0,W);
		++i;
	}
}

void load_game(std::string file) {
	int x_pos, y_pos, height, width;
	int level, experience, notoriety, stats, mana, max_mana, health, max_health, gold, spell_pts;
	int world, vitality, focus, intelligence, willpower, armor;
	Player_Sprite* img = new Player_Sprite("Resources//player//player_sheet.bmp", S, 5, 2, 16, 32);
	std::string items, item_equip, item_held; 

	ifstream file1(file.c_str());
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
			else if (beg.compare("Spell-pts") == 0) stringstream(end) >> spell_pts;
			else if (beg.compare("Mana") == 0) stringstream(end) >> mana;
			else if (beg.compare("Max-mana") == 0) stringstream(end) >> max_mana;
			else if (beg.compare("Health") == 0) stringstream(end) >> health;
			else if (beg.compare("Max-health") == 0) stringstream(end) >> max_health;
			else if (beg.compare("Gold") == 0) stringstream(end) >> gold;
			else if (beg.compare("World") == 0) stringstream(end) >> world;
			else if (beg.compare("Inventory") == 0) stringstream(end) >> items;
			else if (beg.compare("Inventory1") == 0) stringstream(end) >> item_equip;
			else if (beg.compare("Inventory2") == 0) stringstream(end) >> item_held;
			else if (beg.compare("Spell-loadout") == 0) load_spells(end);
			else if (beg.compare("Vitality") == 0) stringstream(end) >> vitality;
			else if (beg.compare("Intellligence") == 0) stringstream(end) >> intelligence;
			else if (beg.compare("Focus") == 0) stringstream(end) >> focus;
			else if (beg.compare("Willpower") == 0) stringstream(end) >> willpower;
			else if (beg.compare("Armor") == 0) stringstream(end) >> armor;

		}
	}
	load_inventory(items,item_equip,item_held);
	world_name = static_cast<WorldName>(world);
	Player_Accessor::create_player(x_pos, y_pos, img, width, height, 0, 18);
	Player*	hero = Player_Accessor::get_player();
	hero->level = level;
	hero->experience = experience;
	hero->notoriety = notoriety;
	hero->statPoints = stats;
	hero->spellPoints = spell_pts;
	hero->mana = mana;
	hero->max_mana = max_mana;
	hero->health = health;
	hero->max_health = max_health;
	hero->gold = gold;
	hero->vitality = vitality;
	hero->intelligence = intelligence;
	hero->focus = focus;
	hero->willpower = willpower;
	hero->armor = armor;
	create_view(hero);
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
	file1 << "Experience " << hero->experience << endl;
	file1 << "Notoriety " << hero->notoriety << endl;
	file1 << "Stats " << hero->statPoints << endl; 
	file1 << "Spell-pts " << hero->spellPoints << endl;
	file1 << "Mana " << hero->get_current_mana() << endl;
	file1 << "Max-mana " << hero->get_max_mana() << endl;
	file1 << "Health " << hero->get_current_health() << endl;
	file1 << "Max-health " << hero->get_max_health() << endl;
	file1 << "Gold " << hero->gold << endl;
	file1 << "Vitality " << hero->vitality << endl;
	file1 << "Intelligence " << hero->intelligence << endl;
	file1 << "Focus " << hero->focus << endl;
	file1 << "Willpower " << hero->willpower << endl;
	file1 << "Armor " << hero->armor << endl;

	// world
	int world = v->current_world->my_name;
	file1 << "World " << world << endl;

	// player inventory
	std::string items;
	std::string item_equip;
	std::string item_held;
	std::stringstream line; // id's
	std::stringstream line1; // equipped
	std::stringstream line2; // number_held
	std::stringstream line3; // attacks
	std::vector<Equipment*> inventory = *hero->get_inventory();
	std::vector<Equipment*>::iterator end = inventory.end();
	for (std::vector<Equipment*>::iterator iter = inventory.begin(); iter != end; ++iter){
		line << (*iter)->item_id;
		line1 << (*iter)->equipped;
		line2 << (*iter)->number_held;
		if (iter < (end-1)) { 
			line << ",";
			line1 << ",";
			line2 << ",";
		}
	}
	items = line.str();
	item_equip = line1.str();
	item_held = line2.str();
	file1 << "Inventory " << items << endl;
	file1 << "Inventory1 " << item_equip << endl;
	file1 << "Inventory2 " << item_held << endl;

	int i=0;
	for (i; i<MAX_ATTACKS; i++) {
		if(hero->attack_loadout[i] != NULL){
			line3 << hero->attack_loadout[i]->spell_id;
			if (hero->attack_loadout[i+1] != NULL) line3 << ",";
		}
	}

	items.clear();
	items = line3.str();
	file1 << "Spell-loadout " << items << endl;

	file1.close();
	
	//if (!started) show_intro();
}

void show_inv(void) { // show inventory items in a list as well as quanitty (click each item to view what they do)
	inv_screen_bitmap = create_bitmap(SCREENW,SCREENH);
	Player* hero = Player_Accessor::get_player();
	int menu_sel = 0;
	int max_sel = 0;
	std::vector<Equipment*> inventory = *hero->get_inventory();
	clear_keybuf();

	while (game_state == INVENTORY_GAME || game_state == IN_GAME) {
		max_sel = inventory.size()-1;
		if(keyrel(KEY_I))
			goto exit_loop;
		if (keypressed()) {
			int k = readkey();
			switch(k >> 8) {
				case KEY_ESC: goto exit_loop; break;
				case KEY_UP: menu_sel--; if (menu_sel < 0) menu_sel = 0; break;
				case KEY_DOWN: menu_sel++; if (menu_sel > max_sel) menu_sel = max_sel; break;
				case KEY_ENTER:
					if(menu_sel < inventory.size())
						show_screen(inventory[menu_sel]);break;
				case KEY_DEL: 
					if(menu_sel < inventory.size()){
						Equipment* delete_Item = inventory[menu_sel];
						if(delete_Item->type != QuestItem){
							hero->remove_from_inventory(delete_Item);
							delete delete_Item;
							inventory = *hero->get_inventory();
						}
						break;
					}
			}
			clear_keybuf();
		}

		// drawing
		blit(inv_screen_bitmap, buffer, 0, 0, 0, 0, SCREENW, SCREENH);

		textprintf_ex(buffer, font2, 50, 20, makecol(255,051,102), -1, "Character");
		textprintf_ex(buffer, font3, 50, 900, makecol(255,255,255), -1, "Use the i key to return to the game or esc to exit the menu to the pause screen.");
		textprintf_ex(buffer, font3, 50, 145, makecol(255,255,255), -1, "Use the arrow keys to scroll trough the items. Only 25 items can be kept in your inventory");
		textprintf_ex(buffer, font3, 50, 175, makecol(255,255,255), -1, "at a time. Use the Delete key to remove items permanently. [Quest Items cannot be deleted]");
		textprintf_ex(buffer, font3, 50, 220, makecol(56,235,181), -1, "Inventory");
		textprintf_ex(buffer, font3, 540, 220, makecol(56,235,181), -1, "Equipment");
		textprintf_ex(buffer, font3, 950, 220, makecol(56,235,181), -1, "Stats");
		textprintf_ex(buffer, font, 540, 260, makecol(236,221,9), -1, "Armor");
		textprintf_ex(buffer, font, 540, 280, makecol(236,221,9), -1, "Weapon");
		textprintf_ex(buffer, font, 540, 300, makecol(236,221,9), -1, "Helmet");
		textprintf_ex(buffer, font, 540, 320, makecol(236,221,9), -1, "Boots");
		textprintf_ex(buffer, font, 540, 340, makecol(236,221,9), -1, "Jewelry");
		textprintf_ex(buffer, font, 950, 260, makecol(236,221,9), -1, "Vitality");
		textprintf_ex(buffer, font, 950, 280, makecol(236,221,9), -1, "Willpower");
		textprintf_ex(buffer, font, 950, 300, makecol(236,221,9), -1, "Intelligence");
		textprintf_ex(buffer, font, 950, 320, makecol(236,221,9), -1, "Focus");
		textprintf_ex(buffer, font, 950, 340, makecol(236,221,9), -1, "Armor");
		textprintf_ex(buffer, font, 1055, 260, makecol(236,145,9), -1, "%d", hero->vitality);
		textprintf_ex(buffer, font, 1055, 280, makecol(236,145,9), -1, "%d", hero->willpower);
		textprintf_ex(buffer, font, 1055, 300, makecol(236,145,9), -1, "%d", hero->intelligence);
		textprintf_ex(buffer, font, 1055, 320, makecol(236,145,9), -1, "%d", hero->focus);
		textprintf_ex(buffer, font, 1055, 340, makecol(236,145,9), -1, "%d", hero->armor);

		int count = 0;
		int start_x = 50;
		int start_y = 260;
		while (count < inventory.size()) {
			if (inventory[count]->equipable == true) {
				if (inventory[count]->equipped == true) {
					if (inventory[count]->type == Armor) {
						textprintf_ex(buffer, font, 600, 260, makecol(236,145,9), -1, "%s", inventory[count]->name.c_str());
					}
					if (inventory[count]->type == Weapon){
						textprintf_ex(buffer, font, 600, 280, makecol(236,145,9), -1, "%s", inventory[count]->name.c_str());
					}
					if (inventory[count]->type == Helmet){
						textprintf_ex(buffer, font, 600, 300, makecol(236,145,9), -1, "%s", inventory[count]->name.c_str());
					}
					if (inventory[count]->type == Boots){
						textprintf_ex(buffer, font, 600, 320, makecol(236,145,9), -1, "%s", inventory[count]->name.c_str());
					}
					if (inventory[count]->type == Jewelry){
						textprintf_ex(buffer, font, 600, 340, makecol(236,145,9), -1, "%s", inventory[count]->name.c_str());
					}
				}
			}
			if (menu_sel != count) { 
				textprintf_ex(buffer, font, start_x,  start_y, makecol(0,255,255), -1, "%d: (%d) %s",count+1, inventory[count]->number_held, inventory[count]->name.c_str());
			} else {
				textprintf_ex(buffer, font3, 540,450, makecol(56,235,181), -1, "Selected Item");
				textprintf_ex(buffer, font, 540, 500, makecol(255,255,255), -1, "%s", inventory[count]->name.c_str());
				textprintf_ex(buffer, font, 540, 520, makecol(255,255,255), -1, "%s", inventory[count]->description.c_str());
				textprintf_ex(buffer, font, 540, 540, makecol(255,255,255), -1, "Armor: %d", inventory[count]->armor);
				textprintf_ex(buffer, font, 540, 560, makecol(255,255,255), -1, "Vitality: %d", inventory[count]->vitality);
				textprintf_ex(buffer, font, 540, 580, makecol(255,255,255), -1, "Intelligence: %d", inventory[count]->intelligence);
				textprintf_ex(buffer, font, 540, 600, makecol(255,255,255), -1, "Willpower: %d", inventory[count]->willpower);
				textprintf_ex(buffer, font, 540, 620, makecol(255,255,255), -1, "Focus: %d", inventory[count]->focus);
				textprintf_ex(buffer, font, 540, 640, makecol(255,255,255), -1, "Current Number: %d", inventory[count]->number_held);

				if (inventory[count]->equipable == true) {
					if (inventory[count]->equipped == false) {
						textprintf_ex(buffer, font, start_x,  start_y, makecol(255,255,255), -1, "%s [Enter to equip]", inventory[count]->name.c_str());
					} else {
						textprintf_ex(buffer, font, start_x,  start_y, makecol(255,255,255), -1, "%s [Enter to unequip]", inventory[count]->name.c_str());
					}
				} else {
					textprintf_ex(buffer, font, start_x,  start_y, makecol(255,255,255), -1, "%d: %s",count+1, inventory[count]->name.c_str());
				}
			}
			count+=1;
			start_y+=20;
		}
		if (menu_sel == 26) {
			textprintf_ex(buffer, font1, 400,  start_y*MAX_HELD_ITEMS, makecol(255,0,51), -1, "Return");
		} else {
			textprintf_ex(buffer, font1, 400,  start_y*MAX_HELD_ITEMS, makecol(255,255,0), -1, "RETURN");
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
	Equipment* item = new Equipment();//(Equipment*)malloc(sizeof(Equipment));
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
	Equipment* item = new Equipment();
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
	if (mute==0) {
		stop_sample(theme);
		rest(1000);
	}
	LevelUp^ menu = gcnew LevelUp(Player_Accessor::get_player());
	menu->StartPosition = FormStartPosition::CenterScreen;
	menu->ShowDialog();
	v->draw_updated_loadout(Player_Accessor::get_player()->attack_loadout);
	if (mute==0) {
		play_sample(theme,255,128,1000,1);
	}
	delete menu;
}
