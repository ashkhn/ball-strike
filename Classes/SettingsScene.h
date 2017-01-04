#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include "Constants.h"
#include "HomeScene.h"
#include "LoginScene.h"
#include "NetworkUtils.h"

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
		void logoutUser(Ref* sender, ui::Widget::TouchEventType type);
		void onLogoutRequestCompleted(network::HttpClient* sender, network::HttpResponse* response);
		Size _screen_size;
		ui::ScrollView* container;
		ui::Text* status_label;
};
