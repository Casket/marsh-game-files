#include "Main.h"


using namespace std;

Reject::Reject(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* ways)
:Mob(x,y,vel,vel_d,img,ways){

	this->my_type = Monster;

}
Reject::~Reject(void){

}
void Reject::set_ranges(void){
	this->in_melee_range = check_variable_range(NO_RANGE);
	this->in_short_range = check_variable_range(SHORT_RANGE);
	this->in_mid_range = check_variable_range(MID_RANGE);
	this->in_long_range = check_variable_range(LONG_RANGE);
}
int Reject::choose_attack(){
	if(this->in_mid_range){
		int random = rand() % 2;
		if(random == 0){
			if(this->cooldowns[1].first == 0){
				return 1;
			}if(this->cooldowns[2].first == 0){
				return 2;
			}
		}
		if(random == 1){
			if(this->cooldowns[2].first == 0){
				return 2;
			}else if(this->cooldowns[1].first == 0){
				return 1;
			}
		}
	}
	if(this->in_melee_range && this->cooldowns[1].first == 0){
		return 0;
	}
	return -1;	
}