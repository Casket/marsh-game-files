#include "Quest.h"

using namespace std;

Quest::Quest(QuestDescription desc, IQuestObjective obj){
	this->description = desc;
	this->objectives = new std::vector<IQuestObjective>();
	this->objectives->push_back(obj);
	this->rewards = new std::list<QuestReward>();
	this->current_objective = 0;
	this->total_objectives = 1;
}

Quest::~Quest(void){

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
	return true;
}