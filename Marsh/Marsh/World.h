#ifndef WORLD_H
#define WORLD_H 1
#define TILE_SIZE 32

#include "Main.h"

using namespace std;
class Ground_Sprite;

typedef struct Tile{
	int row;
	int col;
	Ground_Sprite* background_image;
	std::list<iDrawable*> *contents;
	bool can_walk;
}Tile;

enum WorldName{main_world,town,marsh_n,marsh_s,test_map};

class World{
public:
	int tiles_wide, tiles_high;
	Tile*** tile_map;
	int current_mission;
	std::list<iDrawable*> *active_entities;
	std::list<iDrawable*> *visible_entities;
	std::list<iDrawable*> *removal_queue;
	Player* playa;
	WorldName my_name;

	World(WorldName this_world);
	~World();
	void load_world();
	void load_mission(char* mission_file);
	Tile*** get_tile_map(void);
	int get_tiles_wide(void);
	int get_tiles_high(void);
	bool equals(World*);
	std::list<iDrawable*>* get_visible_entities(void);
	std::list<iDrawable*>* get_active_entities(void);
	void insert_entity(iDrawable*);
	void remove_entity(iDrawable*);
	void set_player(Player*);
	void remove_destroyed(void);
	

private:
	void convert_to_tile(char a, char b, int pos_x, int pos_y);
	int find_x(char b);
	int list_to_int(char* given, int size);
	void make_drawable(char* type, char* x, char* y, int size_x, int size_y,int type_size);
	void make_world();
	void drawble_info_finder(char* type);
	char* get_file();
	void designate_drawable(char* back_ground_tiles);
	static WorldName get_WorldName(char*,int);
	void make_portal(char*);
	
	// room for expansion packs! (and easter eggs, and stuffz)


};

#endif