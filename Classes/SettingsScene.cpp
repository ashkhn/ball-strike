#include "SettingsScene.h"
#include "Database.h"
#include "HomeScene.h"

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
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(SettingsScene::handleBack, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

/* Handle back button press on android */
void SettingsScene::handleBack(EventKeyboard::KeyCode key_code, Event* event){
	if (key_code == EventKeyboard::KeyCode::KEY_BACK){
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HomeScreen::createScene()));
	}
}

/* Initialize the settings screen */
void SettingsScene::initOptions(){
	_screen_size = Director::getInstance()->getVisibleSize();
	container = ui::ScrollView::create();
	container->setPosition(Point(15, 5));
	container->setContentSize(Size(_screen_size.width - 30.0f, _screen_size.height - 30.0f));
	container->setDirection(ui::ScrollView::Direction::VERTICAL);
	auto container_size = Size(_screen_size.width, _screen_size.height);
	container->setInnerContainerSize(container_size);
	container->setLayoutType(ui::Layout::Type::VERTICAL);
	getPreviousValues();
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

/* Load the previous game configuration from the database */
void SettingsScene::getPreviousValues(){
	std::string get_stmt = "select * from game_data";
	std::vector<std::vector<std::string>> results = Database::getQueryResults(get_stmt.c_str());
	chosen_num_enemies = std::stoi(results[1][0]);
	chosen_num_balls = std::stoi(results[1][1]);
	chosen_scale = std::stof(results[1][2]);
}

/* Setup the number of enemies slider and label text */
void SettingsScene::setNumEnemies(){
	const std::string format_string = "Number of enemies : %d";
	char num_enemies_label[100];
	sprintf(num_enemies_label, format_string.c_str(), chosen_num_enemies);
	auto num_enemies_hint = ui::Text::create("Number of enemies", "fonts/arial.ttf", SETTINGS_FONT_SIZE);

	//Initialize slider
	auto num_enemies_slider = ui::Slider::create();
	num_enemies_slider->loadBarTexture("slider_back.png");
	num_enemies_slider->loadSlidBallTextures("slidernode_normal.png", "slidernode_pressed.png", "slidernode_disable.png");
	num_enemies_slider->loadProgressBarTexture("slider_pressbar.png");

	// Calculate the initial position based on previously saved values
	int initial_idx = std::find(num_enemy_values.begin(), num_enemy_values.end(), chosen_num_enemies) - num_enemy_values.begin();
	//TODO add checks
	int initial_percent = ( (float) (initial_idx + 1) / num_enemy_values.size()) * 100;
	num_enemies_slider->setPercent(initial_percent);
	auto &local_num_enemies = chosen_num_enemies;

	//Setup listener to change for slide change event
	num_enemies_slider->addEventListener([num_enemies_hint, &local_num_enemies, format_string](Ref* sender, ui::Slider::EventType type){
			auto slider = dynamic_cast<ui::Slider*>(sender);
			if (type ==ui::Slider::EventType::ON_PERCENTAGE_CHANGED){
					// Get the chosen value based on the values vector and current percentage
					int chosen_value_idx = (num_enemy_values.size()) * slider->getPercent() / 100;
					chosen_value_idx = (slider->getPercent() == 100 ? chosen_value_idx - 1 : chosen_value_idx);
					local_num_enemies = num_enemy_values[chosen_value_idx];

					// Set the label based on chosen value
					char num_enemies_label[100];
					sprintf(num_enemies_label, format_string.c_str(),local_num_enemies);
					num_enemies_hint->setString(num_enemies_label);
			}
			});
	num_enemies_hint->setString(num_enemies_label);

	//Postion the label and slider
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

/* Setup the number of balls slider and label text */
void SettingsScene::setNumBalls(){
	const std::string format_string = "Number of balls : %d";
	char num_balls_label[100];
	sprintf(num_balls_label, format_string.c_str(), chosen_num_balls);
	auto num_balls_hint = ui::Text::create("Number of balls", "fonts/arial.ttf",SETTINGS_FONT_SIZE);

	// Initialize the slider
	auto num_balls_slider = ui::Slider::create();
	num_balls_slider->loadBarTexture("slider_back.png");
	num_balls_slider->loadSlidBallTextures("slidernode_normal.png", "slidernode_pressed.png", "slidernode_disable.png");
	num_balls_slider->loadProgressBarTexture("slider_pressbar.png");

	// Calculate the initial position based on previously saved values
	int initial_idx = std::find(num_ball_values.begin(), num_ball_values.end(), chosen_num_balls) - num_ball_values.begin();
	int initial_percent = ( (float) (initial_idx + 1) / num_ball_values.size()) * 100;
	num_balls_slider->setPercent(initial_percent);
	auto &local_num_balls = chosen_num_balls;

	//Setup listener to listen for slider change event
	num_balls_slider->addEventListener([num_balls_hint, &local_num_balls, format_string](Ref* sender, ui::Slider::EventType type){
			auto slider = dynamic_cast<ui::Slider*>(sender);
			if (type ==ui::Slider::EventType::ON_PERCENTAGE_CHANGED){
					// Get the chosen value based on the values vector and current percentage
					int chosen_value_idx = (num_ball_values.size()) * slider->getPercent() / 100;
					chosen_value_idx = (slider->getPercent() == 100 ? chosen_value_idx - 1 : chosen_value_idx);
					local_num_balls = num_ball_values[chosen_value_idx];

					//Set the label based on chosen value
					char num_balls_label[100];
					sprintf(num_balls_label, format_string.c_str(), local_num_balls);
					num_balls_hint->setString(num_balls_label);
			}
			});
	num_balls_hint->setString(num_balls_label);

	// Position the label and slider
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

/* Setup the ball scale slider and label */
void SettingsScene::setScale(){
	const std::string format_string = "Scaling for balls : %f";
	char scale_label[100];
	sprintf(scale_label, format_string.c_str(), chosen_scale);
	auto scale_hint = ui::Text::create("Scaling for balls", "fonts/arial.ttf", SETTINGS_FONT_SIZE);
	
	// Initialize the slider
	auto scale_slider = ui::Slider::create();
	scale_slider->loadBarTexture("slider_back.png");
	scale_slider->loadSlidBallTextures("slidernode_normal.png", "slidernode_pressed.png", "slidernode_disable.png");
	scale_slider->loadProgressBarTexture("slider_pressbar.png");

	// Calculate the initial position based on previously saved values
	int initial_idx = std::find(scale_values.begin(), scale_values.end(), chosen_scale) - scale_values.begin();
	int initial_percent = ( (float) (initial_idx + 1) / scale_values.size()) * 100;
	scale_slider->setPercent(initial_percent);
	auto &local_scale = chosen_scale;
	scale_slider->addEventListener([scale_hint, &local_scale, format_string](Ref* sender, ui::Slider::EventType type){
			auto slider = dynamic_cast<ui::Slider*>(sender);
			if (type ==ui::Slider::EventType::ON_PERCENTAGE_CHANGED){
				// Get the chosen value based on the values vector and current percentage
				int chosen_value_idx = (scale_values.size()) * slider->getPercent() / 100;
				chosen_value_idx = (slider->getPercent() == 100 ? chosen_value_idx - 1 : chosen_value_idx);
				local_scale = scale_values[chosen_value_idx];
				
				// Set the label based on the chosen value
				char scale_label[100];
				sprintf(scale_label, format_string.c_str(), local_scale);
				scale_hint->setString(scale_label);
			}
			});
	scale_hint->setString(scale_label);
	
	// Position the label and slider
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

// Saves the selected values to database
void SettingsScene::saveValues(Ref* sender, ui::Widget::TouchEventType type){

	char update_stmt[200];
	const std::string format_string = "update game_data set num_enemies=%d, num_balls=%d, scale=%f";
	if (type == ui::Widget::TouchEventType::ENDED){
		sprintf(update_stmt, format_string.c_str(), chosen_num_enemies, chosen_num_balls, chosen_scale);
	}
}
