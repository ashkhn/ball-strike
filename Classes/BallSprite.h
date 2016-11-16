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
		CC_SYNTHESIZE(Vec2, _velocity, Velocity);
		CC_SYNTHESIZE(Vec2, _next_position, NextPosition);
		BallSprite();
		virtual ~BallSprite();
		static BallSprite* gameSpriteWithFile(std::string file_name);
		static BallSprite* generateRandomSprite();
		float radius();
		Vector<SpriteFrame*> anim_frames;
		Color color; // Contains the class of the ball
		int id; // Unique identifier for the sprite
		const static std::string sprite_paths[];  // Contains the paths to the sprite image files
		void setupAnimation(const char* format);
		void setSelected(bool selected);
		bool getSelected();
		virtual void setPosition(const Point& pos);
	private:
		bool _is_selected;
};
