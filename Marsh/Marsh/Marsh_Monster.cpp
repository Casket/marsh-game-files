#include "Main.h"


using namespace std;


Marsh_Monster::Marsh_Monster(int x, int y, int vel, int vel_d, Sprite* img, std::vector<std::pair<int,int>>* waypoints)
:Mob(x, y,vel, vel_d, img, waypoints){
	
	this->my_type = Monster;

}
Marsh_Monster::~Marsh_Monster(void){

	
}


void Marsh_Monster::check_collisions(void){
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
		if (!nearby[i]->can_walk && !nearby[i]->flyable){
				check_walkable(my_x, my_y, my_height, my_width, 
				nearby[i]->col*TILE_SIZE, nearby[i]->row*TILE_SIZE, TILE_SIZE, TILE_SIZE, 
				left_right_skew, top_bottom_skew);
			}
	}
}

void choose_attack(void){
	
	

}
