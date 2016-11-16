#include "EnemySprite.h"

EnemySprite::EnemySprite(void){}

EnemySprite::~EnemySprite(void){}

const std::string EnemySprite::sprite_paths[] = {"goku.png", "vegeta.png"};
const std::vector<Size> EnemySprite::sprite_sizes = {Size(178, 209), Size(148, 238)};

EnemySprite* EnemySprite::gameSpriteWithFile(std::string file_name){
	auto sprite = new EnemySprite();
	if(sprite && sprite->initWithFile(file_name)){
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return sprite = nullptr;

}

EnemySprite* EnemySprite::generateRandomSprite(){
	int rand_idx = std::rand() % 2;
	auto sprite = gameSpriteWithFile(sprite_paths[rand_idx]);
	sprite->enemy_class = static_cast<EnemyClass> (rand_idx);
	return sprite;
}

float EnemySprite::width(){
	return getTexture()->getContentSize().width * 0.5;
}

float EnemySprite::height(){
	return getTexture()->getContentSize().height * 0.5;
}

Vec2 EnemySprite::getTextPosition(){
	auto sprite_posn = getPosition();
	auto x_cord = sprite_posn.x;
	auto y_cord = sprite_posn.y + (getTexture()->getContentSize().height / 2);
	return Vec2(x_cord, y_cord);
}
