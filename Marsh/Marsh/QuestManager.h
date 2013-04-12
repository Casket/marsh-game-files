#ifndef QUEST_MANAGER_H
#define QUEST_MANAGER_H 1
#include "Main.h"
#include <map>

class Quest;

using namespace std;

class QuestManager{
protected:
	std::multimap<EntityType, Quest*>* tracking_kills;
	std::multimap<EntityType, Quest*>* tracking_interaction;
	std::multimap<int, Quest*>* tracking_items;

public:
	QuestManager(void);
	~QuestManager(void);

	void register_tracking_kill(EntityType, Quest*);
	void register_tracking_interaction(EntityType, Quest*);
	void register_tracking_items(int, Quest*);

	void received_item(int);
	void killed_entity(EntityType);
	void interacted_with(EntityType);
};

#endif