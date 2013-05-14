#ifndef MAIN_H
#define MAIN_H 1

class Attack;
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>
#include <winalleg.h>
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
#include "Mob.h"
#include "Town_Guard.h"
#include "Marsh_Monster.h"
#include "Dragon.h"
#include "GuardCaptain.h"
#include "Reject.h"
#include "Vampire.h"
#include "Player_Sprite.h"
#include "World.h"
#include "View.h"
#include "AttackDB.h"
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
#include "PersistentAttack.h"
#include "AttackCharge.h"
#include "AttackDB.h"
#include "TeleportAttack.h"
#include "SprintSpell.h"
#include "HealthDrainAttack.h"
#include "StationaryAttack.h"
#include "TemporalModifier.h"
#include "ProtectionAttack.h"
#include "ItemDB.h"
#include "SpikeLauncher.h"

#define VISIBLE_W 1400
#define VISIBLE_H 770
#define UI_WIDTH 1400
#define UI_HEIGHT 230
#define STAT_INCREASE 10
#define DEFAULT_WORLD_TIME_DELAY 1

extern volatile int world_time_counter;
extern volatile int world_time_delay;

using namespace std;
using namespace Marsh;

bool keyrel(int k);
bool rectangle_intersection(int x1, int y1, int w1,int h1, int x2, int y2, int w2, int h2);
Equipment* get_new_equipment(void);
Equipment* get_item_clone(Equipment* old_item);

/*class Attack_Accessor{
public:
	static AttackDB* attacks;

	static void create_attacks(void){
		Attack_Accessor::attacks = new AttackDB();
	}

	static AttackDB* get_attacks(void){
		return Attack_Accessor::attacks;
	}
};*/

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