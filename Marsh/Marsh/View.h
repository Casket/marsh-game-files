#ifndef VIEW_H
#define VIEW_H 1
#define MAX_NUMBER_WORLDS 10
#define PAD 50
#define BAR_PAD 5
// TODO GABE FIND THIS SHIT, thanks

#define TEST_WORLD_SIZE 75

#define HEALTH_BAR_X_POS 828
#define HEALTH_BAR_Y_POS 50
#define MANA_BAR_X_POS 1093
#define MANA_BAR_Y_POS 50
#define EXP_BAR_X_POS 215
#define EXP_BAR_Y_POS 220
#define EXP_BAR_WIDTH 970
#define EXP_BAR_HEIGHT 5
#define RESOURCE_BAR_WIDTH 258
#define RESOURCE_BAR_HEIGHT 20
#define CAST_BAR_X_POS 575
#define CAST_BAR_Y_POS 35
#define CAST_BAR_WIDTH 230
#define CAST_BAR_HEIGHT 42
#define CONSOLE_X_POS 43
#define CONSOLE_Y_POS 42
#define CONSOLE_WIDTH 312
#define CONSOLE_HEIGHT 155
#define BACK_LAYER_X 585
#define BACK_LAYER_Y 40


#define HEALTH_COLOR makecol(165, 33, 33)
#define MANA_COLOR makecol(33, 104, 165)
#define EXP_COLOR makecol(255,255,255)

#include "Main.h"
#include <map>
class Guard;
class Portal;

using namespace std;
namespace Marsh {

class View{
	public:
		BITMAP* world_buffer;
		BITMAP* ui_buffer;
		BITMAP* resource_bars[2];
		BITMAP* behind_bars;
		BITMAP* spell_slot;
		BITMAP* clear_console;
		BITMAP* in_use_console;
		Player* playa;
		World* current_world;
		std::map<WorldName, World*>* loaded_worlds;
		BITMAP* ui_image;
		BITMAP* black_overlay;

		View(Player* hero);
		~View(void);
		void load_world(WorldName world);
		void update(void);
		void destroy_worlds(void);
		void draw_active_world(void);
		void save_game(void);
		void put_world_in_loaded(World* world);
		void draw_interface(Player*);
		void draw_sprites(BITMAP*, Tile***, int, int);
		void draw_drawables(BITMAP* buffer, std::list<iDrawable*> *sprites);
		void draw_to_screen(void);
		void switch_current_world(WorldName desired_world);

private:
	void insert_testing_entities(void);
};
}

#endif