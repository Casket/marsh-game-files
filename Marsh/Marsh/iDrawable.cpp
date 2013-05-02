#include "Main.h"

iDrawable::iDrawable(int x, int y, int vel, int vel_d, Sprite* img){
	this->x_pos = x;
	this->y_pos = y;
	this->velocity = vel;
	this->velocity_delay = vel_d;
	this->movement_counter = 0;
	this->image = img;
	this->alive = true;
	this->can_speak = false;
}

iDrawable::~iDrawable(void){
	if (this->image != NULL)
		delete this->image;
}

void iDrawable::set_sprite(Sprite* spr){
	this->image = spr;
}

void iDrawable::set_world(World* world){
	this->my_world = world;
}

Sprite* iDrawable::get_image(void){
	return this->image;
}

int iDrawable::get_x_pos(void){
	return this->x_pos;
}

int iDrawable::get_y_pos(void){
	return this->y_pos;
}

int iDrawable::get_reference_x(void){
	return this->x_pos + this->reference_horizontal;
}

int iDrawable::get_reference_y(void){
	return this->y_pos + this->reference_vertical;
}

void iDrawable::set_boundary_value(int bounding_width, int bounding_height, int reference_horizontal, int reference_vertical){
	this->bounding_width = bounding_width;
	this->bounding_height = bounding_height;
	this->reference_horizontal = reference_horizontal;
	this->reference_vertical = reference_vertical;
}

int iDrawable::get_bounding_width(void){
	return this->bounding_width; 
}

int iDrawable::get_bounding_height(void){
	return this->bounding_height;
}

World* iDrawable::get_world(void){
	return this->my_world;
}

void iDrawable::set_x_pos(int x){
	this->x_pos = x;
}
void iDrawable::set_y_pos(int y){
	this->y_pos = y;
}

void iDrawable::set_my_type(EntityType et){
	this->my_type = et;
}

EntityType iDrawable::get_my_type(void){
	return this->my_type;
}

Attack* iDrawable::fetch_me_as_attack(void){
	return NULL;
}

Portal* iDrawable::fetch_me_as_portal(void){
	return NULL;
}

Combat* iDrawable::fetch_me_as_combat(void){
	return NULL;
}

void iDrawable::speak(void){};
void iDrawable::append_dialogue(std::string message){};
void iDrawable::clear_dialogue(void){};

void iDrawable::check_collisions(void){
	std::list<iDrawable*>* entities = this->get_world()->get_active_entities();
	int my_x = this->get_reference_x();
	int my_y = this->get_reference_y();
	int my_width = this->get_bounding_width();
	int my_height = this->get_bounding_height();
	int top_bottom_skew = 0;
	int left_right_skew = 0;
	Direction facing = this->get_image()->get_facing();

	if (facing == W || facing == E)
		top_bottom_skew = SKEW_FACTOR;
	if (facing == N || facing == S)
		left_right_skew = SKEW_FACTOR;

	this->can_walk_down = true;
	this->can_walk_left = true;
	this->can_walk_right = true;
	this->can_walk_up = true;

	bool* unblocked_facing;
	if (this->my_type == Hero){
		switch(facing){
	case N:
		unblocked_facing = &this->can_walk_up;
		break;
	case S:
		unblocked_facing = &this->can_walk_down;
		break;
	case E:
		unblocked_facing = &this->can_walk_right;
		break;
	case W:
		unblocked_facing = &this->can_walk_left;
		break;
	default:
		unblocked_facing = &this->can_walk_up;
		// eh whatever
		break;
		}
	} else
		unblocked_facing = &this->can_walk_up;

	int check_x, check_y, check_width, check_height;

	std::list<iDrawable*>::iterator iter;
	std::list<iDrawable*>::iterator end = entities->end();
	for (iter = entities->begin(); iter != end; iter++){
		iDrawable* check = (*iter);
		if (check == this)
			continue;

		check_x = check->get_reference_x();
		check_y = check->get_reference_y();
		check_width = check->get_bounding_width();
		check_height = check->get_bounding_height();

		bool was_unblocked_flag = *unblocked_facing ? true : false;

		this->check_walkable(my_x, my_y, my_height, my_width, 
			check_x, check_y, check_width, check_height, 
			left_right_skew, top_bottom_skew);

		if (this->my_type == Hero && check->can_speak && !(*unblocked_facing) && was_unblocked_flag){
			if (Player_Accessor::hero->wants_to_talk())
				check->speak();
		}

	}


}

void iDrawable::check_walkable(int my_x, int my_y, int my_height, int my_width, int check_x, int check_y, int check_width, int check_height, int left_right_skew, int top_bottom_skew){
	if (my_y <= (check_y + check_height) && (my_y) >= check_y){
		if (check_x <= (my_x + my_width - left_right_skew) && check_x >= (my_x)){
			this->can_walk_up = false;	
		} else if ((my_x + left_right_skew) <= (check_x + check_width) && my_x >= (check_x)){
			this->can_walk_up = false;
		}
	}

	if ((my_y + my_height) >= (check_y) && (my_y + my_width) <= (check_y + check_height)){
		if (check_x <= (my_x + my_width - left_right_skew) && check_x >= (my_x)){
			this->can_walk_down = false;	
		} else if ((my_x + left_right_skew) <= (check_x + check_width) && my_x >= (check_x)){
			this->can_walk_down = false;
		}
	}

	if ((my_x ) <= (check_x + check_width) && (my_x ) >= check_x){
		if (check_y <= (my_y + my_height - top_bottom_skew) && check_y >= (my_y)){
			this->can_walk_left = false;	
		} else if ((my_y + top_bottom_skew) <= (check_y + check_height) && my_y >= (check_y)){
			this->can_walk_left = false;
		}
	}

	if ((my_x + my_width) >= (check_x) && (my_x + my_width) <= (check_x + check_width)){
		if (check_y <= (my_y + my_height - top_bottom_skew) && check_y >= (my_y)){
			this->can_walk_right = false;	
		} else if ((my_y + top_bottom_skew) <= (check_y + check_height) && my_y >= (check_y)){
			this->can_walk_right = false;
		}
	}

}