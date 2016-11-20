#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define SETTINGS_FONT_SIZE 50

USING_NS_CC;


class SettingsScene : public Layer {
	public:
		SettingsScene();
		virtual ~SettingsScene();
		static Scene* createScene();
		virtual bool init() override;
		CREATE_FUNC(SettingsScene);
		void initOptions();	
		void setNumEnemies();
		void setNumBalls();
		void setScale();
		void setNumMoves();
		Size _screen_size;
		ui::ScrollView* container;
};
