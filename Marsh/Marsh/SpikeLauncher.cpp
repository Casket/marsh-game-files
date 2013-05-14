#include "SpikeLauncher.h"
#include "Main.h"

using namespace std;

SpikeLauncher::SpikeLauncher(int x, int y, Sprite* img, Attack* to_launch, AttackStatistics stats, int delay, int spawn_num)
:Attack(x, y, 0, 0, img, stats)
{
	this->spawn_counter = 0;
	this->spawn_delay = delay;
	this->attack_spawn = to_launch;
	this->spawn_num = spawn_num;
}

SpikeLauncher::~SpikeLauncher(void){
	
}

Attack* SpikeLauncher::clone(int x, int y, Direction dir){
	AttackStatistics stats;
	stats.base_damage = this->base_damage;
	stats.charge_time = this->charge_time;
	stats.exp_date = this->expiration_date;
	stats.penetration = this->penetration;
	stats.range = this->range;
	stats.tree_depth = this->tree_depth_level;
	SpikeLauncher* dup = new SpikeLauncher(x, y, this->get_image()->clone(dir), this->attack_spawn, stats, this->spawn_delay, this->spawn_num);
	dup->set_boundary_value(this->bounding_width, this->bounding_height, this->reference_horizontal, this->reference_vertical);
	dup->set_my_caster(this->my_caster);
	dup->spell_id = this->spell_id;
	return dup;
}

void SpikeLauncher::update(void){
	// this bad boy needs to spawn some spikage
	int x_pos = this->get_x_pos();
	int y_pos = this->get_y_pos();
	int delta_x = 0;
	int delta_y = 0;
	switch(this->get_image()->get_facing()){
	case N:
		x_pos += this->reference_horizontal + this->get_bounding_width()/2;
		y_pos -= this->range;
		delta_y = -1*this->attack_spawn->get_bounding_height();
		break;
	case S:
		x_pos += this->reference_horizontal + this->get_bounding_width()/2;
		y_pos += this->range;
		delta_y = this->attack_spawn->get_bounding_height();
		break;
	case W:
		x_pos -= this->range;
		y_pos -= this->get_bounding_height()/2;
		delta_x = -1*this->attack_spawn->get_bounding_width();
		break;
	case E:
		x_pos += this->range;
		y_pos -= this->get_bounding_height()/2;
		delta_x = this->attack_spawn->get_bounding_width();
		break;
	}

	for (int i=0; i < this->spawn_num; i++){
		Attack* launched = this->attack_spawn->clone(x_pos, y_pos, this->get_image()->get_facing());
		x_pos += delta_x;
		y_pos += delta_y;
		launched->set_world(this->get_world());
		this->get_world()->insert_entity(launched);
	}
	this->get_world()->remove_entity(this);


}

void SpikeLauncher::start_death_sequence(void){

}
