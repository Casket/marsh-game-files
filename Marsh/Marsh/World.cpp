#include "Main.h"
#include <fstream>
#include <string>
#include <iostream>
#include <map>

using namespace std;
BITMAP* draw_loading(void);
volatile int execution_count = 0;

World::World(WorldName this_world){
	BITMAP *loading_screen_bitmap = draw_loading();
	this->tiles_high = 0;
	this->tiles_wide = 0;
	this->my_name = this_world;
	this->active_entities = new std::list<iDrawable*>();
	this->visible_entities = new std::list<iDrawable*>();
	this->removal_queue = new std::set<iDrawable*>();
	load_world();
	destroy_bitmap(loading_screen_bitmap);
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

BITMAP* draw_loading(void) {
	BITMAP* loading_screen_bitmap = load_bitmap("Resources//LoadScreen.bmp",NULL);
	//BITMAP *tip = load_bitmap("Resources//boom.bmp",NULL);
	if (!loading_screen_bitmap){ //|| !tip){
		allegro_message("Failed to load the loading screen."); 
		exit(1);	
	}
	blit(loading_screen_bitmap, screen, 0,0, 0,0, 1400, 1000);
	//blit(tip, screen, 0,0,304,230,773,413);
	return loading_screen_bitmap;
}

void World::load_world(){ 

	std::string items;

	//opens the file-stream
	fstream fin;

	std::string filename = get_file(this->my_name);

	fin.open(filename.c_str(), ios::in);

	this->intial_drawable_map();

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

				make_portal(items, false);

			}else if(items.at(0) == '%'){

				make_portal(items, true);

			}else if(items.at(0) == '~'){

				make_AI(items);

			}else{
				int size = 2*this->tiles_wide;

				char first = ' ';
				char second = ' ';

				//loops through the line grabbing every char pair and creating the neccessary tile and then adding it to the class object map array 

				//				omp_lock_t readlock;
				//				omp_init_lock(&readlock);
				//#pragma omp parallel for
				for(int i = 0; i < size ; i+=2){					
					//					omp_set_lock(&readlock);
					first = items.at(i);
					second = items.at(i+1);
					//					omp_unset_lock(&readlock);
					convert_to_tile(first, second, row_count,i/2);
				}


				row_count += 1;
			}
		}

		fin.close();
	}else{
		return;
	}

	delete this->drawables;
	this->drawables = NULL;
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
	this->tile_map[pos_x][pos_y]->flyable = false;

	if(a == '0'){
		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "Resources//back_ground//general.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;

		if(sprite_x == 5 || sprite_x == 9){
			this->tile_map[pos_x][pos_y]->can_walk = false;
		}
		if(sprite_x == 5){
			this->tile_map[pos_x][pos_y]->flyable = true;
		}


	}else if(a == '1'){
		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "Resources//back_ground//water.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;
		this->tile_map[pos_x][pos_y]->can_walk = false;
		this->tile_map[pos_x][pos_y]->flyable = true;

	}else if(a == '2'){
		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "Resources//back_ground//water_marsh.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;

		this->tile_map[pos_x][pos_y]->flyable = true;
		this->tile_map[pos_x][pos_y]->can_walk = false;

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


		strcpy_s(file, sizeof(char) * 100, "Resources//back_ground//dungeon.bmp");

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
	if (da_d == NULL){
		throw std::exception("Broke");
	}
	this->active_entities->push_back(da_d);
}

void World::remove_entity(iDrawable* dat){
	this->removal_queue->insert(dat);
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
	std::set<iDrawable*>::iterator iter;
	std::set<iDrawable*>::iterator end = this->removal_queue->end();
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
		}
	}
	this->tile_map = map;


}



