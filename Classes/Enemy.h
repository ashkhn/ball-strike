#pragma once
#include "cocos2d.h"
#include "EnemySprite.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class Enemy {
	public:
		Enemy(int max_hits);
		virtual ~Enemy();
		EnemySprite* sprite;
		Label* hits_left;
		int max_hits;
		int current_hits;
};
