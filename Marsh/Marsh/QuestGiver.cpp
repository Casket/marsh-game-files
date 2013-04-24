#include "Main.h"
#include "QuestGiver.h"

using namespace std;

bool keyrel(int k);


QuestGiver::QuestGiver(Quest* give, int x, int y, int vel, int vel_d, Sprite* img)
:Combat(x, y, vel, vel_d, img)
{
	this->pre_quest_dialogue = new std::vector<std::string>();
	this->post_quest_dialogue = new std::vector<std::string>();
	this->current_dialogue = 0;
	this->should_free_player = false;
	this->has_player_hostage = false;
	this->delivered_quest = false;
	this->should_give_quest = false;
	this->quest_to_give = give;
	this->can_speak = true;

}
QuestGiver::~QuestGiver(void){
	if (this->quest_to_give != NULL)
		delete this->quest_to_give;
	delete this->pre_quest_dialogue;
	delete this->post_quest_dialogue;
}

void QuestGiver::append_dialogue(std::string message){
	if (this->delivered_quest)
		this->post_quest_dialogue->push_back(message);
	else
		this->pre_quest_dialogue->push_back(message);
}

void QuestGiver::append_post_dialogue(std::string message){
	this->post_quest_dialogue->push_back(message);
}

void QuestGiver::append_pre_dialogue(std::string message){
	this->pre_quest_dialogue->push_back(message);
}

void QuestGiver::clear_dialogue(void){
	this->pre_quest_dialogue->clear();
	this->post_quest_dialogue->clear();
}

void QuestGiver::speak(void){
	Combat::face_speaker();
	if (this->should_give_quest){
		this->quest_to_give->begin_quest();
		Player_Accessor::get_player()->quest_manager->flush_queues();
		this->should_free_player = true;
		this->should_give_quest = false;
		this->delivered_quest = true;
	}

	if (this->should_free_player){
		Player_Accessor::get_player()->interacting = false;
		this->has_player_hostage = false;
		this->should_free_player = false;
		return;
	}

	if (this->delivered_quest){
		Player_Accessor::get_player()->interacting = true;
		this->has_player_hostage = true;
		this->should_free_player = false;
		Player_Accessor::get_player()->display_to_user(this->post_quest_dialogue->at(this->current_dialogue++));
		if (this->current_dialogue >= this->post_quest_dialogue->size()){
			this->current_dialogue = 0;
			this->should_free_player = true;
		}
	} else {
		Player_Accessor::get_player()->interacting = true;
		this->has_player_hostage = true;
		this->should_free_player = false;
		this->should_give_quest = false;

		Player_Accessor::get_player()->display_to_user(this->pre_quest_dialogue->at(this->current_dialogue++));
		if (this->current_dialogue >= this->pre_quest_dialogue->size()){
			this->current_dialogue = 0;
			this->should_give_quest = true;
		}
	}
}
