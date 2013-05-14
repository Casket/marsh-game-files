#include "Main.h"
#include "StationaryAttack.h"

StationaryAttack::StationaryAttack(int x, int y, Sprite* img, AttackStatistics stats)
:Attack(x, y, 0, 0, img, stats)
{
	this->alive = false;
}

StationaryAttack::~StationaryAttack(void){

}

Attack* StationaryAttack::clone(int x, int y, Direction dir){
	Sprite* image = this->get_image()->clone(E);
	image->set_facing(dir);
	AttackStatistics stats;
	stats.base_damage = this->base_damage;
	stats.charge_time = this->charge_time;
	stats.exp_date = this->expiration_date;
	stats.penetration = this->penetration;
	stats.range = this->range;
	stats.tree_depth = this->tree_depth_level;

	StationaryAttack* dup = new StationaryAttack(x, y, image, stats);
	dup->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
	dup->set_my_caster(this->my_caster);
	dup->set_position_adjustment(this->x_adjustment, this->y_adjustment);

	switch(dir){
	case N:
		x -= this->x_adjustment;
		y -= this->y_adjustment + this->range + this->get_bounding_height();
		break;
	case S:
		x -= this->x_adjustment;
		y += this->range;
		break;
	case W:
		x -= this->x_adjustment + this->range;
		y -= this->y_adjustment;
		break;
	case E:
		x += this->range + this->get_bounding_width();
		y -= this->y_adjustment;
		break;
	}

	dup->set_x_pos(x);
	dup->set_y_pos(y);
	dup->spell_id = this->spell_id;
	return dup;
}

void StationaryAttack::update_position(void){
	// do nothing... you don't move
}

void StationaryAttack::start_death_sequence(void){
	// will die on its own time
}