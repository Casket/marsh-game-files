#ifndef ATTACK_H
#define ATTACK_H 1

#include "Drawable.h"
#include "Main.h"


using namespace std;

enum Type {Melee, Fire, Indirect, Energy, Defensive};

class Attack: public iDrawable {
	Type spell_type;
	int base_damage, penetration;
	int range, distance_traveled;
	int tree_depth_level;
	int death_timer, expiration_date;
	int charge_time;

	public:
		Attack(int x, int y, int vel, int vel_d, Sprite* img);
		~Attack(void);
		virtual void update(void);
		virtual void deal_with_attack(Attack*);
		Attack* clone(void);

	private:
		// super private methods go here
		

};


#endif