#ifndef GROUND_SPRITE_H
#define GROUND_SPRITE_H 1

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#include <allegro.h>

// States for the sprites, will add as needed and whatnots

class Ground_Sprite{

	public:
		BITMAP* sprite_sheet;
		int frame, total_frames, x, y;

		Ground_Sprite(char* file_name, int x, int y);
		~Ground_Sprite(void);
		void load_sprite_sheet(char* name);
		BITMAP* get_current_frame(void);
		void change_frame(int frame);

	private:
		// shhh... it's for secret cereal




};



#endif