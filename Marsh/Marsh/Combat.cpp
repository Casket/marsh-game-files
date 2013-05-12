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

	for (int i=0; i < MAX_ATTACKS; i++)
		this->attack_loadout[i] = NULL;


	AttackDB* attacks = new AttackDB();
	this->attack_loadout[0] = attacks->fetch_attack(GUARD_MELEE);
	this->attack_loadout[0] = this->attack_loadout[0]->clone(0, 0, W);
	this->attack_loadout[0]->set_my_caster(this);

	Attack_Sprite* death = new Attack_Sprite("Resources//Attack Sprites//Death_Beam_2.bmp", N, 10, 1, 3, 3, 76, 59);
	death->set_state_frame_counts(0, 2, 2);
	AttackStatistics stats;
	stats.base_damage = 0;
	stats.charge_time = 100;
	stats.exp_date = 30;
	stats.penetration = 15;
	stats.range = MID_RANGE;
	stats.tree_depth = 1;

	Attack* death_beam = new Attack(800, 800, 2, 10, death, stats);
	death_beam->set_boundary_value(75, 20, 5, 20);
	death_beam->set_my_caster(this);
	death_beam->set_position_adjustment(40, 20);
	death_beam->spell_id = DEATH_BEAM;

	this->attack_loadout[1] = death_beam;
	
	Attack_Sprite* shooter_img = new Attack_Sprite("Resources//Attack Sprites//Death_Beam_2_charge.bmp", N, 5, 1, 1, 1, 120, 55);
	shooter_img->set_state_frame_counts(0, 2, 0);

	stats.base_damage = 0;
	stats.charge_time = 100;
	stats.exp_date = 1000;
	stats.penetration = 15;
	stats.range = 250;
	stats.tree_depth = 2;
	MultiSpawnAttack* beamer = new MultiSpawnAttack(800, 800, 2, 10, shooter_img, stats, death_beam, 20);
	beamer->set_position_adjustment(50, 0);
	beamer->set_boundary_value(50, 19, 0, 0);
	beamer->set_my_caster(this);
	beamer->spell_id = DEATH_BEAM;
	this->attack_loadout[2] = beamer;

	this->attack_loadout[3] = attacks->fetch_attack(GUARD_MELEE)->clone(0, 0, W);
	this->attack_loadout[3]->set_my_caster(this);

	this->attack_loadout[4] = attacks->fetch_attack(MONSTER_MELEE)->clone(0, 0, W);
	this->attack_loadout[4]->set_my_caster(this);

	Attack_Sprite* ward_img = new Attack_Sprite("Resources//Attack Sprites//ward.tga", W, 7, 1, 13, 13, 66, 65);
	ward_img->set_state_frame_counts(0, 13, 0);
	ward_img->is_translucent = true;
	
	
	this->attack_loadout[5] = new ProtectionAttack(800, 800, 2, 10, ward_img, stats, ShieldAttack);
	this->attack_loadout[5]->set_boundary_value(50, 60, 0, 10);
	this->attack_loadout[5]->set_position_adjustment(0,0);
	this->attack_loadout[5]->set_my_caster(this);
	this->attack_loadout[5]->spell_id = SHIELD;
	
	
	this->attack_loadout[0]->set_my_caster(this);
	/*
	fireball_spr->set_state_frame_counts(0, 5, 3);
	this->attack_loadout[1] = new Attack(800, 800, 2, 10, fireball_spr, 100, 0, 500, 3, 50, 100);

	this->attack_loadout[1]->my_type = Wallop;
	this->attack_loadout[1]->set_boundary_value(26, 26, 2, 2);
	this->attack_loadout[1]->set_my_caster(this);
	*/
	
	Attack_Sprite* fire = new Attack_Sprite("Resources//Attack Sprites//Nova.tga", W, 10, 1, 11, 11, 580/11, 48);

	fire->is_translucent = true;
	fire->set_state_frame_counts(0, 1, 10);
	
	stats.base_damage = 0;
	stats.charge_time = 100;
	stats.exp_date = 1000;
	stats.penetration = 15;
	stats.range = 250;
	stats.tree_depth = 2;
	fire->is_translucent = true;
	this->attack_loadout[6] = new StunningAttack(800, 800, 1, 10, fire, stats);
	this->attack_loadout[6]->set_boundary_value(35, 25, 10, 15);
	this->attack_loadout[6]->set_my_caster(this);
	this->attack_loadout[6]->set_position_adjustment(0, 20);
	this->attack_loadout[6]->spell_id = SHADOW_NOVA;
	//Attack_Sprite* fireball_spr = new Attack_Sprite("Resources//magic//fireball.bmp", W, 5, 1, 5, 8, 26, 26);
	//this->attack_loadout[7] = new PersistentAttack(800, 800, 1, 10, fireball_spr, stats);
	//this->attack_loadout[7]->set_boundary_value(26, 26, 2, 2);
	//this->attack_loadout[7]->set_my_caster(this);

	this->attack_loadout[8] = new TeleportAttack(0, 0, 0, 0, fire, stats, 100);
	this->attack_loadout[8]->set_my_caster(this);
	this->attack_loadout[8]->spell_id = TELEPORT;


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
	this->max_health = calculate_health(this->vitality);
	this->health = this->max_health;
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
		Attack_Sprite* animation = new Attack_Sprite("Resources//Attack Sprites//Charging.bmp", E, 5, 1, 4, 4, 296/4, 76);
		animation->set_state_frame_counts(0, 4, 0);
		AttackCharge* charge_amination = new AttackCharge(this->get_x_pos() - 17, this->get_y_pos() - 40, animation, this->casted_spell->get_charge_time());
		charge_amination->set_boundary_value(0, 0,0, animation->height);
		charge_amination->set_world(this->get_world());
		this->get_world()->insert_entity(charge_amination);
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

Combat* Combat::fetch_me_as_combat(void){
	return this;
}

void Combat::deal_with_attack(Attack* attack){

	int armor = this->armor;
	int adjusted_armor = armor - attack->penetration;
	if (adjusted_armor <= 0)
		adjusted_armor = 1;
	this->health -= (attack->base_damage*attack->get_my_caster()->intelligence)/adjusted_armor;
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
