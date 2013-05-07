#include "Main.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

volatile int execution_count = 0;

World::World(WorldName this_world){

	this->tiles_high = 0;
	this->tiles_wide = 0;
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

	std::string items;

	//opens the file-stream
	fstream fin;

	std::string filename = get_file(this->my_name);

	fin.open(filename.c_str(), ios::in);


	if(fin.is_open()){

		int row_count = 0;
		int other_row_count = 0;

		//begin moving line by line down the file
		while(!fin.eof()){

			int constant_index = 0;
			std::pair<std::string,int> values;


			std::getline(fin, items);

			if (items.compare("") == 0){
				break;
			}

			//checks if the line is a 2nd layer drawable or a background

			if(other_row_count == 0){

				//pull out x
				values = pull_out(items, constant_index);

				this->tiles_wide = list_to_int(values.first, values.second); 

				constant_index += (values.second + 1);

				values = pull_out(items, constant_index);

				this->tiles_high = list_to_int(values.first, values.second);

				make_world();

				other_row_count+=1;

			}else if(items.at(0) == '@'){

				make_drawable(items);

				//if background tile line formmated so that 01010101 is a rep of 4 of the same tile
			}else if(items.at(0) == '#'){

				make_portal(items);

			}else if(items.at(0) == '~'){

				make_AI(items);

			}else{
				int size = 2*this->tiles_wide;

				char first = ' ';
				char second = ' ';

				//loops through the line grabbing every char pair and creating the neccessary tile and then adding it to the class object map array 


				omp_lock_t readlock;
				omp_init_lock(&readlock);
#pragma omp parallel for
				for(int i = 0; i < size ; i+=2){
					int t_count = omp_get_num_threads();
					int my_c = omp_get_thread_num();
					
					omp_set_lock(&readlock);
					first = items.at(i);
					second = items.at(i+1);
					omp_unset_lock(&readlock);
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
std::pair<std::string, int> World::pull_out(std::string items, int index){

	std::string x;
	int size = 0;

	while(items.at(index) != '+' && items.at(index) != '!'){
		x.append(1,items.at(index));
		index ++;
		size ++;
	}

	return std::make_pair(x, size);

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
		if (sprite_x == 2){
			int br = -1;
		}

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





int World::list_to_int(std::string given, int size){
	int num_return = 0;

	for(int i = 0; i < size; i++){
		char char_int = given.at(i);
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
	}else if(b == 'E'){
		return 14;
	}else if(b == 'F'){
		return 15;
	}else{
		throw std::exception("Invalid Tile code");
	}

}

bool World::equals(World* w){
	if(this->my_name == w->my_name){
		return true;
	}

	return false;
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
	std::list<iDrawable*>::iterator end = this->active_entities->end();
	for (iter = this->active_entities->begin(); iter != end; ++iter){
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
void World::designate_drawable(std::string type, std::string x, std::string y, int size_x, int size_y, int type_size){

	int x_pos = list_to_int(x,size_x);
	int y_pos = list_to_int(y,size_y);
	Drawable* new_d;

	std::string filename;
	filename.append("Resources//drawable_images//");
	filename.append(type);
	filename.append(".bmp");

	new_d = new Drawable(x_pos, y_pos,0,0, new Solid_Sprite((char*)filename.c_str()));

	if(type.compare( "aisles")==0){

		new_d->set_boundary_value(160,32,0,32);

	}else if(type.compare("altar")==0){

		new_d->set_boundary_value(72,32,2,18);

	}else if(type.compare( "bar")==0){

		new_d->set_boundary_value(89,33,1,9);

	}else if(type.compare( "barn")==0){
		
		new_d->set_boundary_value(96,49,0,49);

	}else if(type.compare( "barrel")==0){

		new_d->set_boundary_value(29,23,0,7);

	}else if(type.compare( "bed_double")==0){

		new_d->set_boundary_value(64,64,0,0);

	}else if(type.compare( "bed_house")==0){

		new_d->set_boundary_value(32,64,0,0);

	}else if(type.compare( "bed_hut")==0){

		new_d->set_boundary_value(70,60,0,20);

	}else if(type.compare( "beer")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "bookshelf")==0){

		new_d->set_boundary_value(64,32,0,32);

	}else if(type.compare( "boat")==0){

		new_d->set_boundary_value(0,0,0,0);		

	}else if(type.compare( "box")==0){

		new_d->set_boundary_value(32,24,0,8);

	}else if(type.compare( "bread1")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "bread2")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "bread3")==0){

		new_d->set_boundary_value(29,23,0,7);

	}else if(type.compare( "chair_front")==0){

		new_d->set_boundary_value(21,30,0,10);

	}else if(type.compare( "chair_left")==0){

		new_d->set_boundary_value(21,30,0,10);

	}else if(type.compare( "chair_right")==0){

		new_d->set_boundary_value(21,30,0,10);

	}else if(type.compare( "church")==0){

		new_d->set_boundary_value(449,600,0,98);

	}else if(type.compare( "couch")==0){

		new_d->set_boundary_value(42,85,0,8);

	}else if(type.compare( "chest")==0){

		new_d->set_boundary_value(32,32,0,0);

	}else if(type.compare( "dinig_table")==0){

		new_d->set_boundary_value(73,54,0,6);

	}else if(type.compare( "dirt_clump")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "door_front")==0){
		
		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "dresser")==0){

		new_d->set_boundary_value(32,32,0,32);

	}else if(type.compare( "farm_house")==0){

		new_d->set_boundary_value(165,129,30,45);

	}else if(type.compare( "feed")==0){
		
		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "fence_horiz")==0){

		new_d->set_boundary_value(160,28,0,4);

	}else if(type.compare( "fence_vert")==0){

		new_d->set_boundary_value(5,96,0,0);

	}else if(type.compare( "fireplace")==0){

		new_d->set_boundary_value(67,64,0,0);

	}else if(type.compare( "flower1")==0){

		new_d->set_boundary_value(0,0,0,0);		

	}else if(type.compare( "flower2")==0){

		new_d->set_boundary_value(0,0,0,0);		

	}else if(type.compare( "flower3")==0){

		new_d->set_boundary_value(0,0,0,0);		

	}else if(type.compare( "flower4")==0){

		new_d->set_boundary_value(0,0,0,0);		

	}else if(type.compare( "flower5")==0){

		new_d->set_boundary_value(0,0,0,0);		

	}else if(type.compare( "flower6")==0){

		new_d->set_boundary_value(0,0,0,0);		

	}else if(type.compare( "flower7")==0){

		new_d->set_boundary_value(0,0,0,0);		

	}else if(type.compare( "flower8")==0){

		new_d->set_boundary_value(0,0,0,0);		

	}else if(type.compare( "flowers")==0){

		new_d->set_boundary_value(0,0,0,0);		

	}else if(type.compare( "grass_tuft")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "grass_tuft_marsh")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "hay")==0){

		new_d->set_boundary_value(70,44,0,20);	

	}else if(type.compare( "hut1")==0){

		new_d->set_boundary_value(220,220,0,20);

	}else if(type.compare( "hut2")==0){

		new_d->set_boundary_value(165,170,0,20);

	}else if(type.compare( "hut3")==0){
		
		new_d->set_boundary_value(120,161,0,30);
				

	}else if(type.compare( "hut4")==0){

		new_d->set_boundary_value(163,115,0,30);		

	}else if(type.compare( "kitchen_cabinet")==0){

		new_d->set_boundary_value(32,40,0,24);

	}else if(type.compare( "kitchen_stuff")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "lazy_chair")==0){

		new_d->set_boundary_value(32,40,0,10);

	}else if(type.compare( "livestock_food")==0){

		new_d->set_boundary_value(64,35,0,4);

	}else if(type.compare( "livestock_water")==0){

		new_d->set_boundary_value(32,61,0,4);

	}else if(type.compare( "mirror")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "nice_house1")==0){

		new_d->set_boundary_value(176,257,0,50);		

	}else if(type.compare( "nice_house2")==0){

		new_d->set_boundary_value(364,290,0,26);

	}else if(type.compare( "outhouse")==0){

		new_d->set_boundary_value(33,32,0,33);
	
	}else if(type.compare( "organ")==0){

		new_d->set_boundary_value(63,34,15,30);

	}else if(type.compare( "oven")==0){

		new_d->set_boundary_value(32,32,0,32);

	}else if(type.compare( "piano")==0){

		new_d->set_boundary_value(63,34,15,30);

	}else if(type.compare( "plant1")==0){

		new_d->set_boundary_value(0,0,0,0);
	
	}else if(type.compare( "plant2")==0){

		new_d->set_boundary_value(0,0,0,0);
	
	}else if(type.compare( "plant3")==0){

		new_d->set_boundary_value(0,0,0,0);
	
	}else if(type.compare( "plant4")==0){

		new_d->set_boundary_value(0,0,0,0);
	
	}else if(type.compare( "plant5")==0){

		new_d->set_boundary_value(0,0,0,0);
	
	}else if(type.compare( "plant6")==0){

		new_d->set_boundary_value(0,0,0,0);
	
	}else if(type.compare( "plant7")==0){

		new_d->set_boundary_value(0,0,0,0);
	
	}else if(type.compare( "plant8")==0){

		new_d->set_boundary_value(0,0,0,0);
	
	}else if(type.compare( "plant9")==0){

		new_d->set_boundary_value(0,0,0,0);
	
	}else if(type.compare( "portrait")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "rug")==0){

		new_d->set_boundary_value(0,0,0,0);
	
	}else if(type.compare( "rug_bear")==0){

		new_d->set_boundary_value(0,0,0,0);
	
	}else if(type.compare( "side_dresser")==0){

		new_d->set_boundary_value(30,80,0,7);
	
	}else if(type.compare( "stairs_down_right")==0){
		
		new_d->set_boundary_value(0,0,0,0);
		
	}else if(type.compare( "stairs_left")==0){
		
		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "stairs_right")==0){
		
		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare( "statue")==0){

		new_d->set_boundary_value(32,32,0,32);

	}else if(type.compare( "statue2")==0){

		new_d->set_boundary_value(32,32,0,32);

	}else if(type.compare( "table")==0){

		

	}else if(type.compare( "table_hut")==0){

		new_d->set_boundary_value(32,27,0,5);

	}else if(type.compare( "throne")==0){

		new_d->set_boundary_value(67,32,0,32);

	}else if(type.compare("tree_dead")==0){

		

	}else if(type.compare("tree_oak")==0){

		

	}else if(type.compare("tree_oak2")==0){

		

	}else if(type.compare("tree_pine")==0){

		new_d->set_boundary_value(35,30,60,123);

	}else if(type.compare("tree_stump")==0){

		new_d->set_boundary_value(27,18,2,7);

	}else if(type.compare("watchtower")==0){


	}else if(type.compare("well")==0){

		

	}else if(type.compare("wine")==0){

		new_d->set_boundary_value(0,0,0,0);

	}else if(type.compare("wine_cabinet")==0){

		new_d->set_boundary_value(64,32,0,32);

	}else{
		throw std::exception("Invalid drawable");
	}

	insert_entity(new_d);

}

