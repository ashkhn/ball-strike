#pragma once

#include "cocos2d.h"

#define MENU_FONT_SIZE 92

USING_NS_CC;


class HomeScreen: public Layer{
	public:
		HomeScreen();
		virtual ~HomeScreen();
		static Scene* createScene();
		virtual bool init() override;
		MenuItemLabel* resume_game;
		MenuItemLabel* start_game;
		MenuItemLabel* quit_game;
		MenuItemLabel* settings;
		Size _screen_size;
		CREATE_FUNC(HomeScreen);
		void initMenu();
		void gameQuitCallback(Ref* sender);
		void startGameCallback(Ref* sender);
		void resumeGameCallback(Ref* sender);
		void settingsCallback(Ref* sender);
};
