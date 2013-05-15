#include "Quest.h"

using namespace std;

Quest::Quest(QuestDescription desc, IQuestObjective* obj){
	this->description = desc;
	this->objectives = new std::list<IQuestObjective*>();
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

void Quest::add_objective(IQuestObjective* obj){
	this->objectives->push_back(obj);
	this->total_objectives++;
}

void Quest::begin_quest(void){
	this->objectives->front()->register_objective(this);
	Player_Accessor::get_player()->display_to_user("You've started your new quest.\n" + this->description.text);
}

void Quest::end_quest(void){
	Player* hero = Player_Accessor::get_player();
	hero->display_to_user("Congratulations, you've completed your quest of: \n" + this->description.text);
	std::list<QuestReward>::iterator iter;
	std::list<QuestReward>::iterator end = this->rewards->end();
	for (iter = this->rewards->begin(); iter != end; ++iter){
		hero->gold += (iter)->gold;
		hero->grant_experience((iter)->exp);
		hero->notoriety += (iter)->rep;
		hero->add_to_inventory(itemDB->fetch_item((iter)->item));
		if((iter)->mission == 1)
			hero->new_mission = true;
	}
}

bool Quest::mark_progress(void){
	IQuestObjective* obj = this->objectives->front();
	bool flag = obj->mark_progress();
	if (flag){
		this->objectives->pop_front();
		if (this->objectives->empty()){
			end_quest();
			// quest is over, reward the player
		} else
			this->objectives->front()->register_objective(this);
	}

	return flag;
}