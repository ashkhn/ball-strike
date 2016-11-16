#include "BallSprite.h"

BallSprite::BallSprite(void){
	_velocity = Vec2(0, 0); //Initial velocity is zero
}

BallSprite::~BallSprite(void){}

const std::string BallSprite::sprite_paths[] = {"blue_ball", "green_ball", "purple_ball", "orange_ball", "red_ball"};

// Generate a sprite object from a given file. Looks in the Resources folder by default
// @param filename : defines which image file needs to be used for creating the sprite
BallSprite* BallSprite::gameSpriteWithFile(std::string filename){
	auto sprite = new BallSprite();
	auto sprite_cache = SpriteFrameCache::getInstance();
	std::string format_string = filename + "_001.png";
	if (sprite && sprite->initWithSpriteFrame(sprite_cache->getSpriteFrameByName(format_string))){
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return sprite = nullptr;
}

// Helper function to generate a sprite with a random class
BallSprite* BallSprite::generateRandomSprite(){
	int rand_color_idx = std::rand() % NUM_CLASSES;
	BallSprite* ball = gameSpriteWithFile(sprite_paths[rand_color_idx]);
	std::string format_string = sprite_paths[rand_color_idx] + "_%03d.png";
	ball->setupAnimation(format_string.c_str());
	ball->color = static_cast<Color>(rand_color_idx);
	return ball;
}

void BallSprite::setPosition(const Point& pos){
	Sprite::setPosition(pos);
	if(!_next_position.equals(pos)){
		_next_position = pos;
	}
}

void BallSprite::setSelected(bool selected){
	this->_is_selected = selected;
	if (selected){
		auto animation = Animation::createWithSpriteFrames(anim_frames, 1.0f / 4);
		this->runAction(RepeatForever::create(Animate::create(animation)));
	}else{
		this->stopAllActions();
	}
}

// Setup the flashing animation for the ball using 2 sprite images for each ball
// @param format: String specifying the file naming format
// @return : Vector containing SpriteFrames for the sprite animation
void BallSprite::setupAnimation(const char* format){
	auto sprite_cache = SpriteFrameCache::getInstance();
	char file_name[100];
	for (int i = 1; i <= NUM_SPRITES_PER_CLASS ; i++){
		sprintf(file_name, format, i);
		anim_frames.pushBack(sprite_cache->getSpriteFrameByName(file_name));

	}
	
}

// Calculates the radius to help calculate the positioning of the sprite
// @return the 'radius' of the sprite
float BallSprite::radius(){
    //  getContentSize returns the size of the spritesheet used. Hence the factor 
    // and since each ball has the same dimensions
	/* return getTexture()->getContentSize().width * 0.5; */
	return (float) 297 / 2;
}
