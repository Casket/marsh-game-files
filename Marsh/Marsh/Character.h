#ifndef CHARACTER_H
#define CHARACTER_H 1

#include "Main.h"

using namespace std;

class Character: public Drawable {
	char** dialogue;
	int dialogue_length;

	public:
		Character(int x, int y, int vel, int vel_d, Sprite* img);
		~Character(void);
		void speak(void);
		void update_dialogue(char**, int);

	private:
};



#endif