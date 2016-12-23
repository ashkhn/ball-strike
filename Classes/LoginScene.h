#pragma once

#include "cocos2d.h"
#include "network/HttpClient.h"

USING_NS_CC;

class LoginScene: public Layer{
	public:
		LoginScene();
		virtual ~LoginScene();
		virtual bool init() override;
		static Scene* createScene();
		CREATE_FUNC(LoginScene);
		void onRequestCompleted(network::HttpClient *sender, network::HttpResponse *response);
};
