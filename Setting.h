#pragma once
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "ui/UICheckBox.h"
#include "ui/UISlider.h"
using namespace cocos2d::ui;

class Setting :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Setting);

	static bool developerMode;
	static int percent;

protected:
	void homeButtonTouch(Ref* sender);

private:
	cocos2d::ui::Slider* soundSlider;
	cocos2d::ui::CheckBox* checkBox;
};

