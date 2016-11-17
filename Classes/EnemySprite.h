#pragma once
#include "cocos2d.h"
USING_NS_CC;

enum EnemyClass{
	BLUE,
	PURPLE
};


class EnemySprite : public Sprite{
	public:
		EnemySprite();
		virtual ~EnemySprite();
		static EnemySprite* gameSpriteWithFile(std::string file_name);
		float width();
		float height();
		float radius();
		EnemyClass enemy_class;
		const static std::string sprite_paths[];
		const static std::vector<Size> sprite_sizes;
		static EnemySprite* generateRandomSprite();
		Vec2 getTextPosition();
};
