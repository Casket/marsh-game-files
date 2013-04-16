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
#include "Quest.h"

#define VISIBLE_W 1400
#define VISIBLE_H 770
#define UI_WIDTH 1400
#define UI_HEIGHT 230
#define MANA_BAR_X_POS 828
#define MANA_BAR_Y_POS 43
#define HEALTH_BAR_X_POS 1093
#define HEALTH_BAR_Y_POS 43
#define RESOURCE_BAR_WIDTH 258
#define RESOURCE_BAR_HEIGHT 33
#define CAST_BAR_X_POS 590
#define CAST_BAR_Y_POS 40
#define CAST_BAR_WIDTH 220
#define CAST_BAR_HEIGHT 80
#define CONSOLE_X_POS 43
#define CONSOLE_Y_POS 42
#define CONSOLE_WIDTH 312
#define CONSOLE_HEIGHT 155

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