std::string World::get_file(WorldName name){
	if(name == hut_play){
		return "Resources//maps//hut_play.txt";
	}else if(name == hut_1){
		return "Resources//maps//hut_1.txt";
	}else if(name == hut_2){
		return "Resources//maps//hut_2.txt";
	}else if(name == hut_3){
		return "Resources//maps//hut_3.txt";
	}else if(name == hut_4){
		return "Resources//maps//hut_4.txt";
	}else if(name == hut_5){
		return "Resources//maps//hut_5.txt";
	}else if(name == hut_6){
		return "Resources//maps//hut_6.txt";
	}else if(name == hut_7){
		return "Resources//maps//hut_7.txt";
	}else if(name == hut_8){
		return "Resources//maps//hut_8.txt";
	}else if(name == hut_9){
		return "Resources//maps//hut_9.txt";
	}else if(name == main_world11){
		return "Resources//maps//world11.txt";
	}else if(name == main_world12){
		return "Resources//maps//world12.txt";
	}else if(name == main_world13){
		return "Resources//maps//world13.txt";
	}else if(name == main_world14){
		return "Resources//maps//world14.txt";
	}else if(name == main_world15){
		return "Resources//maps//world15.txt";
	}else if(name == main_world16){
		return "Resources//maps//world16.txt";
	}else if(name == main_world17){
		return "Resources//maps//world17.txt";
	}else if(name == main_world18){
		return "Resources//maps//world18.txt";
	}else if(name == main_world19){
		return "Resources//maps//world19.txt";
	}else if(name == house1){
		return "Resources//maps//house1.txt";
	}else if(name == house2){
		return "Resources//maps//house2.txt";
	}else if(name == house3){
		return "Resources//maps//house3.txt";
	}else if(name == house4){
		return "Resources//maps//house4.txt";
	}else if(name == house5){
		return "Resources//maps//house5.txt";
	}else if(name == house6){
		return "Resources//maps//house6.txt";
	}else if(name == house7){
		return "Resources//maps//house7.txt";
	}else if(name == house12){
		return "Resources//maps//house12.txt";
	}else if(name == house22){
		return "Resources//maps//house22.txt";
	}else if(name == house32){
		return "Resources//maps//house32.txt";
	}else if(name == house42){
		return "Resources//maps//house42.txt";
	}else if(name == house52){
		return "Resources//maps//house52.txt";
	}else if(name == house62){
		return "Resources//maps//house62.txt";
	}else if(name == house72){
		return "Resources//maps//house72.txt";
	}else if(name == shop1){
		return "Resources//maps//shop1.txt";
	}else if(name == shop2){
		return "Resources//maps//shop2.txt";
	}else if(name == shop3){
		return "Resources//maps//shop3.txt";
	}else if(name == keep){
		return "Resources//maps//keep.txt";
	}else if(name == church){
		return "Resources//maps//church1.txt";
	}else if(name = dung_vamp1){
		return "Resources//maps//dung_vamp1.txt";
	}
	else{
		throw std::exception("No world");
	}
}
void World::set_boundaries(std::string type, int width, int height, int x, int y){
	drawable_bounds boundaries;

	boundaries.bounds[0] = width;
	boundaries.bounds[1] = height;
	boundaries.bounds[2] = x;
	boundaries.bounds[3] = y;

	this->drawables->insert(std::pair<std::string, drawable_bounds>(type, boundaries));
}
void World::intial_drawable_map(void){
	this->drawables = new std::map<std::string, drawable_bounds>();

	set_boundaries("aisles",160,32,0,32);	
	set_boundaries("altar",72,32,2,18);	
	set_boundaries("bar",89,33,1,9);
	set_boundaries("barn",96,49,0,49);
	set_boundaries("barrel",29,23,0,7);
	set_boundaries("barrel2",29,23,0,7);
	set_boundaries("bed_double",64,64,0,0);
	set_boundaries("bed_house",32,64,0,0);
	set_boundaries("bed_hut",70,60,0,20);
	set_boundaries("beer",0,0,0,0);
	set_boundaries("bookshelf",64,32,0,32);
	set_boundaries("boat",0,0,0,0);		
	set_boundaries("box",32,24,0,8);
	set_boundaries("bread1",0,0,0,0);
	set_boundaries("bread2",0,0,0,0);
	set_boundaries("bread3",29,23,0,7);
	set_boundaries("castle_1_1",7,818,0,10);
	set_boundaries("castle_1_2",711,114,0,0);
	set_boundaries("castle_1_3",7,630,0,10);
	set_boundaries("castle_1_4",224,12,0,7);
	set_boundaries("castle_1_5",320,64,0,10);
	set_boundaries("castle_1_6",334,64,0,10);
	set_boundaries( "castle_1_7",77,50,0,7);
	set_boundaries( "castle_1_8",7,129,0,10);
	set_boundaries( "castle_1_9",7,129,0,10);
	set_boundaries( "castle_1_10",64,28,0,10);
	set_boundaries( "cave",130,145,4,30);
	set_boundaries( "cave_drg_1",64,393,0,0);
	set_boundaries( "cave_drg_2",928,64,0,10);
	set_boundaries( "cave_drg_3",299,319,0,10);
	set_boundaries( "cave_drg_4",12,256,0,10);
	set_boundaries( "cave_drg_5",288,267,0,0);
	set_boundaries( "cave_drg_6",928,12,0,0);
	set_boundaries( "cave_drg_7",68,332,0,0);
	set_boundaries( "cell",151,133,0,20);
	set_boundaries( "cell_wall",151,96,0,0);
	set_boundaries( "chair_front",21,30,0,10);
	set_boundaries( "chair_left",21,30,0,10);
	set_boundaries( "chair_right",21,30,0,10);
	set_boundaries( "church",449,600,0,98);
	set_boundaries( "church_1_1",167,32,0,0);
	set_boundaries( "church_1_2",7,384,0,0);
	set_boundaries( "church_1_3",8,350,0,10);
	set_boundaries( "church_1_4",526,105,0,10);
	set_boundaries( "church_1_5",71,63,0,0);
	set_boundaries( "church_1_6",110,27,0,10);
	set_boundaries( "church_1_7",45,27,0,10);
	set_boundaries( "church_1_8",61,27,0,10);
	set_boundaries( "church_1_9",7,328,0,0);
	set_boundaries( "church_1_10",321,27,0,5);
	set_boundaries( "church_2_1",526,105,0,10);
	set_boundaries( "church_2_2",7,381,0,10);
	set_boundaries( "church_2_3",8,317,0,10);
	set_boundaries( "church_2_4",246,27,0,10);
	set_boundaries( "church_2_5",512,7,0,0);
	set_boundaries( "church_2_6",8,277,0,10);
	set_boundaries( "church_2_7",174,27,0,10);
	set_boundaries( "church_2_8",64,61,0,0);
	set_boundaries( "church_2_9",7,383,0,10);
	set_boundaries( "church_chair1",29,21,0,9);
	set_boundaries( "church_chair2",30,25,0,5);
	set_boundaries( "church_chair3",30,25,0,5);
	set_boundaries( "column",30,32,0,64);
	set_boundaries( "couch",42,85,0,8);
	set_boundaries( "cross",10,30,0,16);
	set_boundaries( "cross_roads",10,5,10,27);
	set_boundaries( "chest",32,32,0,0);
	set_boundaries( "dinig_table",73,54,0,6);
	set_boundaries( "dirt_clump",0,0,0,0);
	set_boundaries( "door_front",0,0,0,0);
	set_boundaries( "dresser",32,32,0,32);
	set_boundaries( "dung_keep1",1582,86,0,10);
	set_boundaries( "dung_keep2",64,96,0,0);
	set_boundaries( "dung_keep3",166,80,0,16);
	set_boundaries( "dung_keep4",755,143,0,10);
	set_boundaries( "dung_keep5",453,138,0,15);
	set_boundaries( "dung_keep6",6,1782,0,10);
	set_boundaries( "dung_keep7",960,40,0,6);
	set_boundaries( "dung_keep8",6,214,0,10);
	set_boundaries( "dung_keep9",96,86,0,10);
	set_boundaries( "dung_keep10",6,64,0,0);
	set_boundaries( "dung_keep11",460,86,0,10);
	set_boundaries( "dung_keep12",6,502,0,10);
	set_boundaries( "dung_keep13",461,36,0,10);
	set_boundaries( "dung_keep14",6,232,0,10);
	set_boundaries( "dung_keep15",96,46,0,0);
	set_boundaries( "dung_keep16",6,616,0,10);
	set_boundaries( "dung_keep17",1088,86,0,10);
	set_boundaries( "dung_keep18",1350,36,0,10);
	set_boundaries( "dung_keep19",6,658,0,10);
	set_boundaries( "dung_keep20",128,46,0,0);
	set_boundaries( "dung_keep21",6,136,0,10);
	set_boundaries( "dung_keep22",364,36,0,10);
	set_boundaries( "dung_keep23",6,608,0,0);
	set_boundaries( "dung_keep24",364,86,0,10);
	set_boundaries( "dung_keep25",6,151,0,10);
	set_boundaries( "dung_keep26",128,80,0,16);
	set_boundaries( "dung_keep27",6,457,0,10);
	set_boundaries( "dung_keep28",614,46,0,0);
	set_boundaries( "dung_keep29",6,1780,0,12);
	set_boundaries( "dung_other1",1440,101,0,10);
	set_boundaries( "dung_other2",1440,16,0,0);
	set_boundaries( "dung_other3",11,208,0,0);
	set_boundaries( "dung_other4",1313,16,0,0);
	set_boundaries( "dung_other5",11,688,0,0);
	set_boundaries( "dung_other6",276,16,0,0);
	set_boundaries( "dung_other7",11,582,0,10);
	set_boundaries( "dung_other8",800,6,0,10);
	set_boundaries( "dung_other9",11,1158,0,10);
	set_boundaries( "dung_other10",150,6,0,10);
	set_boundaries( "dung_other11",287,99,0,10);
	set_boundaries( "dung_other12",11,1251,0,10);
	set_boundaries( "dung_other13",523,101,0,10);
	set_boundaries( "dung_other14",11,439,0,10);
	set_boundaries( "dung_other15",277,16,0,0);
	set_boundaries( "dung_other16",11,1222,0,10);
	set_boundaries( "dung_other17",171,16,0,0);
	set_boundaries( "dung_other18",11,134,0,10);
	set_boundaries( "dung_other19",254,16,0,0);
	set_boundaries( "dung_other20",11,436,0,10);
	set_boundaries( "dung_other21",254,101,0,10);
	set_boundaries( "dung_other22",183,239,0,0);
	set_boundaries( "dung_other23",11,752,0,0);
	set_boundaries( "dung_other24",895,101,0,10);
	set_boundaries( "dung_other25",11,687,0,0);
	set_boundaries( "dung_other26",565,118,0,10);
	set_boundaries( "dung_other27",11,324,0,10);
	set_boundaries( "dung_other28",383,101,0,10);
	set_boundaries( "dung_other29",11,334,0,0);
	set_boundaries( "dung_other30",480,118,0,10);
	set_boundaries( "dung_other31",11,324,0,10);
	set_boundaries( "dung_other32",1472,119,0,10);
	set_boundaries( "dung_other33",1312,119,0,10);
	set_boundaries( "dung_other34",161,791,0,10);
	set_boundaries( "dung_other35",161,1655,0,10);
	set_boundaries( "dung_other36",480,439,0,10);
	set_boundaries( "dung_vamp1",1676,86,0,10);
	set_boundaries( "dung_vamp2",6,1014,0,10);
	set_boundaries( "dung_vamp3",768,36,0,10);
	set_boundaries( "dung_vamp4",6,118,0,10);
	set_boundaries( "dung_vamp5",165,86,0,10);
	set_boundaries( "dung_vamp6",6,173,0,10);
	set_boundaries( "dung_vamp7",863,86,0,10);
	set_boundaries( "dung_vamp8",960,36,0,10);
	set_boundaries( "dung_vamp9",6,1137,0,0);
	set_boundaries( "dung_vamp10",6,1183,0,0);
	set_boundaries( "dung_vamp11",922,86,0,10);
	set_boundaries( "dung_vamp12",6,278,0,10);
	set_boundaries( "dung_vamp13",428,36,0,10);
	set_boundaries( "dung_vamp14",6,278,0,10);
	set_boundaries( "dung_vamp15",1030,86,0,10);
	set_boundaries( "dung_vamp16",6,2166,0,10);
	set_boundaries( "dung_vamp17",1260,87,0,10);
	set_boundaries( "dung_vamp18",6,598,0,10);
	set_boundaries( "dung_vamp19",357,86,0,10);
	set_boundaries( "dung_vamp20",6,502,0,10);
	set_boundaries( "dung_vamp21",715,36,0,10);
	set_boundaries( "dung_vamp22",6,502,0,10);
	set_boundaries( "dung_vamp23",166,86,0,10);
	set_boundaries( "dung_vamp24",6,392,0,10);
	set_boundaries( "dung_vamp25",864,36,0,10);
	set_boundaries( "dung_vamp26",6,1672,0,10);
	set_boundaries( "dung_vamp27",1761,86,0,10);
	set_boundaries( "dung_vamp28",6,1184,0,0);
	set_boundaries( "dung_vamp29",166,86,0,10);
	set_boundaries( "dung_vamp30",6,82,0,0);
	set_boundaries( "dung_vamp31",774,36,0,10);
	set_boundaries( "dung_vamp32",6,1014,0,10);
	set_boundaries( "dung_room_change",14,14,0,50);
	set_boundaries( "farm_house",165,129,30,45);
	set_boundaries( "feed",0,0,0,0);
	set_boundaries( "fence_horiz",160,28,0,4);
	set_boundaries( "fence_vert",5,96,0,0);
	set_boundaries( "firepit",32,27,0,5);
	set_boundaries( "fireplace",67,64,0,0);
	set_boundaries( "flower1",0,0,0,0);
	set_boundaries( "flower2",0,0,0,0);
	set_boundaries( "flower3",0,0,0,0); 
	set_boundaries( "flower4",0,0,0,0); 
	set_boundaries( "flower5",0,0,0,0); 
	set_boundaries( "flower6",0,0,0,0);
	set_boundaries( "flower7",0,0,0,0);
	set_boundaries( "flower8",0,0,0,0);
	set_boundaries( "flower9",0,0,0,0);
	set_boundaries( "flowers",0,0,0,0);
	set_boundaries( "grass_tuft",0,0,0,0);
	set_boundaries( "grass_tuft_marsh",0,0,0,0);
	set_boundaries( "hay",70,44,0,20);	
	set_boundaries( "hut1",220,220,0,20);
	set_boundaries( "hut2",165,170,0,20);
	set_boundaries( "hut3",120,161,0,30);
	set_boundaries( "hut4",163,115,0,30);
	set_boundaries( "hut_1",8,177,0,0);
	set_boundaries( "hut_2",96,32,0,0);
	set_boundaries( "hut_3",95,32,0,0);
	set_boundaries( "hut_5",239,55,0,0);
	set_boundaries( "hut_cheese",0,0,0,0);
	set_boundaries( "hut_counter",64,24,0,10);
	set_boundaries( "hut_grain",0,0,0,0);
	set_boundaries( "hut_oven",65,43,0,0);
	set_boundaries( "hut_table",52,40,0,12);
	set_boundaries( "keep",416,370,0,60);
	set_boundaries( "keep_stair",64,119,0,0);
	set_boundaries( "keep1",957,700,0,47);
	set_boundaries( "kitchen_cabinet",32,40,0,24);
	set_boundaries( "kitchen_stuff",0,0,0,0);
	set_boundaries( "laundry",0,0,0,0);
	set_boundaries( "lantern",0,0,0,0);
	set_boundaries( "lazy_chair",32,40,0,10);
	set_boundaries( "livestock_food",64,35,0,4);
	set_boundaries( "livestock_water",32,61,0,4);
	set_boundaries( "mirror",0,0,0,0);
	set_boundaries( "nice_house1",176,257,0,50);		
	set_boundaries( "nice_house2",364,290,0,26);
	set_boundaries( "outcast_bed_roll",0,0,0,0);
	set_boundaries( "outcast_box1",29,18,0,23);
	set_boundaries( "outcast_box2",29,18,0,23);
	set_boundaries( "outcast_bucket",29,12,0,18);
	set_boundaries( "outhouse",33,32,0,33);
	set_boundaries( "organ",63,34,15,30);
	set_boundaries( "oven",32,32,0,32);
	set_boundaries( "piano",63,34,15,30);
	set_boundaries( "plant1",0,0,0,0); 
	set_boundaries("plant2",0,0,0,0);
	set_boundaries("plant3",0,0,0,0);
	set_boundaries("plant4",0,0,0,0);
	set_boundaries("plant5",0,0,0,0);
	set_boundaries("plant6",0,0,0,0);
	set_boundaries("plant7",0,0,0,0);
	set_boundaries("plant8",0,0,0,0);
	set_boundaries("plant9",0,0,0,0);
	set_boundaries( "pew",90,30,0,0);
	set_boundaries( "portrait",0,0,0,0);
	set_boundaries( "rug",0,0,0,0);
	set_boundaries("ruins1",465,230,0,32);
	set_boundaries("ruins2",465,230,0,32);
	set_boundaries( "rock1",84,68,0,0);
	set_boundaries( "rock2",56,39,0,0);
	set_boundaries( "rock3",38,39,0,0);
	set_boundaries( "rock4",50,41,0,0);
	set_boundaries( "rug_bear",0,0,0,0);
	set_boundaries("skeleton1",0,0,0,0);
	set_boundaries("skeleton2",0,0,0,0); 
	set_boundaries("skeleton3",0,0,0,0);
	set_boundaries("stained_window",0,0,0,0);
	set_boundaries( "side_dresser",30,80,0,7);
	set_boundaries( "stairs_down_right",0,0,0,0);
	set_boundaries( "stairs_left",0,0,0,0);
	set_boundaries( "stairs_right",0,0,0,0);
	set_boundaries( "statue",32,32,0,32);
	set_boundaries( "statue2",32,32,0,32);
	set_boundaries( "table", 28,90,0,6);
	set_boundaries( "table_hut",32,27,0,5);
	set_boundaries( "table_library",75,63,0,8);
	set_boundaries( "tent",136,155,0,12);
	set_boundaries( "tent_back",136,155,0,12);
	set_boundaries( "throne",67,32,0,32);
	set_boundaries("tree_dead",32,32,42,127);
	set_boundaries("tree_dead1",32,32,35,150);
	set_boundaries("tree_dead2",32,32,43,117);
	set_boundaries("tree_dead3",32,32,37,80);
	set_boundaries("tree_dead4",32,32,16,77);
	set_boundaries("tree_oak",32,32,29,81);
	set_boundaries("tree_oak2",32,32,64,168);
	set_boundaries("tree_pine",35,30,60,123);
	set_boundaries("tree_stump",27,18,2,7);
	set_boundaries("watchtower",64,60,0,100);
	set_boundaries( "wall_front",32,16,0,16);
	set_boundaries("well",32,32,0,32);		
	set_boundaries("wine",0,0,0,0);
	set_boundaries("nice_1_1",400,81,0,0);
	set_boundaries("nice_1_2",8,257,0,0);
	set_boundaries("nice_1_3",195,32,0,0);
	set_boundaries("nice_1_4",8,46,0,0);
	set_boundaries("nice_1_5",8,80,0,0);
	set_boundaries("nice_1_6",142,32,0,0);
	set_boundaries("nice_1_7",171,32,0,0);
	set_boundaries("nice_1_8",165,32,0,0);
	set_boundaries("nice_1_9",400,32,0,0);
	set_boundaries("nice_2_1",400,81,0,0);
	set_boundaries("nice_2_2",8,256,0,0);
	set_boundaries("nice_2_3",195,32,0,0);
	set_boundaries("nice_2_4",8,46,0,0);
	set_boundaries("nice_2_5",8,99,0,0);
	set_boundaries("nice_2_6",107,32,0,0);
	set_boundaries("nice_2_7",121,32,0,0);
	set_boundaries("nice_2_8",8,130,0,0);
	set_boundaries("nice_2_9",8,82,0,0);
	set_boundaries("nice_2_10",164,32,0,0);
	set_boundaries("nice_2_11",400,32,0,0);

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
		iDrawable* d = designate_drawable(type.first, x.first, y.first,x.second,y.second, type.second);
		if(d == NULL){
			throw std::exception("Broke");
		}
		d->set_my_type(Stationary);
		this->insert_entity(d);

	}
}
WorldName World::get_WorldName(std::string name, int name_size){

	std::string worldName = name;


	if(worldName.compare("hut_play") == 0){
		return hut_play;
	}else if(worldName.compare("hut_1") == 0){
		return hut_1;
	}else if(worldName.compare("hut_2") == 0){
		return hut_2;
	}else if(worldName.compare("hut_3") == 0){
		return hut_3;
	}else if(worldName.compare("hut_4") == 0){
		return hut_4;
	}else if(worldName.compare("hut_5") == 0){
		return hut_5;
	}else if(worldName.compare("hut_6") == 0){
		return hut_6;
	}else if(worldName.compare("hut_7") == 0){
		return hut_7;
	}else if(worldName.compare("hut_8") == 0){
		return hut_8;
	}else if(worldName.compare("hut_9") == 0){
		return hut_9;
	}else if(worldName.compare("world11") == 0){
		return main_world11;
	}else if(worldName.compare("world12") == 0){
		return main_world12;
	}else if(worldName.compare("world13") == 0){
		return main_world13;
	}else if(worldName.compare("world14") == 0){
		return main_world14;
	}else if(worldName.compare("world15") == 0){
		return main_world15;
	}else if(worldName.compare("world16") == 0){
		return main_world16;
	}else if(worldName.compare("world17") == 0){
		return main_world17;
	}else if(worldName.compare("world18") == 0){
		return main_world18;
	}else if(worldName.compare("world19") == 0){
		return main_world19;
	}else if(worldName.compare("house1") == 0){
		return house1;
	}else if(worldName.compare("house2") == 0){
		return house2;
	}else if(worldName.compare("house3") == 0){
		return house3;
	}else if(worldName.compare("house4") == 0){
		return house4;
	}else if(worldName.compare("house5") == 0){
		return house5;
	}else if(worldName.compare("house6") == 0){
		return house6;
	}else if(worldName.compare("house7") == 0){
		return house7;
	}else if(worldName.compare("house12") == 0){
		return house12;
	}else if(worldName.compare("house22") == 0){
		return house22;
	}else if(worldName.compare("house32") == 0){
		return house32;
	}else if(worldName.compare("house42") == 0){
		return house42;
	}else if(worldName.compare("house52") == 0){
		return house52;
	}else if(worldName.compare("house62") == 0){
		return house62;
	}else if(worldName.compare("house72") == 0){
		return house72;
	}else if(worldName.compare("church") == 0){
		return church;
	}else if(worldName.compare("church_upper") == 0){
		return church_upper;
	}else if(worldName.compare("keep") == 0){
		return keep;
	}else if(worldName.compare("shop1") == 0){
		return shop1;
	}else if(worldName.compare("shop2") == 0){
		return shop2;
	}else if(worldName.compare("shop3") == 0){
		return shop3;
	}else if(worldName.compare("dung_vamp1") == 0){
		return dung_vamp1;
	}else{
		throw std::exception("Broke");
	}
	return main_world11;

}

