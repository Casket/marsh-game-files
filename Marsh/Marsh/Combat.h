#ifndef COMBAT_H
#define COMBAT_H 1
#define MAX_ATTACKS 10

#define BASE_VIT 400
#define BASE_INTEL 1
#define BASE_FOCUS 1
#define BASE_WILL 1
#define BASE_ARMOR 1
#define SKEW_FACTOR 1;

#include "Main.h"
using namespace std;

class Dispatcher;


class Combat: public iDrawable{
public:
	bool player_credit;
	int experience_worth;
	std::list<Dispatcher*>* active_dispatchers;

	//	protected:
	Attack* attack_loadout[MAX_ATTACKS];
	int health, armor, max_health;
	int vitality, intelligence, focus, willpower; // combat stats
	bool casting;
	bool entangled;
	int casting_timer;
	Attack* casted_spell;
	std::vector<std::string>* dialogue;
	int current_dialogue;
	bool should_free_player;
	bool has_player_hostage;
	std::list<Attack*>* active_wards;


public:
	Combat(int x, int y, int vel, int vel_d, Sprite* img);
	~Combat(void);
	void initiate_attack(Attack*);
	virtual void update(void);
	virtual void deal_with_attack(Attack* attack);
	void set_my_type(EntityType);
	virtual void set_stats(int vitality, int intelligence, int focus, int willpower, int armor);
	EntityType Combat::get_my_type(void);
	int get_current_health(void);
	int get_max_health(void);
	virtual void append_dialogue(std::string);
	virtual void clear_dialogue(void);
	virtual void speak(void);
	void face_speaker(void);
	virtual Combat* fetch_me_as_combat(void);
	virtual void upon_death(void);
	virtual bool check_new_pos(int x, int y);
	void update_dispatchers(void);
	virtual void apply_ward(Attack* ward);
	void testing_attacks(void);
	void insert_db_attacks(void);
	void insert_utility_spells(int);
	void insert_damage_attacks(int);

protected:
	int calculate_health(int);
	void casting_update(void);
	void launch_attack(int);
	virtual void check_collisions(void);



};




#endif