#include "LoginScene.h"
#include "spine/Json.h"
#include "ui/CocosGUI.h"
#include "Constants.h"

LoginScene::LoginScene(void){}

LoginScene::~LoginScene(void){}

Scene* LoginScene::createScene(){
	auto scene = Scene::create();
	auto layer = LoginScene::create();

	scene->addChild(layer);

	return scene;
}

bool LoginScene::init(){
	if(!Layer::init()){
		return false;
	}

	UserDefault::getInstance()->setBoolForKey(Constants::is_user_logged_in, true);

	network::HttpRequest *request = new network::HttpRequest();
	request->setUrl("http://192.168.100.172:3000/api/users/1");
	request->setRequestType(network::HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onLoginRequestCompleted, this));
	network::HttpClient::getInstance()->send(request);
	request->release();
	return true;
}

void LoginScene::onLoginRequestCompleted(network::HttpClient *sender, network::HttpResponse *response){
	std::vector<char> *buffer = response->getResponseData();

	for (int i = 0; i < buffer->size(); i++){
		log("The response was %c", (*buffer)[i]);
	}

	if( 200 == response->getResponseCode() ){
		log("Success");
	}
	else{
		log("Failure");
	}
}
