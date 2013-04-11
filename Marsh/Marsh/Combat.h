#ifndef COMBAT_H
#define COMBAT_H 1
#define MAX_ATTACKS 10

#define BASE_VIT 0
#define BASE_INTEL 0
#define BASE_FOCUS 0
#define BASE_WILL 0
#define BASE_ARMOR 0
#define SKEW_FACTOR 2;

#include "Main.h"
using namespace std;

enum EntityType {Guard, Monster, Outcast, Hero, Rival};

class Combat: public iDrawable{
	protected:
		Attack* attack_loadout[MAX_ATTACKS];
		EntityType my_type;
		int health, mana, armor;
		int vitality, intelligence, focus, willpower; // combat stats
		bool casting;
		int casting_timer;
		Attack* casted_spell;
		
	
	public:
		Combat(int x, int y, int vel, int vel_d, Sprite* img);
		~Combat(void);
		void initiate_attack(Attack*);
		virtual void update(void);
		virtual void deal_with_attack(Attack* attack);
		void set_my_type(EntityType);
		void set_stats(int vitality, int intelligence, int focus, int willpower, int armor);

	protected:
		int calculate_health(int);
		int calculate_mana(int);
		void casting_update(void);
		void launch_attack(int);
		virtual void check_collisions(void);


};




#endif