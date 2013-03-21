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

	for (int i=0; i < tiles_h; i++){
		for (int j=0; j < tiles_w; j++){
			map[i][j] = (Tile*)malloc(sizeof(Tile));
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

		while(!fin.eof()){
			
			fin.getline(back_ground_tiles, 500);

			if(back_ground_tiles[0] == '+'){
				
			}else{
				int size = 500;

				char first = ' ';
				char second = ' ';

				for(int i = 1; i < size ; i+=2){
					Tile* t_to_add = (Tile*)malloc(sizeof(Tile));
					first = back_ground_tiles[i];
					second = back_ground_tiles[i+1];
					t_to_add = convert_to_tile(first, second, row_count, i);
				}
				row_count += 1;
			}
		}


		fin.close();

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

Tile* World::convert_to_tile(char a, char b, int pos_x, int pos_y){
	int sprite_x = -1;
	int sprite_y = -1;

	Tile* new_tile = (Tile*)malloc(sizeof(Tile));

	if(a == '0'){
		sprite_x = find_x(b);
		sprite_y = 0;

		char* file = (char*)malloc(sizeof(char) * 100);
		strcpy_s(file, sizeof(char) * 100, "variable.bmp");

		Ground_Sprite* tile_sprite = new Ground_Sprite(file, sprite_x,sprite_y);


		new_tile->background_image = tile_sprite;
		new_tile->row = pos_x;
		new_tile->col = pos_y;
		new_tile->can_walk = true;

		return new_tile;

	}else if(a == '1'){
		sprite_x = find_x(b);
		sprite_y = 0;
		


	}else if(a == '2'){
		sprite_x = find_x(b);
		sprite_y = 0;

	}else if(a == '3'){
		sprite_x = find_x(b);
		sprite_y = 0;

	}else if(a == '4'){
		sprite_x = find_x(b);
		sprite_y = 0;

	}else{
		return NULL;
	}

	return NULL;
}
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