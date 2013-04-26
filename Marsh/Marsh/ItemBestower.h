#pragma once

#include "Main.h"

using namespace std;

class ItemBestower: public Combat{
public:
	std::vector<std::string>* dialogue;
	std::vector<std::pair<Equipment*, int>>* inventory;
	int current_dialogue;
	bool should_give_items;
	bool bestow_all_items;


	ItemBestower(int x, int y, int vel, int vel_d, Sprite* img);
	~ItemBestower(void);

	virtual void append_dialogue(std::string);
	void append_inventory(Equipment* item, int price);
	virtual void clear_dialogue(void);
	virtual void speak(void);
	virtual void update(void);
};