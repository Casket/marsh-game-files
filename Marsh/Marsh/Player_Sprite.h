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

#define NORTH_EAST_START 18
#define NORTH_WEST_START 19

#define SOUTH_EAST_START 17
#define SOUTH_WEST_START 16

#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32


class Player_Sprite : public Sprite{
public:
	Player_Sprite(char* file_name, Direction cur_dir, int ani_delay, int rows, int cols, int total_frames);
	~Player_Sprite(void);
	BITMAP* get_current_frame(void);
	virtual void update(void);
	virtual void casting_update(void);
	virtual void check_casting(void);
	virtual Sprite* clone(Direction);


};

#endif 