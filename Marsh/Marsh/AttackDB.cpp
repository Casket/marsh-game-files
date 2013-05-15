#include "AttackDB.h"


AttackDB::AttackDB(void){
	this->attacks_by_id = new std::map<int, Attack*>();
	this->populate_damage_spells();
	this->populate_penetration_spells();
	this->populate_utility_spells();
	this->populate_melee();
}
AttackDB::~AttackDB(void){
	delete this->attacks_by_id;
}

AttackDB::AttackDB(int x){
	this->attacks_by_id = new std::map<int, Attack*>();
	this->populate_damage_spells();
	this->populate_penetration_spells();
	this->populate_utility_spells();
}

Attack* AttackDB::fetch_attack(int attack_id){
	std::map<int, Attack*>::iterator location;
	location = this->attacks_by_id->find(attack_id);
	if (location == this->attacks_by_id->end())
		return NULL;
	return (*location).second;
}

void AttackDB::populate_penetration_spells(void){
	int base_spell_speed = 2;
	int base_spell_v_delay = 5;
	int base_spell_damage = 15;
	int base_spell_penetration = 5;
	int base_charge_time = 20;
	int animation_delay = 5;
	int base_mana = 5;

	AttackStatistics stats_for_spell;
	
	//Shadow Needle
	stats_for_spell.base_damage = base_spell_damage;
	stats_for_spell.charge_time = base_charge_time + 5;
	stats_for_spell.exp_date = animation_delay * 0;
	stats_for_spell.penetration = base_spell_penetration;
	stats_for_spell.range = LONG_RANGE;
	stats_for_spell.tree_depth = 1;
	Attack_Sprite* needle = new Attack_Sprite("Resources//Attack Sprites//Energy_Needle.bmp", N, animation_delay, 1, 14, 14, 100, 26);
	needle->set_state_frame_counts(3, 5, 6);
	Attack* shadow_needle = new Attack(0, 0, base_spell_speed, base_spell_v_delay, needle, stats_for_spell);
	shadow_needle->set_boundary_value(14, 70, 2, 4);
	shadow_needle->set_mana_cost(base_mana);
	shadow_needle->spell_id = SHADOW_NEEDLE;
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_NEEDLE, shadow_needle));
	
	//Shadow Needle Plus
	stats_for_spell.base_damage = base_spell_damage + 10;
	stats_for_spell.charge_time = base_charge_time + 5;
	stats_for_spell.exp_date = animation_delay * 0;
	stats_for_spell.penetration = base_spell_penetration + 5;
	stats_for_spell.range = LONG_RANGE;
	stats_for_spell.tree_depth = 2;
	Attack* shadow_needle_plus = new Attack(0, 0, base_spell_speed, base_spell_v_delay, needle, stats_for_spell);
	shadow_needle_plus->set_boundary_value(14, 70, 2, 4);
	shadow_needle_plus->set_mana_cost(base_mana+5);
	shadow_needle_plus->spell_id = SHADOW_NEEDLE_PLUS;
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_NEEDLE_PLUS, shadow_needle_plus));

	//Shadow spikes
	stats_for_spell.base_damage = base_spell_damage + 10;
	stats_for_spell.charge_time = base_charge_time + 100;
	stats_for_spell.exp_date = 7*5;
	stats_for_spell.penetration = base_spell_penetration + 5;
	stats_for_spell.range = 0;
	stats_for_spell.tree_depth = 2;

	Attack_Sprite* spikes = new Attack_Sprite("Resources//Attack Sprites//Shadow_Spike.bmp", W, 4, 1, 7, 7, 147/7, 45);
	spikes->set_state_frame_counts(0, 7, 0);
	
	StationaryAttack* spike_attack = new StationaryAttack(0, 0, spikes, stats_for_spell);
	spike_attack->set_boundary_value(20, 20, 0, 23);

	Attack* shadow_spikes = new SpikeLauncher(0, 0, spikes, spike_attack,stats_for_spell, 5, 4);
	shadow_spikes->set_boundary_value(20, 23, 0, 0);
	shadow_spikes->set_mana_cost(base_mana + 10);
	shadow_spikes->spell_id = SHADOW_SPIKES;
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_SPIKES, shadow_spikes));

	//Death Beam
	Attack_Sprite* death = new Attack_Sprite("Resources//Attack Sprites//NEW_death_beam_attack.bmp", W, 10, 1, 6, 6, 32, 53);
	death->set_state_frame_counts(0, 3, 3);
	stats_for_spell.base_damage = base_spell_damage - 5;
	stats_for_spell.charge_time = 100;
	stats_for_spell.exp_date = 30;
	stats_for_spell.penetration = 15;
	stats_for_spell.range = MID_RANGE;
	stats_for_spell.tree_depth = 3;
	Attack* death_beam = new Attack(800, 800, 2, 10, death, stats_for_spell);
	death_beam->set_boundary_value(25, 34, 2, 11);
	death_beam->set_position_adjustment(0, 0);
	
	stats_for_spell.base_damage = 0;
	stats_for_spell.charge_time = 50;
	stats_for_spell.exp_date = 300;
	stats_for_spell.tree_depth = 3;
	Attack_Sprite* shooter_img = new Attack_Sprite("Resources//Attack Sprites//NEW_death_beam_charge.bmp", N, 5, 1, 3, 3, 55, 51);
	shooter_img->set_state_frame_counts(0, 3, 0);
	MultiSpawnAttack* beamer = new MultiSpawnAttack(800, 800, 2, 10, shooter_img, stats_for_spell, death_beam, 10);
	beamer->set_mana_cost(base_mana + 20);
	beamer->set_position_adjustment(50, 0);
	beamer->set_boundary_value(50, 34, 5, 10);
	beamer->spell_id = DEATH_BEAM;
	this->attacks_by_id->insert(std::pair<int, Attack*>(DEATH_BEAM, beamer));

}
void AttackDB::populate_utility_spells(void){
	AttackStatistics stats_for_spell;
	Attack_Sprite* sprite;
	Attack_Sprite* blank_sprite = new Attack_Sprite("Resources//Misc//blank_image.bmp", W, 5, 1, 1, 1, 10, 10);
	blank_sprite->set_state_frame_counts(0, 1, 0);
	int base_spell_speed = 2;
	int base_spell_v_delay = 5;
	int base_spell_damage = 15;
	int base_spell_penetration = 5;
	int base_charge_time = 20;
	int animation_delay = 5;
	int base_mana = 5;

	//Drain
	stats_for_spell.base_damage = base_spell_damage - 5;
	stats_for_spell.charge_time = 5 + 5;
	stats_for_spell.exp_date = 10;
	stats_for_spell.penetration = 5;
	stats_for_spell.range = MID_RANGE;
	stats_for_spell.tree_depth = 1;
	Attack_Sprite* drain = new Attack_Sprite("Resources//Attack Sprites//Drain.bmp", W, 5, 1, 5, 5, 24, 19);
	drain->set_state_frame_counts(0, 5, 0);
	Attack* shadow_drain = new HealthDrainAttack(0, 0, 2, 10, drain, stats_for_spell);
	shadow_drain->set_boundary_value(17, 25, 0, 0);
	shadow_drain->set_mana_cost(base_mana+5);
	shadow_drain->spell_id = DRAIN_HEALTH;
	this->attacks_by_id->insert(std::pair<int, Attack*>(DRAIN_HEALTH, shadow_drain));


	//Shadow Dash
	Attack* dash = new SprintSpell(15, 250, base_charge_time - 10);

	dash->set_mana_cost(base_mana);
	dash->spell_id = SHADOW_DASH;
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_DASH, dash));

	//Teleport
	Attack* teleport = new TeleportAttack(0, 0, 0, 0, blank_sprite, stats_for_spell, 1000);
	teleport->spell_id = TELEPORT;
	teleport->set_mana_cost(base_mana + 20);
	this->attacks_by_id->insert(std::pair<int, Attack*>(TELEPORT, teleport));

	//Shield
	Attack_Sprite* ward_img = new Attack_Sprite("Resources//Attack Sprites//ward-spaced.tga", W, 20, 1, 13, 13, 60, 65);
	ward_img->set_state_frame_counts(0, 13, 0);
	ward_img->is_translucent = true;
	Attack* shield = new ProtectionAttack(0, 0, 2, 10, ward_img, stats_for_spell, ShieldAttack);
	shield->set_boundary_value(20, 10, 15, 30);
	shield->set_position_adjustment(0,0);
	shield->set_mana_cost(base_mana+5);
	shield->spell_id = SHIELD;
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHIELD, shield));

	//Deflect
	Attack_Sprite* ward2_img = new Attack_Sprite("Resources//Attack Sprites//deflect_ward.tga", W, 20, 1, 13, 13, 60, 65);
	ward2_img->set_state_frame_counts(0, 13, 0);
	ward2_img->is_translucent = true;
	Attack* deflect = new ProtectionAttack(800, 800, 2, 10, ward2_img, stats_for_spell, ReflectAttack);
	deflect->set_boundary_value(20, 10, 15, 30);
	deflect->set_position_adjustment(0,0);
	deflect->set_mana_cost(base_mana+5);
	deflect->spell_id = DEFLECT;
	this->attacks_by_id->insert(std::pair<int, Attack*>(DEFLECT, deflect));

	//Bind
	Attack_Sprite* fire = new Attack_Sprite("Resources//Attack Sprites//bind_prefixed.tga", W, 10, 1, 8, 8, 45, 36);

	fire->is_translucent = true;
	fire->set_state_frame_counts(0, 1, 7);
	
	stats_for_spell.base_damage = 0;
	stats_for_spell.charge_time = 100;
	stats_for_spell.exp_date = 250;
	stats_for_spell.penetration = 15;
	stats_for_spell.range = MID_RANGE;
	stats_for_spell.tree_depth = 2;
	fire->is_translucent = true;
	Attack* bind = new StunningAttack(800, 800, 1, 10, fire, stats_for_spell);
	bind->set_boundary_value(45, 10, 0, 5);
	bind->set_mana_cost(base_mana);
	bind->set_position_adjustment(0, 20);
	bind->spell_id = BIND_IN_SHADOWS;
	this->attacks_by_id->insert(std::pair<int, Attack*>(BIND_IN_SHADOWS, bind));

	//Mass Slow
	stats_for_spell.base_damage = 0;
	stats_for_spell.charge_time = 0;
	stats_for_spell.exp_date = 1000;
	stats_for_spell.penetration = 15;
	stats_for_spell.range = 250;
	stats_for_spell.tree_depth = 2;
	Attack* slow = new StunningAttack(800, 800, 1, 10, blank_sprite, stats_for_spell);
	slow->set_boundary_value(35, 25, 10, 15);
	slow->set_position_adjustment(0, 20);
	slow->spell_id = MASS_SLOW;
	slow->set_mana_cost(base_mana+20);
	this->attacks_by_id->insert(std::pair<int, Attack*>(MASS_SLOW, slow));

	// target seeker
	stats_for_spell.base_damage = base_spell_damage;
	stats_for_spell.charge_time = base_charge_time+10;
	stats_for_spell.exp_date = 0;
	stats_for_spell.penetration = 20;
	stats_for_spell.range = LONG_RANGE;
	stats_for_spell.tree_depth = 5;
	Attack_Sprite* seeker_spr = drain;
	Attack* seeker = new TargetSeeker(0, 0, 1, 10, seeker_spr, stats_for_spell, NULL);
	seeker->spell_id = TARGET_SEEKER;
	seeker->set_mana_cost(15);
	seeker->set_boundary_value(20, 20, 0, 0);
	this->attacks_by_id->insert(std::pair<int, Attack*>(TARGET_SEEKER, seeker));

}

