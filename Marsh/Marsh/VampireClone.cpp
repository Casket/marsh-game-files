#include "VampireClone.h"

VampireClone::VampireClone(int x, int y, int vel, int vel_d, Sprite *img, std::vector<std::pair<int,int> > *ways, VampireBoss* boss)
:Vampire(x, y, vel, vel_d, img, ways)
{
	this->owner = boss;
	this->set_stats(1, 0, 0, 0, 0);
	this->attack_loadout[0]->base_damage = 0;
	this->attack_loadout[1]->base_damage = 0;
	this->attack_loadout[2]->base_damage = 0;
	this->my_type = Vamp;
}

VampireClone::~VampireClone(void){

}

void VampireClone::upon_death(void){
	this->owner->alert_clone_death(this);
	Combat::upon_death();
}