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
	this->attack_loadout[0] = new Attack(800, 800, 2, 10, new Attack_Sprite("Resources//magic//fireball.bmp", W, 5, 1, 5, 5, 26,26), 51,0,0, 0,0,1);
	this->attack_loadout[0]->set_boundary_value(26, 26, 2, 2);
	this->attack_loadout[0]->set_my_caster(this);
	this->attack_loadout[1] = new Attack(800, 800, 10, 10, new Attack_Sprite("Resources//magic//fireball.bmp", W, 5, 1, 5, 5, 26, 26), 100, 0, 0, 3, 0, 100);
	this->attack_loadout[1]->set_boundary_value(26, 26, 2, 2);
	this->attack_loadout[1]->set_my_caster(this);
	this->health = calculate_health(this->vitality);
	this->mana = calculate_mana(this->willpower);
	this->casted_spell = NULL;
	this->targeted = false;
	this->player_credit = false;
	this->experience_worth = 10;
}

Combat::~Combat(void) {
	if(casted_spell != NULL)
		delete casted_spell;
}

void Combat::set_stats(int vitality, int intelligence, int focus, int willpower, int armor) {
	this->vitality = vitality;
	this->intelligence = intelligence;
	this->focus = focus;
	this->willpower = willpower;
	this->armor = armor;
	this->health = calculate_health(this->vitality);
	this->mana = calculate_mana(this->willpower);
}

EntityType Combat::get_my_type(void){
	return this->my_type;
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


	if (++this->casting_timer >= this->casted_spell->get_charge_time()){
		// ready to fire that bad boy
		this->casting = false;
		this->casting_timer = 0;
		// TODO put the casted spell into the world
		int spell_x = this->get_reference_x();
		int spell_y = this->get_reference_y();
		
		switch(this->image->get_facing()){
			case N:
				spell_y -= this->get_bounding_height();
				break;
			case S:
				spell_y += this->get_bounding_height() / 2;
				break;
			case W:
				spell_x -= this->get_bounding_width();
				spell_y -= this->get_bounding_height() / 2;
				break;
			case E:
				spell_x += this->get_bounding_width();
				spell_y -= this->get_bounding_height() / 2;
				break;
			case NE:
				spell_x += this->get_bounding_width() / 2;
				spell_y -= this->get_bounding_height();
				break;
			case NW:
				spell_y -= this->get_bounding_height();
				spell_x -= this->get_bounding_width() / 2;
				break;
			case SW:
				spell_y += this->get_bounding_height() / 2;
				spell_x -= this->get_bounding_width() / 2;
				break;
			case SE:
				spell_y += this->get_bounding_height() / 2;
				spell_x += this->get_bounding_width() / 2;
				break;
		}
		this->casted_spell = this->casted_spell->clone(spell_x, spell_y,this->intelligence,this->focus, this->get_image()->get_facing());
		this->casted_spell->set_world(this->get_world());
		this->get_world()->insert_entity(this->casted_spell);
		this->casted_spell = NULL;
	}
}

void Combat::launch_attack(int attack_num) {

	if ((attack_num >= 0) && (attack_num < MAX_ATTACKS )) {
		this->casting = true;
		this->casted_spell = this->attack_loadout[attack_num];
	}
	
}

void Combat::update(){
	check_collisions();
}

void Combat::check_collisions(void){
	iDrawable::check_collisions();

	Tile*** map = this->get_world()->get_tile_map();
	int my_x = this->get_reference_x();
	int my_y = this->get_reference_y();
	int my_width = this->get_bounding_width();
	int my_height = this->get_bounding_height();
	int top_bottom_skew = 0;
	int left_right_skew = 0;
	Direction facing = this->get_image()->get_facing();

	if (facing == W || facing == E)
		top_bottom_skew = SKEW_FACTOR;
	if (facing == N || facing == S)
		left_right_skew = SKEW_FACTOR;

	Tile* nearby[4];

	int delta = 2;

	// what tile is his top left corner in?
	nearby[0] = map[(this->get_reference_y() - delta) / TILE_SIZE][(this->get_reference_x()- delta) / TILE_SIZE];

	// top right?
	nearby[1] = map[(this->get_reference_y() - delta) / TILE_SIZE][(this->get_reference_x() + this->get_bounding_width() + delta) / TILE_SIZE];

	// bottom left
	nearby[2] = map[(this->get_reference_y() + this->get_bounding_height() + delta) / TILE_SIZE][(this->get_reference_x() - delta) / TILE_SIZE];

	// bottom right
	nearby[3] = map[(this->get_reference_y() + this->get_bounding_height() + delta) / TILE_SIZE][(this->get_reference_x() + this->get_bounding_width() + delta) / TILE_SIZE];

	for (int i=0; i < 4; i++){
		if (!nearby[i]->can_walk)
			check_walkable(my_x, my_y, my_height, my_width, 
				nearby[i]->col*TILE_SIZE, nearby[i]->row*TILE_SIZE, TILE_SIZE, TILE_SIZE, 
				left_right_skew, top_bottom_skew);
	}
}

void Combat::deal_with_attack(Attack* attack){

	int armor = this->armor;
	if(armor == 0)
		armor = 1;
	this->health -= (attack->base_damage*attack->get_my_caster()->intelligence)/armor;
	if(!this->player_credit){
		if(attack->get_my_caster() == Player_Accessor::get_player()){
			this->player_credit = true;
		}
	}
	if(this->health <= 0){	
		Player_Accessor::get_player()->credit_death(this);
	}
	attack->start_death_sequence();
}
