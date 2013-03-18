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
	Direction old_dir = this->image->get_facing();
	Direction new_dir = old_dir;
	if (key[AIM_LEFT]) {
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
		if (key[AIM_RIGHT]) {
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



		if (new_dir != old_dir)
			this->image->set_facing(new_dir);
		// reset the animation counter if the facing direction changed


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
		new_x -= MOVEMENT_DELTA;
		if (!walking) {
			walking = true;
			new_dir = W;
		}
	}
	
	if (key[MOVE_RIGHT]) {
		new_x += MOVEMENT_DELTA;
		if (!walking) {
			walking = true;
			new_dir = E;
		}
	}
	
	if (key[MOVE_DOWN]) {
		new_y += MOVEMENT_DELTA;
		if (!walking) {
			walking = true;
			new_dir = S;
		}
	}

	if (key[MOVE_UP]) {
		new_y -= MOVEMENT_DELTA;
		if (!walking) {
			walking = true;
				new_dir = N;
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


