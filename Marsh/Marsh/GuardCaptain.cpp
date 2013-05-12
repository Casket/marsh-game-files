#include "Main.h"


using namespace std;

GuardCaptain::GuardCaptain(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* ways)
:Mob(x,y,vel,vel_d,img,ways){
	
	this->my_type = Guard;

}
GuardCaptain::~GuardCaptain(void){

}
void GuardCaptain::set_ranges(void){
	this->in_melee_range = check_variable_range(NO_RANGE);
	this->in_short_range = check_variable_range(SHORT_RANGE);
	this->in_mid_range = false;
	this->in_long_range = false;
}
int GuardCaptain::choose_attack(){
	if(this->in_melee_range && this->cooldowns[1].first == 0){
		return 1;
	}
	if(this->in_short_range && this->cooldowns[0].first == 0){
		return 0;
	}
	return -1;	
}