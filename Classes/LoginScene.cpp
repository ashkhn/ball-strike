#include "LoginScene.h"

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
	network::HttpRequest *request = new network::HttpRequest();
	request->setUrl("http://www.sonarlearning.co.uk/extras/cocostutorial/get.php");
	request->setRequestType(network::HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onRequestCompleted, this));
	network::HttpClient::getInstance()->send(request);
	request->release();
	return true;
}

void LoginScene::onRequestCompleted(network::HttpClient *sender, network::HttpResponse *response){
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
