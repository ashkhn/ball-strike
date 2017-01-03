#include "LoginScene.h"
#include "Constants.h"
#include "spine/Json.h"
#include "HomeScene.h"
#include "Database.h"

LoginScene::LoginScene(){}

LoginScene::~LoginScene(){}

Scene* LoginScene::createScene(){
	auto scene = Scene::create();
	auto layer = LoginScene::create();

	scene->addChild(layer);

	return scene;
}

bool LoginScene::init(){
	if(!Layer::init()){
		return false;
	}
		
	_screen_size = Director::getInstance()->getVisibleSize();

	// Set the background
	auto bg_sprite = Sprite::create("grass.png");
	auto scale_x = _screen_size.width / bg_sprite->getContentSize().width;
	auto scale_y = _screen_size.height / bg_sprite->getContentSize().height;
	bg_sprite->setPosition(Vec2(_screen_size.width * 0.5, _screen_size.height * 0.5));
	bg_sprite->setScale(scale_x, scale_y);
	this->addChild(bg_sprite);

	initViews();

	this->addChild(container);
	return true;
}

void LoginScene::initViews(){
	container = ui::ScrollView::create();
	container->setPosition(Point(0,0));
	container->setContentSize(Size(_screen_size.width - 30.0f, _screen_size.height - 30.0f));
	container->setDirection(ui::ScrollView::Direction::VERTICAL);
	container->setInnerContainerSize(_screen_size);
	container->setLayoutType(ui::Layout::Type::VERTICAL);
	
	email_field = ui::TextField::create("","fonts/Marker Felt.ttf", 60);
	password_field = ui::TextField::create("","fonts/Marker Felt.ttf",60);
	email_field->setPlaceHolder("Enter email");
	password_field->setPlaceHolder("Enter password");
	email_field->setPlaceHolderColor(Color3B::WHITE);
	password_field->setPlaceHolderColor(Color3B::WHITE);
	password_field->setPasswordEnabled(true);
	
	login_btn = ui::Button::create("button_normal.png", "button_pressed.png", "button_disabled.png");
	register_btn = ui::Button::create("button_normal.png", "button_pressed.png", "button_disabled.png");
	login_btn->setTitleText("Login");
	register_btn->setTitleText("Register");
	login_btn->setTitleColor(Color3B::BLACK);
	register_btn->setTitleColor(Color3B::BLACK);
	login_btn->setScale(6);
	register_btn->setScale(6);
	login_btn->addTouchEventListener(CC_CALLBACK_2(LoginScene::loginUser,this));
	register_btn->addTouchEventListener(CC_CALLBACK_2(LoginScene::registerUser, this));

	status_label = ui::Text::create("", "fonts/Marker Felt.ttf", 60);


	auto layout_param = ui::LinearLayoutParameter::create();
	layout_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	layout_param->setMargin(ui::Margin(0, 175, 175, 75));
	email_field->setLayoutParameter(layout_param);
	password_field->setLayoutParameter(layout_param);
	login_btn->setLayoutParameter(layout_param);
	register_btn->setLayoutParameter(layout_param);
	status_label->setLayoutParameter(layout_param);

	container->addChild(email_field);
	container->addChild(password_field);
	container->addChild(login_btn);
	container->addChild(register_btn);
	container->addChild(status_label);

}

void LoginScene::loginUser(Ref* sender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		log("Starting login request");
		status_label->setString("Loading..");
		network::HttpRequest *login_req = new network::HttpRequest();
		std::string login_url = Constants::API_BASE_URL;
		login_url += "/sessions";
		login_req->setUrl(login_url);
		login_req->setRequestType(network::HttpRequest::Type::POST);
		login_req->setResponseCallback(CC_CALLBACK_2(LoginScene::onLoginRequestCompleted, this));
		std::vector<std::string> headers;
		headers.push_back("Content-Type:application/json; charset=utf-8");
		std::string login_username = email_field->getString();
		std::string login_password = password_field->getString();
		std::string post_data = "{\"email\":\"" + login_username + "\",\"password\":\"" + login_password + "\"}"; 
		log("Posting data %s", post_data.c_str());
		login_req->setRequestData(post_data.c_str(), post_data.length());
		login_req->setHeaders(headers);
		login_btn->setEnabled(false);
		register_btn->setEnabled(false);
		network::HttpClient::getInstance()->send(login_req);
		login_req->release();

	}
}

