#include "Main.h"

using namespace std;

Player::Player(int x, int y, int vel, int vel_d, Sprite* img)
:Combat(x, y, vel, vel_d, img)
{
	this->casting = false;
	// TODO implement a constructor
}

void Player::update(void) {
	// need to figure out where this guy wants to go
	listen_to_keyboard();

}

void Player::listen_to_keyboard(void) {
	if (this->casting)
		// only accept aiming
		accept_aiming();
	else {
		// deal with all other potential input
		accept_movement();
	}
}

void Player::accept_aiming(void) {
	Direction old_dir = this->image->facing;
	if (key[AIM_LEFT]) {
			switch(old_dir) {
			case N:
				this->image->facing = NW;
				break;
			case NE:
				this->image->facing = N;
				break;
			case NW:
				this->image->facing = W;
				break;
			case S:
				this->image->facing = SE;
				break;
			case SW:
				this->image->facing = S;
				break;
			case SE:
				this->image->facing = E;
				break;
			case E:
				this->image->facing = NE;
				break;
			case W:
				this->image->facing = SW;
				break;
			}
		}
		if (key[AIM_RIGHT]) {
			switch(old_dir) {
			case N:
				this->image->facing = NE;
				break;
			case NE:
				this->image->facing = E;
				break;
			case NW:
				this->image->facing = N;
				break;
			case S:
				this->image->facing = SW;
				break;
			case SW:
				this->image->facing = W;
				break;
			case SE:
				this->image->facing = S;
				break;
			case E:
				this->image->facing = SE;
				break;
			case W:
				this->image->facing = NW;
				break;
			}
		}



		if (this->image->facing != old_dir)
			this->image->animation_counter = 0;
		// reset the animation counter if the facing direction changed


}

void Player::accept_movement(void) {
	int cur_x = this->x_pos;
	int cur_y = this->y_pos;
	Direction cur_dir = this->image->facing;
	int new_x = cur_x;
	int new_y = cur_y;
	bool walking = false;

	if (key[MOVE_LEFT]) {
		new_x -= MOVEMENT_DELTA;
		if (!walking) {
			walking = true;
			if (cur_dir != W) {
				// he must point WEAST!
				this->image->facing = W;
				this->image->animation_counter = 0;
			}
		}
	}
	
	if (key[MOVE_RIGHT]) {
		new_x += MOVEMENT_DELTA;
		if (!walking) {
			walking = true;
			if (cur_dir != E){
				this->image->facing = E;
				this->image->animation_counter = 0;
			}
		}
	}
	
	if (key[MOVE_DOWN]) {
		new_y += MOVEMENT_DELTA;
		if (!walking) {
			walking = true;
			if (cur_dir != S) {
				this->image->facing = S;
				this->image->animation_counter = 0;
			}
		}
	}

	if (key[MOVE_UP]) {
		new_y -= MOVEMENT_DELTA;
		if (!walking) {
			walking = true;
			if (cur_dir != N) {
				this->image->facing = N;
				this->image->animation_counter = 0;
			}
		}
	}
	this->x_pos = new_x;
	this->y_pos = new_y;
	if (walking)
		this->image->update();
}


