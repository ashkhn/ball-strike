#pragma once
#include "cocos2d.h"
#include "BallSprite.h"
#include "EnemySprite.h"


USING_NS_CC;

class GameLevel {
	public:
		GameLevel();
		virtual ~GameLevel();
		std::vector<BallSprite*> attack_balls;
		std::vector<EnemySprite*> enemies;
		void initLevel(int num_balls, int num_enemies);
};
