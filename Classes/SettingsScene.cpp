#include "SettingsScene.h"

SettingsScene::SettingsScene(void){}

SettingsScene::~SettingsScene(void){}


Scene* SettingsScene::createScene(){
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = SettingsScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SettingsScene::init(){
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() ){
		return false;
	}
	initOptions();
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(SettingsScene::handleBack, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

/* Handle back button press on android */
void SettingsScene::handleBack(EventKeyboard::KeyCode key_code, Event* event){
	if (key_code == EventKeyboard::KeyCode::KEY_BACK){
		if(UserDefault::getInstance()->getBoolForKey(Constants::IS_USER_LOGGED_IN, true)){
			Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HomeScreen::createScene()));
		}
		else{
			Director::getInstance()->replaceScene(TransitionFade::create(1.0f, LoginScene::createScene()));
		}
	}
}

/* Initialize the settings screen */
void SettingsScene::initOptions(){
	_screen_size = Director::getInstance()->getWinSizeInPixels();
	container = ui::ScrollView::create();
	container->setPosition(Point(15, 5));
	container->setContentSize(Size(_screen_size.width - 30.0f, _screen_size.height - 30.0f));
	container->setDirection(ui::ScrollView::Direction::VERTICAL);
	auto container_size = Size(_screen_size.width, _screen_size.height);
	container->setInnerContainerSize(container_size);
	container->setLayoutType(ui::Layout::Type::VERTICAL);

    auto logout_btn = ui::Button::create("button_normal.png", "button_pressed.png", "button_disabled.png");
	logout_btn->setTitleText("Logout");
	logout_btn->setTitleColor(cocos2d::Color3B::BLACK);
	logout_btn->addTouchEventListener(CC_CALLBACK_2(SettingsScene::logoutUser, this));
	logout_btn->setScale(6);
	auto layout_param = ui::LinearLayoutParameter::create();
	layout_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	layout_param->setMargin(ui::Margin(75, 475, 75, 75));
	logout_btn->setLayoutParameter(layout_param);

	status_label = ui::Text::create("", "fonts/Marker Felt.ttf", 90);
	status_label->setLayoutParameter(layout_param);

	container->addChild(logout_btn);
	container->addChild(status_label);
	this->addChild(container);

}

/* Sends a logout request to server */
/* Equivalent curl request: curl -X DELETE -H 'Content-Type:application/json; charset=utf-8' API_BASE_URL/sessions/{@auth_token} */
/* where @auth_token : Auth token of the user */
void SettingsScene::logoutUser(Ref* sender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		std::string logout_url = Constants::API_BASE_URL;
		std::string auth_token = UserDefault::getInstance()->getStringForKey(Constants::KEY_AUTH_TOKEN, "");
		logout_url += "/sessions/" + auth_token;
		network::HttpRequest *logout_req = NetworkUtils::createNetworkRequest(logout_url, false);
		logout_req->setRequestType(network::HttpRequest::Type::DELETE);
		logout_req->setResponseCallback(CC_CALLBACK_2(SettingsScene::onLogoutRequestCompleted, this));
		network::HttpClient::getInstance()->send(logout_req);
		logout_req->release();
	}
}


/* Called when the request to logout user is completed */
/* @param sender : HttpClient used for making the request */
/* @param response: HttpResponse object containing the response of the request */
void SettingsScene::onLogoutRequestCompleted(network::HttpClient* client, network::HttpResponse *response){
	std::vector<char> *buffer = response->getResponseData();
	std::string response_data(buffer->begin(), buffer->end());
	if(204 == response->getResponseCode()){
		UserDefault::getInstance()->setBoolForKey(Constants::IS_USER_LOGGED_IN, false);
		status_label->setString("Successfully logged out");
		status_label->setTextColor(Color4B::GREEN);
	}
	else{
		status_label->setString("Error occurred. Please try again later");
		status_label->setTextColor(Color4B::RED);
	}

}
