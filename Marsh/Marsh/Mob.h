#ifndef MOB_H
#define MOB_H 1

#define TG_DELTA 1
#define ERR 5
#define DETECTION_RANGE 300
#define MELEE_CD 20
#define MID_RANGE_CD 100
#define UTIL_CD 300

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"


using namespace std;

enum State {patrol,attack,attack_move,detour_intial,detour_one,detour_two, chilling_out};

class Mob:public Combat{
int chill_timer, chill_time;
public:
	Direction attack_dir;
	std::list<iDrawable*>* eskimo_bros;
	std::vector<std::pair<int, int>>* waypoints;
	std::pair<int, int> cooldowns [10];
	bool x_or_y, patrolling_forward, in_melee_range, in_short_range, in_mid_range, in_long_range;
	iDrawable* target;
	int patrol_node, wait, melee_cd, mid_range_cd, util_cd;
	iDrawable* blocking_entity;
	std::pair<int, int> target_area;
	std::pair<std::pair<int, int>, std::pair<int, int>> detour_pair;
	State current_state, prev_state;


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
	virtual std::pair<int,int> set_target_area(void);
	virtual void set_ranges(void);
	virtual int choose_attack(void);
	virtual bool check_variable_range(int);
	virtual void update_cd_timers(void);
	virtual void set_attack_plan(void);
	virtual Direction get_target_true_dir();
	virtual Mob* fetch_me_as_mob(void);

};

#endif