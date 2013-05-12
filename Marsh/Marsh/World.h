#ifndef WORLD_H
#define WORLD_H 1
#define TILE_SIZE 32

#include "Main.h"

class Quest;
class OptionPresenter;

using namespace std;
class Ground_Sprite;

typedef struct Tile{
	int row;
	int col;
	Ground_Sprite* background_image;
	bool can_walk;
	bool flyable;
}Tile;

enum WorldName{main_world,main_world11,main_world12,main_world13,main_world14,main_world15,main_world16,main_world17,main_world18,main_world19,test_map};

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
	void load_world(void);
	void load_mission(std::string);
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
	static WorldName get_WorldName(std::string,int);

private:
	static std::pair<std::string, int> pull_out(std::string, int);
	static std::string to_string(std::string, int);
	static int find_x(char);
	static int list_to_int(std::string, int);
	static std::string get_file(WorldName);
	static EntityType get_entityType(std::string);
	void convert_to_tile(char, char, int, int);
	void designate_drawable(std::string, std::string, std::string, int, int,int);
	void make_world();
	void make_drawable(std::string);
	void make_portal(std::string);
	void make_AI(std::string);
	void make_op(std::string, int);
	void make_dialouge_op(std::string, int, OptionPresenter*);
	std::pair<Quest*, int> make_quest(std::string,int);
	

	
	// room for expansion packs! (and easter eggs, and stuffz)


};

#endif