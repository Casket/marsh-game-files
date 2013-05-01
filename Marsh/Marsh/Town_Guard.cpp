#include "Main.h"

using namespace std;
bool pair_equals(std::pair<int, int> p1, std::pair<int, int> p2);

Town_Guard::Town_Guard(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* waypoints)
:Combat(x, y, vel, vel_d, img)
{	
	this->set_boundary_value(28, 14, 0, 18);
	this->waypoints = waypoints;
	this->flip_frames = FRAME_CONST;
	this->patrol_node = 0;
	this->casting = false;
	this->in_combat = false;
	this->target = NULL;
	this->movement_blocked = false;
	this->detour = false;
	

}
Town_Guard::~Town_Guard(void){

}

void Town_Guard::update(void){

	if(this->casting){
		casting_update();
		return;
	}
	
	check_collisions();

	bool check = this->get_current_facing_flag();

	if(check && !this->detour){
		if(this->in_combat){
			this->target_area = std::make_pair(this->target->get_x_pos(), this->target->get_y_pos());
			
		}else if(this->on_patrol){
			this->target_area = this->waypoints->at(this->patrol_node);
		}else{
			//
		}
	}else{
		if(this->blocking_entity == NULL){
			//tell him he is the juggernaut
		}
		else if(this->blocking_entity->my_type == Hero && in_combat){
			this->launch_attack(0);
		}
		else if(this->detour){
			if(!check){
				this->detour_pair = this->detour_obstruction();
				this->target_area = this->detour_pair.first;
			}else{

			}
		}else{
			this->detour_pair = this->detour_obstruction();
			this->target_area = this->detour_pair.first;
			this->detour = true;
		}
	}



	bool reached_dest = this->move_towards(this->target_area);
	
	if(reached_dest){
		if(this->in_combat){
			this->launch_attack(0);
		}
		else if(this->detour){
			if(pair_equals(this->target_area, this->detour_pair.first)){
				this->target_area = this->detour_pair.second;
			}else{
				this->detour= false;
			}
		}else if(on_patrol){
			this->increment_patrol();
		}
	}else{
		
	}

/*
	if (this->movement_blocked){
		std::pair<int, int> unblocked_coord = detour_obstruction();
		
		bool reached_dest = this->move_towards(unblocked_coord);
		
		if(reached_dest){
			this->movement_blocked = false;
		}
		return;
	}

	if (this->in_combat){
		// has found a target to attack, let's move toward said target
		bool reached = this->move_towards(
			std::pair<int, int>(this->target->get_reference_x(), this->target->get_reference_y()));
		if(reached){
			this->launch_attack(0);
		}

		return;
	}

	bool reached_destination = this->move_towards(this->waypoints->at(this->patrol_node));

	if (reached_destination){
		this->increment_patrol();
	}

	*/
}
bool pair_equals(std::pair<int, int> p1, std::pair<int, int> p2){
	if(p1.first == p2.first && p1.second == p2.second){
		return true;
	}
	return false;
}
void Town_Guard::increment_patrol(void){

	if (this->patrolling_forward){
		this->patrol_node++;
		if (this->patrol_node >= this->waypoints->size()){
			this->patrol_node--;
			this->patrolling_forward = false;
		}
	} else {
		this->patrol_node--;
		if (this->patrol_node < 0){
			this->patrol_node++;
			this->patrolling_forward = true;
		}
	}	
}

