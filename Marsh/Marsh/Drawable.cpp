#include "Main.h"

Drawable::Drawable(int x, int y, int vel, int vel_d, Sprite *img)
:iDrawable(x, y, vel, vel_d, img)
{

}

Drawable::~Drawable(void) {

}

void Drawable::update(void){
	//Not needed as these objects don't move in this level
	return;
}

void Drawable::deal_with_attack(Attack* attack){
	//cause attack to start its dieing proces
	attack->start_death_sequence();
	// you done gone and hit something hard

}

void Drawable::check_collisions(void){
	return;  // inanimate objects don't need this
}



