#include "GameLevel.h"

GameLevel::GameLevel(Size screen_size){
	this->_screen_size = screen_size;
}

GameLevel::~GameLevel(void){}

void GameLevel::initLevel(){
	
	attack_balls.clear();
	enemies.clear();
	float min_x, min_y, max_x, max_y;


	for (int i = 0; i < NUM_BALLS_PER_LEVEL; i++){
		BallSprite* new_ball = BallSprite::generateRandomSprite();
		new_ball->id = i + 1;
		min_x = new_ball->radius();
		min_y = new_ball->radius();
		max_x = _screen_size.width - min_x;
		max_y = _screen_size.height - min_y;
		float rand_x = min_x + max_x * ( (float) std::rand() / RAND_MAX );
		float rand_y = min_y + max_y * ( (float) std::rand() / RAND_MAX );
		auto position = Vec2(rand_x, rand_y);
		//Check for intersections
		for (auto ball : attack_balls){
			// TODO check for a better way to handle intersections..
			while (ball->getBoundingBox().containsPoint(position)){
				float rand_x = min_x + max_x * ( (float) std::rand() / RAND_MAX );
				float rand_y = min_y + max_y * ( (float) std::rand() / RAND_MAX );
				position = Vec2(rand_x, rand_y);
			}
		}

		new_ball->setPosition(position);
		attack_balls.push_back(new_ball);
	}

	for (int  i = 0; i < NUM_ENEMIES_PER_LEVEL; i++){
		auto enemy = new Enemy(4);
		min_x = enemy->sprite->width();
		min_y = enemy->sprite->height();
		max_x = _screen_size.width - min_x;
		max_y = _screen_size.height - min_y;
		float rand_x = min_x + max_x * ( (float) std::rand() / RAND_MAX );
		float rand_y = min_y + max_y * ( (float) std::rand() / RAND_MAX );
		auto position = Vec2(rand_x, rand_y);

		//TODO check for intersections
		
		enemy->sprite->setPosition(position);
        auto text_posn = enemy->sprite->getTextPosition();
		enemy->hits_left->setPosition(text_posn);
		enemies.push_back(enemy);
	}


}