std::string World::get_file(WorldName name){
	if(name == test_map){
		return "Resources//maps//ATestMap.txt";
	}else if(name == main_world){
		return "Resources//maps//friday_map.txt";
	}else if(name == main_world11){
		return "Resources//maps//main_world11.txt";
	}else if(name == main_world12){
		return "Resources//maps//main_world12.txt";
	}else if(name == main_world13){
		return "Resources//maps//main_world13.txt";
	}else if(name == main_world14){
		return "Resources//maps//main_world14.txt";
	}else if(name == main_world15){
		return "Resources//maps//main_world15.txt";
	}else if(name == main_world16){
		return "Resources//maps//main_world16.txt";
	}else if(name == main_world17){
		return "Resources//maps//main_world17.txt";
	}else if(name == main_world18){
		return "Resources//maps//main_world18.txt";
	}else if(name == main_world19){
		return "Resources//maps//main_world19.txt";
	}else{
		throw std::exception("No world");
	}
}
void World::make_drawable(std::string items){

	int constant_index = 1;
	std::pair<std::string,int> type, x, y;
	//drawables the format of the line will be @type(2)+xpos(...)+ypos!
	bool not_escaped = true;


	while(not_escaped){


		//grabs type
		type = pull_out(items, constant_index);

		constant_index += type.second + 1;

		//grabs xpos
		x = pull_out(items, constant_index);

		constant_index += x.second + 1;
		//grabs ypos and records num of digits xpos was 

		y = pull_out(items, constant_index);

		int check = constant_index + y.second;

		if(items.at(check) == '!'){
			not_escaped	= false;
		}

		constant_index += (y.second + 1);
		//passes the things gathered to another function that will make the object
		designate_drawable(type.first, x.first, y.first,x.second,y.second, type.second);
	}
}
WorldName World::get_WorldName(std::string name, int name_size){

	std::string worldName = name;


	if(worldName.compare("test_map")==0){
		return test_map;
	}else if(worldName.compare("main_world") == 0){
		return main_world;
	}else if(worldName.compare("main_world11") == 0){
		return main_world11;
	}else if(worldName.compare("main_world12") == 0){
		return main_world12;
	}else if(worldName.compare("main_world13") == 0){
		return main_world13;
	}else if(worldName.compare("main_world14") == 0){
		return main_world14;
	}else if(worldName.compare("main_world15") == 0){
		return main_world15;
	}else if(worldName.compare("main_world16") == 0){
		return main_world16;
	}else if(worldName.compare("main_world17") == 0){
		return main_world17;
	}else if(worldName.compare("main_world18") == 0){
		return main_world18;
	}else if(worldName.compare("main_world19") == 0){
		return main_world19;
	}
	else{
		throw std::exception("Broke");
	}
	return main_world;

}

