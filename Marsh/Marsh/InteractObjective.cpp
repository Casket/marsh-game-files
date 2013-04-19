#include "Main.h"
#include "InteractObjective.h"

using namespace std;

InteractObjective::InteractObjective(EntityType object, int total)
:IQuestObjective()
{
	this->desired_interaction = object;
	this->needed_interaction_count = total;
	this->current_interaction_count = 0;
}
InteractObjective::~InteractObjective(void){

}

bool InteractObjective::mark_progress(void){
	if (++this->current_interaction_count == this->needed_interaction_count)
		return true;
	else
		return false;
}

bool InteractObjective::complete(void){
	return this->current_interaction_count == this->needed_interaction_count;
}

void InteractObjective::register_objective(Quest* quest){
	Player* target = Player_Accessor::hero;
	QuestManager* qm = target->quest_manager;
	qm->register_tracking_interaction(this->desired_interaction, quest);
}