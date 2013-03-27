#include "Main.h"

using namespace std;

Combat::Combat(int x, int y, int vel, int vel_d, Sprite* img)
:iDrawable(x, y, vel, vel_d, img)
{
	this->vitality = BASE_VIT;
	this->intelligence = BASE_INTEL;
	this->focus = BASE_FOCUS;
	this->willpower = BASE_WILL;
	this->armor = BASE_ARMOR;

	this->health = calculate_health(this->vitality);
	this->mana = calculate_mana(this->willpower);
	this->casted_spell = NULL;
}

Combat::~Combat(void) {
	if(casted_spell != NULL)
		delete casted_spell;
}

void Combat::set_my_type(EntityType e) {
	this->my_type = e;
}

int Combat::calculate_health(int stat){
	return stat;
	//TODO add some calculation here
}

int Combat::calculate_mana(int stat) {
	return stat;
	// TODO do some calculation
}

void Combat::casting_update(void) {
	// should only be called if he's casting, but let's be careful
	if (!this->casting)
		return;
	if (this->casted_spell == NULL) {
		// oh balls... this is bad... this is bad...
		// player is casting AND has no spell cast... idk
		throw std::exception("COMBAT: spell casting, no current spell");
	}

	/*
	if (++this->casting_timer >= this->casted_spell->get_charge_time()){
	// ready to fire that bad boy
	this->casting = false;
	this->casting_timer = 0;
	// TODO put the casted spell into the world
	this->casted_spell = NULL;
	}
	*/ 
}

void Combat::launch_attack(int attack_num) {
	/*
	if ((attack_num >= 0) && (attack_num < MAX_ATTACKS ) {
	this->casting = true;
	Attack* used_attack = this->attack_loadout[attack_num];
	this->casted_spell = used_attack->clone();
	}
	*/ 
}

void Combat::update(){
	check_collisions();
}

void Combat::check_collisions(void){
	std::list<iDrawable*>* entities = this->get_world()->get_active_entities();
	Tile*** map = this->get_world()->get_tile_map();
	int my_x = this->get_reference_x();
	int my_y = this->get_reference_y();
	int my_width = this->get_bounding_width();
	int my_height = this->get_bounding_height();
	Direction facing = this->get_image()->get_facing();

	this->can_walk = true;
	Tile* nearby;
	switch(facing){
	case N:
		nearby = map[my_y / TILE_SIZE - 1][my_x / TILE_SIZE];
		break;
	case S:
		nearby = map[my_y / TILE_SIZE + 1][my_x / TILE_SIZE];
		break;
	case E:
		nearby = map[my_y / TILE_SIZE][my_x / TILE_SIZE + 1];
		break;
	case W:
		nearby = map[my_y / TILE_SIZE][my_x / TILE_SIZE - 1];
		break;
	default:
		break;
	}

	if (nearby != NULL)
		if (!nearby->can_walk)
			this->can_walk = false;


	int check_x, check_y, check_width, check_height;

	std::list<iDrawable*>::iterator iter;
	std::list<iDrawable*>::iterator end = entities->end();
	for (iter = entities->begin(); iter != end; iter++){
		iDrawable* check = (*iter);
		check_x = check->get_reference_x();
		check_y = check->get_reference_y();
		check_width = check->get_bounding_width();
		check_height = check->get_bounding_height();

		switch(facing){
		case N:
			if (my_y <= (check_y + check_height) && (my_y + my_height) >= check_y){
				if (check_x <= (my_x + my_width) && check_x >= (my_x)){
					can_walk = false;	
				} else if (my_x <= (check_x + check_width) && my_x >= (check_x)){
					can_walk = false;
				}
			}
			break;
		case S:
			if ((my_y + my_height) >= (check_y) && my_y >= (check_y + check_height)){
				if (check_x <= (my_x + my_width) && check_x >= (my_x)){
					can_walk = false;	
				} else if (my_x <= (check_x + check_width) && my_x >= (check_x)){
					can_walk = false;
				}
			}
			break;

		case W:
			if (my_x <= (check_x + check_width) && (my_x + my_width) >= check_x){
				if (check_y <= (my_y + my_height) && check_y >= (my_y)){
					can_walk = false;	
				} else if (my_y <= (check_y + check_height) && my_y >= (check_y)){
					can_walk = false;
				}
			}
			break;

		case E:
			if ((my_x + my_width) >= (check_x) && my_x >= (check_x + check_width)){
				if (check_y <= (my_y + my_height) && check_y >= (my_y)){
					can_walk = false;	
				} else if (my_y <= (check_y + check_height) && my_y >= (check_y)){
					can_walk = false;
				}
			}
			break;
		}
	}


}

void Combat::deal_with_attack(Attack* attack){

}
