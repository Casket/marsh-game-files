#ifndef ATTACK_H
#define ATTACK_H 1

#include "Drawable.h"
#include "Main.h"


using namespace std;

enum AttackType {Melee, Fire, Indirect, Energy, Defensive};

class Attack: public iDrawable {
	AttackType spell_type;
	int base_damage, penetration;
	int range, distance_traveled;
	int tree_depth_level;
	int death_timer, expiration_date;
	int charge_time;

	public:
		Attack(int x, int y, int vel, int vel_d, Sprite* img, int base_damage, int penetration, int range, int tree_depth, int exp_date, int charge_time);
		~Attack(void);
		virtual void update(void);
		virtual void deal_with_attack(Attack*);
		Attack* clone(int, int, int, int);

	private:
		// super private methods go here
		

};


#endif