#include "WorldPortal.h"
#include "Portal.h"

using namespace std;

WorldPortal::WorldPortal(int x, int y, Sprite *img, WorldName togo)
:Portal(x,y,img,togo,0,0)
{

}

WorldPortal::~WorldPortal(void){

}

void WorldPortal::update(void){
	
	this->can_walk_down = true;
	this->can_walk_left = true;
	this->can_walk_right = true;
	this->can_walk_up = true;
	
	Player* hero = Player_Accessor::get_player();

	int my_x = this->get_reference_x();
	int my_y = this->get_reference_y();
	int my_w = this->get_bounding_width()+hero->velocity;
	int my_h = this->get_bounding_height()+hero->velocity;
	
	Direction facing = hero->get_image()->get_facing();

	if(facing == N){
		this->target_x_pos = hero->x_pos;
		this->target_y_pos = 124 * 32;
	}else if(facing == S){
		this->target_x_pos = hero->x_pos;
		this->target_y_pos = 16;
	}else if(facing == E){
		this->target_x_pos = 16;
		this->target_y_pos = hero->y_pos;
	}else if(facing == W){
		this->target_x_pos = 124 * 32;
		this->target_y_pos = hero->y_pos;
	}else{
		//
	}
	iDrawable::check_walkable(my_x, my_y, my_h, my_w, 
		hero->get_reference_x(), hero->get_reference_y(), 
		hero->get_bounding_width(), hero->get_bounding_height(), 0, 0);

	if (!this->can_walk_down || !this->can_walk_left ||
		!this->can_walk_right || !this->can_walk_up){
			this->activated = true;
	}
}

