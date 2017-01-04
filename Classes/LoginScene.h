#pragma once

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "spine/Json.h"
#include "ui/CocosGUI.h"
#include "Constants.h"
#include "HomeScene.h"
#include "Database.h"


USING_NS_CC;

class LoginScene: public Layer{
	public:
		LoginScene();
		virtual ~LoginScene();
		virtual bool init() override;
		static Scene* createScene();
		CREATE_FUNC(LoginScene);
		Size _screen_size;
		std::string user_email;
		std::string user_password;
		ui::ScrollView* container;
		ui::TextField* email_field;
		ui::TextField* password_field;
		ui::Text* status_label;
		ui::Button* login_btn;
		ui::Button* register_btn;
		void initViews();
		void loginUser(Ref* sender, ui::Widget::TouchEventType type);
		void registerUser(Ref* sender, ui::Widget::TouchEventType type);
		void fetchGameLevels(std::string auth_token);
		void onLoginRequestCompleted(network::HttpClient *sender, network::HttpResponse *response);
		void onRegisterRequestCompleted(network::HttpClient *sender, network::HttpResponse *response);
		void onLevelFetchRequestCompleted(network::HttpClient *sender, network::HttpResponse *response);

};
