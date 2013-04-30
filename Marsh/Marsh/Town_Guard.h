#ifndef GUARD_H
#define GUARD_H 1

#define TG_DELTA 1
#define ERR 5
#define DETECTION_RANGE 400
#define FRAME_CONST 20

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;

enum guard_states {steady, pathing, attacking};

class Town_Guard:public Combat{

	std::vector<std::pair<int, int>>* waypoints;
	
	bool on_patrol, in_combat, x_or_y;
	bool patrolling_forward, detour;
	std::vector<std::pair<int,Direction>>* path, ret_path;
	Direction cur_dir;
	iDrawable* target;
	int flip_frames;
	int patrol_node;
	bool movement_blocked;
	iDrawable* blocking_entity;
	std::pair<int, int> target_area;
	std::pair<std::pair<int, int>, std::pair<int, int>> detour_pair;
	std::list<iDrawable*> TargsToChoose; 

	public:
		Town_Guard(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* waypoints);
		~Town_Guard(void);
		virtual void update(void);
		bool move(Direction new_dir);
	
	protected:
		virtual void check_collisions(void);
		std::pair<std::pair<int, int>, std::pair<int, int>> detour_obstruction(void);
		bool get_current_facing_flag(void);


	private:
		Direction switch_dir(Direction old_dir);
		bool detect_enemies(iDrawable* to_check);
		bool get_visible_to_guard(iDrawable*);
		void other_check_collisions(void);
		void find_path(void);
		bool move_towards(std::pair<int, int>);
		void increment_patrol(void);
};

#endif