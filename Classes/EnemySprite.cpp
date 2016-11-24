#include "EnemySprite.h"

EnemySprite::EnemySprite(void){}

EnemySprite::~EnemySprite(void){}

const std::string EnemySprite::sprite_paths[] = {"goku.png", "vegeta.png"};

/* Generate a sprite based on the file name */
/* @param file_name : File name of the image */ 
EnemySprite* EnemySprite::gameSpriteWithFile(std::string file_name){
	auto sprite = new EnemySprite();
	if (sprite && sprite->initWithFile(file_name)){
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return sprite = nullptr;

}

/* Generate a random sprite */
EnemySprite* EnemySprite::generateRandomSprite(){
	int rand_idx = std::rand() % 2;
	auto sprite = gameSpriteWithFile(sprite_paths[rand_idx]);
	sprite->enemy_class = static_cast<EnemyClass> (rand_idx);
	return sprite;
}

/* Calculate the half width for positioning*/
/* @return half width of the sprite */
float EnemySprite::width(){
	return getTexture()->getContentSize().width * 0.5;
}

/* Calculate the half height for positoning */
/* @return half height of the sprite */
float EnemySprite::height(){
	return getTexture()->getContentSize().height * 0.5;
}

/* Calculate the half diagonal length as an estimate for collision radius */
/* @return collision radius based on height and width of the sprite */
float EnemySprite::radius(){
	float height = EnemySprite::height();
	float width = EnemySprite::width();
	auto rad_sq = std::pow(height, 2) + std::pow(width, 2);
	return std::pow(rad_sq , 0.5);
}

/* Calculate the position of the hits_left text label for the sprite */
/* @return vector containing the calculated positions */
Vec2 EnemySprite::getTextPosition(){
	auto sprite_posn = getPosition();
	auto x_cord = sprite_posn.x;
	auto y_cord = sprite_posn.y + (getTexture()->getContentSize().height / 2);
	return Vec2(x_cord, y_cord);
}
