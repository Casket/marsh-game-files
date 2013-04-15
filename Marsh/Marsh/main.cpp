#include "Main.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


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

Player* Player_Accessor::hero;

volatile int ticks, framerate;
volatile bool rested;

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
void show_screen(int);
World* generate_world(void); // TODO add world identifying thingymahinger
View* create_view(Player*);

// gamestate
int game_state = INTRO_GAME;
BITMAP* buffer;
FONT *font1;
FONT *font2;
SAMPLE *theme;
int mute = 0;

int main(void)
{
	//initialize everything
	set_up_game();
	font1 = load_font("font1.pcx",NULL,NULL);
	font2 = load_font("font2.pcx",NULL,NULL);
	theme = load_wav("Resources//Music//main_theme.wav");
	if (!theme) allegro_message("error theme wav");
	else play_sample(theme,255,128,1000,1);

	buffer = create_bitmap(SCREENW,SCREENH);

	while(game_state != FINISH_GAME) {
		if (game_state == INTRO_GAME) show_intro();
		if (game_state == SAVE_GAME) save_game();
		if (game_state == LOAD_GAME) load_game();
		if (game_state == INVENTORY_GAME) show_inv();
	}

	destroy_bitmap(buffer);
	destroy_sample(theme);
	allegro_exit();
	return 0;
}
END_OF_MAIN()

View* create_view(Player* hero){
	View* v = new View(hero);
	v->load_world("Resources//friday_map.txt");
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

	srand(time(NULL));

	LOCK_VARIABLE(framerate);
	LOCK_VARIABLE(ticks);
	LOCK_VARIABLE(rested);
	LOCK_FUNCTION(timer_frame_counter);
	LOCK_FUNCTION(timer_game_rester);
	install_int(timer_frame_counter, 1000);
	install_int(timer_game_rester, FRAME_RATE_DELAY);

	// default data for inventory
	Player_Sprite* img = new Player_Sprite("Resources//player//player_sheet.bmp", S, 5, 1, 16, 16);
	Player_Accessor::create_player(300, 400, img, 28, 14, 0, 18);
	Player*	hero = Player_Accessor::get_player();
	Equipment* equip = new Equipment();
	equip->name = "Empty";
	equip->description = "There is no description for this item.";
	equip->item_id = -1;
	int i = 0;
	while (i<MAX_HELD_ITEMS) {
		hero->add_to_inventory(equip);
		i += 1;
	}
}

