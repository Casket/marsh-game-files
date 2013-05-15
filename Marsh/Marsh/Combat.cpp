#include "Main.h"

#include "ProtectionAttack.h"

using namespace std;

Combat::Combat(int x, int y, int vel, int vel_d, Sprite* img)
:iDrawable(x, y, vel, vel_d, img)
{
	this->vitality = BASE_VIT;
	this->intelligence = BASE_INTEL;
	this->focus = BASE_FOCUS;
	this->willpower = BASE_WILL;
	this->armor = BASE_ARMOR;
	this->entangled = false;
	this->active_dispatchers = new std::list<Dispatcher*>();
	this->active_wards = new std::list<Attack*>();

	for (int i=0; i < MAX_ATTACKS; i++)
		this->attack_loadout[i] = NULL;

	this->health = calculate_health(this->vitality);
	this->casted_spell = NULL;
	this->targeted = false;
	this->player_credit = false;
	this->experience_worth = 10;
	this->dialogue = new std::vector<std::string>();
	this->current_dialogue = 0;
	this->should_free_player = false;
	this->has_player_hostage = false;
	this->casting_timer = 0;
	this->set_stats(this->vitality, this->intelligence, this->focus, this->willpower, this->armor);

	this->insert_db_attacks();
	//this->insert_utility_spells(4);
	this->insert_damage_attacks(4);
}

Combat::~Combat(void) {
	if(casted_spell != NULL)
		delete casted_spell;
}

void Combat::insert_damage_attacks(int n){
	AttackDB* attacks = new AttackDB();
	this->attack_loadout[n] = attacks->fetch_attack(SHADOW_BALL)->clone(0, 0, N);
	this->attack_loadout[n++]->set_my_caster(this);

	this->attack_loadout[n] = attacks->fetch_attack(SHADOW_BALL_PLUS)->clone(0, 0, N);
	this->attack_loadout[n++]->set_my_caster(this);

	this->attack_loadout[n] = attacks->fetch_attack(SHADOW_WAVE)->clone(0, 0, W);
	this->attack_loadout[n++]->set_my_caster(this);

	this->attack_loadout[n] = attacks->fetch_attack(SHADOW_NOVA)->clone(0, 0, N);
	this->attack_loadout[n++]->set_my_caster(this);

	delete attacks;
}

void Combat::insert_db_attacks(void){
	AttackDB* attacks = new AttackDB();
	this->attack_loadout[0] = attacks->fetch_attack(SHADOW_NEEDLE)->clone(0, 0, N);
	this->attack_loadout[0]->set_my_caster(this);

	this->attack_loadout[1] = attacks->fetch_attack(SHADOW_NEEDLE_PLUS)->clone(0, 0, N);
	this->attack_loadout[1]->set_my_caster(this);

	this->attack_loadout[2] = attacks->fetch_attack(SHADOW_SPIKES)->clone(0, 0, N);
	this->attack_loadout[2]->set_my_caster(this);

	this->attack_loadout[3] = attacks->fetch_attack(DEATH_BEAM)->clone(0, 0, W);
	this->attack_loadout[3]->set_my_caster(this);
	delete attacks;
}

void Combat::insert_utility_spells(int start){
	AttackDB* attacks = new AttackDB();

	this->attack_loadout[start] = attacks->fetch_attack(DRAIN_HEALTH)->clone(0, 0, N);
	this->attack_loadout[start++]->set_my_caster(this);

	this->attack_loadout[start] = attacks->fetch_attack(SHIELD)->clone(0, 0, N);
	this->attack_loadout[start]->set_my_caster(this);
	start++;

	this->attack_loadout[start] = attacks->fetch_attack(DEFLECT)->clone(0, 0, N);
	this->attack_loadout[start]->set_my_caster(this);
	start++;

	this->attack_loadout[start] = attacks->fetch_attack(SHADOW_DASH)->clone(0, 0, N);
	this->attack_loadout[start]->set_my_caster(this);
	start++;

	this->attack_loadout[start] = attacks->fetch_attack(TELEPORT)->clone(0, 0, N);
	this->attack_loadout[start]->set_my_caster(this);
	start++;

	this->attack_loadout[start] = attacks->fetch_attack(BIND_IN_SHADOWS)->clone(0, 0, N);
	this->attack_loadout[start]->set_my_caster(this);


	delete attacks;
}

void Combat::apply_ward(Attack* ward){
	this->active_wards->push_back(ward);
}

void Combat::set_stats(int vitality, int intelligence, int focus, int willpower, int armor) {
	this->vitality = vitality;
	this->intelligence = intelligence;
	this->focus = focus;
	this->willpower = willpower;
	this->armor = armor;
	this->max_health = calculate_health(this->vitality);
	this->health = this->max_health;
}

EntityType Combat::get_my_type(void){
	return this->my_type;
}

void Combat::update_dispatchers(void){
	/*
	std::list<std::list<Dispatcher*>::iterator> bad_iters;
	std::list<Dispatcher*>::iterator end = this->active_dispatchers->end();
	for (std::list<Dispatcher*>::iterator iter = this->active_dispatchers->begin(); iter != end; ++iter){
		(*iter)->update();
		if (!(*iter)->active){
			bad_iters.push_back(iter);
		}
	}

	for (std::list<std::list<Dispatcher*>::iterator>::iterator iter = bad_iters.begin(); iter != bad_iters.end(); ++iter){
		this->active_dispatchers->erase((*iter));
	}
	*/
}

