#include "Main.h"

using namespace std;




MultiSpawnAttack::MultiSpawnAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats, Attack* to_duplicate)
:Attack(x, y, vel, vel_d, img, stats)
{
	
}
MultiSpawnAttack::~MultiSpawnAttack(void){

}
void MultiSpawnAttack::update(void){

}
void MultiSpawnAttack::start_death_sequence(void){

}
void MultiSpawnAttack::deal_with_attack(Attack*){

}
Attack* MultiSpawnAttack::clone(int, int, Direction){
	return NULL;

}