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
		void handleBack(EventKeyboard::KeyCode key_code, Event* event);
		template <class T> void addSlider(std::string format_string, std::vector<T> values, T &chosen_value);
		void setNumEnemies();
		void setNumBalls();
		void setScale();
		void getPreviousValues();
		void saveValues(Ref* sender, ui::Widget::TouchEventType type);
		Size _screen_size;
		ui::ScrollView* container;
		int chosen_num_enemies;
		int chosen_num_balls;
		float chosen_scale;
};
