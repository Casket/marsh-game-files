#include "Main.h"

TeleportAttack::TeleportAttack(int x, int y, int vel, int vel_d, Sprite* img, AttackStatistics stats, int distance)
:Attack(x, y, vel, vel_d, img, stats)
{
	this->distance = distance;
}

TeleportAttack::~TeleportAttack(void){

}

void TeleportAttack::update(void){
	// as soon as the teleport hits the world, it's time to GO GO PEW PEW
	int x = this->my_caster->get_x_pos();
	int y = this->my_caster->get_y_pos();
	switch(this->get_image()->get_facing()){
	case N:
		y -= this->distance;
		break;
	case S:
		y += this->distance;
		break;
	case W:
		x -= this->distance;
		break;
	case E:
		x += this->distance;
		break;
	}

	if (check_new_coord(x, y)){
		this->my_caster->set_x_pos(x);
		this->my_caster->set_y_pos(y);
	} else {
		Player_Accessor::get_player()->display_to_user("I cannot teleport there.  The path is blocked.");
	}

	// since he moved, it's time to die
	// farewell cruel world
	this->get_world()->remove_entity(this);
}

bool check_walkable_area(int my_x, int my_y, int my_width, int my_height, int check_x, int check_y, int check_width, int check_height){
	if (my_y <= (check_y + check_height) && (my_y) >= check_y){
		if (check_x <= (my_x + my_width) && check_x >= (my_x)){
			return true;
		} else if ((my_x) <= (check_x + check_width) && my_x >= (check_x)){
			return true;
		}
	}

	if ((my_y + my_height) >= (check_y) && (my_y + my_width) <= (check_y + check_height)){
		if (check_x <= (my_x + my_width) && check_x >= (my_x)){
			return true;	
		} else if ((my_x) <= (check_x + check_width) && my_x >= (check_x)){
			return true;
		}
	}

	if ((my_x ) <= (check_x + check_width) && (my_x ) >= check_x){
		if (check_y <= (my_y + my_height) && check_y >= (my_y)){
			return true;
		} else if ((my_y) <= (check_y + check_height) && my_y >= (check_y)){
			return true;
		}
	}

	if ((my_x + my_width) >= (check_x) && (my_x + my_width) <= (check_x + check_width)){
		if (check_y <= (my_y + my_height) && check_y >= (my_y)){
			return true;
		} else if ((my_y) <= (check_y + check_height) && my_y >= (check_y)){
			return true;
		}
	}
	return false;
}

bool drawable_in_drawable(int x, int y, iDrawable* target, iDrawable* check){
	int check_x = check->get_reference_x();
	int check_y = check->get_reference_y();
	int check_width = check->get_bounding_width();
	int check_height = check->get_bounding_height();

	int my_x = x;
	int my_y = y;
	int my_width = target->get_bounding_width();
	int my_height = target->get_bounding_height();

	return check_walkable_area(my_x, my_y, my_width, my_height, check_x, check_y, check_width, check_height);
}

bool check_walkable_tiles(int x, int y, iDrawable* target){
	Tile*** map = target->get_world()->get_tile_map();
	int my_x = x;
	int my_y = y;
	int my_width = target->get_bounding_width();
	int my_height = target->get_bounding_height();
	int top_bottom_skew = 0;
	int left_right_skew = 0;
	Direction facing = target->get_image()->get_facing();

	Tile* nearby[4];

	int delta = 2;

	// what tile is his top left corner in?
	nearby[0] = map[(y - delta) / TILE_SIZE][(x- delta) / TILE_SIZE];

	// top right?
	nearby[1] = map[(y - delta) / TILE_SIZE][(x + my_width + delta) / TILE_SIZE];

	// bottom left
	nearby[2] = map[(y + my_height + delta) / TILE_SIZE][(x - delta) / TILE_SIZE];

	// bottom right
	nearby[3] = map[(y + my_height + delta) / TILE_SIZE][(x + my_width + delta) / TILE_SIZE];

	for (int i=0; i < 4; i++){
		if (!nearby[i]->can_walk)
			if (check_walkable_area(my_x, my_y, my_height, my_width, 
				nearby[i]->col*TILE_SIZE, nearby[i]->row*TILE_SIZE, TILE_SIZE, TILE_SIZE))
				return true;
	}
	return false;

}

bool TeleportAttack::check_new_coord(int x, int y){
	iDrawable* target = this->my_caster;

	std::list<iDrawable*>* entities = this->get_world()->get_active_entities();
	std::list<iDrawable*>::iterator end = entities->end();

	if (check_walkable_tiles(x, y, target))
		return false;

	for (std::list<iDrawable*>::iterator iter = entities->begin(); iter != end; ++iter){
		iDrawable* check = (*iter);
		if (check == target)
			continue;
		if (check->my_type == Wallop)
			continue;
		if (check->get_bounding_width() == 0 && check->get_bounding_height() == 0)
			continue;
		if(drawable_in_drawable(x, y, target, check))
			return false;
	}
	return true;
}

Attack* TeleportAttack::clone(int x, int y, Direction dir){
	Sprite* image = this->get_image()->clone(W);
	image->set_facing(dir);
	AttackStatistics stats;
	stats.base_damage = this->base_damage;
	stats.charge_time = this->charge_time;
	stats.exp_date = this->expiration_date;
	stats.penetration = this->penetration;
	stats.range = this->range;
	stats.tree_depth = this->tree_depth_level;


	TeleportAttack* dup = new TeleportAttack(x, y, this->velocity, this->velocity_delay, image, stats, this->distance);
	dup->set_boundary_value(this->get_bounding_width(), this->get_bounding_height(), this->reference_horizontal, this->reference_vertical);
	dup->set_my_caster(this->my_caster);
	dup->set_position_adjustment(this->x_adjustment, this->y_adjustment);

	switch(dir){
	case N:
		x -= this->x_adjustment;
		y -= this->y_adjustment;
		break;
	case S:
		x -= this->x_adjustment;
		break;
	case W:
		x -= this->x_adjustment;
		y -= this->y_adjustment;
		break;
	case E:
		y -= this->y_adjustment;
		break;
	}


	dup->set_x_pos(x);
	dup->set_y_pos(y);


	return dup;

}