#include "GameScene.h"
#include "GameLevel.h"
#include "HomeScene.h"
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
	bg_sprite->setScale(scale_x, scale_y * 1.2);
	this->addChild(bg_sprite);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
	
	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyReleased = CC_CALLBACK_2(Game::handleBack, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);

	generateLevel(is_resumed);
	this->scheduleUpdate();

	return true;
}

void Game::handleBack(EventKeyboard::KeyCode key_code, Event* event){
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HomeScreen::createScene()));
}

void Game::generateLevel(bool is_resumed){

	game_level = new GameLevel(_screen_size);
	game_level->initLevel();

	auto adv_sprite = Sprite::create("type_advantage.png");
	adv_sprite->setScale(2);
	auto sprite_h = adv_sprite->getBoundingBox().size.height / 2;
	auto sprite_w = adv_sprite->getBoundingBox().size.width / 2;
	adv_sprite->setPosition(Vec2(_screen_size.width - sprite_w, _screen_size.height - sprite_h ));
	this->addChild(adv_sprite);

	for(auto ball: game_level->attack_balls){
		this->addChild(ball);
	}

	for (auto enemy: game_level->enemies){
		this->addChild(enemy->sprite);
		this->addChild(enemy->hits_left);
	}
}


bool Game::onTouchBegan(Touch* touch, Event* event){
	if(touch != nullptr && !game_level->is_busy){
		auto tap = touch->getLocation();
		for (auto ball : game_level->attack_balls){
			if (ball->getBoundingBox().containsPoint(tap)){
				ball->setTouch(touch);
				game_level->is_busy = true;
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
                auto angle = std::atan2(-diff_y, diff_x);
                game_level->arrow->setRotation(CC_RADIANS_TO_DEGREES(angle) - 180);
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
				auto diff_x = touch->getStartLocation().x - tap.x;
				auto diff_y = touch->getStartLocation().y - tap.y;
				ball->setVelocity(Vec2(VELOCITY_SCALE_FACTOR * diff_x, VELOCITY_SCALE_FACTOR * diff_y));
			}
		}
	}
}

void Game::update(float dt){
	for (auto ball : game_level->attack_balls){
		if (ball->getVelocity() != Vec2(0,0)){
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
			//check for collision with other balls;
			for (auto other_ball : game_level->attack_balls){
				if (other_ball-> id != ball->id){
					auto dist_vec = other_ball->getPosition() - ball->getPosition();
					auto future_dist_vec = other_ball->getNextPosition() - ball_next_posn;
					auto sum_radii = std::pow(2 * ball->radius(), 2);
					if (dist_vec.lengthSquared() <= sum_radii || future_dist_vec.lengthSquared() <= sum_radii){
						log("Collision about to happen");
						auto collision_axis = other_ball->getPosition() - ball->getPosition();
						float ball_vel_mag = ball_velocity.length();
						float other_vel_mag = other_ball->getVelocity().length();
						
						//Calculate angles
						float alpha = getAngle(ball_velocity, collision_axis);
						float beta = getAngle(other_ball->getVelocity(), collision_axis);
						float phi = getAngle(collision_axis, Vec2(0,0));
						ball_velocity.x = other_vel_mag * std::cos(beta) * std::cos(phi) - ball_vel_mag * std::sin(alpha) * std::sin(phi);
						ball_velocity.y = other_vel_mag * std::cos(beta) * std::sin(phi) + ball_vel_mag * std::sin(alpha) * std::cos(phi);
						auto other_vel_x = ball_vel_mag * std::cos(alpha) * std::cos(phi) - other_vel_mag * std::sin(beta) * std::sin(phi);
						auto other_vel_y = ball_vel_mag * std::cos(alpha) * std::sin(phi) + other_vel_mag * std::sin(beta) * std::cos(phi);
						other_ball->setVelocity(Vec2(other_vel_x, other_vel_y));
						auto other_step_x = other_ball->getVelocity().x;
						auto other_step_y = other_ball->getVelocity().y;
						auto norm_axis = collision_axis.getNormalized();
						ball_next_posn -= 0.5f * ball->radius() * norm_axis;
					}
				}
			}

			//check for colllisions with enemy
			for (auto enemy : game_level->enemies){
				auto dist_vec = enemy->sprite->getPosition() - ball->getPosition();
				auto future_dist_vec = enemy->sprite->getPosition() - ball_next_posn;
				auto sum_radii = std::pow(ball->radius() + enemy->sprite->radius(), 2);
				if(dist_vec.lengthSquared() <= sum_radii || future_dist_vec.lengthSquared() <= sum_radii){
					if(enemy->current_hits < enemy->max_hits){
						enemy->getHit();
						log("Enemy collision about to happen");
						auto collision_axis = dist_vec;
						float ball_vel_mag = ball_velocity.length();
						float alpha = getAngle(ball_velocity, collision_axis);
						float phi = getAngle(collision_axis, Vec2(0,0));
						ball_velocity.x = -1.0 * ball_vel_mag * std::cos(alpha) * std::cos(phi) - ball_vel_mag * std::sin(alpha) * std::sin(phi);
						ball_velocity.y = -1.0 * ball_vel_mag * std::cos(alpha) * std::sin(phi) + ball_vel_mag * std::sin(alpha) * std::cos(phi);
						auto norm_axis = collision_axis.getNormalized();
						ball_next_posn -= 30 * norm_axis;
					}
					else{
						auto enemy_posn = std::find(game_level->enemies.begin(), game_level->enemies.end(), enemy);
						game_level->enemies.erase(enemy_posn);
						enemy->sprite->runAction(RemoveSelf::create());
						enemy->hits_left->runAction(RemoveSelf::create());
					}
				}
			}

			ball_velocity *= BALL_DECELERATION;
			//Update info
			if (ball_velocity.lengthSquared() < 10){
				ball_velocity = Vec2(0, 0);
				ball->setTouch(nullptr);
				game_level->is_busy = false;
			}
			ball->setVelocity(ball_velocity);
			ball->setNextPosition(ball_next_posn);
			ball->setPosition(ball->getNextPosition());
		}
			
	}
}


float Game::getAngle(Vec2 a, Vec2 b){
	float num = b.x * a.y - b.y * a.x;
	float den = a.x * b.x + b.y * a.y;
	return std::atan2(den, num);
}
