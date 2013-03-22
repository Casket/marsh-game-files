#ifndef WORLD_H
#define WORLD_H 1
#define TILE_SIZE 32

#include "Main.h"
class Ground_Sprite;

using namespace std;

typedef struct Tile{
	int row;
	int col;
	Ground_Sprite* background_image;
	std::list<Drawable*> contents;
	bool can_walk;
}Tile;

enum WorldName{ main_world };
// TODO GABE TYPE THIS SHIT, thank you

class World{
public:
	int tiles_wide, tiles_high;
	Tile*** tile_map;
	int current_mission;


	World(int tiles_w, int tiles_h);
	~World();
	void load_world(char* load_file);
	void load_mission(char* mission_file);
	Tile*** get_tile_map(void);
	int get_tiles_wide(void);
	int get_tiles_high(void);
	bool equals(World*);

private:
	void convert_to_tile(char a, char b, int pos_x, int pos_y);
	int find_x(char b);
	int list_to_int(char* given, int size);
	Drawable* make_drawable(char* type, char* x, char* y, int size_x, int size_y);
	// room for expansion packs! (and easter eggs, and stuffz)


};

#endif