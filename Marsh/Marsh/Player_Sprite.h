#ifndef PLAYER_SPRITE_H
#define PLAYER_SPRITE_H 1

#include "Main.h"

using namespace std;

#define NORTH_START 12
#define NORTH_END 15

#define SOUTH_START 0
#define SOUTH_END 3

#define EAST_START 8
#define EAST_END 11

#define WEST_START 4
#define WEST_END 7

#define NORTH_EAST_START 12
#define NORTH_WEST_START 4

#define SOUTH_EAST_START 8
#define SOUTH_WEST_START 0

#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32
#define FLICKER_FRAME_NUMBER 1000000


class Player_Sprite : public Sprite{
public:
	BITMAP* blank_frame;
	bool frame_intial;
	bool flicker;
	BITMAP** image_frames;

	Player_Sprite(std::string file_name, Direction cur_dir, int ani_delay, int rows, int cols, int total_frames);
	~Player_Sprite(void);
	virtual BITMAP* get_current_frame(void);
	virtual void update(void);
	virtual void casting_update(void);
	virtual void check_casting(void);
	virtual Sprite* clone(Direction);
	void intial_blank_frame(void);

};

#endif 