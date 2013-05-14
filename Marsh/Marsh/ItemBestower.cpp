#include "Main.h"

using namespace std;




ItemBestower::ItemBestower(int x, int y, int vel, int vel_d, Sprite* img)
:Combat(x, y, vel, vel_d, img){
	this->dialogue = new std::vector<std::string>();
	this->inventory = new std::vector<std::pair<Equipment*, int>>();
	this->current_dialogue = 0;
	this->should_free_player = false;
	this->has_player_hostage = false;
	this->should_give_items = false;
	this->bestow_all_items = false;
}
ItemBestower::~ItemBestower(void){
	delete this->dialogue;
	delete this->inventory;
} 


void ItemBestower::update(void){
	if (!this->should_give_items)
		return;

	if (this->bestow_all_items){
		while (!this->inventory->empty()){
			std::pair<Equipment*, int> item = this->inventory->back();
			int success = Player_Accessor::get_player()->add_to_inventory(get_item_clone(item.first));
			this->inventory->pop_back();
			if (success == -1){
				Player_Accessor::get_player()->display_to_user("Your inventory is full.  Please make some room before taking more items.");				
				this->inventory->push_back(item);
				break;
			}
		}
		this->should_give_items = false;

	} else {
		int key_pressed = -1;
		if (keyrel(KEY_1)){
			key_pressed = 0;
		} else if (keyrel(KEY_2)){
			key_pressed = 1;
		} else if (keyrel(KEY_3)){
			key_pressed = 2;
		} else if (keyrel(KEY_4)){
			key_pressed = 3;
		} else if (keyrel(KEY_5)){
			key_pressed = 4;
		} else if (keyrel(KEY_6)){
			key_pressed = 5;
		} else if (keyrel(KEY_7)){
			key_pressed = 6;
		} else if (keyrel(KEY_8)){
			key_pressed = 7;
		} else if (keyrel(KEY_9)){
			key_pressed = 8;
		} else if (keyrel(KEY_0)){
			key_pressed = 9;
		}

		if (key_pressed >= 0 && key_pressed < this->inventory->size()){
			Player* hero = Player_Accessor::get_player();
			if (hero->gold >= this->inventory->at(key_pressed).second){
				hero->gold -= this->inventory->at(key_pressed).second;
				int success = hero->add_to_inventory(get_item_clone(this->inventory->at(key_pressed).first));
				if (success == 1)
					return;
				if (success == -1)
					hero->display_to_user("Your inventory is full.  Please make some room before taking more items.");
				// TODO : remove gold from player's inventory
				if (success == -2)
					hero->display_to_user("You can't have duplicate of that.");

			} else {
				hero->display_to_user("You need more gold to purchase that item.");
			}
			this->current_dialogue--;
			this->should_give_items = false;
			this->should_free_player = false;

			return;


		}
	}


}

void ItemBestower::append_inventory(Equipment* item, int price){
	this->inventory->push_back(std::pair<Equipment*, int>(item, price));
}

void ItemBestower::append_dialogue(std::string msg){
	this->dialogue->push_back(msg);
}

void ItemBestower::clear_dialogue(void){
	this->dialogue->clear();
}

void ItemBestower::speak(void){
	Combat::face_speaker();
	if (this->should_free_player){
		Player_Accessor::get_player()->display_to_user("");
		Player_Accessor::get_player()->interacting = false;
		this->has_player_hostage = false;
		this->should_free_player = false;
		this->should_give_items = false;
		this->current_dialogue = 0;
		return;
	}

	Player_Accessor::get_player()->interacting = true;
	this->has_player_hostage = true;
	this->should_free_player = false;
	if (this->current_dialogue < this->dialogue->size()){
		Player_Accessor::get_player()->display_to_user(this->dialogue->at(this->current_dialogue++));
		if (this->current_dialogue >= this->dialogue->size() - 1){
			this->should_give_items = true;
			this->should_free_player = true;
		}
	}


}