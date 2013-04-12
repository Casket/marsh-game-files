#include "RetrieveObjective.h"

using namespace std;

RetrieveObjective::RetrieveObjective(int item_id, int needs)
:IQuestObjective()
{
	this->desired_item_id = item_id;
	this->number_needed = needs;
	this->number_has = 0;
}

RetrieveObjective::~RetrieveObjective(void){

}

bool RetrieveObjective::mark_progress(void){
	if (++this->number_has == this->number_needed)
		return true;
	return false;;
}

bool RetrieveObjective::complete(void){
	return this->number_has == this->number_needed;
}