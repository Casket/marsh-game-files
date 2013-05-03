#include "Main.h"

using namespace std;

PersistentAttack::PersistentAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats)
:Attack(x, y, vel, vel_d, img, stats)
{

}
PersistentAttack::~PersistentAttack(void){

}

void PersistentAttack::start_death_sequence(void){
	// this one does not need a death sequence... it only dies upon reaching
	//  the maximum range.. like a BAWS
}

bool PersistentAttack::detect_collisions(void){
	Attack::detect_collisions();
	return false;
}

Attack* PersistentAttack::clone(int x, int y, Direction dir){
	Sprite* image = this->get_image()->clone(dir);
	AttackStatistics stats;
	stats.base_damage = this->base_damage;
	stats.charge_time = this->charge_time;
	stats.exp_date = this->expiration_date;
	stats.penetration = this->penetration;
	stats.range = this->range;
	stats.tree_depth = this->tree_depth_level;
	PersistentAttack* dup = new PersistentAttack(x, y, this->velocity, this->velocity_delay, image, stats);
	dup->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
	dup->set_my_caster(this->my_caster);
	return dup;
}