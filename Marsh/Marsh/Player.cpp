#include "Main.h"

using namespace std;

bool keyrel(int k);

Player::Player(int x, int y, int vel, int vel_d, Sprite* img)
:Combat(x, y, vel, vel_d, img)
{
	this->casting = false;
	this->casting_timer = 0;
	this->keyboard_counter = 0;
	this->keyboard_delay = 4;
	this->quest_manager = new QuestManager();
	// TODO implement a constructor
	}

Player::~Player(void){

}

Equipment** Player::get_inventory(void) {
	return this->inventory;
}

bool Player::add_to_inventory(Equipment* e) {
	for (int i=0; i < MAX_HELD_ITEMS; i++) {
		if (this->inventory[i] != NULL){
			this->inventory[i] = e;
			return true;
		}
	}
	// inventory is full, so it wasn't added
	return false;
}

void Player::deal_with_attack(Attack* attack){

}


void Player::update(void) {
	// need to figure out where this guy wants to go
	check_collisions();
	listen_to_keyboard();

	if (this->casting)
		casting_update();


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
				new_dir = SE;
				break;
			case SW:
				new_dir = S;
				break;
			case SE:
				new_dir = E;
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
				new_dir = SW;
				break;
			case SW:
				new_dir = W;
				break;
			case SE:
				new_dir = S;
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


