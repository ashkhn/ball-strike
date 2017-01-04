#pragma once
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "Constants.h"
USING_NS_CC;


class NetworkUtils {
	public:
		static network::HttpRequest* createNetworkRequest(std::string url, bool is_authenticated);
};
