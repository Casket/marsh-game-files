#pragma once

#include "Main.h"

using namespace std;

class ItemBestower: public Combat{
public:
	std::vector<std::string>* pre_quest_dialogue;
	std::vector<std::string>* post_quest_dialogue;
	std::vector<std::pair<Equipment*, int>>* inventory;
	int current_dialogue;
	bool should_free_player;
	bool has_player_hostage;
	bool delivered_quest;
	bool should_give_quest;
	bool should_bestow_all;


	ItemBestower(Quest* give, int x, int y, int vel, int vel_d, Sprite* img);
	~ItemBestower(void);

	virtual void append_dialogue(std::string);
	void append_post_dialogue(std::string);
	void append_pre_dialogue(std::string);
	virtual void clear_dialogue(void);
	virtual void speak(void);
};