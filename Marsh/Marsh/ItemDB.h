#pragma once

#include "Main.h"

class ItemDB{
	std::map<int, Equipment*>* items_by_id;

public:
	ItemDB(void);
	~ItemDB(void);
	Equipment* fetch_item(int item_id);

private:
	void load_From_File(void);
	void clear_Item(Equipment*);
};