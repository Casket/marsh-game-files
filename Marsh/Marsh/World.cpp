#include "Main.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

World::World(int tiles_w, int tiles_h){
	this->tiles_wide = tiles_w;
	this->tiles_high = tiles_h;



	Tile*** map = (Tile***)malloc(sizeof(Tile**)*tiles_h);
	for (int i=0; i < tiles_h; i++){
		map[i] = (Tile**)malloc(sizeof(Tile*)*tiles_w);
	}

	Ground_Sprite* sprite = new Ground_Sprite("grass.bmp", 0, 0);

	for (int i=0; i < tiles_h; i++){
		for (int j=0; j < tiles_w; j++){
			map[i][j] = (Tile*)malloc(sizeof(Tile));
			map[i][j]->row = i;
			map[i][j]->col = j;
			map[i][j]->background_image = sprite;
			map[i][j]->can_walk = true;
			map[i][j]->contents = new std::list<Drawable*>();
		}
	}


	this->tile_map = map;
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
}

void World::load_world(char* filename){
	char back_ground_tiles[500];

	//opens the file-stream
	fstream fin;

	fin.open(filename, ios::in);


	if(fin.is_open()){

		int row_count = 0;

		//begin moving line by line down the file
		while(!fin.eof()){

			fin.getline(back_ground_tiles, 500);

			//checks if the line is a 2nd layer drawable or a background
			if(back_ground_tiles[0] == '@'){
				int inner_index = 0; 
				int outer_index = 1;
				//drawables the format of the line will be @type(2)+xpos(...)+ypos!
				char* type = (char*)malloc((sizeof(char)) * 8);
				char* x = (char*)malloc((sizeof(char)) * 8);
				char* y = (char*)malloc((sizeof(char)) * 8);

				//grabs type
				while(back_ground_tiles[outer_index] != '+'){
					type[inner_index] = back_ground_tiles[outer_index];
					inner_index += 1;
					outer_index += 1;
				}
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
					y[inner_index] = back_ground_tiles[outer_index];
					inner_index += 1;
					outer_index += 1;
				}

				//passes the things gathered to another function that will make the object
				Drawable* to_draw = make_drawable(type, x, y, size_x, inner_index);

				free(type);
				free(x);
				free(y);
				//if background tile line formmated so that 01010101 is a rep of 4 of the same tile
			}else{
				int size = this->tiles_wide;

				char first = ' ';
				char second = ' ';

				//loops through the line grabbing every char pair and creating the neccessary tile and then adding it to the class object map array 
				for(int i = 0; i < size ; i+=2){
					first = back_ground_tiles[i];
					second = back_ground_tiles[i+1];
					convert_to_tile(first, second, row_count,i);
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

		strcpy_s(file, sizeof(char) * 100, "graphics//general.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;



	}else if(a == '1'){
		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "back_ground//water.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;
		this->tile_map[pos_x][pos_y]->can_walk = false;

	}else if(a == '2'){
		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "back_ground//grass.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;

	}else if(a == '3'){
		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "back_ground//dirt.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;


	}else if(a == '4'){

		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "back_ground//grass_marsh.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;


	}else if(a == '5'){

		sprite_x = find_x(b);
		sprite_y = 0;

		strcpy_s(file, sizeof(char) * 100, "back_ground//water_marsh.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);

		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;

		this->tile_map[pos_x][pos_y]->can_walk = false;

	}else if(a == '6'){

		sprite_x = find_x(b);
		sprite_y = 0;


		strcpy_s(file, sizeof(char) * 100, "back_ground//cobblestones.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);

		this->tile_map[pos_x][pos_y]->background_image = tile_sprite;		

	}else{
		//
	}
	free(file);

}
//creates a drawable object from the code given 
Drawable* World::make_drawable(char* type, char* x, char* y, int size_x, int size_y){


	char* file = (char*)malloc(sizeof(char) * 100);

	if(type[0] == ' '){
		strcpy_s(file, sizeof(char) * 100, "variable.bmp");	
	}else{
		//
	}

	Sprite* image = new Sprite(file,S,0,0,0,1);	

	int x_num = list_to_int(x, size_x);//these two are used
	int y_num = list_to_int(y, size_y);//to convert a char array([1,2,3]) to the int 123

	Drawable* to_draw = new Drawable(x_num,y_num,0,0,image);

	return to_draw;
}


//converts [4,2,0] to 420 or something like that based on the given and the size
int World::list_to_int(char* given, int size){
	int num_return = 0;

	for(int i = 0; i < size; i++){
		int start_int = given[i];

		for(int j = 0; j < size - i;j++){
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
	}else if(b == 'a'){
		return 10;
	}else if(b == 'b'){
		return 11;
	}else if(b == 'c'){
		return 12;
	}else{
		return -1;
	}

}

bool World::equals(World* w){
	return false;
	// TODO WRITE THIS, eventually
}