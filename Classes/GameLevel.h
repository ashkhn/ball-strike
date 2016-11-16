#pragma once
#include "cocos2d.h"
#include "BallSprite.h"
#include "Enemy.h"

#define NUM_BALLS_PER_LEVEL 1
#define NUM_ENEMIES_PER_LEVEL 0
USING_NS_CC;


class GameLevel {
	public:
		GameLevel(Size screen_size);
		virtual ~GameLevel();
		std::vector<BallSprite*> attack_balls;
		std::vector<Enemy*> enemies;
		Sprite* arrow;
		void initLevel();
		Size _screen_size;
};