void LoginScene::registerUser(Ref* sender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		log("Starting register request");
		status_label->setString("Loading..");
		network::HttpRequest *register_req = new network::HttpRequest();
		std::string register_url = Constants::API_BASE_URL;
		register_url += "/users";
		register_req->setUrl(register_url);
		register_req->setRequestType(network::HttpRequest::Type::POST);
		register_req->setResponseCallback(CC_CALLBACK_2(LoginScene::onRegisterRequestCompleted, this));
		std::vector<std::string> headers;
		headers.push_back("Content-Type:application/json; charset=utf-8");
		std::string register_username = email_field->getString();
		std::string register_password = password_field->getString();
		std::string post_data = "{\"user\": {\"email\":\"" + register_username + "\",\"password\":\"" + register_password + "\"}}"; 
		log("Posting data %s", post_data.c_str());
		register_req->setRequestData(post_data.c_str(), post_data.length());
		register_req->setHeaders(headers);
		register_btn->setEnabled(false);
		login_btn->setEnabled(false);
		network::HttpClient::getInstance()->send(register_req);
		register_req->release();

	}
}

void LoginScene::fetchGameLevels(std::string auth_token){
	log("Fetching game levels");
	status_label->setString("Fetching game levels...");
	network::HttpRequest *fetch_req = new network::HttpRequest();
	std::string fetch_url = Constants::API_BASE_URL;
	fetch_url += "/gamelevels";
	fetch_req->setUrl(fetch_url);
	fetch_req->setRequestType(network::HttpRequest::Type::GET);
	fetch_req->setResponseCallback(CC_CALLBACK_2(LoginScene::onLevelFetchRequestCompleted, this));
	std::vector<std::string> headers;
	headers.push_back("Content-Type:application/json; charset=utf-8");
	headers.push_back("Authorization:"+auth_token);
	fetch_req->setHeaders(headers);
	network::HttpClient::getInstance()->send(fetch_req);
	fetch_req->release();
}

void LoginScene::onLoginRequestCompleted(network::HttpClient *sender, network::HttpResponse *response){
	std::vector<char> *buffer = response->getResponseData();

	std::string response_data(buffer->begin(), buffer->end());
	log("Response data : %s", response_data.c_str());
	

	Json *json = Json_create(response_data.c_str());
	if( 200 == response->getResponseCode() ){
		log("Success");
		status_label->setString("Successfully logged in");
		status_label->setTextColor(Color4B::GREEN);
		std::string auth_token = Json_getString(json, Constants::KEY_AUTH_TOKEN, "error");
		UserDefault::getInstance()->setStringForKey(Constants::KEY_AUTH_TOKEN, auth_token);
		UserDefault::getInstance()->setBoolForKey(Constants::IS_USER_LOGGED_IN, true);
		fetchGameLevels(auth_token);
	}
	else{
		log("Failure");
		status_label->setTextColor(Color4B::RED);
		login_btn->setEnabled(true);
		register_btn->setEnabled(true);
		status_label->setString(Json_getString(json, Constants::KEY_ERRORS, "error"));
	}

}

void LoginScene::onRegisterRequestCompleted(network::HttpClient *sender, network::HttpResponse *response){
	std::vector<char> *buffer = response->getResponseData();
	std::string response_data(buffer->begin(), buffer->end());
	
	log("The response was %s", response_data.c_str());

	if( 201 == response->getResponseCode() ){
		log("User registered");
		status_label->setString(" User registered.\n Please login using credentials");
		status_label->setTextColor(Color4B::GREEN);
	}
	else{
		log("Failure");
		status_label->setTextColor(Color4B::RED);
		status_label->setString("Invalid data");
	}
	login_btn->setEnabled(true);
	register_btn->setEnabled(true);

}

void LoginScene::onLevelFetchRequestCompleted(network::HttpClient *sender, network::HttpResponse *response){
	std::vector<char> *buffer = response->getResponseData();
	std::string response_data(buffer->begin(), buffer->end());
	log("The response was %s", response_data.c_str());
	login_btn->setEnabled(true);
	register_btn->setEnabled(true);

	
	if( 200 == response->getResponseCode() ){
		Json* json = Json_create(response_data.c_str());
		log("Success");
		//TODO save game levels in db;
		Json* level_content = json->child;
		int i = 0;
		bool insert_status = true;
		while ( i < json->size ){
			int num_enemies = Json_getInt(level_content, "num_enemies", -1);
			int num_balls = Json_getInt(level_content, "num_balls", -1);
			int num_hits_per_enemy  = Json_getInt(level_content, "num_hits_per_enemy", -1);
			insert_status &= Database::createLevel(num_enemies, num_balls, num_hits_per_enemy);
			i++;
		}
		if(insert_status){
			status_label->setString("Game data updated.");
			Director::getInstance()->replaceScene(TransitionFade::create(3.0f, HomeScreen::createScene()));
		}else{
			status_label->setTextColor(Color4B::RED);
			status_label->setString("Error saving game data. Please login again");
		}
			
	}
	else{
		log("Failure");
		status_label->setTextColor(Color4B::RED);
		status_label->setString("Error receiving game data. Please login again");
	}
}
