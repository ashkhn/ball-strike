#pragma once
#include "cocos2d.h"

USING_NS_CC;

#define NUM_CLASSES 5 // Number of classes for the ball
#define NUM_SPRITES_PER_CLASS 2 // Number of sprite image files available for each class

enum class Color{
	BLUE,
	GREEN,
	PURPLE,
	ORANGE,
	RED
};

class BallSprite : public Sprite{
	public:
		CC_SYNTHESIZE(Touch*, _touch, Touch);
		BallSprite();
		virtual ~BallSprite();
		static BallSprite* gameSpriteWithFile(std::string file_name);
		static BallSprite* generateRandomSprite();
		float radius();
		Color color; // Contains the class of the ball
		int id; // Unique identifier for the sprite
		const static std::string sprite_paths[];  // Contains the paths to the sprite image files
		static Vector<SpriteFrame*> setupAnimation(const char* format);

};