void World::make_portal(std::string items){
	int outer_index = 1;
	int inner_index = 0;

	std::string worldName;
	std::string x;
	std::string y;
	std::string x_targ;
	std::string y_targ;

	while(items.at(outer_index) != '+'){
		worldName.append(1, items.at(outer_index));
		inner_index += 1;
		outer_index += 1;
	}
	int name_size = inner_index;
	outer_index += 1;
	inner_index = 0;
	//grabs xpos
	while(items.at(outer_index) != '+'){
		x.append(1, items.at(outer_index));
		inner_index += 1;
		outer_index += 1;
	}
	//grabs ypos and records num of digits xpos was 
	outer_index += 1;
	int size_x = inner_index;
	inner_index = 0;

	while(items.at(outer_index) != '+'){
		y.append(1, items.at(outer_index));
		inner_index += 1;
		outer_index += 1;
	}
	outer_index += 1;
	int size_y = inner_index;
	inner_index = 0;

	while(items.at(outer_index) != '+'){
		x_targ.append(1, items.at(outer_index));
		inner_index += 1;
		outer_index += 1;
	}
	//grabs ypos and records num of digits xpos was 
	outer_index += 1;
	int size_tar = inner_index;
	inner_index = 0;

	while(items.at(outer_index) != '+'){
		if(items.at(outer_index) == '!'){
			break;
		}

		y_targ.append(1, items.at(outer_index));
		inner_index += 1;
		outer_index += 1;
	}

	int x_pos = list_to_int(x,size_x);
	int y_pos = list_to_int(y,inner_index);
	int x_tar = list_to_int(x_targ, size_tar); 
	int y_tar = list_to_int(y_targ, inner_index);
	WorldName converted_name = get_WorldName(worldName, name_size);

	iDrawable* new_portal = new Portal(x_pos,y_pos,new Ground_Sprite("Resources//back_ground//general.bmp",10,0),converted_name, x_tar, y_tar);
	new_portal->set_boundary_value(32, 32, 0, 0);
	insert_entity(new_portal);

}