void World::make_portal(std::string items, bool is_world){
	std::pair<std::string,int> values;
	int const_index = 1;

	if(is_world){
		//world_name
		values = pull_out(items, const_index);
		WorldName converted_name = get_WorldName(values.first, values.second);
		const_index += values.second + 1;

		values = pull_out(items, const_index);
		const_index += values.second + 1;
		int x = 0;
		int y = 0;
		int xb = 0;
		int yb = 0;

		std::string str = values.first;
		
		if( str.compare("N") == 0){
			y = 32; 
			xb = 127 * 32;
			yb = 4;
		}

		else if(str.compare("S") == 0){
			y = 126 * 32;
			xb = 127 * 32;
			yb = 4;
		}
		else if(str.compare("E") == 0){
			x = 126 * 32;
			xb = 4;
			yb = 127*32;
		}

		else if(str.compare("W")==0){
			x = 32;
			xb = 4;
			yb = 127*32;
		}else{
			throw std::exception("Broke");
		}

		iDrawable* new_portal = new Portal(x,y,new Ground_Sprite("Resources//back_ground//dungeon.bmp",9,0),converted_name, 0, 0, true);
		new_portal->set_boundary_value(xb,yb, 0, 0);
		insert_entity(new_portal);
		
		return;

	}

	//world_name
	values = pull_out(items, const_index);
	WorldName converted_name = get_WorldName(values.first, values.second);
	const_index += values.second + 1;

	//x
	values = pull_out(items, const_index);
	int x = list_to_int(values.first, values.second);
	const_index += values.second + 1;

	//y
	values = pull_out(items, const_index);	
	int y = list_to_int(values.first, values.second);
	const_index += values.second + 1;

	//target x
	values = pull_out(items, const_index);
	int tarx = list_to_int(values.first, values.second);
	const_index += values.second + 1;

	//target y
	values = pull_out(items, const_index);
	int tary = list_to_int(values.first, values.second);
	const_index += values.second + 1;






	iDrawable* new_portal = new Portal(x,y,new Ground_Sprite("Resources//back_ground//dungeon.bmp",9,0),converted_name, tarx, tary, false);
	new_portal->set_boundary_value(32, 32, 0, 0);
	insert_entity(new_portal);

}

