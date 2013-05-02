#ifndef MAIN_H
#define MAIN_H 1

class Attack;


#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>
#include <list>
#include <string>
#include <vector>
#include <cmath>
#include <omp.h>
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
#include "Town_Guard.h"
#include "Player_Sprite.h"
#include "World.h"
#include "View.h"
#include "Portal.h"
#include "QuestManager.h"
#include "IQuestObjective.h"
#include "RetrieveObjective.h"
#include "KillObjective.h"
#include "InteractObjective.h"
#include "Quest.h"
#include "QuestGiver.h"
#include "OptionPresenter.h"
#include "ItemBestower.h"
#include "MultiSpawnAttack.h"
#include "StunningAttack.h"


#define VISIBLE_W 1400
#define VISIBLE_H 770
#define UI_WIDTH 1400
#define UI_HEIGHT 230
#define STAT_INCREASE 10

using namespace std;

bool keyrel(int k);
Equipment* get_new_equipment(void);
Equipment* get_item_clone(Equipment* old_item);


class Player_Accessor{
public:
	static Player* hero;
	
	static void create_player(int x, int y, Sprite* img, int w, int h, int x_in, int y_in){
		Player_Accessor::hero = new Player(x, y, 0, 0, img);
		Player_Accessor::hero->set_boundary_value(w, h, x_in, y_in);
		Player_Accessor::hero->set_my_type(Hero);
	}

	static Player* get_player(void){
		return Player_Accessor::hero;
	}
};


#endif