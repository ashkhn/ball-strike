#include "HomeScene.h"
#include "GameScene.h"


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

	auto bg_sprite = Sprite::create("grass.png");
	auto scale_x = _screen_size.width / bg_sprite->getContentSize().width;
	auto scale_y = _screen_size.height / bg_sprite->getContentSize().height;
	bg_sprite->setPosition(Vec2(_screen_size.width * 0.5, _screen_size.height * 0.5));
	bg_sprite->setScale(scale_x * 1.4, scale_y * 1.2);
	this->addChild(bg_sprite);

	initMenu();
	return true;
}

void HomeScreen::initMenu(){

	bool save_exists = UserDefault::getInstance()->getBoolForKey("save_exists");

	Menu *menu;

	auto start_label = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", MENU_FONT_SIZE);
	auto quit_label = Label::createWithTTF("Quit","fonts/Marker Felt.ttf", MENU_FONT_SIZE);
	start_game = MenuItemLabel::create(start_label, CC_CALLBACK_1(HomeScreen::startGameCallback, this));
	quit_game = MenuItemLabel::create(quit_label, CC_CALLBACK_1(HomeScreen::gameQuitCallback, this));
	
	if (save_exists){
		auto resume_label = Label::createWithTTF("Resume game", "fonts/Marker Felt.ttf", MENU_FONT_SIZE);
		resume_game = MenuItemLabel::create(resume_label, CC_CALLBACK_1(HomeScreen::resumeGameCallback, this));
		resume_game->setPosition(Point(_screen_size.width * 3 / 5, (_screen_size.height / 4) * 3));
		start_game->setPosition(Point(_screen_size.width * 3 / 5, (_screen_size.height / 4) * 2));
		quit_game->setPosition(Point(_screen_size.width * 3 / 5, (_screen_size.height / 4) * 1));

		menu = Menu::create(resume_game, start_game, quit_game, NULL);
	}
	else{
		start_game->setPosition(Point(_screen_size.width * 3 / 5, (_screen_size.height / 3) * 2));
		quit_game->setPosition(Point(_screen_size.width * 3 / 5, (_screen_size.height / 3) * 1));

		menu = Menu::create(start_game, quit_game, NULL);
	}

	menu->setPosition(Point(0,0));
	
	this->addChild(menu);

}

void HomeScreen::startGameCallback(Ref* sender){
	Director::getInstance()->replaceScene(TransitionFade::create(1, Game::createScene()));
}

void HomeScreen::resumeGameCallback(Ref* sender){
	UserDefault::getInstance()->setBoolForKey("is_resumed", true);
}


void HomeScreen::gameQuitCallback(Ref* sender){
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