void World::make_AI(std::string items){

	int constant_index = 1;

	std::pair<std::string, int> values;

	values = pull_out(items, constant_index);
	std::string type;
	std::string file;

	constant_index += (values.second + 1);

	type = values.first;

	values = pull_out(items, constant_index);

	constant_index += (values.second + 1);
	file = values.first;

	std::string	filename = "Resources//AI_characters//";
	filename.append(file);
	filename.append(".bmp");
	Sprite* img = new Player_Sprite(filename, S, 5, 1, 16, 16);
	iDrawable* to_add;

	values = pull_out(items, constant_index);
	int x_pos = this->list_to_int(values.first, values.second);
	constant_index += (values.second+1);

	values = pull_out(items, constant_index);
	int y_pos = this->list_to_int(values.first, values.second);
	constant_index += (values.second + 1);
	std::vector<std::pair<int,int>>* ways = new std::vector<std::pair<int,int>>();

	if(type.compare("OptionPresenter") != 0 && type.compare("QuestGiver") != 0 && type.compare("ItemBestower") != 0){
		values = pull_out(items, constant_index);
		std::string cur_x;
		std::string cur_y;
		bool first = true;

		constant_index += (values.second + 1);

		for(unsigned i = 0; i < values.first.size(); i++){
			if(values.first.at(i)== ',' && first){
				first = false;
			}else if(values.first.at(i) == ',' && !first){
				first = true;
				ways->push_back(std::make_pair(list_to_int(cur_x,cur_x.size()),list_to_int(cur_y,cur_y.size())));
				cur_x = "";
				cur_y = "";
			}else if(first){
				cur_x.append(1,values.first.at(i));	
			}else{
				cur_y.append(1,values.first.at(i));
			}
		}
	}

	if(type.compare("OptionPresenter") == 0){

		make_op(items, constant_index, filename, x_pos, y_pos);
		return;

	}else if(type.compare("QuestGiver") == 0){

		make_quest_giver(items, constant_index, filename, x_pos, y_pos);
		return;

	}else if(type.compare("ItemBestower") == 0){

		make_item_bestower(items, constant_index, filename, x_pos, y_pos);
		return;

	}else if(type.compare("Guard") == 0){
		
		to_add = new Town_Guard(x_pos, y_pos, 2, 3, img, ways);
		to_add->set_boundary_value(30, 18, 1, 14);
		to_add->fetch_me_as_mob()->set_world(this);
		to_add->fetch_me_as_mob()->set_stats(1000, 1000, 1000, 1000, 1000);
		this->insert_entity(to_add);

	}else if(type.compare("Guard_Captain") == 0){

		to_add = new GuardCaptain(x_pos, y_pos, 2, 3, img, ways);
		to_add->set_boundary_value(30, 18, 1, 14);
		to_add->fetch_me_as_mob()->set_world(this);
		to_add->fetch_me_as_mob()->set_stats(1000, 1000, 1000, 1000, 1000);
		
	}else if(type.compare("Skeleton") == 0){

		//		to_add = new Skeleton(x_pos, y_pos, 2, 3, img, ways);
		//		to_add->set_boundary_value(30, 18, 1, 14);



	}else if(type.compare("Dragon") == 0){



	}else if(type.compare("Vampire") == 0){

		to_add = new Vampire(x_pos, y_pos, 2, 3, img, ways);
		to_add->set_boundary_value(30, 18, 1, 14);
		to_add->fetch_me_as_mob()->set_world(this);
		to_add->fetch_me_as_mob()->set_stats(1000, 1000, 1000, 1000, 1000);
		this->insert_entity(to_add);
		return;


	}else if(type.compare("VampireBoss")== 0){

		to_add = new VampireBoss(x_pos, y_pos, 2, 3, img, ways);
		to_add->set_boundary_value(30, 18, 1, 14);
		to_add->fetch_me_as_mob()->set_world(this);
		to_add->fetch_me_as_mob()->set_stats(1000, 1000, 1000, 1000, 1000);
		this->insert_entity(to_add);
		return;
	}else if(type.compare("Statue_Boss")== 0){


	}else if(type.compare("MarshMonster")== 0){
		to_add = new Marsh_Monster(x_pos, y_pos, 2, 3, new Player_Sprite(filename, S, 5, 1, 16, 16), ways);
		to_add->set_boundary_value(30, 18, 1, 14);
		to_add->fetch_me_as_mob()->set_world(this);
		to_add->fetch_me_as_mob()->set_stats(1000, 1000, 1000, 1000, 1000);
		this->insert_entity(to_add);
		return;


	}else if(type.compare("Rival")== 0){
		//		to_add = new Rival(x_pos, y_pos, 2, 3, img, ways);
		//	to_add->set_boundary_value(30, 18, 1, 14);



	}else if(type.compare("Person")== 0){



	}else{
		throw std::exception("Invalid AI");
	}

}

