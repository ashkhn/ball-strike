#pragma once
#include "cocos2d.h"

class Enemy {
	public:
		int max_health;
		int curr_health;
		void attack();
		void addNewEnemy();
};
