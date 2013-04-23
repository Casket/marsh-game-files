#include "Main.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

World::World(WorldName this_world){

	this->tiles_high = 20;
	this->tiles_wide = 20;
	this->my_name = this_world;
	this->active_entities = new std::list<iDrawable*>();
	this->visible_entities = new std::list<iDrawable*>();
	this->removal_queue = new std::list<iDrawable*>();
	load_world();
}

World::~World(void){
	int rows = this->tiles_high;
	int cols = this->tiles_wide;

	for (int i=0; i < rows; i++) {
		for (int j=0; j < cols; j++){
			free(this->tile_map[i][j]);
		}
	}

	for (int i=0; i < rows; i++){
		free(this->tile_map[i]);
	}

	free(this->tile_map);

	delete this->active_entities;
	delete this->visible_entities;
}

void World::load_world(){

	char back_ground_tiles[700];

	//opens the file-stream
	fstream fin;

	char* filename = get_file();

	fin.open(filename, ios::in);


	if(fin.is_open()){

		int row_count = 0;
		int other_row_count = 0;

		//begin moving line by line down the file
		while(!fin.eof()){

			fin.getline(back_ground_tiles, 500);

			if (strcmp(back_ground_tiles, "") == 0){
				break;
			}

			//checks if the line is a 2nd layer drawable or a background

			if(other_row_count == 0){

				char* x = (char*)malloc((sizeof(char)) * 8);
				char* y = (char*)malloc((sizeof(char)) * 8);
				int outer_index = 0;
				int inner_index = 0;

				//grabs xpos
				while(back_ground_tiles[outer_index] != '+'){
					x[inner_index] = back_ground_tiles[outer_index];
					inner_index += 1;
					outer_index += 1;
				}
				//grabs ypos and records num of digits xpos was 
				outer_index += 1;
				int size_x = inner_index;
				inner_index = 0;

				while(back_ground_tiles[outer_index] != '+' && back_ground_tiles[outer_index] != '!'){
					y[inner_index] = back_ground_tiles[outer_index];
					inner_index += 1;
					outer_index += 1;
				}

				this->tiles_wide = list_to_int(x,size_x); 
				this->tiles_high = list_to_int(y,inner_index);

				make_world();
				other_row_count+=1;

			}else if(back_ground_tiles[0] == '@'){

				designate_drawable(back_ground_tiles);

				//if background tile line formmated so that 01010101 is a rep of 4 of the same tile
			}else if(back_ground_tiles[0] == '#'){
				
				make_portal(back_ground_tiles);

			}else{
				int size = 2*this->tiles_wide;

				char first = ' ';
				char second = ' ';

				//loops through the line grabbing every char pair and creating the neccessary tile and then adding it to the class object map array 
				for(int i = 0; i < size ; i+=2){
					first = back_ground_tiles[i];
					second = back_ground_tiles[i+1];
					convert_to_tile(first, second, row_count,i/2);
				}
				row_count += 1;
			}
		}

		fin.close();
	}else{
		return;
	}
}
Tile*** World::get_tile_map(void){
	return this->tile_map;
}

int World::get_tiles_wide(void){
	return this->tiles_wide;

}
int World::get_tiles_high(){
	return this->tiles_high;
}

void World::set_player(Player* p){
	this->playa = p;
}


