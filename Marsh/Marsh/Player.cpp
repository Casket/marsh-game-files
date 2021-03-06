#include "Main.h"

using namespace std;

bool keyrel(int k);
int move_closer(int, int);
void print_to_console(std::string, BITMAP*);
std::pair<int, std::string> substr_word_boundaries(std::string str, int pos, int max_len);

Player::Player(int x, int y, int vel, int vel_d, Sprite* img)
:Combat(x, y, vel, vel_d, img)
{
	this->my_type = Hero;
	this->casting = false;
	this->new_mission = false;
	this->dead  = false;
	this->quest_manager = new QuestManager();
	this->experience = 0;
	this->notoriety = 0;
	this->level = 0;
	// TODO implement a constructor
	this->inventory = new std::vector<Equipment*>();
	this->set_new_inventory();
	this->interacting = false;
	this->set_stats(20000, 45, 10, 10, 5);
	this->mana = this->max_mana;
	this->gold = 100;
	this->statPoints = 0;
	this->spellPoints = 0;
	this->velocity = MOVEMENT_DELTA;
}

Player::~Player(void){

}

void Player::set_world(World* world){
	iDrawable::set_world(world);
	std::list<Attack*>::iterator end = this->active_wards->end();
	for (std::list<Attack*>::iterator iter = this->active_wards->begin(); iter != end; ++iter){
		(*iter)->get_world()->active_entities->remove((*iter));
		(*iter)->set_world(world);
		world->insert_entity((*iter));
	}
}

void Player::upon_death(void){
	this->dead = true;
}

void Player::increment_notoriety(int increase){
	this->notoriety += increase;
	if (this->notoriety >= NOTORIETY_CAP){
		// TODO push the game forward
	}
}

void Player::unequip_item(Equipment* equip){
	std::vector<Equipment*>::iterator end = this->inventory->end();
	for (std::vector<Equipment*>::iterator iter = this->inventory->begin(); iter != end; ++iter){
		Equipment* check = (*iter);
		if (check->item_id == equip->item_id){
			remove_item_stats(check);
			check->equipped = false;
			break;
		}
	}
}

void Player::equip_item(Equipment* equip){
	if (!equip->equipable)
		return;
	EquipmentType equipped_type = equip->type;
	std::vector<Equipment*>::iterator end = this->inventory->end();
	for (std::vector<Equipment*>::iterator iter = this->inventory->begin(); iter != end; ++iter){
		Equipment* check = (*iter);
		if (check->equipped && check->type == equipped_type){
			remove_item_stats(check);
			check->equipped = false;
			break;
		}
	}
	equip->equipped = true;
	this->vitality += equip->vitality;
	this->focus += equip->focus;
	this->intelligence += equip->intelligence;
	this->willpower += equip->willpower;
	this->armor += equip->armor;
	this->max_health = Combat::calculate_health(this->vitality);
	this->max_mana = this->calculate_mana(this->intelligence);

}

void Player::remove_item_stats(Equipment* check){
	this->vitality -= check->vitality;
	this->max_health = Combat::calculate_health(this->vitality);
	if (this->health > this->max_health)
		this->health = this->max_health;
	this->focus -= check->focus;
	this->intelligence -= check->intelligence;
	this->max_mana = this->calculate_mana(this->intelligence);
	if (this->mana > this->max_mana)
		this->mana = this->max_mana;
	this->willpower -= check->willpower;
	this->armor -= check->armor;
}

void Player::use_consumable(Equipment* cons){
	if (cons->type != Consumable)
		return;

	this->health += cons->vitality;
	if (this->health > this->max_health)
		this->health = this->max_health;
	this->mana += cons->intelligence;
	if (this->mana > this->max_mana)
		this->mana = this->max_mana;

	cons->number_held--;
	if (cons->number_held <= 0)
		remove_from_inventory(cons);
}

void Player::remove_from_inventory(Equipment* item){
	std::vector<Equipment*>::iterator end = this->inventory->end();
	for (std::vector<Equipment*>::iterator iter = this->inventory->begin(); iter != end; ++iter){
		Equipment* check = (*iter);
		if (check->item_id == item->item_id){
			if (check->equipped)
				remove_item_stats(check);
			this->inventory->erase(iter);
			return;
		}
	}
}

void Player::set_stats(int vit, int intel, int focus, int will, int armor){
	Combat::set_stats(vit, intel, focus, will, armor);
	this->max_mana = calculate_mana(intel);
}

int Player::calculate_mana(int stat) {
	return stat;
	// TODO do some calculation
}

std::vector<Equipment*>* Player::get_inventory(void) {
	return this->inventory;
}

