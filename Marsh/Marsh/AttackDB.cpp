#include "AttackDB.h"

using namespace std;



AttackDB::AttackDB(void){
	this->attacks_by_id = new std::map<int, Attack*>();
	this->populate_damage_spells();
	this->populate_penetration_spells();
	this->populate_utility_spells();
}
AttackDB::~AttackDB(void){
	delete this->attacks_by_id;
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
	int base_charge_time = 30;
	int animation_delay = 5;

	AttackStatistics stats_for_spell;

	stats_for_spell.base_damage = base_spell_damage;
	stats_for_spell.charge_time = 25;
	stats_for_spell.exp_date = animation_delay * 6;
	stats_for_spell.penetration = base_spell_penetration;
	stats_for_spell.range = LONG_RANGE;
	stats_for_spell.tree_depth = 1;
	Attack_Sprite* needle = new Attack_Sprite("Resources//Attack Sprites//Shadow_Needle_5.bmp", N, animation_delay, 1, 14, 14, 100, 26);
	needle->set_state_frame_counts(3, 5, 6);
	Attack* shadow_needle = new Attack(0, 0, base_spell_speed, base_spell_v_delay, needle, stats_for_spell);
	shadow_needle->set_boundary_value(34, 17, 2, 4);
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_NEEDLE, shadow_needle));

	stats_for_spell.base_damage = base_spell_damage + 10;
	stats_for_spell.charge_time = base_charge_time;
	stats_for_spell.exp_date = animation_delay * 6;
	stats_for_spell.penetration = base_spell_penetration + 5;
	stats_for_spell.range = LONG_RANGE;
	stats_for_spell.tree_depth = 2;
	Attack* shadow_needle_plus = new Attack(0, 0, base_spell_speed, base_spell_v_delay, needle, stats_for_spell);
	shadow_needle_plus->set_boundary_value(34, 17, 2, 4);
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_NEEDLE_PLUS, shadow_needle_plus));

	Attack_Sprite* spikes = new Attack_Sprite("Resources//Attack Sprites//Shadow_Spike.bmp", N, animation_delay, 1, 7, 7, 147/7, 45);
	spikes->set_state_frame_counts(0, 7, 0);
	stats_for_spell.base_damage = base_spell_damage;
	stats_for_spell.charge_time = base_charge_time;
	stats_for_spell.exp_date = animation_delay * 7;
	stats_for_spell.penetration = base_spell_penetration + 5;
	stats_for_spell.range = SHORT_RANGE;
	stats_for_spell.tree_depth = 2;
	Attack* shadow_spikes = new Attack(0, 0, base_spell_speed, base_spell_v_delay, spikes, stats_for_spell);
	shadow_spikes->set_boundary_value(20, 23, 0, 0);
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_SPIKES, shadow_spikes));


}
void AttackDB::populate_utility_spells(void){
}

void AttackDB::populate_damage_spells(void){
	int base_spell_speed = 2;
	int base_spell_v_delay = 5;
	int base_spell_damage = 25;
	int base_spell_penetration = 0;
	int base_charge_time = 30;
	int animation_delay = 5;

	AttackStatistics stats_for_spell;

	stats_for_spell.base_damage = base_spell_damage;
	stats_for_spell.charge_time = base_charge_time;
	stats_for_spell.exp_date = animation_delay * 6;
	stats_for_spell.penetration = base_spell_penetration;
	stats_for_spell.range = LONG_RANGE;
	stats_for_spell.tree_depth = 1;
	Attack_Sprite* ball = new Attack_Sprite("Resources//Attack Sprites//Energy_Bolt.bmp", N, animation_delay, 1, 4, 14, 188/4, 31);
	ball->set_state_frame_counts(0, 4, 0);
	Attack* shadow_ball = new Attack(0, 0, base_spell_speed, base_spell_v_delay, ball, stats_for_spell);
	shadow_ball->set_boundary_value(34, 18, 5, 8);
	this->attacks_by_id->insert(std::pair<int, Attack*>(SHADOW_BALL, shadow_ball));



}