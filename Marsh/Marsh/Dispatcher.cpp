#include "Main.h"

using namespace std;

bool sort_east(Mob* m1, Mob* m2);
bool sort_north(Mob* m1, Mob* m2);

Dispatcher::Dispatcher(iDrawable* target, EntityType designated_type){

	this->designated_target = target;
	this->designated_type = designated_type;
	this->underlings = new std::list<Mob*>();
	this->north_front = std::make_pair(new std::list<Mob*>(), true);
	this->south_front = std::make_pair(new std::list<Mob*>(), true);
	this->east_front = std::make_pair(new std::list<Mob*>(), true);
	this->west_front = std::make_pair(new std::list<Mob*>(), true);
	this->active = true;

}
Dispatcher::~Dispatcher(void){

}

void Dispatcher::update(void){
	this->locate_followers();
}

void Dispatcher::locate_followers(void){
	
	std::list<iDrawable*>::iterator iter = this->designated_target->get_world()->get_active_entities()->begin();
	std::list<iDrawable*>::iterator end = this->designated_target->get_world()->get_active_entities()->end();
	int prev_follower_count = this->underlings->size();
	this->underlings->clear();
		
	for(;iter != end;iter++){
		Mob* check = (*iter)->fetch_me_as_mob();
		if (check == NULL)
			continue;
		if(check->get_my_type() == this->designated_type && check->target == this->designated_target){
			this->underlings->push_back(check);
			check->set_attack_plan();
		}else{
			//
		}
	}

	if (this->underlings->empty()){
		this->active = false;
		return;
	}

	if (prev_follower_count != this->underlings->size()){
		// you had a gain or a loss... recalculate, rearm, fight harder
		this->set_directives();
	}

	int i=0;
	std::list<Mob*>::iterator end3 = this->underlings->end();
	std::list<std::pair<Direction, int>> directions;
	for (std::list<Mob*>::iterator iter = this->underlings->begin(); iter != end3; ++iter){
		directions.push_back(std::pair<Direction, int>((*iter)->attack_dir, i++));
	}

	std::list<std::pair<Direction, int>>::iterator end2 = directions.end();
	std::string message;
	for (std::list<std::pair<Direction, int>>::iterator iter = directions.begin(); iter != end2; ++iter){
		message.append("guard wants to go ");
		switch ((*iter).first){
			case N:
				message.append("north");
				break;
			case S:
				message.append("south");
				break;
			case E:
				message.append("east");
				break;
			case W:
				message.append("west");
				break;
		}
		message.append("  ");
	}
	Player_Accessor::get_player()->display_to_user(message);
}

void Dispatcher::set_directives(void){
	std::list<Mob*>::iterator iter = this->underlings->begin();
	std::list<Mob*>::iterator end = this->underlings->end();
	
	for(;iter != end; iter++){
		Mob* cur_under = (*iter);
		Direction first = cur_under->priorities->front();
		cur_under->priorities->pop_front();

		switch (first){
			case N:
				if(this->north_front.second){
					this->north_front.first->push_back(cur_under);
				}else{
					//	
				}
				break;
			case S:
				if(this->south_front.second){
					this->south_front.first->push_back(cur_under);
				}else{
					//
				}
				break;
			case E:
				if(this->east_front.second){
					this->east_front.first->push_back(cur_under);
				}else{
					//
				}
				break;
			case W:
				if(this->west_front.second){
					this->west_front.first->push_back(cur_under);
				}else{
					//
				}
				break;
		}
	}
	// all the lists are sorted and ready for extraction
	this->designate_directions(0);
}

void Dispatcher::disassemble_queue(std::list<Mob*>* queue){
	std::list<Mob*>::iterator end = queue->end();
	for (std::list<Mob*>::iterator iter = queue->begin(); iter != end; ++iter){
		if ((*iter)->priorities->empty())
			continue;
		switch((*iter)->priorities->front()){
			case N:
				if (this->north_front.second) this->north_front.first->push_back((*iter));
				break;
			case S:
				if (this->south_front.second) this->south_front.first->push_back((*iter));
				break;
			case W:
				if (this->west_front.second) this->west_front.first->push_back((*iter));
				break;
			case E:
				if (this->east_front.second) this->east_front.first->push_back((*iter));
				break;
		}
		(*iter)->priorities->pop_front();
	}
	queue->clear();
}

void Dispatcher::designate_directions(int call_count){
	// use the sorted queues to assign directions
	// start off w/ the north one, for no particular reason
	
	this->north_front.first->sort(sort_north);
	if (!this->north_front.first->empty()){
		Mob* norther = this->north_front.first->front();
		this->north_front.first->pop_front();
		this->north_front.second = false;
		// assign norther to attack from the north
		norther->attack_dir = N;
		disassemble_queue(this->north_front.first);
	} else {
		// no entities wanted to attack north... yet...
	}

	this->south_front.first->sort(sort_north);
	if (!this->south_front.first->empty()){
		Mob* souther = this->south_front.first->front();
		this->south_front.first->pop_front();
		this->south_front.second = false;
		souther->attack_dir = S;
		disassemble_queue(this->south_front.first);
	} else {
	}

	this->east_front.first->sort(sort_east);
	if (!this->east_front.first->empty()){
		//becasue easter
		Mob* zombie_jesus = this->east_front.first->front();
		this->east_front.first->pop_front();
		this->east_front.second = false;
		zombie_jesus->attack_dir = E;
		disassemble_queue(this->east_front.first);
	} else {
	}

	this->west_front.first->sort(sort_east);
	if (!this->west_front.first->empty()){
		Mob* wester = this->west_front.first->front();
		this->west_front.first->pop_front();
		this->west_front.second = false;//should this be east.
		wester->attack_dir = W;
		disassemble_queue(this->west_front.first);
	} else {
	}

	if (call_count < 4){
		this->designate_directions(call_count+1);
	}
}

bool sort_north(Mob* m1, Mob* m2){
	int axis = m1->target->get_reference_x();
	int m1_distance = abs(m1->get_reference_x() - axis);
	int m2_distance = abs(m2->get_reference_x() - axis);
	return m1_distance < m2_distance;
}

bool sort_east(Mob* m1, Mob* m2){
	int axis = m1->target->get_reference_y();
	return abs(m1->get_reference_y() - axis) < abs(m2->get_reference_y() - axis);
}