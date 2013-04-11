#include "Portal.h"

Portal::Portal(int x, int y, Sprite *img, WorldName togo)
:iDrawable(x, y, 0, 0, img)
{
	//this->portal_to = togo;	
}

Portal::~Portal(void){

}

void Portal::update(void){
	int my_x = this->get_reference_x();
	int my_y = this->get_reference_y();

}

void Portal::deal_with_attack(Attack *attack){

}