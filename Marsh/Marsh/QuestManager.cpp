#include "QuestManager.h"
#include "Main.h"

class Quest;

using namespace std;

QuestManager::QuestManager(void){
	this->tracking_kills = new multimap<EntityType, Quest*>();
	this->tracking_interaction = new multimap<EntityType, Quest*>();
	this->tracking_items = new multimap<int, Quest*>();

	this->kill_queue = new std::queue<std::pair<EntityType, Quest*>>();
	this->interaction_queue = new std::queue<std::pair<EntityType, Quest*>>();
	this->item_queue = new std::queue<std::pair<int, Quest*>>();

}

QuestManager::~QuestManager(void){

}

void QuestManager::register_tracking_kill(EntityType et, Quest* q){
	this->kill_queue->push(std::pair<EntityType, Quest*>(et, q));
}

void QuestManager::register_tracking_interaction(EntityType et, Quest* q){
	this->interaction_queue->push(std::pair<EntityType, Quest*>(et, q));
}

void QuestManager::register_tracking_items(int id, Quest* q){
	this->item_queue->push(std::pair<int, Quest*>(id, q));
}

void QuestManager::received_item(int id){
	std::pair<std::multimap<int, Quest*>::iterator, std::multimap<int, Quest*>::iterator> range;
	range = this->tracking_items->equal_range(id);

	for (std::multimap<int, Quest*>::iterator it=range.first; it!=range.second; ++it){
		Quest* interested_quest = it->second;
		bool flag = interested_quest->mark_progress();
		if (flag)
			this->tracking_items->erase(it);
	}



}
void QuestManager::killed_entity(EntityType et){

}
void QuestManager::interacted_with(EntityType et){

}