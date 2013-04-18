#include "Main.h"

using namespace std;

Town_Guard::Town_Guard(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,Direction>>* waypoints)
:Combat(x, y, vel, vel_d, img)
{	
	this->set_boundary_value(28, 14, 0, 18);
	this->waypoints = waypoints;
	this->flip_frames = FRAME_CONST;
	this->patrol_frame_count = 0;
	this->patrol_node = 0;
	this->on_patrol = true;
	this->in_combat = false;
	this->up_or_down = false;
	this->paused  = false;
	this->initial_detection = true;
	this->target = NULL;

}
Town_Guard::~Town_Guard(void){

}

void Town_Guard::update(void){
	check_collisions();	
	
	if(this->on_patrol){	
		patrol();
	}else if(this->in_combat){
		
		move_towards();

		if((this->target->alive)){
			
		}else{
			this->target = NULL;
		}
	}else{
		//

	}
}
	void Town_Guard::patrol(){

		std::pair<int, Direction> current;
		current = this->waypoints->at(this->patrol_node);

		if(current.first < 0){
			this->up_or_down = !this->up_or_down;
		}else{
			//
		}

		if(up_or_down){
			forward(current);
		}else{
			reverse(current);
		}




	}
	void Town_Guard::forward(std::pair<int, Direction> cur_node){
		if(this->patrol_frame_count > 0){
			this->paused = !move(cur_node.second);
			if(!this->paused){
				this->patrol_frame_count -= 1;
			}else{
				return;
			}
		}else if(this->patrol_frame_count == 0){
			this->patrol_node += 1;
			this->patrol_frame_count = this->waypoints->at(this->patrol_node).first;
		}else{
			this->patrol_node += 1;
			this->patrol_frame_count = this->waypoints->at(this->patrol_node).first;
		}
	}

	void Town_Guard::reverse(std::pair<int, Direction> cur_node){
		if(this->patrol_frame_count > 0){
			this->paused = !move(switch_dir(cur_node.second));
			if(!this->paused){
				this->patrol_frame_count -= 1;
			}else{
				return;
			}
		}else if(this->patrol_frame_count == 0){
			this->patrol_node -= 1;
			this->patrol_frame_count = this->waypoints->at(this->patrol_node).first;
		}else{
			this->patrol_node -= 1;
			this->patrol_frame_count = this->waypoints->at(this->patrol_node).first;
		}
	}

	bool Town_Guard::move(Direction new_dir){

		Direction cur_dir = this->image->get_facing();

		if (new_dir != cur_dir) {
			this->image->set_facing(new_dir);
		}

		if(new_dir == N && this->can_walk_up){
			this->y_pos -= MOVEMENT_DELTA;
			this->image->update();
		}else if(new_dir == S && this->can_walk_down){
			this->y_pos += MOVEMENT_DELTA;
			this->image->update();
		}else if(new_dir == E && this->can_walk_right){
			this->x_pos += MOVEMENT_DELTA;
			this->image->update();
		}else if(new_dir == W && this->can_walk_left){
			this->x_pos -= MOVEMENT_DELTA;
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

	void Town_Guard::detect_enemies(iDrawable* to_check){

		EntityType type_check = to_check->my_type;

		if(type_check == Monster || type_check == Outcast){
			this->target = to_check;
			this->on_patrol = false;

		}else if(type_check == Hero){
			if(to_check->get_image()->wearing_mask){
				this->target = to_check;
				this->on_patrol = false;
				this->in_combat = true;
			}else{
				this->on_patrol = true;
				return;
			}
		}else{
			return;
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
				detect_enemies(check);
			}else{
				//
			}

			if (check == this)
				continue;
			check_x = check->get_reference_x();
			check_y = check->get_reference_y();
			check_width = check->get_bounding_width();
			check_height = check->get_bounding_height();



			check_walkable(my_x, my_y, my_height, my_width, 
				check_x, check_y, check_width, check_height, 
				left_right_skew, top_bottom_skew);

		}
	}
	void Town_Guard::find_path(){
		
	}
	bool Town_Guard::move_towards(){
		bool moved;

		int x_dif = (target->x_pos - this->x_pos);
		int y_dif = (target->y_pos - this->y_pos);
		
		//go left and down
		if((x_dif > 0) && (y_dif > 0)){
			if(this->x_or_y){
				moved = move(E);
			}else{
				moved = move(S);
			}
			
			this->flip_frames -= 1;

			if(this->flip_frames == 0){
				this->x_or_y = !(this->x_or_y);
				this->flip_frames = FRAME_CONST;
			}

		//go right and up
		}else if((x_dif > 0) && (y_dif < 0)){
			if(this->x_or_y){
				moved = move(E);
			}else{
				moved = move(N);
			}

			this->flip_frames -= 1;

			if(this->flip_frames == 0){
				this->x_or_y = !(this->x_or_y);
				this->flip_frames = FRAME_CONST;
			}
			
		//go left and down
		}else if((x_dif < 0) && (y_dif > 0)){
			if(this->x_or_y){
				moved = move(W);
			}else{
				moved = move(S);
			}
			
			this->flip_frames -= 1;

			if(this->flip_frames == 0){
				this->x_or_y = !(this->x_or_y);
				this->flip_frames = FRAME_CONST;
			}

		//go left and up
		}else if((x_dif < 0) && (y_dif < 0)){
			if(this->x_or_y){
				moved = move(W);
			}else{
				moved = move(N);
			}

			this->flip_frames -= 1;

			if(this->flip_frames == 0){
				this->x_or_y = !(this->x_or_y);
				this->flip_frames = FRAME_CONST;
			}
		//go right
		}else if((x_dif < 0) && ((-5 < y_dif) && (y_dif< 5))){
			moved = move(W);
		//go left
		}else if((x_dif > 0) && ((-5 < y_dif) && (y_dif< 5))){
			moved = move(E);
		//go up
		}else if(((-5 < x_dif) && (x_dif < 5)) && (y_dif  < 0)){
			moved = move(N);
		//go down
		}else if(((-5 < x_dif) && (x_dif < 5)) && (y_dif  > 0)){
			moved = move(S);
		//you are there
		}else{
			return false;	
		}
		return true;
	}