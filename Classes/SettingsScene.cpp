#include "SettingsScene.h"


static int num_enemy_values[] = {2, 4, 8, 10};
static int num_ball_values[] = {1, 2, 3, 4};
static float scale_values[] = {0.3, 0.4, 0.5, 0.8, 1.0, 1.3, 1.4, 1.5, 1.8, 2.0};

SettingsScene::SettingsScene(void){}

SettingsScene::~SettingsScene(void){}


Scene* SettingsScene::createScene(){
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = SettingsScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SettingsScene::init(){
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() ){
		return false;
	}
	initOptions();
	return true;
}

void SettingsScene::initOptions(){
	_screen_size = Director::getInstance()->getVisibleSize();
	container = ui::ScrollView::create();
	container->setPosition(Point(15, 5));
	container->setContentSize(Size(_screen_size.width - 30.0f, _screen_size.height - 30.0f));
	container->setDirection(ui::ScrollView::Direction::VERTICAL);
	auto container_size = Size(_screen_size.width, _screen_size.height);
	container->setInnerContainerSize(container_size);
	container->setLayoutType(ui::Layout::Type::VERTICAL);
	setNumEnemies();
	setNumBalls();
	setScale();
	this->addChild(container);

}

void SettingsScene::setNumEnemies(){
	auto num_enemies_hint = ui::Text::create("Number of enemies", "fonts/arial.ttf", SETTINGS_FONT_SIZE);
	auto num_enemies_slider = ui::Slider::create();
	num_enemies_slider->loadBarTexture("slider_back.png");
	num_enemies_slider->loadSlidBallTextures("slidernode_normal.png", "slidernode_pressed.png", "slidernode_disable.png");
	num_enemies_slider->loadProgressBarTexture("slider_pressbar.png");
	num_enemies_slider->addEventListener([](Ref* sender, ui::Slider::EventType type){
			auto slider = dynamic_cast<ui::Slider*>(sender);
			if(type ==ui::Slider::EventType::ON_PERCENTAGE_CHANGED){
					log("Slider percent %d", slider->getPercent());
			}
			});

	num_enemies_slider->setPosition(Vec2(_screen_size.width / 2, _screen_size.height / 2));
	num_enemies_slider->setScale(4);
	auto layout_param = ui::LinearLayoutParameter::create();
	layout_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	layout_param->setMargin(ui::Margin(50, 50, 50, 50));
	num_enemies_hint->setLayoutParameter(layout_param);
	num_enemies_slider->setLayoutParameter(layout_param);
	container->addChild(num_enemies_hint);
	container->addChild(num_enemies_slider);

}

void SettingsScene::setNumBalls(){
	auto num_balls_hint = ui::Text::create("Number of balls", "fonts/arial.ttf",SETTINGS_FONT_SIZE);
	auto num_balls_slider = ui::Slider::create();
	num_balls_slider->loadBarTexture("slider_back.png");
	num_balls_slider->loadSlidBallTextures("slidernode_normal.png", "slidernode_pressed.png", "slidernode_disable.png");
	num_balls_slider->loadProgressBarTexture("slider_pressbar.png");
	num_balls_slider->addEventListener([](Ref* sender, ui::Slider::EventType type){
			auto slider = dynamic_cast<ui::Slider*>(sender);
			if(type ==ui::Slider::EventType::ON_PERCENTAGE_CHANGED){
					log("Slider percent %d", slider->getPercent());
			}
			});

	num_balls_slider->setPosition(Vec2(_screen_size.width / 2, _screen_size.height / 2));
	num_balls_slider->setScale(4);
	auto layout_param = ui::LinearLayoutParameter::create();
	layout_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	layout_param->setMargin(ui::Margin(50, 50, 50, 50));
	num_balls_hint->setLayoutParameter(layout_param);
	num_balls_slider->setLayoutParameter(layout_param);
	container->addChild(num_balls_hint);
	container->addChild(num_balls_slider);

}

void SettingsScene::setScale(){
	auto scale_hint = ui::Text::create("Scaling for balls", "fonts/arial.ttf", SETTINGS_FONT_SIZE);
	auto scale_slider = ui::Slider::create();
	scale_slider->loadBarTexture("slider_back.png");
	scale_slider->loadSlidBallTextures("slidernode_normal.png", "slidernode_pressed.png", "slidernode_disable.png");
	scale_slider->loadProgressBarTexture("slider_pressbar.png");
	scale_slider->addEventListener([](Ref* sender, ui::Slider::EventType type){
			auto slider = dynamic_cast<ui::Slider*>(sender);
			if(type ==ui::Slider::EventType::ON_PERCENTAGE_CHANGED){
					log("Slider percent %d", slider->getPercent());
			}
			});

	scale_slider->setPosition(Vec2(_screen_size.width / 2, _screen_size.height / 2));
	scale_slider->setScale(4);
	auto layout_param = ui::LinearLayoutParameter::create();
	layout_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	layout_param->setMargin(ui::Margin(50, 50, 50, 50));
	scale_hint->setLayoutParameter(layout_param);
	scale_slider->setLayoutParameter(layout_param);
	container->addChild(scale_hint);
	container->addChild(scale_slider);

}


