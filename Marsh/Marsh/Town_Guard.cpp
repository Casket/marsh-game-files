#include "Main.h"

using namespace std;


Town_Guard::Town_Guard(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* waypoints)
:Mob(x, y,vel, vel_d, img, waypoints){
	this->my_type = Guard;
	this->velocity = 1;
	this->velocity_delay = 2;
	this->cooldowns[0].second = MELEE_CD;
	this->attack_loadout[0] = attackDB->fetch_attack(GUARD_MELEE)->clone(0, 0, W);
	this->attack_loadout[0]->set_my_caster(this);
}

Town_Guard::~Town_Guard(void){


}
