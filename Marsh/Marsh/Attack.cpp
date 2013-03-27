#include "Main.h"


Attack::Attack(int x, int y, int vel, int vel_d, Sprite* img, 
			   int base_damage, int penetration, int range, int tree_depth, int exp_date, int charge_time)
			   :iDrawable(x, y, vel, vel_d, img) 
{
	this->base_damage = base_damage;
	this->penetration = penetration;
	this->range = range;
	this->tree_depth_level = tree_depth;
	this->expiration_date = exp_date;
	this->charge_time = charge_time;
}

Attack::~Attack(void)
{
	
}

void Attack::update(void){
	
}

void Attack::deal_with_attack(Attack* attack){
	
}

Attack* Attack::clone(int x, int y, int intelligence, int focus){
	int damage, penetrate, charge;
	Sprite* image = this->get_image()->clone();
	Attack* result = new Attack(x, y, this->velocity, this->velocity_delay, image, damage, penetrate, this->range, this->tree_depth_level, this->expiration_date, charge);
	return result;
}