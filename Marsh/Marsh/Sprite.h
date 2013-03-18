#ifndef SPRITE_H
#define SPRITE_H 1

#define NORTH_START 12
#define NORTH_END 15

#define SOUTH_START 0
#define SOUTH_END 3

#define EAST_START 8
#define EAST_END 11

#define WEST_START 4
#define WEST_END 7

#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32

#include <allegro.h>

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

	public:
		Sprite(char* file_name, Direction cur_dir, int ani_delay, int rows, int cols, int total_frames);
		~Sprite(void);
		void load_sprite_sheet(char* name, int cols, int rows, int total);
		BITMAP* get_current_frame(void);
		void update(void);
		void casting_update(void);
		void check_casting(void);
		Direction get_facing(void);
		void set_facing(Direction);
		bool get_lighted(void);
		void set_lighted(bool);

	private:
		// shhh... it's for secret cereal




};



#endif