#include "Main.h"


using namespace std;

Dragon::Dragon(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* ways)
:Mob(x,y,vel,vel_d,img,ways){

	this->my_type = Monster;

}
Dragon::~Dragon(void){

}
void Dragon::set_ranges(void){
	this->in_melee_range = check_variable_range(NO_RANGE);
	this->in_short_range = check_variable_range(SHORT_RANGE);
	this->in_mid_range = false;
	this->in_long_range = false;
}

int Dragon::choose_attack(){
	if(this->in_melee_range && this->cooldowns[0].first == 0){
		return 0;
	}
	if(this->in_short_range && this->cooldowns[1].first == 0){
		return 1;
	}
	return -1;	
}
