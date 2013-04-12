#include "Portal.h"

Portal::Portal(int x, int y, Sprite *img, WorldName togo)
:iDrawable(x, y, 0, 0, img)
{
	//this->portal_to = togo;	
}

Portal::~Portal(void){

}

void Portal::update(void){
	this->can_walk_down = true;
	this->can_walk_left = true;
	this->can_walk_right = true;
	this->can_walk_up = true;

	int my_x = this->get_reference_x();
	int my_y = this->get_reference_y();
	int my_w = this->get_bounding_width();
	int my_h = this->get_bounding_height();

	Player* hero = Player_Accessor::get_player();

	iDrawable::check_walkable(my_x, my_y, my_h, my_w, 
		hero->get_reference_x(), hero->get_reference_y(), 
		hero->get_bounding_width(), hero->get_bounding_height(), 0, 0);

	if (!this->can_walk_down || !this->can_walk_left ||
		!this->can_walk_right || !this->can_walk_up){
			// TODO implemented world swapping
	}
}

void Portal::deal_with_attack(Attack *attack){
	attack->start_death_sequence();
}