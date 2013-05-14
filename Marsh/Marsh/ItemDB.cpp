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

void ItemDB::clear_Item(Equipment* item){
	item->item_id = -1;
	item->vitality = 0;
	item->focus = 0;
	item->intelligence = 0;
	item->willpower = 0;
	item->armor = 0;
	item->equipped = false;
	item->equipable = false;
	item->stackable = false;
	item->number_held = 0;
	item->type = Unitialized;
}


void ItemDB::load_From_File(void){
	std::string name = "";
	std::string description = "";
	std::string type_string = "";
	int item_id = 0, vitality = 0, focus = 0, intelligence = 0, willpower = 0, armor = 0;
	bool equipable = false, stackable = false;
	EquipmentType type = Unitialized;
	Equipment* item = new Equipment();
	clear_Item(item);
	std::map<std::string, EquipmentType>* typeMap = new std::map<std::string, EquipmentType>();
	type_string = "Boots";
	typeMap->insert(std::pair<std::string,EquipmentType>(type_string, Boots));
	type_string = "Armor";
	typeMap->insert(std::pair<std::string,EquipmentType>(type_string, Armor));
	type_string = "QuestItem";
	typeMap->insert(std::pair<std::string,EquipmentType>(type_string, QuestItem));
	type_string = "Jewelry";
	typeMap->insert(std::pair<std::string,EquipmentType>(type_string, Jewelry));
	type_string = "Weapon";
	typeMap->insert(std::pair<std::string,EquipmentType>(type_string, Weapon));
	type_string = "Helmet";
	typeMap->insert(std::pair<std::string,EquipmentType>(type_string, Helmet));
	type_string = "Consumable";
	typeMap->insert(std::pair<std::string,EquipmentType>(type_string,Consumable));
	type_string = "Unitialized";
	typeMap->insert(std::pair<std::string,EquipmentType>(type_string, Unitialized));

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
				item->type = type;
				this->items_by_id->insert(std::pair<int, Equipment*>(item_id, item));
				item = new Equipment();
				clear_Item(item);
				name = "";
				description = "";
				type_string = "";
				item_id = 0; vitality = 0; focus = 0; intelligence = 0; willpower = 0; armor = 0;
				bool equipable = false; stackable = false;
				EquipmentType type = Unitialized;
				
			}
			else if (beg.compare("Name") == 0) {
				name = line.substr(5);
			}
			else if (beg.compare("Description") == 0) {
				description = line.substr(12);
			}
			else if (beg.compare("Item_ID") == 0) stringstream(end) >> item_id;
			else if (beg.compare("Vitality") == 0) stringstream(end) >> vitality;
			else if (beg.compare("Focus") == 0) stringstream(end) >> focus;
			else if (beg.compare("Intelligence") == 0) stringstream(end) >> intelligence;
			else if (beg.compare("Willpower") == 0) stringstream(end) >> willpower;
			else if (beg.compare("Armor") == 0) stringstream(end) >> armor;
			else if (beg.compare("Equipable") == 0){	
				std::string compare;
				stringstream(end) >> compare;
				equipable = compare.compare("true") == 0 ? true : false;
			}
			else if (beg.compare("Stackable") == 0){
				std::string compare;
				stringstream(end) >> compare;
				stackable = compare.compare("true") == 0 ? true : false;
			}
			else if (beg.compare("Type") == 0){
				std::map<std::string, EquipmentType>::iterator location;
				location = typeMap->find(line.substr(5));
				type = (*location).second;
			}
		}
	}
}