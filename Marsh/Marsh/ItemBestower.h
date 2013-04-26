#pragma once

#include "Main.h"

using namespace std;

class ItemBestower: public Combat{
public:
	std::vector<std::string>* dialogue;
	std::vector<std::pair<Equipment*, int>>* inventory;
	int current_dialogue;
	bool should_free_player;
	bool has_player_hostage;
	bool should_give_items;
	bool should_bestow_all;


	ItemBestower(int x, int y, int vel, int vel_d, Sprite* img);
	~ItemBestower(void);

	virtual void append_dialogue(std::string);
	void append_inventory(Equipment* item, int price);
	virtual void clear_dialogue(void);
	virtual void speak(void);
	virtual void update(void);
};