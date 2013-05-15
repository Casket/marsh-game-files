#include "TargetSeeker.h"

int move_closer(int cur, int targ, int delta);

TargetSeeker::TargetSeeker(int x, int y, int vel, int vel_d, Sprite *img, AttackStatistics stats, iDrawable *target)
:Attack(x, y, vel, vel_d, img, stats)
{
	this->target = target;
}

TargetSeeker::~TargetSeeker(void){

}

void TargetSeeker::update_position(){
	if (++this->movement_counter < this->velocity_delay)
		return;

	int targ_x = this->target->get_reference_x();
	int targ_y = this->target->get_reference_y();

	int cur_x = this->get_reference_x();
	int cur_y = this->get_reference_y();

	cur_x = move_closer(cur_x, targ_x, this->velocity);
	cur_y = move_closer(cur_y, targ_y, this->velocity);
	this->set_x_pos(cur_x - reference_horizontal);
	this->set_y_pos(cur_y - reference_vertical);
}

Attack* TargetSeeker::clone(int x, int y, Direction dir){
	Sprite* image = this->get_image()->clone(dir);
	AttackStatistics stats;
	stats.base_damage = this->base_damage;
	stats.charge_time = this->charge_time;
	stats.exp_date = this->expiration_date;
	stats.penetration = this->penetration;
	stats.range = this->range;
	stats.tree_depth = this->tree_depth_level;
	TargetSeeker* dup = new TargetSeeker(x, y, this->velocity, this->velocity_delay, image, stats, this->target);
	dup->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
	dup->set_my_caster(this->my_caster);
	dup->target = Player_Accessor::get_player();
	dup->spell_id = this->spell_id;
	return dup;
}


int move_closer(int cur, int targ, int delta){
	int dif = cur - targ;
	if (dif > 0)
		return cur - 1;
	if (dif < 0)
		return cur + 1;
	else
		return cur;
}