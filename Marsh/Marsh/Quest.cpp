#include "Quest.h"

using namespace std;

Quest::Quest(QuestDescription desc, IQuestObjective obj){
	this->description = desc;
	this->objectives = new std::list<IQuestObjective>();
	this->objectives->push_back(obj);
	this->rewards = new std::list<QuestReward>();
	this->current_objective = 0;
	this->total_objectives = 1;
}

Quest::~Quest(void){
	delete this->objectives;
}

void Quest::add_reward(QuestReward reward){
	this->rewards->push_back(reward);
}

void Quest::add_objective(IQuestObjective obj){
	this->objectives->push_back(obj);
	this->total_objectives++;
}

void Quest::begin_quest(void){

}

void Quest::end_quest(void){

}

bool Quest::mark_progress(void){
	IQuestObjective obj = this->objectives->front();
	bool flag = obj.mark_progress();
	if (flag){
		this->objectives->pop_front();
		if (this->objectives->empty()){
			// quest is over, reward the player
		} else
			this->objectives->front().register_objective(this);
	}

	return flag;
}