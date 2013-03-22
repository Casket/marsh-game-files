#ifndef VIEW_H
#define VIEW_H 1
#define MAX_NUMBER_WORLDS 10
// TODO GABE FIND THIS SHIT, thanks

#include "Main.h"

using namespace std;

class View{
	public:
		BITMAP* buffer;
		Player* playa;
		World* current_world;
		World* loaded_worlds[MAX_NUMBER_WORLDS];
		View(Player* hero);
		~View(void);
		void load_world(char* world_file);
		void update(void);
		void destroy_worlds(void);
		void draw_active_world(void);
		void save_game(void);
};

#endif