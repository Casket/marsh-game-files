#ifndef MAIN_H
#define MAIN_H 1

class Attack;


#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>
#include <list>
#include <string>
#include "Sprite.h"
#include "Ground_Sprite.h"
#include "Solid_Sprite.h"
#include "Attack_Sprite.h"
#include "iDrawable.h"
#include "Drawable.h"
#include "Character.h"
#include "Attack.h"
#include "Combat.h"
#include "Player.h"
#include "Guard.h"
#include "Player_Sprite.h"
#include "World.h"
#include "View.h"
#include "Portal.h"
#include "QuestManager.h"
#include "IQuestObjective.h"
#include "RetrieveObjective.h"
#include "KillObjective.h"



using namespace std;


class Player_Accessor{
public:
	static Player* hero;
	
	static void create_player(int x, int y, Sprite* img, int w, int h, int x_in, int y_in){
		Player_Accessor::hero = new Player(x, y, 0, 0, img);
		Player_Accessor::hero->set_boundary_value(w, h, x_in, y_in);
	}

	static Player* get_player(void){
		return Player_Accessor::hero;
	}
};


#endif