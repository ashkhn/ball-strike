#include "Enemy.h"

Enemy::Enemy(int max_hits){
	this->max_hits = max_hits;
	this->sprite = EnemySprite::generateRandomSprite();
	this->current_hits = 0;
	this->hits_left = Label::createWithTTF("2", "fonts/arial.ttf", 72);
}

Enemy::~Enemy(void){}

