#include "Main.h"

StatueBoss::StatueBoss(int x, int y, Sprite* img)
:Mob(x, y, 0, 0, img, NULL)
{
	
}

StatueBoss::~StatueBoss(void){

}

void StatueBoss::populate_spells(void){
	this->attack_loadout[0] = attackDB->fetch_attack(TARGET_SEEKER)->clone(0, 0, W);
	this->attack_loadout[0]->set_my_caster(this);
	this->attack_loadout[0]->target = Player_Accessor::get_player();

	this->cooldowns[0].second = UTIL_CD;
}

int StatueBoss::choose_attack(void){
	if (this->cooldowns[0].first == 0)
		return 0;
	return -1;
}