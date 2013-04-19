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
	delete this->tracking_interaction;
	delete this->tracking_items;
	delete this->tracking_kills;
	delete this->item_queue;
	delete this->kill_queue;
	delete this->interaction_queue;
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

	this->flush_queues();
}

void QuestManager::killed_entity(EntityType et){
	std::pair<std::multimap<EntityType, Quest*>::iterator, std::multimap<EntityType, Quest*>::iterator> range;
	range = this->tracking_kills->equal_range(et);
	std::list<std::pair<EntityType, Quest*>> delete_ls;

	for (std::multimap<EntityType, Quest*>::iterator it=range.first; it!=range.second; ++it){
		Quest* interested_quest = it->second;
		bool flag = interested_quest->mark_progress();
		if (flag)
			delete_ls.push_back(*it);
	}

	for (std::list<std::pair<EntityType, Quest*>>::iterator it = delete_ls.begin(); it!= delete_ls.end(); ++it)
	{
		this->tracking_kills->erase(it->first);
	}

	this->flush_queues();
}

void QuestManager::interacted_with(EntityType et){
	std::pair<std::multimap<EntityType, Quest*>::iterator, std::multimap<EntityType, Quest*>::iterator> range;
	range = this->tracking_interaction->equal_range(et);

	for (std::multimap<EntityType, Quest*>::iterator it=range.first; it!=range.second; ++it){
		Quest* interested_quest = it->second;
		bool flag = interested_quest->mark_progress();
		if (flag)
			this->tracking_interaction->erase(it);
	}

	this->flush_queues();
}

void QuestManager::flush_queues(void){
	while(!this->item_queue->empty()){
		this->tracking_items->insert(this->item_queue->front());
		this->item_queue->pop();
	}

	while(!this->interaction_queue->empty()){
		this->tracking_interaction->insert(this->interaction_queue->front());
		this->interaction_queue->pop();
	}

	while(!this->kill_queue->empty()){
		this->tracking_kills->insert(this->kill_queue->front());
		this->kill_queue->pop();
	}
}