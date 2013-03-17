#include "Main.h"


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