void World::make_AI(std::string items){

	int constant_index = 1;

	std::pair<std::string, int> values;

	values = pull_out(items, constant_index);
	std::string type;

	constant_index += (values.second + 1);

	type = values.first;

	if(type.compare("OptionPresenter") == 0){

		make_op(items, constant_index);

	}else if(type.compare("Guard") == 0){

	}else{
		throw std::exception("Invalid AI");
	}

}

void World::make_op(std::string items, int constant_index){	

	std::pair<Quest*, int> quest_data;
	std::pair<std::string, int> values = pull_out(items, constant_index);
	std::string dialouge;

	//file
	std::string filename;

	filename = values.first;

	constant_index += (values.second + 1);

	//x
	values = pull_out(items, constant_index);

	int x_pos  = list_to_int(values.first, values.second);

	constant_index += (values.second + 1);

	//y
	values = pull_out(items, constant_index);

	int y_pos  = list_to_int(values.first, values.second);

	constant_index += (values.second + 1);

	//make the guy
	OptionPresenter* character = new OptionPresenter(x_pos, y_pos, 0, 0, new Player_Sprite((char*)filename.c_str(), S, 5, 1, 16, 16));

	//set values
	character->set_boundary_value(30, 30, 0, 0);
	character->set_world(this);

	//add number of quests
	values = pull_out(items, constant_index);
	int number_of_quests  = list_to_int(values.first, values.second);
	constant_index += (values.second + 1);	

	//create the quests
	for(int j = 0; j < number_of_quests; j++){
		quest_data = make_quest(items, constant_index);
		character->append_quest(quest_data.first);
		constant_index = quest_data.second;
	}

	//dialouge
	make_dialouge_op(items, constant_index, character);

	//put in the d
	this->insert_entity(character);

}
EntityType World::get_entityType(std::string to_convert){

	std::string str = to_convert;

	if(str.compare("Tg")==0){
		return Guard;
	}else if(str.compare("Mon")==0){
		return Monster;
	}else if(str.compare("Out")==0){
		return Outcast;
	}else if(str.compare("Riv")==0){
		return Rival;
	}else if(str.compare("Ch")==0){
		return Chicken;
	}else{
		throw std::exception("Invalid entity code");
	}

}
std::string World::to_string(std::string to_convert, int size){

	std::string str;
	for(int i =0; i < size;i++){
		str.append(1,to_convert[i]);
	}

	return str;
}
std::pair<Quest*, int> World::make_quest(std::string items, int constant_index){

	std::pair<std::string,int> values;
	QuestDescription quest_desc;
	//quest desc
	values = pull_out(items, constant_index);

	quest_desc.text = values.first;

	constant_index += (values.second + 1);	
	QuestReward loot;
	//loot (gold)
	values = pull_out(items, constant_index);
	loot.gold = list_to_int(values.first, values.second);
	constant_index += (values.second + 1);
	IQuestObjective* obj;

	if(items[constant_index] == 'K'){
		constant_index += 1;
		//target type
		values = pull_out(items, constant_index);
		constant_index += (values.second + 1);
		EntityType ent = get_entityType(values.first);
		//number to kill
		values = pull_out(items, constant_index);
		constant_index += (values.second + 1);
		obj = new KillObjective(ent, list_to_int(values.first, values.second));

	}else if(items[constant_index] == 'R'){
		constant_index += 1;
		//target id
		values = pull_out(items, constant_index);
		constant_index += (values.second + 1);
		int item_id= list_to_int(values.first, values.second);
		//number required
		values = pull_out(items, constant_index);
		constant_index += (values.second + 1);
		obj = new RetrieveObjective(item_id, list_to_int(values.first, values.second));
	}else{
		throw std::exception("Wrong objective code");
	}

	Quest* quest = new Quest(quest_desc, obj);
	quest->add_reward(loot);

	return std::make_pair(quest, constant_index);
}
void World::make_dialouge_op(std::string items, int constant_index, OptionPresenter* op){
	std::pair<std::string,int> values;

	while(true){

		char check = items.at(constant_index);
		if( check == '^'){
			constant_index++;
			values = pull_out(items, constant_index);
			op->append_pre_dialogue(to_string(values.first, values.second));

		}else if(items.at(constant_index) == '*'){
			constant_index++;
			values = pull_out(items, constant_index);
			op->append_post_dialogue(to_string(values.first, values.second));
		}else{
			throw std::exception("dialouge OP code error");
		}

		constant_index += (values.second + 1);

		if(items.at(constant_index-1) == '!'){
			break;
		}
	}

}
