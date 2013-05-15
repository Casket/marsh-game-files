#include "VampireBoss.h"

VampireBoss::VampireBoss(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* ways)
:Vampire(x, y, vel, vel_d, img, ways)
{
	this->first_clone = NULL;
	this->second_clone = NULL;
	this->my_type = Vamp;
}

VampireBoss::~VampireBoss(void){

}

void VampireBoss::populate_spells(void){
	Vampire::populate_spells();
	
	this->cooldowns[3].first = 0;
	this->cooldowns[3].second = 5*UTIL_CD; //util_cd is ~ 6 seconds
}

int VampireBoss::choose_attack(){
	// start by checking for clonage
	if (this->first_clone == NULL && this->second_clone == NULL && this->cooldowns[3].first == 0){
		this->spawn_clones();
		return -1;
	}

	return Vampire::choose_attack();
	
}

void VampireBoss::spawn_clones(){
	if (this->first_clone != NULL || this->second_clone != NULL)
		return;

	int first_x = this->get_reference_x();
	int first_y = this->get_reference_y();

	int sec_x = first_x;
	int sec_y = first_y;

	switch (this->get_image()->get_facing()){
	case N:
		// let the south case spawn left/right
	case S:
		first_x -= this->get_bounding_width() + 50;
		sec_x += this->get_bounding_width() + 50;
		break;
	case E:
		// again let it fall to west and go up/down
	case W:
		first_y -= this->get_bounding_height() + 50;
		sec_y += this->get_bounding_height() + 50;
		break;
	}

	AttackStatistics fake_stats;

	TeleportAttack* bum_checker = new TeleportAttack(0, 0, 0, 0, NULL, fake_stats, 0);
	bum_checker->set_my_caster(this);
	bum_checker->set_world(this->get_world());

	bool first_spawn = bum_checker->check_new_coord(first_x, first_y);
	if (first_spawn){
		this->first_clone = new VampireClone(first_x, first_y, this->velocity, this->velocity_delay, this->get_image()->clone(this->get_image()->get_facing()), NULL, this);
		this->get_world()->insert_entity(this->first_clone);
		this->first_clone->set_world(this->get_world());
	}
	bool second_spawn = bum_checker->check_new_coord(sec_x, sec_y);
	if (second_spawn){
		this->second_clone = new VampireClone(sec_x, sec_y, this->velocity, this->velocity_delay, this->get_image()->clone(this->get_image()->get_facing()), NULL, this);
		this->get_world()->insert_entity(this->second_clone);
		this->second_clone->set_world(this->get_world());
	}
}

void VampireBoss::alert_clone_death(VampireClone* clone){
	if (clone == this->first_clone)
		this->first_clone = NULL;
	if (clone == this->second_clone)
		this->second_clone = NULL;

}