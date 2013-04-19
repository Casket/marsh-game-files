/*#ifndef QUEST_GIVER_H
#define QUEST_GIVER_H 1

#include "Main.h"

using namespace std;

class QuestGiver: public Combat{

public:
	std::vector<std::string>* pre_quest_dialogue;
	std::vector<std::string>* post_quest_dialogue;
	int current_dialogue;
	bool should_free_player;
	bool has_player_hostage;
	bool delivered_quest;
	Quest* quest_to_give;


	QuestGiver(Quest* give, int x, int y, int vel, int vel_d, Sprite* img);
	~QuestGiver(void);

	virtual void append_dialogue(std::string);
	virtual void clear_dialogue(void);
	virtual void speak(void);

};


#endif*/