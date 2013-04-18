#ifndef PLAYER_H
#define PLAYER_H 1

#define EXPERIENCE_MULTIPLIER 1.5
#define MAX_HELD_ITEMS 11
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
#define OPEN_MENU KEY_ESC	// TODO define that

#define MOVEMENT_DELTA 2



#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"

class QuestManager;

using namespace std;


enum EquipmentType{ Boots, Belt, Necklace, Ring, Mask, Tunic, Cape, Pants, Book, Key, Dagger, Consumable};

typedef struct Equipment{
	char* name;
	char* description;
	int item_id;
	int vitality, focus, intelligence, willpower;
	bool equipped;
	bool equipable;
	bool stackable;
	int number_held;
	EquipmentType type;
}Equipment;

class Player: public Combat{
	Equipment** inventory;	
	int level, current_experience, mana, max_mana;
	int keyboard_counter;
	int keyboard_delay;
	BITMAP* clear_console;
	BITMAP* in_use_console;

	public:
		int experience;
		QuestManager* quest_manager;
		Player(int x, int y, int vel, int vel_d, Sprite* img);
		~Player(void);
		virtual void update(void);
		Equipment** get_inventory(void);
		bool add_to_inventory(Equipment* equip);
		virtual void deal_with_attack(Attack* attack);
		void credit_death(Combat*);
		virtual void set_stats(int vitality, int intelligence, int focus, int willpower, int armor);
		void display_to_user(std::string message);
		void set_consoles(BITMAP* clear, BITMAP* in_use);

	
	private:
		// private stuff
		int calculate_mana(int);
		void listen_to_keyboard(void);
		void accept_aiming(void);
		void accept_movement(void);
		void check_casting(void);
		void accept_interaction(void);
		void set_new_inventory(void);

};



#endif