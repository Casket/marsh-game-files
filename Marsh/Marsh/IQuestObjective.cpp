#include "Main.h"
#include "IQuestObjective.h"

using namespace std;

IQuestObjective::IQuestObjective(void){

}

IQuestObjective::~IQuestObjective(void){

}

bool IQuestObjective::mark_progress(void){return true;}
bool IQuestObjective::complete(void){return true;}
void IQuestObjective::register_objective(Quest* q){};