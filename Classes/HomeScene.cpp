#include "HomeScene.h"

USING_NS_CC;

HomeScreen::HomeScreen(void){}
HomeScreen::~HomeScreen(void){}


Scene* HomeScreen::createScene(){
	auto scene = Scene::create();

	auto layer = HomeScreen::create();

	scene->addChild(layer);

	return scene;
}

bool HomeScreen::init(){
	if(!Layer::init()){
		return false;
	}
	
	_screen_size = Director::getInstance()->getVisibleSize();

	initMenu();
	return true;
}

void HomeScreen::initMenu(){

	bool save_exists = UserDefault::getInstance()->getBoolForKey("save_exists");

	Menu *menu;

	auto start_label = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf",92);
	auto score_label = Label::createWithTTF("High Scores", "fonts/Marker Felt.ttf",92);
	auto quit_label = Label::createWithTTF("Quit","fonts/Marker Felt.ttf", 92);
	start_game = MenuItemLabel::create(start_label, CC_CALLBACK_1(HomeScreen::startGameCallback, this));
	high_scores = MenuItemLabel::create(score_label, CC_CALLBACK_1(HomeScreen::scoreCallback, this));
	quit_game = MenuItemLabel::create(quit_label, CC_CALLBACK_1(HomeScreen::gameQuitCallback, this));
	
	if (save_exists){
		auto resume_label = Label::createWithTTF("Resume game", "fonts/Marker Felt.ttf", 92);
		resume_game = MenuItemLabel::create(resume_label, CC_CALLBACK_1(HomeScreen::resumeGameCallback, this));
		resume_game->setPosition(Point(_screen_size.width / 2, (_screen_size.height / 5) * 4));
		start_game->setPosition(Point(_screen_size.width / 2, (_screen_size.height / 5) * 3));
		high_scores->setPosition(Point(_screen_size.width / 2, (_screen_size.height / 5) * 2));
		quit_game->setPosition(Point(_screen_size.width / 2, (_screen_size.height / 5) * 1));

		menu = Menu::create(resume_game, start_game, high_scores, quit_game, NULL);
	}
	else{
		start_game->setPosition(Point(_screen_size.width / 2, (_screen_size.height / 4) * 3));
		high_scores->setPosition(Point(_screen_size.width / 2, (_screen_size.height / 4) * 2));
		quit_game->setPosition(Point(_screen_size.width / 2, (_screen_size.height / 4) * 1));

		menu = Menu::create(start_game, high_scores, quit_game, NULL);
	}

	menu->setPosition(Point(0,0));
	
	this->addChild(menu);

}

void HomeScreen::startGameCallback(Ref* sender){
}

void HomeScreen::resumeGameCallback(Ref* sender){
	UserDefault::getInstance()->setBoolForKey("is_resumed", true);
}

void HomeScreen::scoreCallback(Ref* sender){

}

void HomeScreen::gameQuitCallback(Ref* sender){
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
