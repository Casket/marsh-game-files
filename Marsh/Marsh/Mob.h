#ifndef MOB_H
#define MOB_H 1

#define TG_DELTA 1
#define ERR 5
#define DETECTION_RANGE 300
#define FRAME_CONST 20
#define ATTACK_RANGE 16

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;

enum State {patrol,attack,attack_move,detour_intial,detour_one,detour_two};

class Mob:public Combat{

	std::vector<std::pair<int, int>>* waypoints;
	
	bool x_or_y, patrolling_forward;
	iDrawable* target;
	int patrol_node;
	iDrawable* blocking_entity;
	std::pair<int, int> target_area;
	std::pair<std::pair<int, int>, std::pair<int, int>> detour_pair;
	State current_state, prev_state;

	public:
		Mob(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* waypoints);
		~Mob(void);
		virtual void update(void);
		virtual bool move(Direction new_dir);
		virtual void check_collisions(void);
		virtual std::pair<std::pair<int, int>, std::pair<int, int>> detour_obstruction(void);
		virtual bool get_current_facing_flag(Direction);
		virtual Direction switch_dir(Direction old_dir);
		virtual bool detect_enemies(iDrawable* to_check);
		virtual bool get_visible_to_guard(iDrawable*);
		virtual void other_check_collisions(void);
		virtual bool move_towards(std::pair<int, int>);
		virtual void increment_patrol(void);
		virtual Direction get_direction_moving(void);
		virtual bool target_in_range(void);
};

#endif