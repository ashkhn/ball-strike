#include "Enemy.h"

/* Construct an Enemy object with the max_hits it can take */
/* @param max_hits: Defines the max number of hits that the this enemy can take before dying */
Enemy::Enemy(int max_hits){
	this->max_hits = max_hits;
	this->sprite = EnemySprite::generateRandomSprite();
	this->current_hits = 0;
	std::string format = "%d";
	char max_hit_str[100];
	sprintf(max_hit_str, format.c_str(), max_hits);
	this->hits_left = Label::createWithTTF(max_hit_str, "fonts/arial.ttf", 72);
}


Enemy::~Enemy(void){}

/* Helper method to carry out hit action for the enemy object */
/* Updates the remaining hits label and current hits */
void Enemy::getHit(){
	current_hits +=1;
	int num_hits_left = max_hits - current_hits;
	std::string format = "%d";
	char hit_string[100];
	sprintf(hit_string, format.c_str(), num_hits_left);
	hits_left->setString(hit_string);
}
