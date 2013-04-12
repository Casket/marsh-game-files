#include "QuestManager.h"

using namespace std;

QuestManager::QuestManager(void){
	this->tracking_kills = new multimap<EntityType, Quest*>();
	this->tracking_interaction = new multimap<EntityType, Quest*>();
	this->tracking_items = new multimap<int, Quest*>();
}

QuestManager::~QuestManager(void){

}

void QuestManager::register_tracking_kill(EntityType, Quest*){

}

void QuestManager::register_tracking_interaction(EntityType, Quest*){

}

void QuestManager::register_tracking_items(int, Quest*){

}

void QuestManager::received_item(int){

}
void QuestManager::killed_entity(EntityType){

}
void QuestManager::interacted_with(EntityType){

}