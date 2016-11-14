#pragma once

#include "cocos2d.h"

#define NUM_BALLS_PER_LEVEL 4
#define NUM_ENEMIES_PER_LEVEL 2

USING_NS_CC;

class Game : public Layer{
	public:
		Game();
		virtual ~Game();
		float _max_x;
		float _max_y;
		float _min_x;
		float _min_y;
		
		static Scene* createScene();
		virtual bool init() override;
		Size _screen_size;
		CREATE_FUNC(Game);
		void generateLevel(bool is_resumed);
};
