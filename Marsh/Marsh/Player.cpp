#include "Main.h"

using namespace std;

Player::Player(int x, int y, int vel, int vel_d, Sprite* img)
:Combat(x, y, vel, vel_d, img)
{
	this->casting = false;
	// TODO implement a constructor
}

void Player::update(void) {
	int cur_x = this->x_pos;
	int cur_y = this->y_pos;

	// need to figure out where this guy wants to go
	listen_to_keyboard();
}


void Player::listen_to_keyboard(void) {
	if (this->casting) {
		// only accept aiming
	}

	else {
		// deal with all other potential input
	}
}
