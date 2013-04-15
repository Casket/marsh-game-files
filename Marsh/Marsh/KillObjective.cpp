#include "Main.h"
#include "KillObjective.h"

using namespace std;

KillObjective::KillObjective(EntityType enemy, int total)
:IQuestObjective()
{
	this->desired_enemy = enemy;
	this->needed_kill_count = total;
	this->current_kill_count = 0;
}
KillObjective::~KillObjective(void){

}

bool KillObjective::mark_progress(void){
	if (++this->current_kill_count == this->needed_kill_count)
		return true;
	else
		return false;
}

bool KillObjective::complete(void){
	return this->current_kill_count == this->needed_kill_count;
}

void KillObjective::register_objective(Quest* quest){
	Player* target = Player_Accessor::hero;
	QuestManager* qm = target->quest_manager;
	qm->register_tracking_kill(this->desired_enemy, quest);
}