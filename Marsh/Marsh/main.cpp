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

int main(void)
{
    //initialize everything
	set_up_game();

	BITMAP* blank = create_bitmap(SCREENW, SCREENH);
	clear_bitmap(blank);

	World* w = new World(30, 30);
	
	char* filename = (char*)malloc(sizeof(char) * 100);
	
	strcpy_s(filename, sizeof(char) * 100, "mapTEST.txt");

	while(!key[KEY_ESC]) {
		if (!rested) {
			rest(4);
			continue;
		}
		rested = false;

		ticks++;

		blit(blank, screen, 0,0, 0,0, SCREENW, SCREENH);

		textprintf_centre_ex(screen,font,100,20,makecol(255,255,255),-1,"FRAMERATE %d", framerate);		
		
	}
	
	delete w;
	allegro_exit();

    return 0;
}
END_OF_MAIN()


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

