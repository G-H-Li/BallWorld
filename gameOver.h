#pragma once
#include "cocostudio/CocoStudio.h"
#include "ui/UIText.h"
#include "cocos2d.h"
using namespace cocos2d::ui;

class gameOver : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(gameOver);

	void homeButtonTouch(Ref* sender);
	void againButtonTouch(Ref* sender);
	void showNum(cocos2d::ui::Text* text,const int num);

private:
	int scoreNum;
	int timeNum;
};

