#include "AppDelegate.h"
#include "HomeScene.h"

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

	auto scene = HomeScreen::createScene();
	director->runWithScene(scene);
	
	return true;
}

void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
}

void AppDelegate::initGLContextAttrs(){
	GLContextAttrs gl_context_attrs = {8, 8, 8, 8, 24, 8};
	GLView::setGLContextAttrs(gl_context_attrs);
}
