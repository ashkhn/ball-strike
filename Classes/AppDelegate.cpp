#include "AppDelegate.h"
#include "HomeScene.h"
#include "Database.h"
#include "LoginScene.h"
#include "Constants.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate(){}

bool AppDelegate::applicationDidFinishLaunching() {
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::create("Test Game");
		auto screen_size = glview->getFrameSize();
		auto design_size = Size(768,1024);
		glview->setDesignResolutionSize(design_size.width,design_size.height, ResolutionPolicy::EXACT_FIT);
		director->setOpenGLView(glview);
	}
	
	director->setDisplayStats(true);
	director->setAnimationInterval(1.0 / 60);
	bool is_user_logged_in = UserDefault::getInstance()->getBoolForKey(Constants::IS_USER_LOGGED_IN);
	is_user_logged_in = false;
	if(is_user_logged_in){
		auto scene = HomeScreen::createScene();
		director->runWithScene(scene);
	}
	else{
		auto scene = LoginScene::createScene();
		director->runWithScene(scene);
	}

	// Create database tables
	Database::open();
	if (Database::createDataTables() && Database::createSaveTables() && Database::createLevelTables()){
		log("Tables created successfully");
	}
	return true;
}

/* Called when the application enters the background.
 * Use this to stop all running animations and close the database connection
 */
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
	Database::close();
}

/* Called when the application is about to enter foreground */
/* Use this to resume animations and open the database connection again 
*/
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
	Database::open();
}

void AppDelegate::initGLContextAttrs(){
	GLContextAttrs gl_context_attrs = {8, 8, 8, 8, 24, 8};
	GLView::setGLContextAttrs(gl_context_attrs);
}
