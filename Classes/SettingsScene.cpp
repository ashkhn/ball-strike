#include "SettingsScene.h"
#include "Database.h"

static std::vector<int> num_enemy_values = {2, 4, 8, 10};
static std::vector<int> num_ball_values = {1, 2, 3, 4};
static std::vector<float> scale_values = {0.3, 0.4, 0.5, 0.8, 1.0, 1.3, 1.4, 1.5, 1.8, 2.0};

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
	auto save_btn = ui::Button::create("button_normal.png", "button_pressed.png", "button_disabled.png");
	save_btn->setTitleText("Save");
	save_btn->setTitleColor(cocos2d::Color3B::BLACK);
	save_btn->addTouchEventListener(CC_CALLBACK_2(SettingsScene::saveValues, this));
	save_btn->setScale(6);
	auto layout_param = ui::LinearLayoutParameter::create();
	layout_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	layout_param->setMargin(ui::Margin(MARGIN, MARGIN, MARGIN, MARGIN));
	save_btn->setLayoutParameter(layout_param);

	container->addChild(save_btn);
	this->addChild(container);

}

void SettingsScene::setNumEnemies(){
	auto num_enemies_hint = ui::Text::create("Number of enemies", "fonts/arial.ttf", SETTINGS_FONT_SIZE);
	auto num_enemies_slider = ui::Slider::create();
	num_enemies_slider->loadBarTexture("slider_back.png");
	num_enemies_slider->loadSlidBallTextures("slidernode_normal.png", "slidernode_pressed.png", "slidernode_disable.png");
	num_enemies_slider->loadProgressBarTexture("slider_pressbar.png");
	auto &local_num_enemies = chosen_num_enemies;
	num_enemies_slider->addEventListener([num_enemies_hint, &local_num_enemies](Ref* sender, ui::Slider::EventType type){
			auto slider = dynamic_cast<ui::Slider*>(sender);
			if(type ==ui::Slider::EventType::ON_PERCENTAGE_CHANGED){
					int chosen_value_idx = (num_enemy_values.size()) * slider->getPercent() / 100;
					chosen_value_idx = (slider->getPercent() == 100 ? chosen_value_idx - 1 : chosen_value_idx);
					char num_enemies_label[100];
					std::string format_string = "Number of enemies : %d";
					local_num_enemies = num_enemy_values[chosen_value_idx];
					sprintf(num_enemies_label, format_string.c_str(),local_num_enemies);
					num_enemies_hint->setString(num_enemies_label);

			}
			});

	num_enemies_slider->setPosition(Vec2(_screen_size.width / 2, _screen_size.height / 2));
	num_enemies_slider->setScale(SLIDER_SCALE);
	auto layout_param = ui::LinearLayoutParameter::create();
	layout_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	layout_param->setMargin(ui::Margin(MARGIN, MARGIN, MARGIN, MARGIN));
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
	auto &local_num_balls = chosen_num_balls;
	num_balls_slider->addEventListener([num_balls_hint, &local_num_balls](Ref* sender, ui::Slider::EventType type){
			auto slider = dynamic_cast<ui::Slider*>(sender);
			if(type ==ui::Slider::EventType::ON_PERCENTAGE_CHANGED){
					int chosen_value_idx = (num_ball_values.size()) * slider->getPercent() / 100;
					chosen_value_idx = (slider->getPercent() == 100 ? chosen_value_idx - 1 : chosen_value_idx);
					char num_balls_label[100];
					std::string format_string = "Number of balls : %d";
					local_num_balls = num_ball_values[chosen_value_idx];
					sprintf(num_balls_label, format_string.c_str(), local_num_balls);
					num_balls_hint->setString(num_balls_label);
			}
			});

	num_balls_slider->setPosition(Vec2(_screen_size.width / 2, _screen_size.height / 2));
	num_balls_slider->setScale(SLIDER_SCALE);
	auto layout_param = ui::LinearLayoutParameter::create();
	layout_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	layout_param->setMargin(ui::Margin(MARGIN, MARGIN, MARGIN, MARGIN));
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
	auto &local_scale = chosen_scale;
	scale_slider->addEventListener([scale_hint, &local_scale](Ref* sender, ui::Slider::EventType type){
			auto slider = dynamic_cast<ui::Slider*>(sender);
			if(type ==ui::Slider::EventType::ON_PERCENTAGE_CHANGED){
				int chosen_value_idx = (scale_values.size()) * slider->getPercent() / 100;
				chosen_value_idx = (slider->getPercent() == 100 ? chosen_value_idx - 1 : chosen_value_idx);
				char scale_label[100];
				std::string format_string = "Scaling for balls : %.2f";
				local_scale = scale_values[chosen_value_idx];
				sprintf(scale_label, format_string.c_str(), local_scale);
				scale_hint->setString(scale_label);
			}
			});
	scale_slider->setPosition(Vec2(_screen_size.width / 2, _screen_size.height / 2));
	scale_slider->setScale(SLIDER_SCALE);
	auto layout_param = ui::LinearLayoutParameter::create();
	layout_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	layout_param->setMargin(ui::Margin(MARGIN, MARGIN, MARGIN, MARGIN));
	scale_hint->setLayoutParameter(layout_param);
	scale_slider->setLayoutParameter(layout_param);
	container->addChild(scale_hint);
	container->addChild(scale_slider);

}

void SettingsScene::saveValues(Ref* sender, ui::Widget::TouchEventType type){

	char insert_stmt[200];
	std::string format_string = "insert into game_data(num_enemies, num_balls, scale) values(%d, %d, %f)";

	switch(type){
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			sprintf(insert_stmt, format_string.c_str(), chosen_num_enemies, chosen_num_balls, chosen_scale);
			log("The status for save is %d", Database::execute(insert_stmt));
			break;
		default:
			break;
	};
}
