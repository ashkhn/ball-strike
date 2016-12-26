#include "LoginScene.h"
#include "Constants.h"

LoginScene::LoginScene(void){}

LoginScene::~LoginScene(void){}

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


	auto layout_param = ui::LinearLayoutParameter::create();
	layout_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	layout_param->setMargin(ui::Margin(0, 175, 175, 75));
	email_field->setLayoutParameter(layout_param);
	password_field->setLayoutParameter(layout_param);
	login_btn->setLayoutParameter(layout_param);
	register_btn->setLayoutParameter(layout_param);

	container->addChild(email_field);
	container->addChild(password_field);
	container->addChild(login_btn);
	container->addChild(register_btn);

}

void LoginScene::loginUser(Ref* sender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		log("Starting login request");
		network::HttpRequest *login_req = new network::HttpRequest();
		std::string login_url = Constants::api_base_url;
		login_url += "api/sessions";
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
		network::HttpClient::getInstance()->send(login_req);
		login_req->release();

	}
}

void LoginScene::registerUser(Ref* sender, ui::Widget::TouchEventType type){
	log("Register clicked");
}

void LoginScene::onLoginRequestCompleted(network::HttpClient *sender, network::HttpResponse *response){
	std::vector<char> *buffer = response->getResponseData();
	login_btn->setEnabled(true);

	for (int i = 0; i < buffer->size(); i++){
		log("The response was %c", (*buffer)[i]);
	}

	if( 200 == response->getResponseCode() ){
		log("Success");
	}
	else{
		log("Failure");
	}
}

void LoginScene::onRegisterRequestCompleted(network::HttpClient *sender, network::HttpResponse *response){}
