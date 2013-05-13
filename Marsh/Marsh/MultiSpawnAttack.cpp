#include "Main.h"

using namespace std;

MultiSpawnAttack::MultiSpawnAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats, Attack* to_duplicate, int delay)
:Attack(x, y, vel, vel_d, img, stats)
{
	this->attack_spawning = to_duplicate;
	this->spawn_delay = delay;
	this->delay_counter = 0;
}

MultiSpawnAttack::~MultiSpawnAttack(void){
	delete this->attack_spawning;
}

void MultiSpawnAttack::update(void){
	this->image->update();
	this->my_caster->entangled = true;
	if (++this->death_timer >= this->expiration_date || keyrel(CANCEL_CAST_KEY)){
		// time to die
		this->my_caster->entangled = false;
		this->get_world()->remove_entity(this);
	}

	if (++this->delay_counter >= this->spawn_delay){
		this->delay_counter = 0;
		int x = this->get_x_pos();
		int y = this->get_y_pos();
		switch (this->get_image()->get_facing()){
		case W:
			break;
		case E:
			break;
		case N:
			break;
		case S:
			break;
		}
		//Attack* spawned = this->attack_spawning->clone(x, y, this->get_image()->get_facing());
		//spawned->set_world(this->get_world());
		//this->get_world()->insert_entity(spawned);
	}
}

void MultiSpawnAttack::start_death_sequence(void){
	// Something wants this thing to die... not going to happen
	// bhahahahahahahah
}

void MultiSpawnAttack::deal_with_attack(Attack* attack){
	// an attack hit this thing... Ummm... ignore it?
}

iDrawable* MultiSpawnAttack::get_above_target(void){
	return NULL;
}

Attack* MultiSpawnAttack::clone(int x, int y, Direction dir){
	Sprite* image = this->get_image()->clone(dir);
	AttackStatistics stats;
	stats.base_damage = this->base_damage;
	stats.charge_time = this->charge_time;
	stats.exp_date = this->expiration_date;
	stats.penetration = this->penetration;
	stats.range = this->range;
	stats.tree_depth = this->tree_depth_level;
	MultiSpawnAttack* dup = new MultiSpawnAttack(x, y, this->velocity, this->velocity_delay, image, stats, this->attack_spawning, this->spawn_delay);

	switch(dir){
	case N:
		dup->set_boundary_value(this->get_bounding_height(), this->get_bounding_width(), this->reference_horizontal, this->reference_vertical);
		y -= this->y_adjustment + this->get_bounding_height();
		break;
	case S:
		dup->set_boundary_value(this->get_bounding_height(), this->get_bounding_width(), this->reference_horizontal, this->reference_vertical);
		break;
	case W:
		dup->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
		y -= this->y_adjustment + this->get_bounding_height()/2;
		x -= this->get_bounding_width() /2;
		break;
	case E:
		dup->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
		y -= this->y_adjustment + this->get_bounding_height()/2;
		break;
	}

	dup->set_x_pos(x);
	dup->set_y_pos(y);

	dup->set_my_caster(this->my_caster);
	return dup;
}