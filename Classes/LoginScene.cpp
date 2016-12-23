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

}

void LoginScene::registerUser(Ref* sender, ui::Widget::TouchEventType type){

}

void LoginScene::onLoginRequestCompleted(network::HttpClient *sender, network::HttpResponse *response){
	std::vector<char> *buffer = response->getResponseData();

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
