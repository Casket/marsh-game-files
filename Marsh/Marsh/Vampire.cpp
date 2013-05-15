#include "Main.h"


using namespace std;

Vampire::Vampire(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* ways)
:Mob(x,y,vel,vel_d,img,ways){
	this->my_type = Vamp;
	this->populate_spells();
	this->velocity = 1;
	this->velocity_delay = 2;
	this->populate_spells();
}

Vampire::~Vampire(void){

}

void Vampire::populate_spells(void){
	for (int i=0; i < MAX_ATTACKS; i++){
		if (this->attack_loadout[i] != NULL)
			delete this->attack_loadout[i];
		this->attack_loadout[i] = NULL;
	}

	this->attack_loadout[0] = attackDB->fetch_attack(MONSTER_MELEE)->clone(0, 0, W);
	this->attack_loadout[0]->set_my_caster(this);

	this->attack_loadout[1] = attackDB->fetch_attack(DRAIN_HEALTH)->clone(0, 0, W);
	this->attack_loadout[1]->set_my_caster(this);

	this->attack_loadout[2] = attackDB->fetch_attack(SHADOW_DASH)->clone(0, 0, W);
	this->attack_loadout[2]->set_my_caster(this);
	
	this->cooldowns[0].first = 0;
	this->cooldowns[0].second = MELEE_CD;

	this->cooldowns[1].first = 0;
	this->cooldowns[1].second = 2*MID_RANGE_CD;

	this->cooldowns[2].first = 0;
	this->cooldowns[2].second = UTIL_CD;
}

void Vampire::set_ranges(void){
	this->in_melee_range = check_variable_range(NO_RANGE);
	this->in_short_range = check_variable_range(SHORT_RANGE);
	this->in_mid_range = false;
	this->in_long_range = false;
}


int Vampire::choose_attack(){
	if(this->cooldowns[2].first == 0){
		return 2;
	}

	if(this->in_melee_range && this->cooldowns[0].first == 0){
		if(this->cooldowns[1].first == 0 && this->get_current_health() < (this->get_max_health()/2)){
			return 1;
		}
		return 0;
	}
	if(this->in_short_range && this->cooldowns[1].first == 0){
		return 1;
	}
	return -1;	
}