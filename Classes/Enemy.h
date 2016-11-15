#pragma once
#include "cocos2d.h"
#include "EnemySprite.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class Enemy {
	public:
		Enemy();
		virtual ~Enemy();
		int max_health;
		int curr_health;
		EnemySprite sprite;
		ui::LoadingBar health;
		Label health_label;
		void addNewEnemy(Vec2 position);
};
