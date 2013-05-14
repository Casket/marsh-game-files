#ifndef DISPATCH_H
#define DISPATCH_H 1


#include "Main.h"



using namespace std;


class Dispatcher{
	public:
		std::pair<std::list<Mob*>*, bool> north_front, south_front, east_front, west_front;
		std::list<Mob*>* underlings;
		iDrawable* designated_target;
		EntityType designated_type;
		bool active;

		Dispatcher(iDrawable* target, EntityType designated_type);
		~Dispatcher(void);
		void update(void);
		void locate_followers(void);
		void set_directives(void);
		void designate_directions(int call_count);
		void disassemble_queue(std::list<Mob*>*);
		
};

#endif