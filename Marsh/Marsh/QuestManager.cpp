#include "QuestManager.h"

using namespace std;

QuestManager::QuestManager(void){
	this->tracking_kills = new multimap<EntityType, Quest*>();
	this->tracking_interaction = new multimap<EntityType, Quest*>();
	this->tracking_items = new multimap<int, Quest*>();
}

QuestManager::~QuestManager(void){

}

void QuestManager::register_tracking_kill(EntityType et, Quest* q){

}

void QuestManager::register_tracking_interaction(EntityType et, Quest* q){

}

void QuestManager::register_tracking_items(int id, Quest* q){

}

void QuestManager::received_item(int id){

}
void QuestManager::killed_entity(EntityType et){

}
void QuestManager::interacted_with(EntityType et){

}