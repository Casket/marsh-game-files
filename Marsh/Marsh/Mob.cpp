#include "Main.h"

using namespace std;
bool pair_equals(std::pair<int, int> p1, std::pair<int, int> p2);

Mob::Mob(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* waypoints)
:Combat(x, y, vel, vel_d, img)
{	
	this->set_boundary_value(28, 14, 0, 18);
	this->waypoints = waypoints;
	this->patrol_node = 0;
	this->casting = false;
	this->target = NULL;
	this->current_state = patrol;
	this->prev_state = patrol;
	this->melee_cd = 0;
	this->mid_range_cd = 0;
	this->attack_dir = None;
	for(int i = 0; i < 10; i++){
		this->cooldowns[i] = std::make_pair(0, 0);
	}
	this->chill_timer = 0;
	this->priorities = NULL;
}

Mob::~Mob(void){

}

void Mob::update(void){

	update_cd_timers();

	if (this->entangled){
		return;
	}

	bool dest_reached = false;

	if(this->casting){
		casting_update();
		return;
	}

	this->check_collisions();


	Direction dir_moving = this->get_direction_moving();
	bool check = this->get_current_facing_flag(dir_moving);

	int atck;

	switch (this->current_state){
		case chilling_out:
			if (++this->chill_timer >= this->chill_time){
				this->current_state = this->prev_state;
				this->chill_timer = 0;
			}
			break;

		case attack:
			if (target == NULL){
				this->current_state = this->prev_state;
				break;
			}
			if(this->target->alive){
				this->target_area = set_target_area();
			}

			this->current_state = attack_move;

			atck = choose_attack();

			if(atck != -1){
				this->launch_attack(atck);
				this->cooldowns[atck] = std::make_pair(this->cooldowns[atck].second, this->cooldowns[atck].second); 
			}
			break;

		case attack_move:
			if(this->target == NULL){
				this->current_state = patrol;
				break;
			}
			
			this->target_area = set_target_area();
			set_ranges();
			move_towards(this->target_area);

			if(this->in_melee_range || this->in_short_range || this->in_mid_range || this->in_long_range){
				this->current_state = attack;
			}else if(!check){
				this->prev_state = this->current_state;
				this->current_state = detour_intial;
			}

			break;

		case detour_intial:
			if(this->blocking_entity == NULL){
				this->current_state = this->prev_state;
				break;
			}

			if(this->blocking_entity->get_my_type() == this->get_my_type()){
				if(this->blocking_entity->fetch_me_as_mob()->current_state != chilling_out){
					this->current_state = chilling_out;
					int distance_needed = this->blocking_entity->get_bounding_width() + this->blocking_entity->get_bounding_height();
					this->chill_time = (distance_needed / this->velocity);
					//break;
				}
			}

			this->detour_pair = this->detour_obstruction();
			this->current_state = detour_one;


			break;

		case detour_one:
			if(this->blocking_entity != NULL){
				if(this->blocking_entity->x_pos < 0){
					this->current_state = this->prev_state;
					break;
				}
			}

			dest_reached = move_towards(this->detour_pair.first);
			
			dir_moving = this->get_direction_moving();
			check = this->get_current_facing_flag(dir_moving);
			
			if(!check){
				this->current_state = detour_intial;
				break;
			}
			
			if(dest_reached){
				this->current_state = detour_two; 		
			}
			break;

		case detour_two:
			dest_reached = move_towards(this->detour_pair.second);
			
			dir_moving = this->get_direction_moving();
			check = this->get_current_facing_flag(dir_moving);

			if(!check){
				this->current_state = detour_intial;
				break;
			}

			if(dest_reached){
				this->blocking_entity = NULL;
				this->current_state = this->prev_state;
			}

			break;

		case patrol:

			if (this->waypoints == NULL)
				break;

			dest_reached = move_towards(this->waypoints->at(this->patrol_node));
			
			this->prev_state = this->current_state;
			dir_moving = this->get_direction_moving();
			check = this->get_current_facing_flag(dir_moving);

			if(!check){
				this->current_state = detour_intial;
			}

			if(dest_reached){
				this->increment_patrol();
			}

			break;
	}
}
bool pair_equals(std::pair<int, int> p1, std::pair<int, int> p2){
	if(p1.first == p2.first && p1.second == p2.second){
		return true;
	}
	return false;
}
void Mob::increment_patrol(void){

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

std::pair<std::pair<int, int>, std::pair<int, int>> Mob::detour_obstruction(void){
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



bool Mob::move(Direction new_dir){

	Direction cur_dir = this->image->get_facing();

	if (new_dir != cur_dir) {
		this->image->set_facing(new_dir);
	}

	if(++this->movement_counter != this->velocity_delay){
		return false;
	}

	this->movement_counter = 0;

	if(new_dir == N && this->can_walk_up){
		this->y_pos -= this->velocity;
		this->image->update();
	}else if(new_dir == S && this->can_walk_down){
		this->y_pos += this->velocity;
		this->image->update();
	}else if(new_dir == E && this->can_walk_right){
		this->x_pos += this->velocity;
		this->image->update();
	}else if(new_dir == W && this->can_walk_left){
		this->x_pos -= this->velocity;
		this->image->update();
	}else{
		return false;
	}

	return true;

}
Direction Mob::switch_dir(Direction old_dir){
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

bool Mob::detect_enemies(iDrawable* to_check){

	EntityType type_check = to_check->my_type;

	if(type_check == Monster || type_check == Vamp || type_check == Guard || type_check == Outcast){
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

bool Mob::get_visible_to_guard(iDrawable* current){

	float distance = (((this->x_pos - current->x_pos) * (this->x_pos - current->x_pos)) + 
		((this->y_pos - current->y_pos) * (this->y_pos - current->y_pos)));

	distance = sqrt(distance);

	if(distance < DETECTION_RANGE){
		return true;
	}else{
		return false;
	}
}


void Mob::check_collisions(void){
	this->other_check_collisions();

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
void Mob::other_check_collisions(void){

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
	this->blocking_entity = NULL;

	int check_x, check_y, check_width, check_height;

	std::list<iDrawable*>::iterator iter;
	std::list<iDrawable*>::iterator end = entities->end();
	std::list<iDrawable*>* targets = new std::list<iDrawable*>();

	for (iter = entities->begin(); iter != end; iter++){
		iDrawable* check = (*iter);

		if (check == this){
			continue;
		}

		if (check->my_type == Wallop){
			Attack* att_check = check->fetch_me_as_attack();
			if (att_check != NULL){
				if (att_check->my_caster->my_type == this->my_type)
					continue;
			}
		}

		check_x = check->get_reference_x();
		check_y = check->get_reference_y();
		check_width = check->get_bounding_width();
		check_height = check->get_bounding_height();

		bool previously_not_blocked = get_current_facing_flag(this->image->get_facing());

		if(this->get_visible_to_guard(check)){
			if(this->detect_enemies(check)){
				targets->push_back(check);
			}
		}

		check_walkable(my_x, my_y, my_height, my_width, 
			check_x, check_y, check_width, check_height, 
			left_right_skew, top_bottom_skew);

		if (previously_not_blocked && !get_current_facing_flag(this->image->get_facing())){
			this->blocking_entity = check;
		}
	}

	end = targets->end();
	int closest = 1000000000;

	if(targets->size() > 0 && (this->current_state != attack_move && this->current_state != attack) && this->prev_state != attack_move){
		for(iter = targets->begin(); iter != end; iter++){
			iDrawable* potential_tar = (*iter);
			if(potential_tar->get_my_type() == Hero){
				this->target = potential_tar;
				this->current_state = attack_move;
				this->check_target_dispatch();
				break;
			}else{

				float distance = (((this->x_pos - potential_tar->x_pos) * (this->x_pos - potential_tar->x_pos)) + 
					((this->y_pos - potential_tar->y_pos) * (this->y_pos - potential_tar->y_pos)));
				distance = sqrt(distance);

				if(distance < closest){
					this->target = potential_tar;				
				}
			} 
		}
		return;
	}else if(targets->size() > 0){
		return;
	}else{
		this->target = NULL;

		if(this->current_state == attack_move || this->current_state == attack){
			this->current_state = patrol;
		}
		return;
	}
}

void Mob::check_target_dispatch(void){
	/*
	Combat* targ = this->target->fetch_me_as_combat();
	if (targ == NULL)
		return;
	std::list<Dispatcher*>* dispatches = targ->active_dispatchers;
	std::list<Dispatcher*>::iterator end = dispatches->end();
	for (std::list<Dispatcher*>::iterator iter = dispatches->begin(); iter != end; ++iter){
		if ((*iter)->designated_type == this->my_type)
			return;
	}

	// the fact that we did not return means the target lacks an appropriate dispatcher for me... the bastard
	Dispatcher* dispatch = new Dispatcher(targ, this->my_type);
	dispatches->push_back(dispatch);
	*/
}

bool Mob::get_current_facing_flag(Direction dir){
	switch(dir){
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

bool Mob::move_towards(std::pair<int, int> coordinate){
	bool moved;

	int x_dif = (coordinate.first - this->get_reference_x());
	int y_dif = (coordinate.second - this->get_reference_y());
	int dif_x_y = std::abs(x_dif) - std::abs(y_dif);
	int my_width = this->get_bounding_width();
	int my_height = this->get_bounding_height();
	//you are there
	if(((0-ERR < x_dif) && (x_dif < ERR)) && ((0-ERR < y_dif) && (y_dif< ERR))){
		return true; // reached destination

		//go left and down
	}else if((x_dif >= 0) && (y_dif >= 0)){
		if(dif_x_y > 0){
			moved = move(E);
		}else{
			moved = move(S);
		}

		//go right and up
	}else if((x_dif >= 0) && (y_dif <= 0)){
		if(dif_x_y > 0){
			moved = move(E);
		}else{
			moved = move(N);
		}


		//go left and down
	}else if((x_dif <= 0) && (y_dif >= 0)){
		if(dif_x_y > 0){
			moved = move(W);
		}else{
			moved = move(S);
		}

		//go left and up
	}else if((x_dif <= 0) && (y_dif <= 0)){
		if(dif_x_y > 0){
			moved = move(W);
		}else{
			moved = move(N);
		}
	}

	return false;
}

Direction Mob::get_direction_moving(){

	int x_dif = (this->target_area.first - this->get_reference_x());
	int y_dif = (this->target_area.second - this->get_reference_y());
	int dif_x_y = std::abs(x_dif) - std::abs(y_dif);
	int my_width = this->get_bounding_width();
	int my_height = this->get_bounding_height();

	Direction moving = None;
	if(((0-ERR < x_dif) && (x_dif < ERR)) && ((0-ERR < y_dif) && (y_dif< ERR))){
		return None;// reached destination
		//go left and down
	}else if((x_dif >= 0) && (y_dif >= 0)){
		if(dif_x_y > 0){
			moving = E;
		}else{
			moving = S;
		}

		//go right and up
	}else if((x_dif >= 0) && (y_dif <= 0)){
		if(dif_x_y > 0){
			moving = E;
		}else{
			moving = N;
		}
		//go left and down
	}else if((x_dif <= 0) && (y_dif >= 0)){
		if(dif_x_y > 0){
			moving = W;
		}else{
			moving = S;
		}

		//go left and up
	}else if((x_dif <= 0) && (y_dif <= 0)){
		if(dif_x_y > 0){
			moving = W;
		}else{
			moving = N;
		}
	}
	
	return moving == None ? this->get_image()->get_facing() : moving;

}

void Mob::set_ranges(void){
	this->in_melee_range = this->check_variable_range(NO_RANGE);
	this->in_short_range = false;
	this->in_mid_range = false;
	this->in_long_range = false;
}

int Mob::choose_attack(void){
	if(this->cooldowns[0].first == 0){
		return 0;
	}

	return -1;
}
bool Mob::check_variable_range(int dist){

	Direction cur_dir = this->get_direction_moving(); 
	int tar_x = this->target->x_pos;
	int tar_y = this->target->y_pos;

	if(cur_dir == N){
		int y_dif = this->y_pos - this->target->y_pos - 32;
		int x_dif = this->target->x_pos - this->x_pos;
		if(y_dif < dist && (abs(x_dif) < dist)){
			return true;
		}else{
			return false;
		}
	}else if(cur_dir == S){
		int y_dif = this->target->y_pos - this->y_pos - 32;
		int x_dif = this->target->x_pos - this->x_pos;
		if(y_dif < dist && (abs(x_dif) < dist)){
			return true;
		}else{
			return false;
		}
	}else if(cur_dir == W){
		int x_dif = (this->x_pos - this->target->x_pos) - 32;
		int y_dif = this->y_pos - this->target->y_pos;
		if(x_dif < dist && (abs(y_dif) < dist)){
			return true;
		}else{
			return false;
		}
	}else{
		int x_dif = this->target->x_pos - this->x_pos - 32;
		int y_dif = this->y_pos - this->target->y_pos;
		if(x_dif < dist && (abs(y_dif) < dist)){
			return true;
		}else{
			return false;
		}
	}
}
std::pair<int,int> Mob::set_target_area(void){
	if(this->attack_dir == N){
		return std::make_pair(this->target->get_reference_x(), this->target->get_reference_y() - 10);
	}else if(this->attack_dir == S){
		return std::make_pair(this->target->get_reference_x(), this->target->get_reference_y() + 10);
	}else if(this->attack_dir == E){
		return std::make_pair(this->target->get_reference_x() + 10, this->target->get_reference_y());
	}else if(this->attack_dir == W){
		return std::make_pair(this->target->get_reference_x() - 10, this->target->get_reference_y());
	}
	return std::make_pair(this->target->get_reference_x(), this->target->get_reference_y());
}

void Mob::update_cd_timers(void){
	for(int i = 0; i < 10; i++){
		if(this->cooldowns[i].second != -1){
			if(this->cooldowns[i].first > 0){
				this->cooldowns[i] = std::make_pair(this->cooldowns[i].first - 1, this->cooldowns[i].second);

			}
		}
	}
}

void Mob::set_attack_plan(void){
	std::vector<Direction> pris;

	for (int i = 0; i < 4; i++)
		pris.push_back(N);

	/*	if(this->attack_dir != None){
	return;
	}
	*/
	//define priorities

	int x_axis = this->target->get_reference_y();//horizontal
	int y_axis = this->target->get_reference_x();//vertical

	Direction check = None;

	int upward_dif = this->get_reference_y() - x_axis;
	int horz_dif = this->get_reference_x() - y_axis;

	if(upward_dif < 0){
		if(horz_dif < 0){
			check = NW;
		}else if(horz_dif > 0){
			check = NE;
		}else{
			check = N;
		}
	}else if(upward_dif > 0){
		if(horz_dif < 0){
			check = SW;
		}else if(horz_dif > 0){
			check = SE;
		}else{
			check = S;
		}

	}else{
		if(horz_dif < 0){
			check = W;
		}else if(horz_dif > 0){
			check = E;
		}else{
			//put something here??? Should be made not possible
		}
	}

	if(check == N || check == S || check == W || check == E){
		pris[0] = check;
		if(check == N){
			pris[1] = W;
			pris[2] = E;
			pris[3] = S;
		}else if(check == S){
			pris[1] = E;
			pris[2] = W;
			pris[3] = N;
		}else if(check == W){
			pris[1] = S;
			pris[2] = N;
			pris[3] = E;
		}else{
			pris[1] = N;
			pris[2] = S;
			pris[3] = W;
		}
	}else{
		if(check == NW){
			if(std::abs(upward_dif) > std::abs(horz_dif)){
				pris[0] = N;
				pris[1] = W;
				pris[2] = E;
				pris[3] = S;
			}else{
				pris[0] = W;
				pris[1] = N;
				pris[2] = S;
				pris[3] = E;
			}
		}else if(check == NE){
			if(std::abs(upward_dif) > std::abs(horz_dif)){
				pris[0] = N;
				pris[1] = E;
				pris[2] = W;
				pris[3] = S;
			}else{
				pris[0] = E;
				pris[1] = N;
				pris[2] = S;
				pris[3] = W;
			}
		}else if(check == SW){
			if(std::abs(upward_dif) > std::abs(horz_dif)){
				pris[0] = S;
				pris[1] = W;
				pris[2] = E;
				pris[3] = N;
			}else{
				pris[0] = W;
				pris[1] = S;
				pris[2] = N;
				pris[3] = E;
			}

		}else if(check = SE){
			if(std::abs(upward_dif) > std::abs(horz_dif)){
				pris[0] = S;
				pris[1] = E;
				pris[2] = W;
				pris[3] = N;
			}else{
				pris[0] = E;
				pris[1] = S;
				pris[2] = N;
				pris[3] = W;
			}

		}
	}

	std::list<Direction>* list_pris = new std::list<Direction>();
	for(int i = 0; i < pris.size(); i++){
		list_pris->push_back(pris.at(i));
	}

	if(this->priorities != NULL){	
		delete this->priorities;
	}

	this->priorities = list_pris;
}
Direction Mob::get_target_true_dir(){

	int x_dif = (this->target->get_reference_x() - this->get_reference_x());
	int y_dif = (this->target->get_reference_y() - this->get_reference_y());

	int my_width = this->get_bounding_width();
	int my_height = this->get_bounding_height();

	//go left
	if((x_dif < 0) && ((-5 < y_dif) && (y_dif < 5))){
		return W;
		//go right
	}else if((x_dif > 0) && ((-5 < y_dif) && (y_dif< 5))){
		return E;
		//go up
	}else if(((-5 < x_dif) && (x_dif < 5)) && (y_dif  < 0)){
		return N;
		//go down
	}else if(((-5 < x_dif) && (x_dif < 5)) && (y_dif  > 0)){
		return S;
	}

	return this->image->get_facing();

}

Mob* Mob::fetch_me_as_mob(void){
	return this;
}