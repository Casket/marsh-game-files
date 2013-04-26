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


class Town_Guard:public Combat{

	std::vector<std::pair<int, int>>* waypoints;
	
	bool on_patrol, up_or_down, initial_detection, in_combat, paused, x_or_y, follow_detour;
	bool patrolling_forward;
	std::vector<std::pair<int,Direction>>* path, ret_path;
	iDrawable* target;
	int patrol_frame_count, flip_frames, reset_node;
	int patrol_node;
	bool movement_blocked;
	iDrawable* blocking_entity;
	int frame_counter_for_direction_change;

	public:
		Town_Guard(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* waypoints);
		~Town_Guard(void);
		virtual void update(void);
		void patrol(void);
		bool move(Direction new_dir);
	
	protected:
		virtual void check_collisions(void);
		std::pair<int, int> detour_obstruction(void);
		bool get_current_facing_flag(void);


	private:
		Direction switch_dir(Direction old_dir);
		void detect_enemies(iDrawable* to_check);
		bool get_visible_to_guard(iDrawable*);
		void forward(std::pair<int, Direction> cur_node);
		void reverse(std::pair<int, Direction> cur_node);
		void other_check_collisions(void);
		void find_path(void);
		bool move_towards(std::pair<int, int>);
		void reset(void);
		void increment_patrol(void);
};

#endif