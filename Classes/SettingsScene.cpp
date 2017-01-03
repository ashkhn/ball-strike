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
	_screen_size = Director::getInstance()->getWinSizeInPixels();
	container = ui::ScrollView::create();
	container->setPosition(Point(15, 5));
	container->setContentSize(Size(_screen_size.width - 30.0f, _screen_size.height - 30.0f));
	container->setDirection(ui::ScrollView::Direction::VERTICAL);
	auto container_size = Size(_screen_size.width, _screen_size.height);
	container->setInnerContainerSize(container_size);
	container->setLayoutType(ui::Layout::Type::VERTICAL);
	getPreviousValues();
	addSlider("Number of enemies: %d", num_enemy_values, chosen_num_enemies);
	addSlider("Number of balls: %d", num_ball_values, chosen_num_balls);
	addSlider("Scaling of ball: %f", scale_values, chosen_scale);
	
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

/* Add a slider to the settings layout */
/* @param format_string : Format string for the label text of the slider */
/* @param vector<T> values : Vector containing the set of possible values the slider can take */
/* @param chosen_value : The selected value of the slider */ 
template<class T> void SettingsScene::addSlider(std::string format_string, std::vector<T> values, T &chosen_value){
	char slider_label[100];
	sprintf(slider_label, format_string.c_str(), chosen_value);
	auto slider_hint = ui::Text::create(format_string, "fonts/arial.ttf", SETTINGS_FONT_SIZE);

	// Initialize slider
	auto slider = ui::Slider::create();
	slider->loadBarTexture("slider_back.png");
	slider->loadSlidBallTextures("slidernode_normal.png", "slidernode_pressed.png", "slidernode_disable.png");
	slider->loadProgressBarTexture("slider_pressbar.png");

	// Calculate the intial position based on previously saved values
	int initial_idx = std::find(values.begin(), values.end(), chosen_value) - values.begin();
	//TODO add checks
	int initial_percent = ( (float) (initial_idx + 1) / values.size()) * 100;
	slider->setPercent(initial_percent);
	auto &local_value = chosen_value;

	//Setup listener to listen to change event
	slider->addEventListener([values, slider_hint, &local_value, format_string](Ref* sender, ui::Slider::EventType type){
			auto slider = dynamic_cast<ui::Slider*>(sender);
			if(type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED){
				// Get the chosen value based on values vector and current percentage
				int chosen_value_idx = (values.size()) * slider->getPercent() / 100;
				chosen_value_idx = (slider->getPercent() == 100 ? chosen_value_idx - 1 : chosen_value_idx);
				local_value = values[chosen_value_idx];

				// Set label based on chosen value
				char slider_label[100];
				sprintf(slider_label, format_string.c_str(), local_value);
				slider_hint->setString(slider_label);
			}
			});
	slider_hint->setString(slider_label);

	slider->setPosition(Vec2(_screen_size.width / 2, _screen_size.height / 2));
	slider->setScale(SLIDER_SCALE);
	auto layout_param = ui::LinearLayoutParameter::create();
	layout_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	layout_param->setMargin(ui::Margin(MARGIN, MARGIN, MARGIN, MARGIN));
	slider_hint->setLayoutParameter(layout_param);
	slider->setLayoutParameter(layout_param);
	container->addChild(slider_hint);
	container->addChild(slider);
}

// Saves the selected values to database
void SettingsScene::saveValues(Ref* sender, ui::Widget::TouchEventType type){

	char update_stmt[200];
	const std::string format_string = "update game_data set num_enemies=%d, num_balls=%d, scale=%f";
	if (type == ui::Widget::TouchEventType::ENDED){
		sprintf(update_stmt, format_string.c_str(), chosen_num_enemies, chosen_num_balls, chosen_scale);
		log("Status of save is %d", Database::execute(update_stmt));
	}
}
