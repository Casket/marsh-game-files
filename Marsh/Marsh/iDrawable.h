#ifndef IDRAWABLE_H 
#define IDRAWABLE_H 1

#define ANGLE_SHIFT 0.707

#include "Main.h"
class World;

using namespace std;

enum EntityType {Guard, Monster, Outcast, Hero, Rival};

class iDrawable{
public:
	Sprite* image;
	int x_pos, y_pos;
	int velocity, velocity_delay;
	World* my_world;
	int reference_horizontal, reference_vertical;
	int bounding_width, bounding_height;
	bool can_walk_left, can_walk_right, can_walk_up, can_walk_down, targeted;
	int movement_counter;
	EntityType my_type;

public:
	iDrawable(int x, int y, int vel, int vel_d, Sprite* img);
	~iDrawable(void);
	void set_boundary_value(int bounding_width, int bounding_height, 
		int reference_horizontal, int reference_vertical);
	int get_bounding_width(void);
	int get_bounding_height(void);
	void set_sprite(Sprite* spr);
	void set_world(World* world);
	Sprite* get_image(void);
	int get_x_pos(void);
	int get_y_pos(void);
	World* get_world(void);
	void set_x_pos(int x);
	void set_y_pos(int y);
	int get_reference_x(void);
	int get_reference_y(void);
	bool alive;

	virtual void update(void) = 0;
	virtual void deal_with_attack(Attack* attack) = 0;
	virtual void check_collisions(void);
	
	virtual void check_walkable(int my_x, int my_y, int my_height, int my_width, int check_x, int check_y, int check_width, int check_height, int left_right_skew, int top_bottom_skew);

};

#endif 