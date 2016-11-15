#pragma once
#include "cocos2d.h"
#include "BallSprite.h"
#include "EnemySprite.h"

#define NUM_BALLS_PER_LEVEL 5
#define NUM_ENEMIES_PER_LEVEL 2
USING_NS_CC;


class GameLevel {
	public:
		GameLevel();
		virtual ~GameLevel();
		std::vector<BallSprite*> attack_balls;
		std::vector<EnemySprite*> enemies;
		void initLevel();
};
