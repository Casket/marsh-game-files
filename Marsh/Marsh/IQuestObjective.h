#ifndef IQUESTOBJECTIVE_H
#define IQUESTOBJECTIVE_H 1

#include "Main.h"
using namespace std;

class IQuestObjective{
public:
	IQuestObjective(void);
	~IQuestObjective(void);
	virtual void register_objective(void);
	virtual bool mark_progress(void);
	virtual bool complete(void);

};


#endif