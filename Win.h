#pragma once
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
using namespace cocos2d::ui;

class Win :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Win);
	void homeButtonTouch(Ref* sender);
	void rankButtonTouch(Ref* sender);
	void againButtonTouch(Ref* sender);
};


