#include "Main.h"

using namespace std;




ItemBestower::ItemBestower(Quest* give, int x, int y, int vel, int vel_d, Sprite* img)
:Combat(x, y, vel, vel_d, img){
	this->pre_quest_dialogue = new std::vector<std::string>();
	this->post_quest_dialogue = new std::vector<std::string>();
	this->inventory = new std::vector<std::pair<Equipment*, int>>();
	int current_dialogue = 0;
	bool should_free_player = false;
	bool has_player_hostage = false;
	bool delivered_quest  = false;
	bool should_give_quest = false;
	bool should_bestow_all = false;
}
ItemBestower::~ItemBestower(void){
	delete this->pre_quest_dialogue;
	delete this->post_quest_dialogue;
	delete this->inventory;
} 

void ItemBestower::update(void){
	if (this->should_give_items){
		if (this->should_bestow_all){
			
		} else {

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
				Player_Accessor::get_player()->add_to_inventory(this->inventory->at(key_pressed).first);
				// TODO : remove gold from player's inventory
			}
		}
	}

}

void ItemBestower::append_dialogue(std::string msg){
	this->pre_quest_dialogue->push_back(msg);
}

void ItemBestower::append_post_dialogue(std::string msg){
	this->post_quest_dialogue->push_back(msg);
}

void ItemBestower::append_pre_dialogue(std::string msg){
	this->pre_quest_dialogue->push_back(msg);
}
void ItemBestower::clear_dialogue(void){
	this->pre_quest_dialogue->clear();
	this->post_quest_dialogue->clear();
}

void ItemBestower::speak(void){

}