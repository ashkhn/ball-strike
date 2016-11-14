#include "BallSprite.h"

BallSprite::BallSprite(void){}

BallSprite::~BallSprite(void){}

const std::string BallSprite::sprite_paths[] = {"blue_ball", "green_ball", "purple_ball", "orange_ball", "red_ball"};

// Generate a sprite object from a given file. Looks in the Resources folder by default
// @param filename : defines which image file needs to be used for creating the sprite
BallSprite* BallSprite::gameSpriteWithFile(std::string filename){
	auto sprite = new BallSprite();
	std::string format_string = filename + "_%03d.png";
	Vector<SpriteFrame*> frames = setupAnimation(format_string.c_str());
	auto animation = Animation::createWithSpriteFrames(frames, 1.0f/4); // 4 frames per second
	if (sprite && sprite->initWithSpriteFrame(frames.front())){
		sprite->autorelease();
		sprite->runAction(RepeatForever::create(Animate::create(animation)));
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return sprite = nullptr;
}

// Helper function to generate a sprite with a random class
BallSprite* BallSprite::generateRandomSprite(){
	int rand_color_idx = std::rand() % NUM_CLASSES;
	BallSprite* ball = gameSpriteWithFile(sprite_paths[rand_color_idx]);
	ball->color = static_cast<Color>(rand_color_idx);
	return ball;
}

// Setup the flashing animation for the ball using 2 sprite images for each ball
// @param format: String specifying the file naming format
// @return : Vector containing SpriteFrames for the sprite animation
Vector<SpriteFrame*> BallSprite::setupAnimation(const char* format){
	auto sprite_cache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> frames;
	char file_name[100];
	for (int i = 1; i <= NUM_SPRITES_PER_CLASS ; i++){
		sprintf(file_name, format, i);
		frames.pushBack(sprite_cache->getSpriteFrameByName(file_name));
	}
	
	return frames;
}

// Calculates the radius to help calculate the positioning of the sprite
// @return the 'radius' of the sprite
float BallSprite::radius(){
    // Scaling has been modified for current sprite sizes.
	return getTexture()->getContentSize().width;
}
