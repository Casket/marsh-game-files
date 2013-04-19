#ifndef INTERACT_OBJECTIVE_H
#define INTERACT_OBJECTIVE_H 1

#include "Main.h"
#include "IQuestObjective.h"

class InteractObjective: public IQuestObjective{
public:
	EntityType desired_interaction;
	int current_interaction_count;
	int needed_interaction_count;

	InteractObjective(EntityType object, int total);
	~InteractObjective(void);
	virtual void register_objective(Quest*);
	virtual bool mark_progress(void);
	virtual bool complete(void);

};

#endif