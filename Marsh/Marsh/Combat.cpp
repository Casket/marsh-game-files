#include "Main.h"

using namespace std;

Combat::Combat(int x, int y, int vel, int vel_d, Sprite* img)
:Drawable(x, y, vel, vel_d, img)
{
	this->vitality = BASE_VIT;
	this->intelligence = BASE_INTEL;
	this->focus = BASE_FOCUS;
	this->willpower = BASE_WILL;
	this->armor = BASE_ARMOR;

	this->health = calculate_health(this->vitality);
	this->mana = calculate_mana(this->willpower);
}

Combat::~Combat(void) {
	if(casted_spell != NULL)
		delete casted_spell;
}

void Combat::set_my_type(EntityType e) {
	this->my_type = e;
}

int Combat::calculate_health(int stat){
	return stat;
	//TODO add some calculation here
}

int Combat::calculate_mana(int stat) {
	return stat;
	// TODO do some calculation
}

void Combat::casting_update(void) {
	// should only be called if he's casting, but let's be careful
	if (!this->casting)
		return;
	if (this->casted_spell == NULL) {
		// oh balls... this is bad... this is bad...
		// player is casting AND has no spell cast... idk
		throw std::exception("COMBAT: spell casting, no current spell");
	}

	/*
	if (++this->casting_timer >= this->casted_spell->get_charge_time()){
		// ready to fire that bad boy
		this->casting = false;
		this->casting_timer = 0;
		// TODO put the casted spell into the world
		this->casted_spell = NULL;
	}
	*/ 
}

void Combat::launch_attack(int attack_num) {
		/*
	if ((attack_num >= 0) && (attack_num < MAX_ATTACKS ) {
		this->casting = true;
		Attack* used_attack = this->attack_loadout[attack_num];
		this->casted_spell = used_attack->clone();
	}
	*/ 


}
