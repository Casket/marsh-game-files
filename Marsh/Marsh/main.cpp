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
	theme = load_wav("main_theme.wav");
	if (!theme) allegro_message("error theme wav");
	else play_sample(theme,255,128,1000,1);

	buffer = create_bitmap(SCREENW,SCREENH);
	
	while(game_state != FINISH_GAME) {
		if (game_state == INTRO_GAME) show_intro();
		if (game_state == SAVE_GAME) save_game();
		if (game_state == LOAD_GAME) load_game();
	}
	
	destroy_bitmap(buffer);
	destroy_sample(theme);
	allegro_exit();
    return 0;
}
END_OF_MAIN()

View* create_view(Player* hero){
	View* v = new View(hero);
	v->load_world("testMap.txt");
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
		if (mute==0) textprintf_ex(buffer, font, 70, 480, makecol(204,255,204), -1, "Sound On!");
		else textprintf_ex(buffer, font, 70, 480, makecol(204,255,51), -1, "SOUND OFF!");

		// draw to screen
		blit(buffer, screen, 0,0, 0,0, SCREENW, SCREENH);
	}
	exit_loop: ;

	destroy_bitmap(title_screen_bitmap);
	return;
}

void start_game(void) {
	Player_Sprite* img = new Player_Sprite("player//player_hooded_withDark.bmp", S, 5, 1, 16, 16);
	Player* hero = new Player(400, 400, 0, 0, img);
	hero->set_boundary_value(28, 14, 0, 18);
	
	
	View *our_viewer= create_view(hero);

	while(game_state == IN_GAME) {
		if (key[KEY_ESC]) {
			show_intro();
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
