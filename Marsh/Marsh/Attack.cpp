#include "Main.h"


Attack::Attack(int x, int y, int vel, int vel_d, Sprite* img, 
			   int base_damage, int penetration, int range, int tree_depth, int exp_date, int charge_time)
			   :iDrawable(x, y, vel, vel_d, img) 
{
	this->base_damage = base_damage;
	this->penetration = penetration;
	this->range = range;
	this->tree_depth_level = tree_depth;
	this->expiration_date = exp_date;
	this->charge_time = charge_time;
}

Attack::~Attack(void)
{

}

void Attack::update(void){
	this->get_image()->update();
	if (!detect_collisions()){
		if (++this->movement_counter >= this->velocity_delay){
			switch(this->get_image()->get_facing()){
				case N:
					this->set_y_pos(this->get_y_pos() - this->velocity);
					break;
				case S:
					this->set_y_pos(this->get_y_pos() + this->velocity);
					break;
				case W:
					this->set_x_pos(this->get_x_pos() - this->velocity);
					break;
				case E:
					this->set_x_pos(this->get_x_pos() + this->velocity);
					break;
				case NE:
					this->set_x_pos(this->get_x_pos() + (int) (this->velocity * ANGLE_SHIFT));
					this->set_y_pos(this->get_y_pos() - (int) (this->velocity * ANGLE_SHIFT));
					break;
				case SE:
					this->set_x_pos(this->get_x_pos() + (int) (this->velocity * ANGLE_SHIFT));
					this->set_y_pos(this->get_y_pos() + (int) (this->velocity * ANGLE_SHIFT));
					break;
				case SW:
					this->set_x_pos(this->get_x_pos() - (int) (this->velocity * ANGLE_SHIFT));
					this->set_y_pos(this->get_y_pos() + (int) (this->velocity * ANGLE_SHIFT));
					break;
				case NW:
					this->set_x_pos(this->get_x_pos() - (int) (this->velocity * ANGLE_SHIFT));
					this->set_y_pos(this->get_y_pos() - (int) (this->velocity * ANGLE_SHIFT));
					break;

			}
		}
	}

}

bool Attack::detect_collisions(void){
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
		check_x = check->get_reference_x();
		check_y = check->get_reference_y();
		check_width = check->get_bounding_width();
		check_height = check->get_bounding_height();


		if (detect_hit(my_x, my_y, my_height, my_width, 
			check_x, check_y, check_width, check_height)){
				//(*iter)->deal_with_attack(this);
				start_death_sequence();
				return true;
		}
	}
	return false;
}

bool Attack::detect_hit(int my_x, int my_y, int my_height, int my_width, int check_x, int check_y, int check_width, int check_height){
	if (my_y <= (check_y + check_height) && (my_y) >= check_y){
		if (check_x <= (my_x + my_width) && check_x >= (my_x)){
			return true;	
		} else if ((my_x) <= (check_x + check_width) && my_x >= (check_x)){
			return true;
		}
	}

	if ((my_y + my_height) >= (check_y) && (my_y + my_width) <= (check_y + check_height)){
		if (check_x <= (my_x + my_width) && check_x >= (my_x)){
			return true;	
		} else if ((my_x) <= (check_x + check_width) && my_x >= (check_x)){
			return true;
		}
	}

	if ((my_x ) <= (check_x + check_width) && (my_x ) >= check_x){
		if (check_y <= (my_y + my_height) && check_y >= (my_y)){
			return true;	
		} else if ((my_y) <= (check_y + check_height) && my_y >= (check_y)){
			return true;
		}
	}

	if ((my_x + my_width) >= (check_x) && (my_x + my_width) <= (check_x + check_width)){
		if (check_y <= (my_y + my_height) && check_y >= (my_y)){
			return true;
		} else if ((my_y) <= (check_y + check_height) && my_y >= (check_y)){
			return true;
		}
	}
	return false;
}

void Attack::start_death_sequence(void){
	this->get_world()->remove_entity(this);
	//delete this;
}

void Attack::set_my_caster(Combat* caster){
	this->my_caster = caster;
}
Combat* Attack::get_my_caster(void){
	return this->my_caster;
}

void Attack::deal_with_attack(Attack* attack){

}

int Attack::get_charge_time(void){
	return this->charge_time;
}

Attack* Attack::clone(int x, int y, int intelligence, int focus, Direction dir){
	int damage, penetrate, charge;
	damage = this->base_damage;
	penetrate = this->penetration;
	charge = this->charge_time;
	Sprite* image = this->get_image()->clone(dir);
	Attack* result = new Attack(x, y, this->velocity, this->velocity_delay, image, damage, penetrate, this->range, this->tree_depth_level, this->expiration_date, charge);
	result->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
	result->set_my_caster(this->my_caster);
	return result;
}