void show_intro(void) {
	BITMAP *title_screen_bitmap = create_bitmap(SCREENW,SCREENH);
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
				case 1: game_state=LOAD_GAME; break; // load game
				case 2: game_state=FINISH_GAME; break; // exit game 
				case 3: game_state=SAVE_GAME; break; // save game
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
		if (menu_sel == 3) { // back to game
			textprintf_ex(buffer, font1, 50,  340, makecol(0,255,255), -1, "Save Game");
		} else {
			textprintf_ex(buffer, font1, 50,  340, makecol(255,255,255), -1, "SAVE GAME");
		} 
		if (game_state == IN_GAME) {
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

	View *our_viewer= create_view(hero);

	while(game_state == IN_GAME) {
		if (key[KEY_ESC]) {
			show_intro();
		} 
		if (key[KEY_I]) {
			show_inv();
		}


		if (!rested) {
			rest(4);
			continue;
		}
		rested = false;
		ticks++;

		our_viewer->update();
		//hero->update();
		our_viewer->draw_active_world();

		//masked_blit(hero->get_image()->get_current_frame(), screen, 0, 0,
		//	SCREEN_W/2, SCREEN_H/2, 32,30);
		textprintf_centre_ex(screen,font,100,20,makecol(255,255,255),-1,"FRAMERATE %d", framerate);		
		//textprintf_centre_ex(screen,font,100,30,makecol(255,255,255),-1,"Player Coord %d x %d", hero->get_x_pos()/32, hero->get_y_pos()/32);
		//textprintf_centre_ex(screen,font,100,40,makecol(255,255,255),-1,"Player Coord %d x %d", hero->get_x_pos(), hero->get_y_pos());		
		//textprintf_centre_ex(screen,font,100,50,makecol(255,255,255),-1,"Player Can Walk %d", hero->can_walk);		
		clear_keybuf();
	}

	//delete hero;
	delete our_viewer;
}

void load_game(void) {
	// todo
	return;
}

void save_game(void) {
	//todo
	return;
}

void show_inv(void) { // show inventory items in a list as well as quanitty (click each item to view what they do)
	BITMAP *inv_screen_bitmap = create_bitmap(SCREENW,SCREENH);
	Player* hero = Player_Accessor::get_player();
	int menu_sel = 0;
	Equipment** inventory = hero->get_inventory();
	int max_sel = 11;

	while (game_state == INVENTORY_GAME || game_state == IN_GAME) {
		if (keypressed()) {
			int k = readkey();
			switch(k >> 8) {
				case KEY_ESC: break;
				case KEY_UP: menu_sel--; if (menu_sel < 0) menu_sel = 0; break;
				case KEY_DOWN: menu_sel++; if (menu_sel > max_sel) menu_sel = max_sel; break;
				case KEY_ENTER:
					switch (menu_sel) {
				case 0: show_screen(inventory[0]->item_id); break; 
				case 1: show_screen(inventory[1]->item_id); break; 
				case 2: show_screen(inventory[2]->item_id); break; 
				case 3: show_screen(inventory[3]->item_id); break; 
				case 4: show_screen(inventory[4]->item_id); break; 
				case 5: show_screen(inventory[5]->item_id); break;
				case 6: show_screen(inventory[6]->item_id); break;
				case 7: show_screen(inventory[7]->item_id); break;
				case 8: show_screen(inventory[8]->item_id); break;
				case 9: show_screen(inventory[9]->item_id); break;
				case 10: show_screen(inventory[10]->item_id); break;
				case 11: game_state=IN_GAME; goto exit_loop;
					} break;
			}
			clear_keybuf();
		}

		// drawing
		blit(inv_screen_bitmap, buffer, 0, 0, 0, 0, SCREENW, SCREENH);

		textprintf_ex(buffer, font2, 50, 20, makecol(255,051,102), -1, "Inventory");
		if (menu_sel != 0) { 
			textprintf_ex(buffer, font, 50,  140, makecol(0,255,255), -1, "(%d) %s", inventory[0]->number_held, inventory[0]->name);
		} else {
			textprintf_ex(buffer, font, 50,  140, makecol(255,255,255), -1, "%s", inventory[0]->description);
		}
		if (menu_sel != 1) {
			textprintf_ex(buffer, font, 50,  160, makecol(0,255,255), -1, "(%d) %s", inventory[1]->number_held, inventory[1]->name);
		} else {
			textprintf_ex(buffer, font, 50,  160, makecol(255,255,255), -1, "%s", inventory[1]->description);
		}
		if (menu_sel != 2) { 
			textprintf_ex(buffer, font, 50,  180, makecol(0,255,255), -1, "(%d) %s", inventory[2]->number_held, inventory[2]->name);
		} else {
			textprintf_ex(buffer, font, 50,  180, makecol(255,255,255), -1, "%s", inventory[2]->description);
		} 
		if (menu_sel != 3) { 
			textprintf_ex(buffer, font, 50,  200, makecol(0,255,255), -1, "(%d) %s", inventory[3]->number_held, inventory[3]->name);
		} else {
			textprintf_ex(buffer, font, 50,  200, makecol(255,255,255), -1, "%s", inventory[3]->description);
		} 
		if (menu_sel != 4) { 
			textprintf_ex(buffer, font, 50,  220, makecol(0,255,255), -1, "(%d) %s", inventory[4]->number_held, inventory[4]->name);
		} else {
			textprintf_ex(buffer, font, 50,  220, makecol(255,255,255), -1, "%s", inventory[4]->description);
		} 
		if (menu_sel != 5) { 
			textprintf_ex(buffer, font, 50,  240, makecol(0,255,255), -1, "(%d) %s", inventory[5]->number_held, inventory[5]->name);
		} else {
			textprintf_ex(buffer, font, 50,  240, makecol(255,255,255), -1, "%s", inventory[5]->description);
		} 
		if (menu_sel != 6) { 
			textprintf_ex(buffer, font, 50,  260, makecol(0,255,255), -1, "(%d) %s", inventory[6]->number_held, inventory[6]->name);
		} else {
			textprintf_ex(buffer, font, 50,  260, makecol(255,255,255), -1, "%s",inventory[6]->description);
		} 
		if (menu_sel != 7) { 
			textprintf_ex(buffer, font, 50,  280, makecol(0,255,255), -1, "(%d) %s", inventory[7]->number_held, inventory[7]->name);
		} else {
			textprintf_ex(buffer, font, 50,  280, makecol(255,255,255), -1, "%s", inventory[7]->description);
		} 
		if (menu_sel != 8) { 
			textprintf_ex(buffer, font, 50,  300, makecol(0,255,255), -1, "(%d) %s", inventory[8]->number_held, inventory[8]->name);
		} else {
			textprintf_ex(buffer, font, 50,  300, makecol(255,255,255), -1, "%s", inventory[8]->description);
		} 
		if (menu_sel != 9) { 
			textprintf_ex(buffer, font, 50,  320, makecol(0,255,255), -1, "(%d) %s", inventory[9]->number_held, inventory[9]->name);
		} else {
			textprintf_ex(buffer, font, 50,  320, makecol(255,255,255), -1, "%s", inventory[9]->description);
		} 
		if (menu_sel != 10) { 
			textprintf_ex(buffer, font, 50,  340, makecol(0,255,255), -1, "(%d) %s", inventory[10]->number_held, inventory[10]->name);
		} else {
			textprintf_ex(buffer, font, 50,  340, makecol(255,255,255), -1, "%s", inventory[10]->description);
		} 
		if (menu_sel == 11) {
			textprintf_ex(buffer, font1, 50,  400, makecol(255,0,51), -1, "Return");
		} else {
			textprintf_ex(buffer, font1, 50,  400, makecol(255,255,0), -1, "RETURN");
		} 

		// draw to screen
		blit(buffer, screen, 0,0, 0,0, SCREENW, SCREENH);
	}
	exit_loop: ;

	destroy_bitmap(inv_screen_bitmap);
	return;
}

void show_screen(int inv_id) {
	return; // do something with clicked item
}
