#ifndef PLAYER_H
#define PLAYER_H 1

#define EXPERIENCE_MULTIPLIER 1.5
#define MAX_HELD_ITEMS 50
#define BASE_EXPERIENCE_TO_LEVEL 100 // TODO fine tune the game
#define MAX_ITEM_STACK_SIZE 20

#define MOVE_UP KEY_W
#define MOVE_DOWN KEY_S
#define MOVE_LEFT KEY_A
#define MOVE_RIGHT KEY_D

#define AIM_LEFT KEY_LEFT
#define AIM_RIGHT KEY_RIGHT
#define AIM_UP KEY_UP
#define AIM_DOWN KEY_DOWN

#define CAST_ONE KEY_1
#define CAST_TWO KEY_2
#define CAST_THREE KEY_3
#define CAST_FOUR KEY_4
#define CAST_FIVE KEY_5
#define CAST_SIX KEY_6
#define CAST_SEVEN KEY_7
#define CAST_EIGHT KEY_8
#define CAST_NINE KEY_9
#define CAST_TEN KEY_0
#define OPEN_MENU		// TODO define that

#define MOVEMENT_DELTA 2



#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"

using namespace std;


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
	bool casting;
	int casting_timer;
	Attack* casted_spell;

	public:
		Player(int x, int y, int vel, int vel_d, Sprite* img);
		~Player(void);
		void update(void);
	
	private:
		// private stuff
		void listen_to_keyboard(void);
		void accept_aiming(void);
		void accept_movement(void);
		void check_casting(void);
		void check_collisions(void);
		void casting_update(void);
		void accept_interaction(void);


};



#endif