#pragma once
#include "cocos2d.h"
USING_NS_CC;

class EnemySprite : public Sprite{
	public:
		EnemySprite();
		virtual ~EnemySprite();
		static EnemySprite* gameSpriteWithFile(std::string file_name);
		float radius();
		const static std::string sprite_paths[];
		const static std::vector<Size> sprite_sizes;

};
