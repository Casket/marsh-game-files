#include "Main.h"
#include "OptionPresenter.h"

using namespace std;

OptionPresenter::OptionPresenter(int x, int y, int vel, int vel_d, Sprite* img)
:Combat(x, y, vel, vel_d, img){

	this->pre_quest_dialogue = new std::vector<std::string>();
	this->post_quest_dialogue = new std::vector<std::string>();
	this->presentable_quests = new std::vector<Quest*>();
	this->current_dialogue = 0;
	this->has_player_hostage = false;
	this->delivered_quest = false;
	this->should_give_quest = false;
	this->should_free_player = false;
	this->can_speak = true;
}

OptionPresenter::~OptionPresenter(void){
	
	delete this->pre_quest_dialogue;
	delete this->post_quest_dialogue;
	delete this->presentable_quests;
}

void OptionPresenter::update(void){
	if (!this->should_give_quest)
		return;

	int key_pressed = -1;
	if (keyrel(KEY_1)){
		key_pressed = 1;
	} else if (keyrel(KEY_2)){
		key_pressed = 2;
	} else if (keyrel(KEY_3)){
		key_pressed = 3;
	} else if (keyrel(KEY_4)){
		key_pressed = 4;
	} else if (keyrel(KEY_5)){
		key_pressed = 5;
	} else if (keyrel(KEY_6)){
		key_pressed = 6;
	} else if (keyrel(KEY_7)){
		key_pressed = 7;
	} else if (keyrel(KEY_8)){
		key_pressed = 8;
	} else if (keyrel(KEY_9)){
		key_pressed = 9;
	} else if (keyrel(KEY_0)){
		key_pressed = 10;
	} 

	if (key_pressed > 0){
		this->should_give_quest = false;
		this->has_player_hostage = false;
		this->delivered_quest = true;
		this->presentable_quests->at(key_pressed - 1)->begin_quest();
		Player_Accessor::get_player()->interacting = false;
		this->current_dialogue = 0;
	}
}

void OptionPresenter::append_dialogue(std::string message){
	if (this->delivered_quest)
		this->post_quest_dialogue->push_back(message);
	else
		this->pre_quest_dialogue->push_back(message);
}

void OptionPresenter::append_post_dialogue(std::string message){
	this->post_quest_dialogue->push_back(message);
}

void OptionPresenter::append_pre_dialogue(std::string message){
	this->pre_quest_dialogue->push_back(message);
}

void OptionPresenter::append_quest(Quest* q){
	this->presentable_quests->push_back(q);
}

void OptionPresenter::clear_dialogue(void){
	this->pre_quest_dialogue->clear();
	this->post_quest_dialogue->clear();
}

void OptionPresenter::speak(void){
	Combat::face_speaker();
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

		if (this->current_dialogue < this->pre_quest_dialogue->size()){
			Player_Accessor::get_player()->display_to_user(this->pre_quest_dialogue->at(this->current_dialogue++));
			if (this->current_dialogue >= this->pre_quest_dialogue->size() - 1){
				this->should_give_quest = true;
			}
		}
	}
}