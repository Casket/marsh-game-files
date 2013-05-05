#ifndef GUARD_H
#define GUARD_H 1

#define TG_DELTA 1
#define ERR 5
#define DETECTION_RANGE 400
#define FRAME_CONST 20
#define ATTACK_RANGE 16

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;

enum Gstate {patrol,attack,attack_move,detour_intial,detour_one,detour_two};

class Town_Guard:public Combat{

	std::vector<std::pair<int, int>>* waypoints;
	
	bool x_or_y, patrolling_forward;
	iDrawable* target;
	int patrol_node;
	iDrawable* blocking_entity;
	std::pair<int, int> target_area;
	std::pair<std::pair<int, int>, std::pair<int, int>> detour_pair;
	Gstate current_state, prev_state;

	public:
		Town_Guard(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* waypoints);
		~Town_Guard(void);
		virtual void update(void);
		bool move(Direction new_dir);
	
	protected:
		virtual void check_collisions(void);
		std::pair<std::pair<int, int>, std::pair<int, int>> detour_obstruction(void);
		bool get_current_facing_flag(Direction);


	private:
		Direction switch_dir(Direction old_dir);
		bool detect_enemies(iDrawable* to_check);
		bool get_visible_to_guard(iDrawable*);
		void other_check_collisions(void);
		void find_path(void);
		bool move_towards(std::pair<int, int>);
		void increment_patrol(void);
		Direction get_direction_moving(void);
		bool target_in_range(void);
};

#endif