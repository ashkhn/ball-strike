#include "GameScene.h"
#include "BallSprite.h"
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


	generateLevel(is_resumed);

	return true;
}

void Game::generateLevel(bool is_resumed){

	for (int i = 0; i < NUM_BALLS_PER_LEVEL; i++){
		BallSprite* sprite = BallSprite::generateRandomSprite();
		sprite->id = i;	
		//TODO change this 
		_min_x = sprite->radius() ; 
		_min_y = sprite->radius() ;
		_max_x = _screen_size.width  - sprite->radius();
		_max_y = _screen_size.height - sprite->radius();

		float rand_x = _min_x + _max_x * ( (float) std::rand() / RAND_MAX );
		float rand_y = _min_y + _max_y * ( (float) std::rand() / RAND_MAX );
		sprite->setPosition(Vec2(rand_x, rand_y));
		this->addChild(sprite);
	}
}

