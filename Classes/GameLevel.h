#pragma once
#include "cocos2d.h"
#include "BallSprite.h"
#include "Enemy.h"

#define NUM_BALLS_PER_LEVEL 2
#define NUM_ENEMIES_PER_LEVEL 0
#define RESTITUTION_COEFF 0.8f
#define VELOCITY_SCALE_FACTOR 8.0f
#define BALL_DECELERATION 0.98f

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
		bool is_busy;
};
