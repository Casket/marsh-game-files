#include "Main.h"
#include "QuestGiver.h"


QuestGiver::QuestGiver(Quest* give, int x, int y, int vel, int vel_d, Sprite* img)
:Combat(x, y, vel, vel_d, img)
{
	this->pre_quest_dialogue = new std::vector<std::string>();
	this->post_quest_dialogue = new std::vector<std::string>();
	this->current_dialogue = 0;
	this->should_free_player = false;
	this->has_player_hostage = false;
	this->delivered_quest = false;
	this->quest_to_give = give;

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
	this->post_quest_dialoge->push_back(message);
}

void QuestGiver::append_pre_dialogue(std::string message){
	this->pre_quest_dialogue->push_back(message);
}

void QuestGiver::clear_dialogue(void){
	this->pre_quest_dialogue->clear();
	this->post_quest_dialogue->clear();
}

void QuestGiver::speak(void){
	if (this->should_free_player){
		Player_Accessor::get_player()->interacting = false;
		this->has_player_hostage = false;
		this->should_free_player = false;
		Player_Accessor::get_player()->display_to_user("");
		return;
	}




}