void AttackDB::populate_damage_spells(void){
	int base_spell_speed = 2;
	int base_spell_v_delay = 5;
	int base_spell_damage = 25;
	int base_spell_penetration = 0;
	int base_charge_time = 30;
	int animation_delay = 5;
	int base_mana = 5;

	AttackStatistics stats_for_spell;
	
	//Shadow Ball
	stats_for_spell.base_damage = base_spell_damage;
	stats_for_spell.charge_time = base_charge_time;
	stats_for_spell.exp_date = animation_delay * 0;
	stats_for_spell.penetration = base_spell_penetration;
	stats_for_spell.range = LONG_RANGE;
	stats_for_spell.tree_depth = 1;
	Attack_Sprite* ball = new Attack_Sprite("Resources//Attack Sprites//Energy_Bolt.bmp", N, animation_delay, 1, 4, 4, 188/4, 31);
	ball->set_state_frame_counts(0, 4, 0);
	Attack* shadow_ball = new Attack(0, 0, base_spell_speed, base_spell_v_delay, ball, stats_for_spell);
	shadow_ball->set_boundary_value(34, 18, 5, 8);
	shadow_ball->set_mana_cost(base_mana);
	shadow_ball->spell_id = SHADOW_BALL;
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_BALL, shadow_ball));

	//Shadow Ball Plus
	stats_for_spell.base_damage = base_spell_damage + 10;
	stats_for_spell.charge_time = base_charge_time;
	stats_for_spell.exp_date = animation_delay * 0;
	stats_for_spell.penetration = base_spell_penetration;
	stats_for_spell.range = LONG_RANGE;
	stats_for_spell.tree_depth = 2;
	ball->set_state_frame_counts(0, 4, 0);
	Attack* shadow_ball_plus = new Attack(0, 0, base_spell_speed, base_spell_v_delay, ball, stats_for_spell);
	shadow_ball_plus->set_boundary_value(34, 18, 5, 8);
	shadow_ball_plus->set_mana_cost(base_mana+5);
	shadow_ball_plus->spell_id = SHADOW_BALL_PLUS;
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_BALL_PLUS, shadow_ball_plus));
		
	//Shadow Wave
	stats_for_spell.base_damage = base_spell_damage;
	stats_for_spell.charge_time = base_charge_time - 10;
	stats_for_spell.exp_date = animation_delay * 0;
	stats_for_spell.penetration = base_spell_penetration;
	stats_for_spell.range = LONG_RANGE;
	stats_for_spell.tree_depth = 2;
	Attack_Sprite* wave = new Attack_Sprite("Resources//Attack Sprites//Energy_Wave.bmp", N, animation_delay, 1, 4, 15, 900/15, 47);
	wave->set_state_frame_counts(0, 15, 0);
	Attack* energy_wave = new PersistentAttack(0, 0, base_spell_speed, base_spell_v_delay, wave, stats_for_spell);
	energy_wave->set_mana_cost(base_mana+10);
	energy_wave->set_boundary_value(900/15, 35, 0, 12);
	energy_wave->spell_id = SHADOW_WAVE;
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_WAVE, energy_wave));
	
	//Shadow Nova
	Attack_Sprite* fire = new Attack_Sprite("Resources//Attack Sprites//Nova.tga", W, 10, 1, 11, 11, 580/11, 48);

	fire->is_translucent = true;
	fire->set_state_frame_counts(0, 1, 10);
	
	stats_for_spell.base_damage = 0;
	stats_for_spell.charge_time = 100;
	stats_for_spell.exp_date = 1000;
	stats_for_spell.penetration = 15;
	stats_for_spell.range = 250;
	stats_for_spell.tree_depth = 2;
	fire->is_translucent = true;
	Attack* nova = new StunningAttack(800, 800, 1, 10, fire, stats_for_spell);
	nova->set_boundary_value(35, 25, 10, 15);
	nova->set_position_adjustment(0, 20);
	nova->spell_id = SHADOW_NOVA;
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_NOVA, nova));
}
void AttackDB::populate_melee(){

	int base_spell_speed = 2;
	int base_spell_v_delay = 5;
	int base_spell_damage = 25;
	int base_spell_penetration = 0;
	int base_charge_time = 30;
	int animation_delay = 5;
	int base_mana = 0;
		
	AttackStatistics stats_for_spell;

	//monster melee
	stats_for_spell.base_damage = base_spell_damage;
	stats_for_spell.charge_time = base_charge_time - 10;
	stats_for_spell.exp_date = animation_delay;
	stats_for_spell.penetration = base_spell_penetration;
	stats_for_spell.range = NO_RANGE;
	stats_for_spell.tree_depth = 0;
	Attack_Sprite* slash = new Attack_Sprite("Resources//Attack Sprites//monster_melee.tga", N, 3, 1, 4,7,315/7, 58);
	slash->is_translucent = true;
	slash->set_state_frame_counts(0,7,0);
	Attack* monster_melee = new Attack(0,0,base_spell_speed, base_spell_v_delay, slash, stats_for_spell);
	monster_melee->set_mana_cost(base_mana);
	monster_melee->set_position_adjustment(0, 40);
	monster_melee->set_boundary_value(45, 59, 0, 0);
	this->attacks_by_id->insert(std::pair<int, Attack*>(MONSTER_MELEE, monster_melee));


	//guard melee
	stats_for_spell.base_damage = base_spell_damage;
	stats_for_spell.charge_time = base_charge_time - 10;
	stats_for_spell.exp_date = animation_delay;
	stats_for_spell.penetration = base_spell_penetration;
	stats_for_spell.range = NO_RANGE;
	stats_for_spell.tree_depth = 0;
	Attack_Sprite* gslash = new Attack_Sprite("Resources//Attack Sprites//guard_melee_trim.tga", N, 3, 1, 4,4,40, 32);
	gslash->is_translucent = true;
	gslash->set_state_frame_counts(0,4,0);
	Attack* guard_melee = new Attack(0,0,base_spell_speed, base_spell_v_delay, gslash, stats_for_spell);
	guard_melee->set_mana_cost(base_mana);
	guard_melee->set_position_adjustment(-30, 0);
	guard_melee->set_boundary_value(36, 26, 0, 5);
	this->attacks_by_id->insert(std::pair<int, Attack*>(GUARD_MELEE, guard_melee));

		//guard melee
	/*stats_for_spell.base_damage = base_spell_damage - 20;
	stats_for_spell.charge_time = base_charge_time - 10;
	stats_for_spell.exp_date = animation_delay;
	stats_for_spell.penetration = base_spell_penetration;
	stats_for_spell.range = NO_RANGE;
	stats_for_spell.tree_depth = 0;
	Attack_Sprite* gslash = new Attack_Sprite("Resources//Attack Sprites//guard_melee.tga", N, 3, 1, 4,4,252/4, 32);
	gslash->is_translucent = true;
	gslash->set_state_frame_counts(0,4,0);
	Attack* guard_melee = new Attack(0,0,base_spell_speed, base_spell_v_delay, gslash, stats_for_spell);
	monster_melee->set_mana_cost(base_mana);
	guard_melee->set_boundary_value(252/4, 32, 0,0);
	this->attacks_by_id->insert(std::pair<int, Attack*>(GUARD_MELEE, guard_melee));*/
}