int Player::add_to_inventory(Equipment* e) {
	int item_count = 0;
	std::vector<Equipment*>::iterator end = this->inventory->end();
	for (std::vector<Equipment*>::iterator iter = this->inventory->begin(); iter != end; ++iter){
		if((*iter)->type != QuestItem){
			item_count++;	
		}
		if ((*iter)->item_id == e->item_id){
			if (e->stackable) {
				e->number_held++;
				return 1;
			}else 
				// is anything not stackable?  dunno bro
				return -2; // you can't have duplicates
		}
	}
	if (item_count >= MAX_HELD_ITEMS && e->type != QuestItem)
		return -1;

	// player didn't have the item, so give him the e
	this->inventory->push_back(e);
	return 1;
}

void Player::set_new_inventory(void) {
	this->inventory->clear();
}

bool Player::wants_to_talk(void){
	return keyrel(INTERACT_KEY) && !this->image->wearing_mask;
}


void Player::update(void) {
	this->update_dispatchers();
	check_collisions();
	if (this->entangled)
		return;
	if (this->interacting)
		return; // interaction is controlled by the giver
	// need to figure out where this guy wants to go

	listen_to_keyboard();

	if (this->casting)
		casting_update();

	clear_keybuf();
}

bool Player::detect_enemies(iDrawable* to_check){

	EntityType type_check = to_check->my_type;

	if(type_check == Monster || type_check == Outcast){
		return true;
	}else if(type_check == Hero){
		if(to_check->get_image()->wearing_mask){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

bool Player::get_visible(iDrawable* current){

	float distance = (((this->x_pos - current->x_pos) * (this->x_pos - current->x_pos)) + 
		((this->y_pos - current->y_pos) * (this->y_pos - current->y_pos)));

	distance = sqrt(distance);

	if(distance < DETECTION_RANGE){
		return true;
	}else{
		return false;
	}
}

void Player::credit_death(Combat* enemy){
	this->quest_manager->killed_entity(enemy->get_my_type());
	if(enemy->player_credit){
		grant_experience(enemy->experience_worth);
	}	
}

void Player::grant_experience(int experience_worth){
	this->experience += experience_worth;
	if(this->experience >= EXPERIENCE_TO_LEVEL){
		this->experience -= EXPERIENCE_TO_LEVEL;
		this->spellPoints += 1;
		this->statPoints += 5;
		this->level++;
		std::stringstream levelup;
		levelup <<  "You have leveled up to level " << this->level << "! Press L to get a new spell and increase your stats.";
		display_to_user(levelup.str());
	}

}

void Player::credit_interaction(EntityType et){
	this->quest_manager->interacted_with(et);
}

int Player::get_current_mana(void){
	return this->mana;
}

int Player::get_max_mana(void){
	return this->max_mana;
}


void Player::listen_to_keyboard(void) {
	if (this->casting)
		// only accept aiming
		accept_aiming();
	else {
		// deal with all other potential input
		//TODO Get rid of these cheat codes
		
		accept_movement();
		accept_interaction();
		if (this->mana > 0)
			check_casting();
	}
}

void Player::accept_interaction(void) {
	// TODO implement this
	if(keyrel(KEY_Q)){
		this->get_image()->wearing_mask = !this->get_image()->wearing_mask;
	}
}

bool keyrel(int k)
{
	static bool initialized = false;
	static bool keyp[KEY_MAX];

	if(!initialized)
	{
		// Set the keyp (key pressed) flags to false
		for(int i = 0; i < KEY_MAX; i++) keyp[i] = false;
		initialized = true;
	}

	// Now for the checking
	// Check if the key was pressed
	if(key[k] && !keyp[k])
	{
		// Set the flag and return
		keyp[k] = true;
		return false;
	}
	else if(!key[k] && keyp[k])
	{
		// The key was released
		keyp[k] = false;
		return true;
	}
	// Nothing happened?
	return false;
}

void Player::check_casting(void) {
	if(!this->get_image()->wearing_mask)
		return;

	int desired_attack = -1;

	if (keyrel(CAST_ONE)) {
		desired_attack = 0;
	} else if (keyrel(CAST_TWO)) {
		desired_attack = 1;
	} else if (keyrel(CAST_THREE)) {
		desired_attack = 2;
	} else if (keyrel(CAST_FOUR)) {
		desired_attack = 3;
	} else if (keyrel(CAST_FIVE)) {
		desired_attack = 4;
	} else if (keyrel(CAST_SIX)) {
		desired_attack = 5;
	} else if (keyrel(CAST_SEVEN)) {
		desired_attack = 6;
	} else if (keyrel(CAST_EIGHT)) {
		desired_attack = 7;
	} else if (keyrel(CAST_NINE)) {
		desired_attack = 8;
	} else if (keyrel(CAST_TEN)) {
		desired_attack = 9;
	}
	if (desired_attack < 0)
		return;
	try{
		Attack* attack = this->attack_loadout[desired_attack];
		if(attack->my_caster == NULL){
			return;}
		this->mana -= attack->get_mana_cost();
		if (this->mana <= 0){
			this->display_to_user("I need more mana.");
			this->mana = 0;

		}
		this->launch_attack(desired_attack);
	}catch(...){
		this->casting = false;
		return;
	}
}

void Player::accept_aiming(void) {
	Direction old_dir = this->image->get_facing();
	Direction new_dir = old_dir;

	if (keyrel(AIM_LEFT)) {
		old_dir = W;
		/*switch(old_dir) {
			case N:
				new_dir = W;
				break;	
			case S:
				new_dir = W;
				break;
			case E:
				new_dir = W;
				break;
			case W:
				new_dir = W;
				break;
		}*/
	}
	if (keyrel(AIM_RIGHT)) {
		new_dir = E;
		/*switch(old_dir) {
			case N:
				new_dir = E;
				break;
			case S:
				new_dir = E;
				break;
			case E:
				new_dir = E;
				break;
			case W:
				new_dir = E;
				break;
		}*/
	}
	// should rotate the player 180 degrees (turn around... bright eyes... everytime...)
	if (keyrel(AIM_UP)) {
		new_dir = N;
		/*switch(old_dir) {
			case N:
				new_dir = S;
				break;
			case S:
				new_dir = N;
				break;
			case E:
				new_dir = W;
				break;
			case W:
				new_dir = E;
				break;
		}*/
	}
	if(keyrel(AIM_DOWN))
		new_dir = S;

	if (new_dir != old_dir)
		this->image->set_facing(new_dir);
	this->image->casting_update();
}

void Player::accept_movement(void) {
	int cur_x = this->x_pos;
	int cur_y = this->y_pos;
	Direction cur_dir = this->image->get_facing();
	Direction new_dir = cur_dir;
	int new_x = cur_x;
	int new_y = cur_y;
	bool walking = false;

	if (key[MOVE_LEFT]) {
		if (!walking) {
			walking = true;
			new_dir = W;
			if (this->can_walk_left)
				new_x -= this->velocity;
		}
	}

	else if (key[MOVE_RIGHT]) {

		if (!walking) {
			walking = true;
			new_dir = E;
			if (this->can_walk_right)
				new_x += this->velocity;
		}
	}

	else if (key[MOVE_DOWN]) {

		if (!walking) {
			walking = true;
			new_dir = S;
			if (this->can_walk_down)
				new_y += this->velocity;
		}
	}

	else if (key[MOVE_UP]) {

		if (!walking) {
			walking = true;
			new_dir = N;
			if (this->can_walk_up)
				new_y -= this->velocity;
		}
	}

	if (new_dir != cur_dir) {
		this->image->set_facing(new_dir);
	}

	this->x_pos = new_x;
	this->y_pos = new_y;
		if (walking)
			this->image->update();
	
}

int move_closer(int pos, int old_pos){
	if (pos == old_pos)
		return -1;
	if (pos > old_pos){
		return pos--;
	}
	else {
		return pos++;
	}
}

void Player::set_consoles(BITMAP* clear, BITMAP* in_use){
	this->clear_console = clear;
	this->in_use_console = in_use;
}

void Player::display_to_user(std::string message){
	blit(this->clear_console, this->in_use_console, 0, 0, 0, 0, 
		this->clear_console->w, this->clear_console->h);
	print_to_console(message, this->in_use_console);
}

void print_to_console(std::string str, BITMAP* cons){
	int max_line_len = 35;
	int line_height = 10;
	int x = 17;
	int y = 18;
	int len = str.length();
	int i = 0;
	while (i < len){
		std::pair<int, std::string> cur_line = substr_word_boundaries(str, i, max_line_len);
		textprintf_ex(cons,font,x,y,
			makecol(255,255,255),-1,cur_line.second.c_str());
		i = cur_line.first;
		y += line_height;
		if (y > CONSOLE_HEIGHT - line_height)
			break; // bad news, it won't all fit... you suck
	}
}

std::pair<int, std::string> substr_word_boundaries(std::string str, int pos, int max_len){
	int len = str.length() - pos;
	bool saw_space = false;
	int last_word_ending_pos = pos;

	int i=0;
	while (i < max_len){
		if (i >= len){
			last_word_ending_pos = pos+i;
			break;
		}
		if ((str.at(pos+i) == ' ')  && !saw_space){
			last_word_ending_pos = pos+i;
			saw_space = true;
		} else if (str.at(pos+i) != ' ')
			saw_space = false;
		i++;
	}

	// should now have a pos to stop the substring
	int sub_len = last_word_ending_pos - pos + 1;
	std::string ret_str = str.substr(pos, sub_len);

	// now get a forward index, skipping spaces
	if (!(i >= len)){
		while (str.at(last_word_ending_pos) == ' ')
			last_word_ending_pos++;
	}
	return std::pair<int, std::string>(last_word_ending_pos, ret_str);
}


