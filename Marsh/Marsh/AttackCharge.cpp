#include "Main.h"

using namespace std;



AttackCharge::AttackCharge(int x, int y, Sprite* img, int longevity)
:iDrawable(x, y, 0, 0, img)
{
	this->duration = longevity;
	this->living_timer  = 0;
}

AttackCharge::~AttackCharge(void){

}

void AttackCharge::update(void){
	this->get_image()->update();
	if (++this->living_timer >= this->duration)
		this->get_world()->remove_entity(this);
}

void AttackCharge::deal_with_attack(Attack* attack){
	// leave it be, i'm just a stupid animation that looks cool
}