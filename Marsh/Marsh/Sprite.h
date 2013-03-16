#ifndef SPRITE_H
#define SPRITE_H 1

#include "Main.h"

using namespace std;

// States for the sprites, will add as needed and whatnots
enum SpriteState{ Normal, Casting, Explosion, Grazing, Winning};
enum Direction {N, S, E, W, NE, NW, SW, SE};

class Sprite{
	BITMAP* sprite_sheet;
	Direction facing;
	int animation_frame, animation_delay, animation_counter;
	int sheet_cols, sheet_rows, total_frames;
	bool lighted;
	SpriteState current_state;

	public:
		void load_sprite_sheet(char* name);
		BITMAP* get_current_frame(void);
		void update(void);

	private:
		// shhh... it's for secret cereal




};



#endif