void World::make_op(std::string items, int constant_index, std::string file, int x_pos, int y_pos){	

	std::pair<Quest*, int> quest_data;
	std::pair<std::string, int> values = pull_out(items, constant_index);
	std::string dialouge;

	//file
	std::string filename;

	filename = file;

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
	}else if(str.compare("Drag")==0 ){
		return Lizard;
	}else if(str.compare("VampB") == 0){
		return VampBoss;
	}else if(str.compare("Vamp") == 0){
		return Vamp;
	}else if(str.compare("Mon")==0){
		return Monster;
	}else if(str.compare("Out")==0){
		return Outcast;
	}else if(str.compare("Riv")==0){
		return Rival;
	}else if(str.compare("Ch")==0){
		return Chicken;
	}else if(str.compare("GuardCap")==0){
		return GuardCap;
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
	//gold
	values = pull_out(items, constant_index);
	loot.gold = list_to_int(values.first, values.second);
	constant_index += (values.second + 1);

	//rep
	values = pull_out(items, constant_index);
	loot.rep = list_to_int(values.first, values.second);
	constant_index += (values.second + 1);

	//exp
	values = pull_out(items, constant_index);
	loot.exp = list_to_int(values.first, values.second);
	constant_index += (values.second + 1);

	//item
	values = pull_out(items, constant_index);
	loot.item = list_to_int(values.first, values.second);
	constant_index += (values.second + 1);

	//mission
	values = pull_out(items, constant_index);
	loot.mission = list_to_int(values.first, values.second);
	constant_index += (values.second + 1);

	IQuestObjective* obj;
	char thing = items[constant_index];
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

	}else if(items[constant_index] == 'T'){
		constant_index += 1;
		//target id
		values = pull_out(items, constant_index);
		constant_index += (values.second + 1);
		EntityType ent = get_entityType(values.first);

		//number required
		values = pull_out(items, constant_index);
		constant_index += (values.second + 1);

		obj = new InteractObjective(ent, 1);


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


		if(items.at(constant_index-1) == '!'){
			break;
		}

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


	}

}
void World::make_dialouge_qg(std::string items, int constant_index, QuestGiver* op){
	std::pair<std::string,int> values;

	while(true){


		if(items.at(constant_index-1) == '!'){
			break;
		}

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


	}

}

