#ifndef PLAYER_H
#define PLAYER_H 1

#define EXPERIENCE_MULTIPLIER 1.5
#define MAX_HELD_ITEMS 11
#define EXPERIENCE_TO_LEVEL 100 // TODO fine tune the game
#define MAX_ITEM_STACK_SIZE 20
#define NOTORIETY_CAP 500

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

#define INTERACT_KEY KEY_SPACE

#include "Main.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Combat.h"

class QuestManager;

using namespace std;


enum EquipmentType{ Boots, Belt, Necklace, Ring, Mask, Tunic, Cape, Pants, Book, Key, Dagger, Consumable, Unitialized};

typedef struct Equipment{
	char* name;
	char* description;
	int item_id;
	int vitality, focus, intelligence, willpower, armor;
	bool equipped;
	bool equipable;
	bool stackable;
	int number_held;
	EquipmentType type;
}Equipment;

class Player: public Combat{
	std::vector<Equipment*>* inventory;
	BITMAP* clear_console;
	BITMAP* in_use_console;

	public:
		int gold;
		int experience;
		int statPoints;
		int notoriety;
		int level, current_experience, mana, max_mana;
		QuestManager* quest_manager;
		bool interacting;
		Player(int x, int y, int vel, int vel_d, Sprite* img);
		~Player(void);
		virtual void update(void);
		std::vector<Equipment*>* get_inventory(void);
		int add_to_inventory(Equipment* equip);
		void equip_item(Equipment* equip);
		void credit_death(Combat*);
		void credit_interaction(EntityType et);
		virtual void set_stats(int vitality, int intelligence, int focus, int willpower, int armor);
		void display_to_user(std::string message);
		void set_consoles(BITMAP* clear, BITMAP* in_use);
		bool wants_to_talk(void);
		int get_max_mana(void);
		int get_current_mana(void);
		void use_consumable(Equipment* cons);
		void remove_from_inventory(Equipment*);
		void unequip_item(Equipment*);
		void remove_item_stats(Equipment*);
		void increment_notoriety(int increase);
		virtual void upon_death(void);
		virtual bool get_visible(iDrawable* check);
		virtual bool detect_enemies(iDrawable* check);

	
	private:
		// private stuff
		int calculate_mana(int);
		void listen_to_keyboard(void);
		void accept_aiming(void);
		void accept_movement(void);
		void check_casting(void);
		void accept_interaction(void);
		void set_new_inventory(void);
		void Player::grant_experience(int experience_worth);

};



#endif