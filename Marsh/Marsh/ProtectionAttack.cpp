#include "ProtectionAttack.h"
#include "Main.h"

using namespace std;


ProtectionAttack::ProtectionAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats, WardType type)
:Attack(x, y, vel, vel_d, img, stats)
{
	this->ward = type;
	this->has_alerted_player = false;
}

ProtectionAttack::~ProtectionAttack(void){

}

void ProtectionAttack::deal_with_attack(Attack* attack){
	// time to protect!
	switch(this->ward){
	case ReflectAttack:
		attack->death_timer = 0;
		attack->distance_traveled = 0;
		attack->velocity = -1*attack->velocity;
		attack->my_caster = this->my_caster;
		break;

	case ShieldAttack:
		attack->base_damage = 0;
		this->get_world()->remove_entity(attack);
		break;
	}

	this->get_world()->remove_entity(this);
}

void ProtectionAttack::update(void){
	this->get_image()->update();

	if (!this->has_alerted_player){
		this->my_caster->apply_ward(this);
		this->has_alerted_player = true;
	}

	// now need to certain the image on the caster
	int caster_center_x = this->my_caster->get_reference_x();
	int caster_center_y = this->my_caster->get_reference_y();
	
	caster_center_x += this->my_caster->get_bounding_width()/2;
	caster_center_y += this->my_caster->get_bounding_height()/2;
	
	int x = caster_center_x - this->get_bounding_width()/2;
	int y = caster_center_y - this->get_bounding_height()/2;

	x -= (this->get_reference_x() - this->get_x_pos());
	y -= (this->get_reference_y() - this->get_y_pos());

	this->set_x_pos(x);
	this->set_y_pos(y);
}

Attack* ProtectionAttack::clone(int x, int y, Direction dir){
	Sprite* image = this->get_image()->clone(W);
	// just use West - the attack shouldn't rotate
	
	AttackStatistics stats;
	stats.base_damage = this->base_damage;
	stats.charge_time = this->charge_time;
	stats.exp_date = this->expiration_date;
	stats.penetration = this->penetration;
	stats.range = this->range;
	stats.tree_depth = this->tree_depth_level;

	ProtectionAttack* result = new ProtectionAttack(x, y, this->velocity, this->velocity_delay, image, stats, this->ward);
	result->set_my_caster(this->my_caster);
	result->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
	result->spell_id = this->spell_id;
	return result;
}
void ProtectionAttack::start_death_sequence(void){
	// I do what I WANT
}