iDrawable* World::designate_drawable(std::string type, std::string x, std::string y, int size_x, int size_y, int type_size){

	int x_pos = World::list_to_int(x,size_x);
	int y_pos = World::list_to_int(y,size_y);
	iDrawable* new_d;

	std::string filename;

	filename.append("Resources//drawable_images//");
	filename.append(type);
	filename.append(".bmp");

	new_d = new Drawable(x_pos, y_pos,0,0, new Solid_Sprite((char*)filename.c_str()));

	std::map<std::string, drawable_bounds>::iterator location;

	location = this->drawables->find(type);

	if (location == this->drawables->end()){
		throw std::exception("Broke");
	}

	new_d->set_boundary_value((*location).second.bounds[0],(*location).second.bounds[1],(*location).second.bounds[2],(*location).second.bounds[3]);

	return new_d;


}
void World::make_quest_giver(std::string items, int constant_index, std::string filename, int x_pos, int y_pos){

	std::pair<Quest*, int> quest_data;
	std::pair<std::string, int> values = pull_out(items, constant_index);

	std::string dialouge;




	//create the quests
	quest_data = make_quest(items, constant_index);
	constant_index = quest_data.second;


	//make the guy
	QuestGiver* character = new QuestGiver(quest_data.first,x_pos, y_pos, 0, 0, new Player_Sprite((char*)filename.c_str(), S, 5, 1, 16, 16));

	//set values
	character->set_boundary_value(30, 30, 0, 0);
	character->set_world(this);

	//dialouge
	make_dialouge_qg(items, constant_index, character);

	//put in the d
	this->insert_entity(character);

}
void World::make_item_bestower(std::string items, int constant_index, std::string filename, int x_pos, int y_pos){

	std::pair<std::string, int> values = pull_out(items, constant_index);

	//shop keeper or something else
	int chest_or = list_to_int(values.first, values.second);
	constant_index += (values.second + 1); 
	//dialouge	
	std::string dialouge = values.first;
	constant_index += (values.second + 1);

	//make the guy
	ItemBestower* character = new ItemBestower(x_pos, y_pos, 0, 0, new Player_Sprite((char*)filename.c_str(), S, 5, 1, 16, 16));

	if(chest_or == 0){
		character->bestow_all_items = true;
	}

	character->append_dialogue(dialouge);
	character->set_boundary_value(30, 30, 0, 0);
	character->set_world(this);

	while(items[constant_index - 1] != '!'){

		//number he has
		values = pull_out(items, constant_index);
		constant_index += (values.second + 1);

		int numOf = list_to_int(values.first, values.second);

		//id
		values = pull_out(items, constant_index);
		constant_index += (values.second + 1);

		int id = list_to_int(values.first, values.second);

		//cost
		values = pull_out(items, constant_index);
		constant_index += (values.second + 1);

		int cost = list_to_int(values.first, values.second);		

		Equipment* item = itemDB->fetch_item(id);
		item->number_held = numOf;

		character->append_inventory(item, cost);

	}

	this->insert_entity(character);

}
