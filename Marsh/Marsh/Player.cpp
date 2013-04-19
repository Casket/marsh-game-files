#include "Main.h"

using namespace std;

bool keyrel(int k);
void print_to_console(std::string, BITMAP*);
std::pair<int, std::string> substr_word_boundaries(std::string str, int pos, int max_len);

Player::Player(int x, int y, int vel, int vel_d, Sprite* img)
:Combat(x, y, vel, vel_d, img)
{
	this->my_type = Hero;
	this->casting = false;
	this->casting_timer = 0;
	this->keyboard_counter = 0;
	this->keyboard_delay = 4;
	this->quest_manager = new QuestManager();
	this->experience = 0;
	// TODO implement a constructor
	this->inventory = (Equipment**)malloc(sizeof(Equipment*)*MAX_HELD_ITEMS);
	this->set_new_inventory();
	this->interacting = false;
	}

Player::~Player(void){

}

Equipment** Player::get_inventory(void) {
	return this->inventory;
}

bool Player::add_to_inventory(Equipment* e) {
	for (int i=0; i < MAX_HELD_ITEMS; i++) {
		if (this->inventory[i] == NULL){
			this->inventory[i] = e;
			return true;
		}
	}
	// inventory is full, so it wasn't added
	return false;
}

void Player::set_new_inventory(void) {
	for (int i=0; i<MAX_HELD_ITEMS; i++) {
		this->inventory[i] = NULL;
	}
}

void Player::deal_with_attack(Attack* attack){

}


void Player::update(void) {
	if (this->interacting)
		return; // interaction is controlled by the giver
	// need to figure out where this guy wants to go
	check_collisions();
	listen_to_keyboard();

	if (this->casting)
		casting_update();


}

void Player::credit_death(Combat* enemy){
	this->quest_manager->killed_entity(enemy->get_my_type());
	if(enemy->player_credit){
		this->experience += enemy->experience_worth;
	}
	this->my_world->removal_queue->push_back(enemy);	
}


void Player::listen_to_keyboard(void) {
	if (this->casting)
		// only accept aiming
		accept_aiming();
	else {
		// deal with all other potential input
		accept_movement();
		accept_interaction();
		check_casting();
	}
}

void Player::accept_interaction(void) {
	// TODO implement this
	//if (this->keyboard_counter++ <= this->keyboard_delay)
		//return;
	this->keyboard_counter = 0;
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

	if (key[CAST_ONE]) {
		desired_attack = 0;
	} else if (key[CAST_TWO]) {
		desired_attack = 1;
	} else if (key[CAST_THREE]) {
		desired_attack = 2;
	} else if (key[CAST_FOUR]) {
		desired_attack = 3;
	} else if (key[CAST_FIVE]) {
		desired_attack = 4;
	} else if (key[CAST_SIX]) {
		desired_attack = 5;
	} else if (key[CAST_SEVEN]) {
		desired_attack = 6;
	} else if (key[CAST_EIGHT]) {
		desired_attack = 7;
	} else if (key[CAST_NINE]) {
		desired_attack = 8;
	} else if (key[CAST_TEN]) {
		desired_attack = 9;
	}
	if (desired_attack < 0)
		return;
	this->launch_attack(desired_attack);
}

void Player::accept_aiming(void) {
	Direction old_dir = this->image->get_facing();
	Direction new_dir = old_dir;
	
	if (keyrel(AIM_LEFT)) {
		switch(old_dir) {
			case N:
				new_dir = NW;
				break;
			case NE:
				new_dir = N;
				break;
			case NW:
				new_dir = W;
				break;
			case S:
				new_dir = SW;
				break;
			case SW:
				new_dir = W;
				break;
			case SE:
				new_dir = S;
				break;
			case E:
				new_dir = NE;
				break;
			case W:
				new_dir = SW;
				break;
		}
	}
	if (keyrel(AIM_RIGHT)) {
		switch(old_dir) {
			case N:
				new_dir = NE;
				break;
			case NE:
				new_dir = E;
				break;
			case NW:
				new_dir = N;
				break;
			case S:
				new_dir = SE;
				break;
			case SW:
				new_dir = S;
				break;
			case SE:
				new_dir = E;
				break;
			case E:
				new_dir = SE;
				break;
			case W:
				new_dir = NW;
				break;
		}
	}
	// should rotate the player 180 degrees (turn around... bright eyes... everytime...)
	if (keyrel(AIM_UP) || keyrel(AIM_DOWN)) {
		switch(old_dir) {
			case N:
				new_dir = S;
				break;
			case NE:
				new_dir = SW;
				break;
			case NW:
				new_dir = SE;
				break;
			case S:
				new_dir = N;
				break;
			case SW:
				new_dir = NE;
				break;
			case SE:
				new_dir = NW;
				break;
			case E:
				new_dir = W;
				break;
			case W:
				new_dir = E;
				break;
		}
	}
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
				new_x -= MOVEMENT_DELTA;
		}
	}

	else if (key[MOVE_RIGHT]) {

		if (!walking) {
			walking = true;
			new_dir = E;
			if (this->can_walk_right)
				new_x += MOVEMENT_DELTA;
		}
	}

	else if (key[MOVE_DOWN]) {

		if (!walking) {
			walking = true;
			new_dir = S;
			if (this->can_walk_down)
				new_y += MOVEMENT_DELTA;
		}
	}

	else if (key[MOVE_UP]) {

		if (!walking) {
			walking = true;
			new_dir = N;
			if (this->can_walk_up)
				new_y -= MOVEMENT_DELTA;
		}
	}

	if (new_dir != cur_dir) {
		this->image->set_facing(new_dir);
	}

	//if (this->can_walk){
		this->x_pos = new_x;
		this->y_pos = new_y;
		if (walking)
			this->image->update();
	//}
	
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
	int len = str.length();
	bool saw_space = false;
	int last_word_ending_pos = pos;

	int i=0;
	while (i < max_len){
		if (i > len)
			break;
		if ((str.at(pos+i) == ' ')  && !saw_space){
			last_word_ending_pos = pos+i;
			saw_space = true;
		} else if (str.at(pos+i) != ' ')
			saw_space = false;
		i++;
	}

	// should now have a pos to stop the substring
	int sub_len = last_word_ending_pos - pos;
	std::string ret_str = str.substr(pos, sub_len);

	// now get a forward index, skipping spaces
	while (str.at(last_word_ending_pos) == ' ')
		last_word_ending_pos++;

	return std::pair<int, std::string>(last_word_ending_pos, ret_str);
}


