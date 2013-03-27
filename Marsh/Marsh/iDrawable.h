#ifndef IDRAWABLE_H 
#define IDRAWABLE_H 1

#include "Main.h"
class World;

using namespace std;

class iDrawable{
protected:
	Sprite* image;
	int x_pos, y_pos;
	int velocity, velocity_delay;
	World* my_world;
	int reference_horizontal, reference_vertical;
	int bounding_width, bounding_height;

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

	virtual void update(void) = 0;
	virtual void deal_with_attack(Attack* attack) = 0;

};

#endif 