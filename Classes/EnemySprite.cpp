#include "EnemySprite.h"

EnemySprite::EnemySprite(void){}

EnemySprite::~EnemySprite(void){}

const std::string EnemySprite::sprite_paths[] = {"goku.png", "vegeta.png"};
const std::vector<Size> EnemySprite::sprite_sizes = {Size(276, 452), Size(276, 452)};

EnemySprite* EnemySprite::gameSpriteWithFile(std::string file_name){
	auto sprite = new EnemySprite();
	if(sprite && sprite->initWithFile(file_name)){
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return sprite = nullptr;
}
