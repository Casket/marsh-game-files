#ifndef SPRITE_H
#define SPRITE_H 1

#define NORTH_START 12

#define SOUTH_START 0

#define EAST_START 8

#define WEST_START 4

#include <allegro.h>
#include <string>

using namespace std;

// States for the sprites, will add as needed and whatnots
enum SpriteState{ Normal, Casting, Explosion, Grazing, Winning};
enum Direction {N, S, E, W, NE, NW, SW, SE, None};

class Sprite{

protected:
	BITMAP* sprite_sheet;
	Direction facing;
	int animation_frame, animation_delay, animation_counter;
	int sheet_cols, sheet_rows, total_frames;
	bool lighted;
	SpriteState current_state;
	std::string file;
	

public:
	Sprite(std::string file_name, Direction cur_dir, int ani_delay, int rows, int cols, int total_frames);
	~Sprite(void);
	void load_sprite_sheet(std::string name);
	virtual BITMAP* get_current_frame(void) = 0;
	virtual void update(void) = 0;
	virtual void casting_update(void) = 0;
	virtual void check_casting(void) = 0;
	virtual Sprite* clone(Direction);
	Direction get_facing(void);
	virtual void set_facing(Direction);
	bool get_lighted(void);
	void set_lighted(bool);
	virtual void force_direction_update(Direction dir);
	bool wearing_mask;
	bool is_translucent;


private:
	// shhh... it's for secret cereal




};



#endif