#pragma once

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "ui/CocosGUI.h"
#include "spine/Json.h"

USING_NS_CC;

class LevelTransitionScene: public Layer {
	public:
		LevelTransitionScene();
		virtual ~LevelTransitionScene();
		static Scene* createScene();
		virtual bool init() override;
		CREATE_FUNC(LevelTransitionScene);
		ui::Text* status_label;
		void getCurrentLevel();
		void updateCurrentLevel(int new_level_id);
		void onFetchCurrentLevelCompleted(network::HttpClient *sender, network::HttpResponse *response);
		void onUpdateCurrentLevelCompleted(network::HttpClient *sender, network::HttpResponse *response);
	
};
