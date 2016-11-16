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
		auto tap = touch->getLocation();
		for (auto ball : game_level->attack_balls){
			if (ball->getBoundingBox().containsPoint(tap)){
				ball->setTouch(touch);
                game_level->arrow = Sprite::create("red_arrow.png");
                game_level->arrow->setScale(0.5);
				game_level->arrow->setPosition(ball->getPosition());
				this->addChild(game_level->arrow);
				return true;
			}
		}
	}

	return false;
}

void Game::onTouchMoved(Touch* touch, Event* event){
	//TODO add the arrow and scale it on the basis of the length of
	//the moved vector
	if(touch != nullptr){
		auto tap = touch->getLocation();
		for (auto ball : game_level->attack_balls){
			if (ball->getTouch() != nullptr && ball->getTouch() == touch) {
				log("Location of touch moved is (%f, %f)", tap.x, tap.y);
				auto diff_x = tap.x - touch->getStartLocation().x;
				auto diff_y = tap.y - touch->getStartLocation().y;
				auto sq_dist = std::pow(diff_x, 2) + std::pow(diff_y, 2);
				auto orig_length = game_level->arrow->getTexture()->getContentSize().width;
				auto scale = sq_dist / std::pow(orig_length, 2);
				game_level->arrow->setScaleX(scale);
                auto angle = std::atan2(diff_y, diff_x);
                game_level->arrow->setRotation(CC_RADIANS_TO_DEGREES(-1 * angle));
			}
		}
	}
}

void Game::onTouchEnded(Touch* touch, Event* event){
	//TODO calculate the velocity vector and move the ball
    log("touch end called");
    game_level->arrow->runAction(RemoveSelf::create());
	if (touch != nullptr){
		auto tap = touch->getLocation();
		for (auto ball: game_level->attack_balls){
			if (ball->getTouch() != nullptr && ball->getTouch() == touch){
				auto diff_x = tap.x - touch->getStartLocation().x;
				auto diff_y = tap.y - touch->getStartLocation().y;
				ball->setVelocity(Vec2(VELOCITY_SCALE_FACTOR * diff_x, VELOCITY_SCALE_FACTOR * diff_y));
			}
		}
	}
}

void Game::update(float dt){
//	log("Update called within time %f", dt);

	for (auto ball : game_level->attack_balls){
		if (ball->getTouch() != nullptr && ball->getVelocity() != Vec2(0,0)){
			auto ball_velocity = ball->getVelocity();
			auto step_x = ball_velocity.x * dt;
			auto step_y = ball_velocity.y * dt;
			auto ball_next_posn = ball->getNextPosition();
			ball_next_posn.x += step_x;
			ball_next_posn.y += step_y;

			//Check for collision with the walls;
			
			//Left side
			if (ball_next_posn.x < ball->radius()){
				ball_next_posn.x = ball->radius();
				ball_velocity.x *= -RESTITUTION_COEFF;  // Perfect collision so no loss in velocity
			}
			//Right side
			if (ball_next_posn.x > _screen_size.width - ball->radius()){
				ball_next_posn.x = _screen_size.width - ball->radius();
				ball_velocity.x *= -RESTITUTION_COEFF;
			}
			//Top side
			if (ball_next_posn.y > _screen_size.height - ball->radius()){
				ball_next_posn.y = _screen_size.height - ball->radius();
				ball_velocity.y *= -RESTITUTION_COEFF;
			}
			// Bottom side
			if (ball_next_posn.y < ball->radius()){
				ball_next_posn.y = ball->radius();
				ball_velocity.y *= -RESTITUTION_COEFF;
			}
			ball_velocity *= BALL_DECELERATION;
			//Update info
			ball->setVelocity(ball_velocity);
			ball->setNextPosition(ball_next_posn);
			ball->setPosition(ball->getNextPosition());
		}
	}
}

