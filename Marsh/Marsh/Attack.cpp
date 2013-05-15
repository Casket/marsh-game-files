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
	this->my_type = Wallop;
	this->distance_traveled = 0;
	this->death_timer = 0;
	this->mana_cost = 0;
	this->x_adjustment = 0;
	this->y_adjustment = 0;
	this->target = NULL;
}

Attack::Attack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats)
:iDrawable(x, y, vel, vel_d, img)
{
	this->base_damage = stats.base_damage;
	this->penetration = stats.penetration;
	this->range = stats.range;
	this->tree_depth_level = stats.tree_depth;
	this->expiration_date = stats.exp_date;
	this->charge_time = stats.charge_time;
	this->my_type = Wallop;
	this->mana_cost = 0;
	this->distance_traveled = 0;
	this->death_timer = 0;
	this->x_adjustment = 0;
	this->y_adjustment = 0;
	this->target = NULL;
}

Attack::~Attack(void)
{

}

void Attack::set_position_adjustment(int x, int y){
	this->x_adjustment = x;
	this->y_adjustment = y;
}

Attack* Attack::fetch_me_as_attack(void){
	return this;
}

int Attack::get_mana_cost(void){
	return this->mana_cost;
}
void Attack::set_mana_cost(int m){
	this->mana_cost = m;
}

void Attack::update(void){
	this->get_image()->update();
	if (!this->alive){
		if (++this->death_timer >= this->expiration_date){
			this->death_timer = 0;
			this->get_world()->remove_entity(this);
		}
		return;
	}

	
	if (this->distance_traveled >= this->range){
		//this->start_death_sequence();
		this->get_world()->remove_entity(this);
	}

	if (!this->detect_collisions()){
		this->update_position();
	}
}

void Attack::update_position(void){
	if (++this->movement_counter >= this->velocity_delay){
		this->distance_traveled += this->velocity;
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
		if (check->my_type == this->my_caster->my_type)
			continue;
		if (check->my_type == Wallop){
			Attack* check_attack = check->fetch_me_as_attack();
			if (check_attack->my_caster == this->my_caster)
				continue;
			/*if (check_attack->my_caster->my_type == this->my_caster->my_type)
				continue;*/
		}

		check_x = check->get_reference_x();
		check_y = check->get_reference_y();
		check_width = check->get_bounding_width();
		check_height = check->get_bounding_height();
		if (check_width == 0 && check_height == 0)
			continue;


		if (detect_hit(my_x, my_y, my_height, my_width, 
			check_x, check_y, check_width, check_height)){
				this->attack_target((*iter));
				return true;
		}
	}
	return false;
}

void Attack::attack_target(iDrawable* target){
	target->deal_with_attack(this);
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
	this->alive = false;
	this->death_timer = 0;
	this->get_image()->casting_update();
}

void Attack::set_my_caster(Combat* caster){
	this->my_caster = caster;
}
Combat* Attack::get_my_caster(void){
	return this->my_caster;
}

void Attack::deal_with_attack(Attack* attack){
	if (!this->alive || !attack->alive)
		return;
	// which attack should die?
	if (this->tree_depth_level == attack->tree_depth_level){
		// kill both dem
		this->start_death_sequence();
		attack->start_death_sequence();
	} else if(this->tree_depth_level > attack->tree_depth_level){
		// kill that attack
		this->base_damage -= attack->base_damage;
		if (this->base_damage <= 0)
			this->start_death_sequence();
		attack->start_death_sequence();
	} else {
		// i die yo
		attack->base_damage -= this->base_damage;
		if (attack->base_damage <= 0)
			attack->start_death_sequence();
		this->start_death_sequence();
	}
}

int Attack::get_charge_time(void){
	int charge = this->charge_time - FOCUS_EFFECT * this->my_caster->focus;
	if (charge < 0)
		return 0;
	return charge;
}

iDrawable* Attack::get_above_target(void){
	return this->my_caster;
}

Attack* Attack::clone(int x, int y, Direction dir){
	int damage, penetrate, charge;
	damage = this->base_damage;
	penetrate = this->penetration;
	charge = this->charge_time;
	Sprite* image = this->get_image()->clone(dir);

	Attack* result = new Attack(x, y, this->velocity, this->velocity_delay, image, damage, penetrate, this->range, this->tree_depth_level, this->expiration_date, charge);
	int image_width = this->get_image()->get_current_frame()->w;
	switch(dir){
	case N:
		result->set_boundary_value(this->get_bounding_height(), this->get_bounding_width(), this->reference_horizontal, this->reference_vertical);
		y -= this->y_adjustment + this->get_bounding_height();
		break;
	case S:
		result->set_boundary_value(this->get_bounding_height(), this->get_bounding_width(), this->reference_horizontal, this->reference_vertical);
		y += this->y_adjustment;
		break;
	case W:
		result->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
		x -= this->x_adjustment + this->get_bounding_width();
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
	result->spell_id = this->spell_id;
	result->set_position_adjustment(this->x_adjustment, this->y_adjustment);
	return result;
}