void Combat::set_my_type(EntityType e) {
	this->my_type = e;
}

int Combat::calculate_health(int stat){
	return stat;
	//TODO add some calculation here
}

int Combat::get_current_health(void){
	return this->health;
}


int Combat::get_max_health(void){
	return this->max_health;
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
		this->casted_spell = this->casted_spell->clone(spell_x, spell_y, this->get_image()->get_facing());
		this->casted_spell->set_world(this->get_world());
		this->get_world()->insert_entity(this->casted_spell);
		this->casted_spell = NULL;
	}
}

void Combat::launch_attack(int attack_num) {
	if ((attack_num >= 0) && (attack_num < MAX_ATTACKS )) {
		this->casting = true;
		this->casted_spell = this->attack_loadout[attack_num];
		if (this->casted_spell->get_charge_time() == 0)
			return;
		if (this->my_type == Hero || this->my_type == Outcast || this->my_type == Rival){
			int charge_time;
			if ((charge_time = this->casted_spell->get_charge_time()) != 0){
				Attack_Sprite* animation = new Attack_Sprite("Resources//Attack Sprites//Charging.bmp", E, 5, 1, 4, 4, 296/4, 76);
				animation->set_state_frame_counts(0, 4, 0);
				AttackCharge* charge_amination = new AttackCharge(this->get_x_pos() - 17, this->get_y_pos() - 40, animation, this->casted_spell->get_charge_time());
				charge_amination->set_boundary_value(0, 0,0, animation->height);
				charge_amination->set_world(this->get_world());
				this->get_world()->insert_entity(charge_amination);
			}
		}
	}
}

void Combat::update(){
	if (this->entangled)
		return;
	check_collisions();

	if (this->has_player_hostage){
		if (Player_Accessor::hero->wants_to_talk())
			this->speak();
	}
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
		top_bottom_skew = this->velocity*SKEW_FACTOR;
	if (facing == N || facing == S)
		left_right_skew = this->velocity*SKEW_FACTOR;

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

bool Combat::check_new_pos(int x, int y){
	int original_x = this->get_x_pos();
	int original_y = this->get_y_pos();
	int original_vel = this->velocity;

	this->x_pos = x;
	this->y_pos = y;
	this->velocity = 0;

	this->check_collisions();

	this->x_pos = original_x;
	this->y_pos = original_y;
	this->velocity = original_vel;

	return (this->can_walk_down && this->can_walk_left && this->can_walk_right && this->can_walk_up);
}

Combat* Combat::fetch_me_as_combat(void){
	return this;
}

void Combat::deal_with_attack(Attack* attack){
	if (attack->my_caster->my_type == this->my_type)
		return;
	if (!this->active_wards->empty()){
		std::list<Attack*>::iterator pos = this->active_wards->begin();
		(*pos)->deal_with_attack(attack); // tell the ward to handle the attack
		this->active_wards->erase(pos);
		return;
	}

	int armor = this->armor;
	int adjusted_armor = armor - attack->penetration;
	if (adjusted_armor <= 0)
		adjusted_armor = 1;
	this->health -= (attack->base_damage*attack->get_my_caster()->intelligence)/adjusted_armor; // base damage must be scalar - for reasons, don't ask
	if(!this->player_credit){
		if(attack->get_my_caster() == Player_Accessor::get_player()){
			this->player_credit = true;
		}
	}

	attack->start_death_sequence();
	this->get_image()->animation_frame = FLICKER_FRAME_NUMBER;

	if(this->health <= 0){
		this->health = 0;
		if (this->player_credit)
			Player_Accessor::get_player()->credit_death(this);
		this->upon_death();
	}
}

void Combat::upon_death(void){
	this->my_world->removal_queue->push_back(this);
}

void Combat::append_dialogue(std::string message){
	if (this->dialogue == NULL)
		this->dialogue = new std::vector<std::string>();
	this->dialogue->push_back(message);
}
void Combat::clear_dialogue(void){
	this->dialogue->clear();
}

void Combat::face_speaker(void){
	Direction players_dir = Player_Accessor::get_player()->get_image()->get_facing();
	Direction old_dir = this->image->get_facing();
	switch (players_dir){
		case N:
			this->image->set_facing(S);
			break;
		case S:
			this->image->set_facing(N);
			break;
		case E:
			this->image->set_facing(W);
			break;
		case W:
			this->image->set_facing(E);
			break;
	}
	if (old_dir != this->image->get_facing())
		this->image->force_direction_update(this->image->get_facing());
}

void Combat::speak(void){
	if (this->should_free_player){
		Player_Accessor::get_player()->interacting = false;
		this->has_player_hostage = false;
		this->should_free_player = false;
		Player_Accessor::get_player()->display_to_user("");
		return;
	}
	if (this->dialogue->empty())
		return;
	Player_Accessor::get_player()->interacting = true;
	this->has_player_hostage = true;
	Player_Accessor::get_player()->display_to_user(this->dialogue->at(this->current_dialogue++));
	if (this->current_dialogue >= this->dialogue->size()){
		this->current_dialogue = 0;
		this->should_free_player = true;
		Player_Accessor::get_player()->credit_interaction(this->get_my_type());
	}

}
