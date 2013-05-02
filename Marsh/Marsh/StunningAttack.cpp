#include "Main.h"

using namespace std;

StunningAttack::StunningAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats)
:Attack(x, y, vel, vel_d, img, stats)
{
	this->caught_target = NULL;
	this->caught_timer = 0;
	this->caught_duration = 100;

}
StunningAttack::~StunningAttack(void){

}

Attack* StunningAttack::clone(int x, int y, Direction dir){
	Sprite* image = this->get_image()->clone(dir);
	AttackStatistics stats;
	stats.base_damage = this->base_damage;
	stats.charge_time = this->charge_time;
	stats.exp_date = this->expiration_date;
	stats.penetration = this->penetration;
	stats.range = this->range;
	stats.tree_depth = this->tree_depth_level;

	StunningAttack* dup = new StunningAttack(x, y, this->velocity, this->velocity_delay, image, stats);
	dup->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
	dup->set_my_caster(this->my_caster);
	return dup;
}

void StunningAttack::update(void){
	if (!this->alive){
		if (++this->death_timer >= this->expiration_date)
			this->get_world()->remove_entity(this);
		return;
	}

	if (this->caught_target == NULL)
		Attack::update();
	if (this->caught_target != NULL){
		if (++this->caught_timer >= this->caught_duration){
			Attack::start_death_sequence();
			this->caught_target->entangled = false;
		}
		this->caught_target->deal_with_attack(this);
		if (this->caught_target->health <= 0){
			this->caught_target = NULL;
			Attack::start_death_sequence();
		}
	}
}

bool StunningAttack::detect_collisions(void){
	std::list<iDrawable*>* entities = this->get_world()->get_active_entities();
	int my_x = this->get_reference_x();
	int my_y = this->get_reference_y();
	int my_width = this->get_bounding_width();
	int my_height = this->get_bounding_height();

	int check_x, check_y, check_width, check_height;

	std::list<iDrawable*>::iterator iter;
	std::list<iDrawable*>::iterator end = entities->end();
	for (iter = entities->begin(); iter != end; iter++){
		iDrawable* check = (*iter);
		if (check == this)
			continue;
		if (check == this->my_caster)
			continue;
		if (check->my_type == Wallop){
			Attack* check_attack = check->fetch_me_as_attack();
			if (check_attack->my_caster == this->my_caster)
				continue;
		}

		check_x = check->get_reference_x();
		check_y = check->get_reference_y();
		check_width = check->get_bounding_width();
		check_height = check->get_bounding_height();


		if (detect_hit(my_x, my_y, my_height, my_width, 
			check_x, check_y, check_width, check_height)){
				if ((*iter)->fetch_me_as_combat() == NULL){
					(*iter)->deal_with_attack(this); // it should kill YOU
				} else {
					this->caught_target = (*iter)->fetch_me_as_combat();
					this->entrap_my_target();
				}
				return true;
		}
	}
	return false;
}

void StunningAttack::entrap_my_target(void){
	Combat* target = this->caught_target;
	if (target == NULL)
		return;
	target->entangled = true;
	int center_x = this->get_reference_x();
	int center_y = this->get_reference_y();
	center_x += this->get_bounding_width() / 2;
	center_y += this->get_bounding_height() / 2;

	center_x -= target->get_bounding_width() / 2;
	center_y -= target->get_bounding_height() / 2;
	center_x -= (target->get_reference_x() - target->get_x_pos());
	center_y -= (target->get_reference_y() - target->get_y_pos());

	target->set_x_pos(center_x);
	target->set_y_pos(center_y);
}

void StunningAttack::start_death_sequence(void){
	if (this->caught_target == NULL){
		this->alive = false;
		this->get_image()->casting_update();
		this->get_image()->update();
	} else {
		// the target is just taking damage, it'll QQ moar
	}

}