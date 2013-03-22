#include "Main.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


using namespace std;

#define MODE GFX_AUTODETECT_WINDOWED
#define COLOR_DEPTH 32
#define SCREENW 1600
#define SCREENH 900
#define FRAME_RATE_DELAY 18

volatile int ticks, framerate;
volatile bool rested;

void timer_framerate_counter(void);
void timer_game_rester(void);
void set_up_game(void);
void update_world(World*);
void draw_world_to_buffer(World*);
void start_game(void);
void end_game(void);
World* generate_world(void); // TODO add world identifying thingymahinger
View* create_view(Player*);


int main(void)
{
    //initialize everything
	set_up_game();
	

	Sprite* img = new Sprite("edited-chars.bmp", S, 5, 1, 16, 16);
	Player* hero = new Player(400, 400, 0, 0, img);

	View* our_viewer= create_view(hero);


	while(true) {
		if (key[KEY_ESC] && (key_shifts & KB_SHIFT_FLAG))
			break;
		
		if (!rested) {
			rest(4);
			continue;
		}
		rested = false;
		ticks++;

		hero->update();
		//our_viewer->draw_active_world();

		masked_blit(hero->get_image()->get_current_frame(), screen, 0, 0,
			SCREEN_W/2, SCREEN_H/2, 32,30);

		textprintf_centre_ex(screen,font,100,20,makecol(255,255,255),-1,"FRAMERATE %d", framerate);		
		
	}
	//delete hero;
	delete our_viewer;
	allegro_exit();


    return 0;
}
END_OF_MAIN()

View* create_view(Player* hero){
	View* v = new View(hero);
	v->load_world("testMap2.txt");
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

