#include "Setting.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "HelloWorldScene.h"

using namespace cocos2d::ui;
USING_NS_CC;

bool Setting::developerMode = 0;
int Setting::percent = 30;

Scene* Setting::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Setting::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Setting::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto csb = CSLoader::createNode("setting.csb");
	this->addChild(csb);

	soundSlider = static_cast<Slider*>(csb->getChildByName("soundSlider"));
	soundSlider->addEventListener([&](Ref* sender, Slider::EventType type) {
		switch (type)
		{
		case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
		{
			percent = soundSlider->getPercent();
		}
			break;
		}
	});

	checkBox = static_cast<CheckBox*>(csb->getChildByName("checkBox"));
	checkBox->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			developerMode = 1;
			break;
		case ui::Widget::TouchEventType::ENDED:
			developerMode = 1;
			break;
		default:
			break;
		}
	});

	auto home = static_cast<Button*>(csb->getChildByName("homeButton"));
	home->addClickEventListener(CC_CALLBACK_1(Setting::homeButtonTouch, this));

	return true;
}

void Setting::homeButtonTouch(Ref* sender)
{
	Director::getInstance()->setDepthTest(true);	//设置深度检测
	auto home = HelloWorld::createScene();
	auto ss = TransitionProgressInOut::create(1, home);	//创建一个切换场景到特效
	Director::getInstance()->replaceScene(ss);
}