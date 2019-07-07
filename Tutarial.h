#pragma once
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
using namespace cocos2d::ui;

class Tutarial :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Tutarial);
	void homeButtonTouch(Ref* sender);
};

