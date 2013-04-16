#ifndef VIEW_H
#define VIEW_H 1
#define MAX_NUMBER_WORLDS 10
#define PAD 50
#define BAR_PAD 5
// TODO GABE FIND THIS SHIT, thanks

#define TEST_WORLD_SIZE 75

#define MANA_BAR_X_POS 828
#define MANA_BAR_Y_POS 50
#define HEALTH_BAR_X_POS 1093
#define HEALTH_BAR_Y_POS 50
#define RESOURCE_BAR_WIDTH 258
#define RESOURCE_BAR_HEIGHT 20
#define CAST_BAR_X_POS 590
#define CAST_BAR_Y_POS 45
#define CAST_BAR_WIDTH 220
#define CAST_BAR_HEIGHT 20
#define CONSOLE_X_POS 43
#define CONSOLE_Y_POS 42
#define CONSOLE_WIDTH 312
#define CONSOLE_HEIGHT 155


#define HEALTH_COLOR makecol(255, 0, 0)
#define MANA_COLOR makecol(128, 128, 255)

#include "Main.h"

using namespace std;

class View{
	public:
		BITMAP* world_buffer;
		BITMAP* ui_buffer;
		BITMAP* resource_bars[2];
		BITMAP* spell_slot;
		BITMAP* clear_console;
		BITMAP* in_use_console;
		Player* playa;
		World* current_world;
		World* loaded_worlds[MAX_NUMBER_WORLDS];
		BITMAP* ui_image;


		View(Player* hero);
		~View(void);
		void load_world(char* world_file);
		void update(void);
		void destroy_worlds(void);
		void draw_active_world(void);
		void save_game(void);
		void put_world_in_loaded(World* world);
		void draw_interface(Player*);
		void draw_sprites(BITMAP*, Tile***, int, int);
		void draw_drawables(BITMAP* buffer, std::list<iDrawable*> *sprites);
		void draw_to_screen(void);
		void print_to_console(std::string str);
};

#endif