std::pair<std::pair<int, int>, std::pair<int, int>> Town_Guard::detour_obstruction(void){
	// start off figuring out which direction to go
	std::pair<int, int> target_coord_one, target_coord_two;
	int blocker_x = this->blocking_entity->get_reference_x();
	int blocker_y = this->blocking_entity->get_reference_y();


	int blocker_width = this->blocking_entity->get_bounding_width();
	int blocker_height = this->blocking_entity->get_bounding_height();

	Direction blocked_way = this->image->get_facing();

	if(blocked_way == E || blocked_way == W){
		int mid = blocker_y + (blocker_height/2);
		if(this->get_reference_y() >= mid){
			target_coord_one = make_pair(this->get_reference_x(), blocker_y + blocker_height + ERR);
		}else{
			target_coord_one = make_pair(this->get_reference_x(), blocker_y - this->get_bounding_height() - ERR);
		}
		if(blocked_way == E){target_coord_two = make_pair(blocker_x + this->get_bounding_width() + blocker_width + ERR, target_coord_one.second);}
		if(blocked_way == W){target_coord_two = make_pair(blocker_x - this->get_bounding_width() - ERR, target_coord_one.second);}

	}else if(blocked_way == S || blocked_way == N){
		int mid = blocker_x + (blocker_width/2);
		if (this->get_reference_x() >= mid){
			target_coord_one = make_pair(blocker_x + blocker_width + ERR, this->get_reference_y());
		} else {
			target_coord_one = make_pair( blocker_x - this->get_bounding_width() - ERR,this->get_reference_y());
		}
		if(blocked_way == N){target_coord_two = make_pair(target_coord_one.first, blocker_y - this->get_bounding_height()- blocker_height - ERR);}
		if(blocked_way == S){target_coord_two = make_pair(target_coord_one.first, blocker_y + blocker_height + ERR);}
	}

	return make_pair(target_coord_one, target_coord_two);
}



bool Town_Guard::move(Direction new_dir){

	Direction cur_dir = this->image->get_facing();

	if (new_dir != cur_dir) {
		this->image->set_facing(new_dir);
	}

	if(new_dir == N && this->can_walk_up){
		this->y_pos -= TG_DELTA;
		this->image->update();
	}else if(new_dir == S && this->can_walk_down){
		this->y_pos += TG_DELTA;
		this->image->update();
	}else if(new_dir == E && this->can_walk_right){
		this->x_pos += TG_DELTA;
		this->image->update();
	}else if(new_dir == W && this->can_walk_left){
		this->x_pos -= TG_DELTA;
		this->image->update();
	}else{
		return false;
	}

	return true;

}
Direction Town_Guard::switch_dir(Direction old_dir){
	if(old_dir == N){
		return S;
	}else if(old_dir == S){
		return N;
	}else if(old_dir == E){
		return W;
	}else{
		return E;
	}
}

