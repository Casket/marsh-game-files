#ifndef RETRIEVE_OBJECTIVE_H
#define RETRIEVE_OBJECTIVE_H 1

#include "Main.h"
#include "IQuestObjective.h"

using namespace std;

class IQuestObjective;

class RetrieveObjective: public IQuestObjective{
public:
	int desired_item_id;
	int number_needed, number_has;

	RetrieveObjective(int item_id, int needs);
	~RetrieveObjective(void);
	virtual void register_objective(Quest*);
	virtual bool mark_progress(void);
	virtual bool complete(void);
};

#endif