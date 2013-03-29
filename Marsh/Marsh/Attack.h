#ifndef ATTACK_H
#define ATTACK_H 1

#include "iDrawable.h"
#include "Main.h"

class Combat;

using namespace std;

enum AttackType {Melee, Fire, Indirect, Energy, Defensive};

class Attack: public iDrawable {
public:
	AttackType spell_type;
	int base_damage, penetration;
	int range, distance_traveled;
	int tree_depth_level;
	int death_timer, expiration_date;
	int charge_time;
	Combat* my_caster;

	public:
		Attack(int x, int y, int vel, int vel_d, Sprite* img, int base_damage, int penetration, int range, int tree_depth, int exp_date, int charge_time);
		~Attack(void);
		virtual void update(void);
		virtual void deal_with_attack(Attack*);
		Attack* clone(int, int, int, int, Direction);
		bool detect_collisions(void);
		void set_my_caster(Combat*);
		Combat* get_my_caster(void);
		int get_charge_time(void);

	private:
		// super private methods go here
		bool detect_hit(int my_x, int my_y, int my_height, int my_width, int check_x, int check_y, int check_width, int check_height);
		void start_death_sequence(void);
		

};


#endif