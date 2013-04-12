#ifndef KILL_OBJECTIVE_H
#define KILL_OBJECTIVE_H 1

#include "Main.h"
#include "IQuestObjective.h"

class KillObjective: public IQuestObjective{
public:
	EntityType desired_enemy;
	int current_kill_count;
	int needed_kill_count;

	KillObjective(EntityType enemy, int total);
	~KillObjective(void);
	virtual void register_objective(void);
	virtual bool mark_progress(void);
	virtual bool complete(void);

};

#endif