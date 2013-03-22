#ifndef COMBAT_H
#define COMBAT_H 1
#define MAX_ATTACKS 10

#define BASE_VIT 0
#define BASE_INTEL 0
#define BASE_FOCUS 0
#define BASE_WILL 0
#define BASE_ARMOR 0

#include "Main.h"
using namespace std;

enum EntityType {Guard, Monster, Outcast, Hero, Rival};

class Combat: public Drawable{
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
		void update(void);
		void set_my_type(EntityType);

	private:
		int calculate_health(int);
		int calculate_mana(int);

	protected:
		void casting_update(void);
		void launch_attack(int);

};




#endif