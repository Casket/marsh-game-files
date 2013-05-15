#ifndef QUEST_H
#define QUEST_H 1

#define INITIAL_OBJECTIVE_COUNT 2

#include "Main.h"
using namespace std;

typedef struct QuestReward{
	int gold;
	int rep;
	int exp;
	int item;
	int mission;

}QuestReward;

typedef struct QuestDescription{
	std::string text;
}QuestDescription;

class Quest{
public:
	std::list<QuestReward>* rewards;
	std::list<IQuestObjective*>* objectives;
	QuestDescription description;
	int current_objective;
	int total_objectives;

	Quest(QuestDescription desc, IQuestObjective* obj);
	~Quest(void);
	void add_reward(QuestReward reward);
	void add_objective(IQuestObjective* obj);
	void begin_quest(void);
	void end_quest(void);
	bool mark_progress(void);

};


#endif