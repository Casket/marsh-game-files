#ifndef Attack_Sprite_H
#define Attack_Sprite_H 1

#include "Main.h"

using namespace std;

class Attack_Sprite: public Sprite{
 
public:
	BITMAP** image_frames;
	int width;
	int height;
	Attack_Sprite(std::string file_name, Direction cur_dir, int ani_delay, int rows, int cols, int total_frames, int width, int height);
	~Attack_Sprite(void);
	virtual BITMAP* get_current_frame(void);
	virtual void update(void);
	virtual void casting_update(void);
	virtual void check_casting(void);
	virtual Sprite* clone(Direction);

};

#endif