#ifndef PLAYER_H
#define PLAYER_H 1

#define EXPERIENCE_MULTIPLIER 1.5
#define MAX_HELD_ITEMS 50
#define BASE_EXPERIENCE_TO_LEVEL 100 // TODO fine tune the game
#define MAX_ITEM_STACK_SIZE 20

using namespace std;

#include "Main.h"

enum EquipmentType{ Boots, Belt, Necklace, Ring, Mask, Tunic, Cape, Pants, Book, Key,Dagger, Consumable};

typedef struct Equipment{
	char* name;
	int vitality, focus, intelligence, willpower;
	bool equipped;
	bool equipable;
	bool stackable;
	int number_held;
	EquipmentType type;
}Equipment;

class Player: public Combat{
	Equipment* inventory[MAX_HELD_ITEMS];
	int level, current_experience;

	public:
		Player(int x, int y, int vel, int vel_d, Sprite* img);
		~Player(void);
		void update(void);
	
	private:
		// private stuff


};



#endif