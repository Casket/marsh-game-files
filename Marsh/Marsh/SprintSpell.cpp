#include "Main.h"

int velocity_to_receive;
int previous_velocity;

SprintSpell::SprintSpell(int vel, int duration, int charge)
:Attack(0, 0, 0, 0, NULL, 0, 0, 0, 0, duration, charge)
{
	this->velocity_to_receive = vel;
	this->previous_velocity = MOVEMENT_DELTA;
	this->set_boundary_value(0, 0, 0, 0);
	Solid_Sprite* image = new Solid_Sprite("Resources//Misc//blank_image.bmp", 0, 0, 5, 5);
	this->image = image;
	this->death_timer = 0;
	this->prev_framerate = 5;
}

SprintSpell::~SprintSpell(void){

}

void SprintSpell::update(void){
	if (this->death_timer++ == 0){
		this->previous_velocity = this->my_caster->velocity;
		this->prev_framerate = this->my_caster->get_image()->animation_delay;
		this->my_caster->velocity = this->velocity_to_receive;
		this->my_caster->get_image()->animation_delay = 1;
	}
	if (++this->death_timer >= this->expiration_date){
		this->my_caster->velocity = this->previous_velocity;
		this->my_caster->get_image()->animation_delay = this->prev_framerate;
		this->get_world()->remove_entity(this);
	}
}

Attack* SprintSpell::clone(int x, int y, Direction dir){
	Sprite* image = this->get_image()->clone(W);
	SprintSpell* dup = new SprintSpell(this->velocity_to_receive, this->expiration_date, this->charge_time);
	dup->set_my_caster(this->my_caster);
	dup->spell_id = this->spell_id;
	return dup;
}