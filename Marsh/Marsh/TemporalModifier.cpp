#include "Main.h"

TemporalModifier::TemporalModifier(Sprite* img, AttackStatistics stats, int update_rate)
:Attack(0, 0, 0, 0, img, stats)
{
	this->delay = update_rate;
}

TemporalModifier::~TemporalModifier(void){

}

void TemporalModifier::update(void){
	if (this->death_timer <= 0){
		this->death_timer++;
		world_time_delay = this->delay;
	}

	if (++this->death_timer >= this->expiration_date){
		world_time_delay = DEFAULT_WORLD_TIME_DELAY;
		this->get_world()->remove_entity(this);
	}
}

Attack* TemporalModifier::clone(int x, int  y, Direction dir){
	Sprite* image = this->get_image()->clone(E);
	image->set_facing(dir);
	AttackStatistics stats;
	stats.base_damage = this->base_damage;
	stats.charge_time = this->charge_time;
	stats.exp_date = this->expiration_date;
	stats.penetration = this->penetration;
	stats.range = this->range;
	stats.tree_depth = this->tree_depth_level;

	TemporalModifier* dup = new TemporalModifier(image, stats, this->delay);
	dup->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
	dup->set_my_caster(this->my_caster);
	dup->set_position_adjustment(this->x_adjustment, this->y_adjustment);	
	return dup;
}