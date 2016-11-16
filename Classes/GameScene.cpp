#include "GameScene.h"
#include "GameLevel.h"
USING_NS_CC;


Game::Game(void){}

Game::~Game(void){}

Scene* Game::createScene(){
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = Game::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Game::init(){
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() ){
		return false;
	}
	
	//Initialize rand generator and display variables
	std::srand(time(NULL));
	_screen_size = Director::getInstance()->getWinSizeInPixels();
	
	// Load the sprite frame cache
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ball_sprites.plist");
	bool is_resumed = UserDefault::getInstance()->getBoolForKey("is_resumed");

	auto bg_sprite = Sprite::create("grass.png");
	auto scale_x = _screen_size.width / bg_sprite->getContentSize().width;
	auto scale_y = _screen_size.height / bg_sprite->getContentSize().height;
	bg_sprite->setPosition(Vec2(_screen_size.width / 2 , _screen_size.height / 2));
	bg_sprite->setScale(scale_x, scale_y);
	this->addChild(bg_sprite);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	generateLevel(is_resumed);
	this->scheduleUpdate();

	return true;
}

void Game::generateLevel(bool is_resumed){

	game_level = new GameLevel(_screen_size);
	game_level->initLevel();
	
	for(auto ball: game_level->attack_balls){
		this->addChild(ball);
	}

	for (auto enemy: game_level->enemies){
		this->addChild(enemy->sprite);
		this->addChild(enemy->hits_left);
	}
}


bool Game::onTouchBegan(Touch* touch, Event* event){
	if(touch != nullptr){

	}

	return false;
}

void Game::onTouchMoved(Touch* touch, Event* event){
	//TODO add the arrow and scale it on the basis of the length of
	//the moved vector
}

void Game::onTouchEnded(Touch* touch, Event* event){
	//TODO calculate the velocity vector and move the ball 
}

void Game::update(float dt){
//	log("Update called within time %f", dt);
}

