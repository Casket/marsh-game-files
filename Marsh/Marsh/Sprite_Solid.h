#ifndef SPRITE_SOLID_H
#define SPRITE_SOLID_H 1

#include "Ground_Sprite.h"

// States for the sprites, will add as needed and whatnots
class Sprite_Solid{

	public:
		BITMAP* sprite_sheet;
		int focus_x, focus_y, start_x, start_y;
		bool lighted;
		bool over_player, under_player;
		int src_x, src_y, dst_x, dst_y;
		BITMAP* buff_scrl;


		Sprite_Solid(char* file_name, int focus_x, int focus_y, int start_x, int start_y);
		~Sprite_Solid(void);
		void load_sprite_sheet(char* name);
		void set_lighted(bool light);

		BITMAP* get_current_frame(void);
		void update(BITMAP* buffer, BITMAP* scroll, int p_x_pos, int p_y_pos);
		void casting_update(void);
		void check_casting(void);

	private:
		// shhh... it's for secret cereal




};



#endif