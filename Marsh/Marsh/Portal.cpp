#include "Portal.h"

Portal::Portal(int x, int y, Sprite *img, WorldName togo, int targ_x, int targ_y)
:iDrawable(x, y, 0, 0, img)
{
	this->portal_to = togo;	
	this->my_type = StarGate;
	this->activated = false;
	this->target_x_pos = targ_x;
	this->target_y_pos = targ_y;	
}

Portal::~Portal(void){

}

void Portal::update(void){
	this->can_walk_down = true;
	this->can_walk_left = true;
	this->can_walk_right = true;
	this->can_walk_up = true;

	Player* hero = Player_Accessor::get_player();

	int my_x = this->get_reference_x();
	int my_y = this->get_reference_y();
	int my_w = this->get_bounding_width()+hero->velocity;
	int my_h = this->get_bounding_height()+hero->velocity;

	

	iDrawable::check_walkable(my_x, my_y, my_h, my_w, 
		hero->get_reference_x(), hero->get_reference_y(), 
		hero->get_bounding_width(), hero->get_bounding_height(), 0, 0);

	if (!this->can_walk_down || !this->can_walk_left ||
		!this->can_walk_right || !this->can_walk_up){
			this->activated = true;
	}
}

Portal* Portal::fetch_me_as_portal(void){
	return this;
}

void Portal::deal_with_attack(Attack *attack){
	attack->start_death_sequence();
}