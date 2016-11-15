#pragma once

#include "cocos2d.h"
#include "GameLevel.h"

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
