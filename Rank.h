#pragma once
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
using namespace cocos2d::ui;

class Rank :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Rank);
	void homeButtonTouch(Ref* sender);

	//void sort();
	//void fileRead();
};

