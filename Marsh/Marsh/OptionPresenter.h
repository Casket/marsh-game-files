#ifndef OPTION_PRESENTER_H
#define OPTION_PRESENTER_H 1

#include "Main.h"

using namespace std;

class OptionPresenter: public Combat{

public:
	std::vector<std::string>* pre_quest_dialogue;
	std::vector<std::string>* post_quest_dialogue;
	std::vector<Quest*>* presentable_quests;
	int current_dialogue;
	bool should_free_player;
	bool has_player_hostage;
	bool delivered_quest;
	bool should_give_options;


	OptionPresenter(int x, int y, int vel, int vel_d, Sprite* img);
	~OptionPresenter(void);

	virtual void update(void);

	virtual void append_dialogue(std::string);
	void append_post_dialogue(std::string);
	void append_pre_dialogue(std::string);
	void append_quest(Quest*);
	virtual void clear_dialogue(void);
	virtual void speak(void);

};


#endif