//takes in the tile code and then converts it to the actual tile
void World::convert_to_tile(char a, char b, int pos_x, int pos_y){
	int sprite_x = -1;
	int sprite_y = -1;

	char* file = (char*)malloc(sizeof(char) * 100);

	this->tile_map[pos_x][pos_y]->row = pos_x;
	this->tile_map[pos_x][pos_y]->col = pos_y;
	this->tile_map[pos_x][pos_y]->can_walk = true;

	if(a == '0'){
		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "Resources//back_ground//general.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;

		if(sprite_x == 11){
			Ground_Sprite* tile_sprite = new Ground_Sprite(file, 0,sprite_y);
			this->tile_map[pos_x][pos_y]->background_image = tile_sprite;
			this->tile_map[pos_x][pos_y]->can_walk = false;
		}
		if(sprite_x == 5 || sprite_x == 9){

			this->tile_map[pos_x][pos_y]->can_walk = false;
		}



	}else if(a == '1'){
		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "Resources//back_ground//water.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;
		this->tile_map[pos_x][pos_y]->can_walk = false;

	}else if(a == '2'){
		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "Resources//back_ground//water_marsh.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;

	}else if(a == '3'){
		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "Resources//back_ground//dirt.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;


	}else if(a == '4'){

		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "Resources//back_ground//cobblestones.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;


	}else if(a == '5'){

		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "Resources//back_ground//walls.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);

		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;

		this->tile_map[pos_x][pos_y]->can_walk = false;

	}else if(a == '6'){

		sprite_x = find_x(b);
		sprite_y = 0;


		strcpy_s(file, sizeof(char) * 100, "Resources//back_ground//cobblestones.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);

		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;		

	}else{
		throw std::exception("Invalid Tile code");
	}
	free(file);
}





int World::list_to_int(char* given, int size){
	int num_return = 0;

	for(int i = 0; i < size; i++){
		char char_int = given[i];
		int start_int = ((int)char_int)-48;

		for(int j = 0; j < size - i - 1;j++){
			start_int = start_int*10;
		}
		num_return = num_return + start_int;
	}
	return num_return;
}

//just converts so that the background knows which sprite to use
int World::find_x(char b){
	if(b == '0'){
		return 0;
	}else if(b == '1'){
		return 1;
	}else if(b == '2'){
		return 2;
	}else if(b == '3'){
		return 3;
	}else if(b == '4'){
		return 4;
	}else if(b == '5'){
		return 5;
	}else if(b == '6'){
		return 6;
	}else if(b == '7'){
		return 7;
	}else if(b == '8'){
		return 8;
	}else if(b == '9'){
		return 9;
	}else if(b == 'A'){
		return 10;
	}else if(b == 'B'){
		return 11;
	}else if(b == 'C'){
		return 12;
	}else if(b == 'D'){
		return 13;
	}else{
		throw std::exception("Invalid Tile code");
	}

}

bool World::equals(World* w){
	return false;
	// TODO WRITE THIS, eventually
}


bool sort_visibles(iDrawable* d1, iDrawable* d2){
	if (d1->get_reference_y() == d2->get_reference_y()){
		return (d1->get_reference_x() < d2->get_reference_x());
	}
	return (d1->get_reference_y() < d2->get_reference_y());
}

void World::insert_entity(iDrawable* da_d){
	this->active_entities->push_back(da_d);
}

void World::remove_entity(iDrawable* dat){
	this->removal_queue->push_back(dat);
}

std::list<iDrawable*>* World::get_visible_entities(void){
	this->visible_entities->clear();

	int left_most = this->playa->get_x_pos() - VISIBLE_W - PAD;
	int right_most = left_most + 2*SCREEN_W + PAD;

	int top_most = this->playa->get_y_pos() - VISIBLE_H - PAD;
	int bottom_most = top_most + 2*SCREEN_H + PAD;

	std::list<iDrawable*>::iterator iter;
	for (iter = this->active_entities->begin(); iter != this->active_entities->end(); ++iter){
		if ((*iter)->get_x_pos() >= left_most && (*iter)->get_x_pos() <= right_most){
			if ((*iter)->get_y_pos() >= top_most && (*iter)->get_y_pos() <= bottom_most){
				this->visible_entities->push_front((*iter));
			}
		}
	}

	this->visible_entities->sort(sort_visibles);
	return this->visible_entities;
}

std::list<iDrawable*>* World::get_active_entities(void){
	return this->active_entities;
}

void World::remove_destroyed(void){
	std::list<iDrawable*>::iterator iter;
	std::list<iDrawable*>::iterator end = this->removal_queue->end();
	for (iter = this->removal_queue->begin(); iter != end; ++iter){
		this->active_entities->remove((*iter));
		delete (*iter);
	}
	this->removal_queue->clear();
}
void World::make_world(){

	Tile*** map = (Tile***)malloc(sizeof(Tile**)*this->tiles_high);
	for (int i=0; i < this->tiles_high; i++){
		map[i] = (Tile**)malloc(sizeof(Tile*)*this->tiles_wide);
	}

	Ground_Sprite* sprite = new Ground_Sprite("Resources//back_ground//general.bmp", 0, 0);

	for (int i=0; i < this->tiles_high; i++){
		for (int j=0; j < this->tiles_wide; j++){
			map[i][j] = (Tile*)malloc(sizeof(Tile));
			map[i][j]->row = i;
			map[i][j]->col = j;
			map[i][j]->background_image = sprite;
			map[i][j]->can_walk = true;
			map[i][j]->contents = new std::list<iDrawable*>();
		}
	}
	this->tile_map = map;


}
void World::make_drawable(char* type, char* x, char* y, int size_x, int size_y, int type_size){

	int x_pos = list_to_int(x,size_x);
	int y_pos = list_to_int(y,size_y);
	Drawable* new_d;
	std::string type_str;
	char* filename = (char*)malloc((sizeof(char)) * (100));
	strcpy_s(filename,sizeof(char) * 100,"Resources//drawable_images//");
	for(int i = 0; i < type_size;i++){
		type_str.append(1,type[i]);
	}
	strcat(filename, type_str.c_str());
	strcat(filename,".bmp");
	new_d = new Drawable(x_pos, y_pos,0,0, new Solid_Sprite(filename));

	if(type_str.compare( "aisles")==0){

		new_d->set_boundary_value(160,32,0,32);

	}else if(type_str.compare("altar")==0){

		new_d->set_boundary_value(72,32,2,18);

	}else if(type_str.compare( "barrel")==0){

		new_d->set_boundary_value(29,23,0,7);

	}else if(type_str.compare( "bed_double")==0){

		new_d->set_boundary_value(64,64,0,0);

	}else if(type_str.compare( "bed_house")==0){

		new_d->set_boundary_value(32,64,0,0);

	}else if(type_str.compare( "bed_hut")==0){

		new_d->set_boundary_value(32,64,0,0);

	}else if(type_str.compare( "beer")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type_str.compare( "bookshelf")==0){

		new_d->set_boundary_value(64,32,0,32);

	}else if(type_str.compare( "box")==0){

		new_d->set_boundary_value(32,24,0,8);

	}else if(type_str.compare( "chest")==0){

		new_d->set_boundary_value(32,32,0,0);

	}else if(type_str.compare( "dirt_clump")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type_str.compare( "door_front")==0){


	}else if(type_str.compare( "dresser")==0){

		new_d->set_boundary_value(32,32,0,32);

	}else if(type_str.compare( "fireplace")==0){

		new_d->set_boundary_value(67,64,0,0);

	}else if(type_str.compare( "flowers")==0){

		new_d->set_boundary_value(0,0,0,0);		

	}else if(type_str.compare( "grass_tuft")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type_str.compare( "grass_tuft_marsh")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type_str.compare( "kitchen_cabinet")==0){

		new_d->set_boundary_value(32,40,0,24);

	}else if(type_str.compare( "kitchen_stuff")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type_str.compare( "mirror")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type_str.compare( "organ")==0){

		new_d->set_boundary_value(63,34,15,30);

	}else if(type_str.compare( "oven")==0){

		new_d->set_boundary_value(32,32,0,32);

	}else if(type_str.compare( "piano")==0){

		new_d->set_boundary_value(63,34,15,30);

	}else if(type_str.compare( "portrait")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type_str.compare( "stairs_down_right")==0){


	}else if(type_str.compare( "stairs_left")==0){


	}else if(type_str.compare( "stairs_right")==0){


	}else if(type_str.compare( "statue")==0){

		new_d->set_boundary_value(32,32,0,32);

	}else if(type_str.compare( "table_hut")==0){

		new_d->set_boundary_value(32,27,0,5);

	}else if(type_str.compare( "throne")==0){

		new_d->set_boundary_value(67,32,0,32);

	}else if(type_str.compare("tree_pine")==0){

		new_d->set_boundary_value(35,30,60,123);

	}else if(type_str.compare("tree_stump")==0){

		new_d->set_boundary_value(27,18,2,7);

	}else if(type_str.compare("watchtower")==0){


	}else if(type_str.compare("wine")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type_str.compare("wine_cabinet")==0){

		new_d->set_boundary_value(64,32,0,32);

	}else{

	}

	insert_entity(new_d);
	free(filename);
}

char* World::get_file(){
	if(this->my_name == test_map){
		return "Resources//maps//ATestMap.txt";
	}else{
		return "";
	}
}
void World::designate_drawable(char* back_ground_tiles){


	int inner_index = 0; 
	int outer_index = 1;
	//drawables the format of the line will be @type(2)+xpos(...)+ypos!
	char* type = (char*)malloc((sizeof(char)) * 20);
	char* x = (char*)malloc((sizeof(char)) * 8);
	char* y = (char*)malloc((sizeof(char)) * 8);
	bool not_escaped = true;

	while(not_escaped){


		//grabs type
		while(back_ground_tiles[outer_index] != '+'){
			type[inner_index] = back_ground_tiles[outer_index];
			inner_index += 1;
			outer_index += 1;
		}
		int type_size = inner_index;
		outer_index += 1;
		inner_index = 0;
		//grabs xpos
		while(back_ground_tiles[outer_index] != '+'){
			x[inner_index] = back_ground_tiles[outer_index];
			inner_index += 1;
			outer_index += 1;
		}
		//grabs ypos and records num of digits xpos was 
		outer_index += 1;
		int size_x = inner_index;
		inner_index = 0;

		while(back_ground_tiles[outer_index] != '+'){
			if(back_ground_tiles[outer_index] == '!'){
				not_escaped = false;
				break;
			}
			y[inner_index] = back_ground_tiles[outer_index];
			inner_index += 1;
			outer_index += 1;
		}

		outer_index += 1;

		//passes the things gathered to another function that will make the object
		make_drawable(type, x, y, size_x, inner_index, type_size);
		inner_index =0;
	}
	free(type);
	free(x);
	free(y);
}
WorldName World::get_WorldName(char* name, int name_size){

	std::string worldName;

	for(int i = 0; i < name_size;i++){
		worldName.append(1,name[i]);
	}
	if(worldName.compare("test_map")){
		return test_map;
	}
	else{
		//
	}

}

void World::make_portal(char* back_ground_tiles){
	int outer_index = 1;
	int inner_index = 0;

	char* worldName = (char*)malloc((sizeof(char)) * 20);
	char* x = (char*)malloc((sizeof(char)) * 8);
	char* y = (char*)malloc((sizeof(char)) * 8);

	while(back_ground_tiles[outer_index] != '+'){
		worldName[inner_index] = back_ground_tiles[outer_index];
		inner_index += 1;
		outer_index += 1;
	}
	int name_size = inner_index;
	outer_index += 1;
	inner_index = 0;
	//grabs xpos
	while(back_ground_tiles[outer_index] != '+'){
		x[inner_index] = back_ground_tiles[outer_index];
		inner_index += 1;
		outer_index += 1;
	}
	//grabs ypos and records num of digits xpos was 
	outer_index += 1;
	int size_x = inner_index;
	inner_index = 0;

	while(back_ground_tiles[outer_index] != '+'){
		if(back_ground_tiles[outer_index] == '!'){
			break;
		}
		y[inner_index] = back_ground_tiles[outer_index];
		inner_index += 1;
		outer_index += 1;

		int x_pos = list_to_int(x,size_x);
		int y_pos = list_to_int(y,inner_index);
		WorldName converted_name = get_WorldName(worldName, name_size);		
		iDrawable* new_portal = new Portal(x_pos,y_pos,new Ground_Sprite("Resources//back_ground//general.bmp",0,0),converted_name);
		insert_entity(new_portal);

	}
}