#include "LevelTransitionScene.h"

LevelTransitionScene::LevelTransitionScene(){}
LevelTransitionScene::~LevelTransitionScene(){}

Scene* LevelTransitionScene::createScene(){
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = LevelTransitionScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;

}

bool LevelTransitionScene::init(){
	
	if(!Layer::init()){
		return false;
	}
	_screen_size = Director::getInstance()->getWinSizeInPixels();

	status_label = ui::Text::create("Loading...", "fonts/Marker Felt.ttf", 90);
	status_label->setPosition(Vec2(_screen_size.width / 2, _screen_size.height / 2));
	level_reason = UserDefault::getInstance()->getStringForKey(Constants::LEVEL_REASON, "error");

	this->addChild(status_label);
	chooseAction();
	return true;
}

/* Defines what action should be taken based on where this Scene was opened from */
void LevelTransitionScene::chooseAction(){
	if(level_reason == Constants::REASON_LEVEL_FINISH){
		log("Level finished..");
		status_label->setString("Loading new level..");
		int current_level = UserDefault::getInstance()->getIntegerForKey(Constants::KEY_USER_CURRENT_LEVEL, -1);
		updateCurrentLevel(current_level + 1);
	}
	else if(level_reason == Constants::REASON_RESUME_GAME){
		resumeGame();
	}
	else{
		log("Starting new game");
		fetchCurrentLevel();
	}

}

/* Start a new level based on current level of user */
void LevelTransitionScene::startNewGame(){
	UserDefault::getInstance()->setBoolForKey(Constants::IS_RESUMED, false);
	auto scene = Game::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
	log("Transition for new game called");
}

/* Resume a previously saved game */
void LevelTransitionScene::resumeGame(){
	UserDefault::getInstance()->setBoolForKey(Constants::IS_RESUMED, true);
	auto scene = Game::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
	log("Transition for new game called");
}


/* Fetch the current level of user from the api server */
/* Equivalent curl request: curl -H 'Content-Type:application/json; charset=utf-8' -H 'Authorization:{@auth_token}' API_BASE_URL/users/{@user_id} */
/* where @auth_token : Auth token of the user, @user_id: Id of the user */
void LevelTransitionScene::fetchCurrentLevel(){
	network::HttpRequest* fetch_level_req = new network::HttpRequest();
	int user_id = UserDefault::getInstance()->getIntegerForKey(Constants::KEY_USER_ID, -1);
	std::string auth_token = UserDefault::getInstance()->getStringForKey(Constants::KEY_AUTH_TOKEN, "");
	std::string fetch_url = Constants::API_BASE_URL;
	fetch_url+= "/users/" + std::to_string(user_id);
	fetch_level_req->setUrl(fetch_url);
	fetch_level_req->setRequestType(network::HttpRequest::Type::GET);
	fetch_level_req->setResponseCallback(CC_CALLBACK_2(LevelTransitionScene::onFetchCurrentLevelCompleted, this));
	std::vector<std::string> headers;
	headers.push_back("Content-Type:application/json; charset=utf-8");
	headers.push_back("Authorization:" + auth_token);
	fetch_level_req->setHeaders(headers);
	network::HttpClient::getInstance()->send(fetch_level_req);
	fetch_level_req->release();

}


/* Called when the request to fetch levels is completed */
/* @param sender : HttpClient used for making the request */
/* @param response: HttpResponse object containing the response of the request */
void LevelTransitionScene::onFetchCurrentLevelCompleted(network::HttpClient *sender, network::HttpResponse *response){
	std::vector<char> *buffer = response->getResponseData();
	std::string response_data(buffer->begin(), buffer->end());
	log("Response data : %s", response_data.c_str());
	Json* json = Json_create(response_data.c_str());

	if(200 == response->getResponseCode()){
		log("Success");
		Json* user_json = Json_getItem(json, Constants::KEY_USER);
		int current_level = Json_getInt(user_json, Constants::KEY_USER_CURRENT_LEVEL, -1);
		UserDefault::getInstance()->setIntegerForKey(Constants::KEY_USER_CURRENT_LEVEL, current_level);
		status_label->setString("Starting new game");
		startNewGame();
	}
	else{
		//TODO handle failure
	}
}


/* Sends a PUT request to server update the current level of the user */
/* @param new_level_id : The new level of the user to be updated */
/* Equivalent curl request: curl -x PUT -H 'Content-Type:application/json; charset= utf-8' \ */
/* -H 'Authorization:@auth_token' -d '{"user":{"current_level_id": @level_id}}' API_BASE_URL/users/{@user_id} */
/* where @auth_token: Auth Token of the user, @level_id: New level id for user, @user_id: Id of user */
void LevelTransitionScene::updateCurrentLevel(int new_level_id){
	network::HttpRequest* update_level_req = new network::HttpRequest();
	std::string auth_token = UserDefault::getInstance()->getStringForKey(Constants::KEY_AUTH_TOKEN, "");
	int user_id = UserDefault::getInstance()->getIntegerForKey(Constants::KEY_USER_ID, -1);
	std::string update_url = Constants::API_BASE_URL;
	update_url += "/users/" + std::to_string(user_id);
	update_level_req->setUrl(update_url);
	update_level_req->setRequestType(network::HttpRequest::Type::PUT);
	update_level_req->setResponseCallback(CC_CALLBACK_2(LevelTransitionScene::onUpdateCurrentLevelCompleted, this));
	std::vector<std::string> headers;
	headers.push_back("Content-Type:application/json; charset=utf-8");
	headers.push_back("Authorization:" + auth_token);
	update_level_req->setHeaders(headers);
	std::string post_data = "{\"user\":{ \"current_level_id\":" + std::to_string(new_level_id) + "}}";
	update_level_req->setRequestData(post_data.c_str(), post_data.length());
	network::HttpClient::getInstance()->send(update_level_req);
	update_level_req->release();

}


/* Called when the request to update current level of user is completed */
/* @param sender : HttpClient used for making the request */
/* @param response: HttpResponse object containing the response of the request */
void LevelTransitionScene::onUpdateCurrentLevelCompleted(network::HttpClient *sender, network::HttpResponse *response){
	std::vector<char> *buffer = response->getResponseData();
	std::string response_data(buffer->begin(), buffer->end());

	log("Response data: %s ", response_data.c_str());
	Json* json = Json_create(response_data.c_str());

	if(200 == response->getResponseCode()){
		log("Success");
		Json* user_json = Json_getItem(json, Constants::KEY_USER);
		int new_level_id = Json_getInt(user_json, Constants::KEY_USER_CURRENT_LEVEL, -1);
		UserDefault::getInstance()->setIntegerForKey(Constants::KEY_USER_CURRENT_LEVEL, new_level_id);
		status_label->setString("Starting next level");
		startNewGame();	
	}
	else{
		//TODO handle failure
	}
}
