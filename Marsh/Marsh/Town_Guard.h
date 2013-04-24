#ifndef GUARD_H
#define GUARD_H 1

#define MOVEMENT_DELTA 2
#define DETECTION_RANGE 400
#define FRAME_CONST 20

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;


class Town_Guard:public Combat{
	
	bool on_patrol, up_or_down, initial_detection, in_combat, paused, x_or_y;
	std::vector<std::pair<int,Direction>>* waypoints, path, ret_path;
	iDrawable* target;
	int patrol_node, patrol_frame_count, flip_frames, reset_node;
	bool movement_blocked;
	iDrawable* blocking_entity;
	int frame_counter_for_direction_change;

	public:
		Town_Guard(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,Direction>>* waypoints);
		~Town_Guard(void);
		virtual void update(void);
		void patrol(void);
		bool move(Direction new_dir);
	
	protected:
		virtual void check_collisions(void);
		


	private:
		Direction switch_dir(Direction old_dir);
		void detect_enemies(iDrawable* to_check);
		bool get_visible_to_guard(iDrawable*);
		void forward(std::pair<int, Direction> cur_node);
		void reverse(std::pair<int, Direction> cur_node);
		void other_check_collisions(void);
		void find_path(void);
		void move_towards(void);
		void reset(void);
};

#endif