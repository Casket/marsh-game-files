#include "ItemDB.h"


ItemDB::ItemDB(void){
	this->items_by_id = new std::map<int, Equipment*>();
	load_From_File();
}

ItemDB::~ItemDB(void){
	delete this->items_by_id;
}

Equipment* ItemDB::fetch_item(int item_id){
	std::map<int, Equipment*>::iterator location;
	location = this->items_by_id->find(item_id);
	if (location == this->items_by_id->end())
		return NULL;
	return (*location).second;
}

void ItemDB::load_From_File(void){

	char* name = "";
	char* description = "";
	int item_id, vitality, focus, intelligence, willpower, armor;
	bool equipable, stackable;
	EquipmentType type;
	Equipment* item = (Equipment*)malloc(sizeof(Equipment));

	ifstream file1("Items.marsh");
	string line;
	string beg, end;
	if (file1.is_open()) {
		while (file1.good()) {
			getline(file1,line);
			istringstream iss(line);
			iss >> beg;
			iss >> end;
			if (beg.compare("Item") == 0) {
				item->name = name;
				item->description = description;
				item->item_id = item_id;
				item->vitality = vitality;
				item->focus = focus;
				item->intelligence = intelligence;
				item->willpower = willpower;
				item->armor = armor;
				item->equipped = false;
				item->equipable = equipable;
				item->stackable = stackable;
				item->number_held = 0;
				item->type = Unitialized;//TODO fix this
				this->items_by_id->insert(std::pair<int, Equipment*>(item_id, item));
			}
			else if (beg.compare("Name") == 0) {
				std::string str = line.substr(5);
				name = new char[str.length() + 1];
				strcpy(name, str.c_str());
			}
			else if (beg.compare("Description") == 0) {
				std::string str = line.substr(12);
				description = new char[str.length() + 1];
				strcpy(description, str.c_str());
			}
			else if (beg.compare("Item_ID") == 0) stringstream(end) >> item_id;
			else if (beg.compare("Vitality") == 0) stringstream(end) >> vitality;
			else if (beg.compare("Focus") == 0) stringstream(end) >> focus;
			else if (beg.compare("Intelligence") == 0) stringstream(end) >> intelligence;
			else if (beg.compare("Willpower") == 0) stringstream(end) >> willpower;
			else if (beg.compare("Armor") == 0) stringstream(end) >> armor;
			else if (beg.compare("Equipable") == 0) stringstream(end) >> equipable;
			else if (beg.compare("Stackable") == 0) stringstream(end) >> stackable;
			//else if (beg.compare("Type") == 0) stringstream(end) >> type;
		}
	}
}