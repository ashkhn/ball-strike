#include "GameLevel.h"
#include "Database.h"
#include "Constants.h"

/* Initialize the game level object with the screen size */
GameLevel::GameLevel(Size screen_size){
	this->_screen_size = screen_size;
}

GameLevel::~GameLevel(void){}

/* Generate a new game level */
void GameLevel::initLevel(){
	
	attack_balls.clear();
	enemies.clear();
	loadValues(); //Load configuration from database
	float min_x, min_y, max_x, max_y;
	
	// Generate balls
	for (int i = 0; i < num_balls; i++){
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
		new_ball->setScale(ball_scale);
		new_ball->setPosition(position);
		attack_balls.push_back(new_ball);
	}

	// Generate enemies
	for (int  i = 0; i < num_enemies; i++){
		auto enemy = new Enemy(20);
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


/* Loads game configuration from database */
void GameLevel::resumeLevel(){
	attack_balls.clear();
	enemies.clear();
	loadValues();
	
	std::string ball_query_stmt = "select * from ball_save_data";
	std::vector<std::vector<std::string>> ball_results = Database::getQueryResults(ball_query_stmt.c_str());
	log("Number of ball results is %lu", ball_results.size());
	for(int i = 0; i < ball_results.size(); i++){
		std::vector<std::string> curr_ball_values = ball_results[i];
		int ball_color = std::stoi(curr_ball_values[1]);
		auto ball_sprite = BallSprite::gameSpriteWithFile(BallSprite::sprite_paths[ball_color]);
		ball_sprite->id = std::stoi(curr_ball_values[0]);
		ball_sprite->color = static_cast<Color> (ball_color);
		ball_sprite->setScale(std::stof(curr_ball_values[2]));
		auto posn_x = std::stof(curr_ball_values[3]);
		auto posn_y = std::stof(curr_ball_values[4]);
		ball_sprite->setPosition(Vec2(posn_x, posn_y));
		attack_balls.push_back(ball_sprite);
	}

	std::string enemy_query_stmt = "select * from enemy_save_data";
	std::vector<std::vector<std::string>> enemy_results = Database::getQueryResults(enemy_query_stmt.c_str());
	for(int i = 0; i < enemy_results.size(); i++){
		std::vector<std::string> curr_enemy_values = enemy_results[i];
		int enemy_class = std::stoi(curr_enemy_values[1]);
		int hits_left = std::stoi(curr_enemy_values[2]);
		auto enemy = new Enemy(hits_left);
		enemy->sprite = EnemySprite::gameSpriteWithFile(EnemySprite::sprite_paths[enemy_class]);
		enemy->sprite->enemy_class = static_cast<EnemyClass> (enemy_class);

		enemy->sprite->setPositionX(std::stof(curr_enemy_values[3]));
		enemy->sprite->setPositionY(std::stof(curr_enemy_values[4]));
		enemy->hits_left->setPosition(enemy->sprite->getTextPosition());
		enemies.push_back(enemy);
	}


}

void GameLevel::loadValues(){
	std::string query_stmt = "select * from game_data";
	std::vector<std::vector<std::string>> results = Database::getQueryResults(query_stmt.c_str());
	num_enemies = std::stoi(results[0][0]);
	num_balls = std::stoi(results[0][1]);
	ball_scale = std::stof(results[0][2]);
}


void GameLevel::saveLevel(){
	log("Save level called");
	std::string flush_ball_query = "delete from ball_save_data";
	std::string flush_enemy_query = "delete from enemy_save_data";
	
	bool flush_status = Database::execute(flush_ball_query) && Database::execute(flush_enemy_query);
	
	if(flush_status){
		//Save ball data
		bool ball_state_saved = true;
		for(auto ball : attack_balls){
			std::string insert_ball_format = "insert into ball_save_data(ball_class, scale, posn_x, posn_y) values(%d, %f, %f, %f)";
			char insert_ball_stmt[200];
			int ball_class = static_cast<int> (ball->color);
			float ball_scale = ball->getScale();
			float posn_x = ball->getPositionX();
			float posn_y = ball->getPositionY();
			sprintf(insert_ball_stmt, insert_ball_format.c_str(), ball_class, ball_scale, posn_x, posn_y);
			ball_state_saved &= Database::execute(insert_ball_stmt);
		}
		
		//Save enemy data
		bool enemy_data_saved = true;
		for(auto enemy : enemies){
			std::string insert_enemy_format = "insert into enemy_save_data(enemy_class, hits_left, posn_x, posn_y) values(%d, %d, %f, %f)";
			char insert_enemy_stmt[200];
			int enemy_class = static_cast <int> (enemy->sprite->enemy_class);
			int hits_left = enemy->max_hits - enemy->current_hits;
			float posn_x = enemy->sprite->getPositionX();
			float posn_y = enemy->sprite->getPositionY();
			sprintf(insert_enemy_stmt, insert_enemy_format.c_str(), enemy_class, hits_left, posn_x, posn_y);
			enemy_data_saved &= Database::execute(insert_enemy_stmt);
		}

		if(ball_state_saved && enemy_data_saved){
			UserDefault::getInstance()->setBoolForKey(Constants::SAVE_EXISTS, true);
			log("Game saved.");
		}
	}

}

