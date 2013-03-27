#include "Main.h"


Attack::Attack(int x, int y, int vel, int vel_d, Sprite* img)
:iDrawable(x, y, vel, vel_d, img) 
{

}


Attack::~Attack(void)
{

}

void Attack::update(void){

}

void Attack::deal_with_attack(Attack* attack){

}