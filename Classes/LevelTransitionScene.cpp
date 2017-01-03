#include "LevelTransitionScene.h"

LevelTransitionScene::LevelTransitionScene(){}
LevelTransitionScene::~LevelTransitionScene(){}

Scene* LevelTransitionScene::createScene(){
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = LevelTransitionScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;

}

bool LevelTransitionScene::init(){
	
	if(!Layer::init()){
		return false;
	}

	return true;
}


