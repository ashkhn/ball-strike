#include "NetworkUtils.h"


/* Generates a network request object */
/* @param url: Url for the network request */
/* @param is_authenticated : Defines whether the request needs to include authentication headers */
/* @return request initialized with the above parameters */
network::HttpRequest* NetworkUtils::createNetworkRequest(std::string url, bool is_authenticated){
	network::HttpRequest* req = new network::HttpRequest();
	req->setUrl(url);
	std::vector<std::string> headers;
	headers.push_back("Content-Type:application/json; charset=utf-8");
	if(is_authenticated){
		std::string auth_token = UserDefault::getInstance()->getStringForKey(Constants::KEY_AUTH_TOKEN, "");
		headers.push_back("Authorization:" + auth_token);
	}
	req->setHeaders(headers);

	return req;

}
