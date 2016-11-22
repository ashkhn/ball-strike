#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define SETTINGS_FONT_SIZE 90
#define SLIDER_SCALE 5
#define MARGIN 75

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
		void saveValues(Ref* sender, ui::Widget::TouchEventType type);
		Size _screen_size;
		ui::ScrollView* container;
};
