#pragma once
#include "cocos2d.h"
#include "BallSprite.h"
#include "Enemy.h"
#include "Database.h"
#include "Constants.h"

#define RESTITUTION_COEFF 0.8f
#define VELOCITY_SCALE_FACTOR 8.0f
#define BALL_DECELERATION 0.98f
#define VELOCITY_THRESHOLD 10

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
		int num_balls;
		int num_enemies;
		int num_hits_per_enemy;
		void loadValues();
		void saveLevel();
		void resumeLevel();
};
