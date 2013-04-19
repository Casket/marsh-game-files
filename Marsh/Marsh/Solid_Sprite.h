#ifndef Solid_Sprite_H
#define Solid_Sprite_H 1

#include "Ground_Sprite.h"

// States for the sprites, will add as needed and whatnots
class Solid_Sprite: public Sprite{
	int img_width, img_height;

	public:
		Solid_Sprite(char* file_name);
		Solid_Sprite(char* file_name, int src_x, int src_y, int width, int height);
		~Solid_Sprite(void);
		void load_sprite_sheet(char* name);
		void set_lighted(bool light);

		virtual BITMAP* get_current_frame(void);
		virtual void update(void);
		virtual void casting_update(void);
		virtual void check_casting(void);
		virtual Sprite* clone(void);

	private:
		// shhh... it's for secret cereal




};



#endif