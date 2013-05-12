#include "Main.h"

HealthDrainAttack::HealthDrainAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats)
:Attack(x, y, vel, vel_d, img, stats)
{

}
HealthDrainAttack::~HealthDrainAttack(void){

}

void HealthDrainAttack::attack_target(iDrawable* drawable){
	Combat* target = drawable->fetch_me_as_combat();
	if (target == NULL)
		return;
	int prev_health = target->get_current_health();
	target->deal_with_attack(this);
	int cur_health = target->get_current_health();
	
	if (cur_health < 0){
		this->my_caster->health += prev_health;	
	} else
		this->my_caster->health += (prev_health - cur_health);
	this->get_image()->casting_update();
	this->death_timer = 0;
	this->alive = false;
}

void HealthDrainAttack::start_death_sequence(void){
	// it'll kill itself
}

Attack* HealthDrainAttack::clone(int x, int y, Direction dir){

	Sprite* image = this->get_image()->clone(dir);

	AttackStatistics stats;
	stats.base_damage = this->base_damage;
	stats.charge_time = this->charge_time;
	stats.exp_date = this->expiration_date;
	stats.penetration = this->penetration;
	stats.range = this->range;
	stats.tree_depth = this->tree_depth_level;

	HealthDrainAttack* result = new HealthDrainAttack(x, y, this->velocity, this->velocity_delay, image, stats);

	switch(dir){
	case N:
		result->set_boundary_value(this->get_bounding_height(), this->get_bounding_width(), this->reference_horizontal, this->reference_vertical);
		y -= this->y_adjustment;
		break;
	case S:
		result->set_boundary_value(this->get_bounding_height(), this->get_bounding_width(), this->reference_horizontal, this->reference_vertical);
		
		break;
	case W:
		result->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
		x -= this->x_adjustment;
		y -= this->y_adjustment;
		break;
	case E:
		result->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
		y -= this->y_adjustment;
		break;
	}

	result->set_x_pos(x);
	result->set_y_pos(y);


	result->set_my_caster(this->my_caster);
	result->my_type = Wallop;
	result->set_position_adjustment(this->x_adjustment, this->y_adjustment);
	return result;
}