bool Town_Guard::detect_enemies(iDrawable* to_check){

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

bool Town_Guard::get_visible_to_guard(iDrawable* current){

	float distance = (((this->x_pos - current->x_pos) * (this->x_pos - current->x_pos)) + 
		((this->y_pos - current->y_pos) * (this->y_pos - current->y_pos)));

	distance = sqrt(distance);

	if(distance < DETECTION_RANGE){
		return true;
	}else{
		return false;
	}
}


void Town_Guard::check_collisions(void){
	other_check_collisions();

	Tile*** map = this->get_world()->get_tile_map();
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

	Tile* nearby[4];

	int delta = 2;

	// what tile is his top left corner in?
	nearby[0] = map[(this->get_reference_y() - delta) / TILE_SIZE][(this->get_reference_x()- delta) / TILE_SIZE];

	// top right?
	nearby[1] = map[(this->get_reference_y() - delta) / TILE_SIZE][(this->get_reference_x() + this->get_bounding_width() + delta) / TILE_SIZE];

	// bottom left
	nearby[2] = map[(this->get_reference_y() + this->get_bounding_height() + delta) / TILE_SIZE][(this->get_reference_x() - delta) / TILE_SIZE];

	// bottom right
	nearby[3] = map[(this->get_reference_y() + this->get_bounding_height() + delta) / TILE_SIZE][(this->get_reference_x() + this->get_bounding_width() + delta) / TILE_SIZE];

	for (int i=0; i < 4; i++){
		if (!nearby[i]->can_walk)
			check_walkable(my_x, my_y, my_height, my_width, 
			nearby[i]->col*TILE_SIZE, nearby[i]->row*TILE_SIZE, TILE_SIZE, TILE_SIZE, 
			left_right_skew, top_bottom_skew);
	}
}
void Town_Guard::other_check_collisions(void){

	std::list<iDrawable*>* entities = this->my_world->get_active_entities();
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

	int check_x, check_y, check_width, check_height;

	std::list<iDrawable*>::iterator iter;
	std::list<iDrawable*>::iterator end = entities->end();

	for (iter = entities->begin(); iter != end; iter++){
		iDrawable* check = (*iter);

		if(get_visible_to_guard(check)){
			if(detect_enemies(check)){
				this->target = check;
				this->in_combat = true;
			}else{
				//
			}
		}else{
			continue;
		}

		if (check == this)
			continue;

		check_x = check->get_reference_x();
		check_y = check->get_reference_y();
		check_width = check->get_bounding_width();
		check_height = check->get_bounding_height();

		bool previously_not_blocked = get_current_facing_flag();

		check_walkable(my_x, my_y, my_height, my_width, 
			check_x, check_y, check_width, check_height, 
			left_right_skew, top_bottom_skew);

		if (previously_not_blocked && !get_current_facing_flag()){
			// this uber if means that the guard has been blocked by the current 
			//entity which is guranteed to not be the player, maybe, hell if i know
			this->movement_blocked = true;
			this->blocking_entity = check;
		}

	}
}

bool Town_Guard::get_current_facing_flag(void){
	switch(this->image->get_facing()){
	case N:
		return this->can_walk_up;
	case S:
		return this->can_walk_down;
	case E:
		return this->can_walk_right;
	case W:
		return this->can_walk_left;
	default:
		return true;
	}
}

bool Town_Guard::move_towards(std::pair<int, int> coordinate){
	bool moved;

	int x_dif = (coordinate.first - this->get_reference_x());
	int y_dif = (coordinate.second - this->get_reference_y());

	int my_width = this->get_bounding_width();
	int my_height = this->get_bounding_height();
	//you are there
	if(((0-ERR < x_dif) && (x_dif < ERR)) && ((0-ERR < y_dif) && (y_dif< ERR))){
		return true; // reached destination
	}
	//go left and down
	else if((x_dif > 0) && (y_dif > 0)){
		if(this->x_or_y){
			moved = move(E);
		}else{
			moved = move(S);
		}
		/*this->flip_frames -= 1; 
		if(this->flip_frames == 0){ 
			this->x_or_y = !(this->x_or_y); 
			this->flip_frames = FRAME_CONST; 
		}*/

		//go right and up
	}else if((x_dif > 0) && (y_dif < 0)){
		if(this->x_or_y){
			moved = move(E);
		}else{
			moved = move(N);
		}
		/*this->flip_frames -= 1; 
		if(this->flip_frames == 0){ 
			this->x_or_y = !(this->x_or_y); 
			this->flip_frames = FRAME_CONST; 
		}*/


		//go left and down
	}else if((x_dif < 0) && (y_dif > 0)){
		if(this->x_or_y){
			moved = move(W);
		}else{
			moved = move(S);
		}
		/*this->flip_frames -= 1; 
		if(this->flip_frames == 0){ 
			this->x_or_y = !(this->x_or_y); 
			this->flip_frames = FRAME_CONST; 
		}*/


		//go left and up
	}else if((x_dif < 0) && (y_dif < 0)){
		if(this->x_or_y){
			moved = move(W);
		}else{
			moved = move(N);
		}

		/*this->flip_frames -= 1; 
		if(this->flip_frames == 0){ 
			this->x_or_y = !(this->x_or_y); 
			this->flip_frames = FRAME_CONST; 
		}*/

		//go left
	}else if((x_dif < 0) && ((-5 < y_dif) && (y_dif < 5))){
		moved = move(W);
		//go right
	}else if((x_dif > 0) && ((-5 < y_dif) && (y_dif< 5))){
		moved = move(E);
		//go up
	}else if(((-5 < x_dif) && (x_dif < 5)) && (y_dif  < 0)){
		moved = move(N);
		//go down
	}else if(((-5 < x_dif) && (x_dif < 5)) && (y_dif  > 0)){
		moved = move(S);
	}

	return false;
}




