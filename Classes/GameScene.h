#pragma once

#include "cocos2d.h"
#include "GameLevel.h"
#include "LevelTransitionScene.h"

USING_NS_CC;

class Game : public Layer{
	public:
		Game();
		virtual ~Game();
		static Scene* createScene();
		virtual bool init() override;
		Size _screen_size;
		GameLevel* game_level;
		CREATE_FUNC(Game);
		virtual void update(float dt) override;
		void generateLevel(bool is_resumed);
		virtual bool onTouchBegan(Touch* touch, Event* event) override;
		virtual void onTouchMoved(Touch* touch, Event* event) override;
		virtual void onTouchEnded(Touch* touch, Event* event) override;
		void handleBack(EventKeyboard::KeyCode key_code, Event* event);
		float getAngle(Vec2 a, Vec2 b);
};
