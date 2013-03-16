#ifndef WORLD_H
#define WORLD_H 1
#define TILE_SIZE 32

#include "Main.h"
#include "Sprite.h"

using namespace std;

typedef struct Tile{
	int row;
	int col;
	Sprite* background_image;
	std::list<int*> contents;
	bool can_walk;
}Tile;

class World{
	int tiles_wide, tiles_high;
	Tile*** tile_map;

	public:
		World(int tiles_w, int tiles_h);
		~World();
		void load_world(char* load_file);
		void load_mission(char* mission_file);
		Tile*** get_tile_map(void);
		int get_tiles_wide(void);
		int get_tiles_high(void);

	private:
		// room for expansion packs! (and easter eggs, and stuffz)
		

};

#endif