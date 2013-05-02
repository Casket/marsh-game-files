#include "Main.h"

using namespace std;

	


	MultiSpawnAtack::MultiSpawnAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats, Attack* to_duplicate){
		spawn_count;
	spawn_delay;
	delay_counter;
	distance_outward;
	distance_between_spawns;
	std::list<Direction> spawn_directions;
	attack_spawning;
	}
	~MultiSpawnAttack(void);
	virtual void update(void);
	virtual void start_death_sequence(void);
	virtual void deal_with_attack(Attack*);
	virtual Attack* clone(int, int, Direction);