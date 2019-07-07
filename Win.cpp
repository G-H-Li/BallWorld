#include "Win.h"
#include "HelloWorldScene.h"
#include "level_1.h"
#include "Rank.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;

Scene* Win::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Win::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Win::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto csb = CSLoader::createNode("win.csb");
	this->addChild(csb);

	auto home = static_cast<Button*>(csb->getChildByName("homeButton"));
	home->addClickEventListener(CC_CALLBACK_1(Win::homeButtonTouch, this));
	auto rank = static_cast<Button*>(csb->getChildByName("rankButton"));
	rank->addClickEventListener(CC_CALLBACK_1(Win::rankButtonTouch, this));
	auto again = static_cast<Button*>(csb->getChildByName("againButton"));
	home->addClickEventListener(CC_CALLBACK_1(Win::againButtonTouch, this));
	return true;
}

void Win::homeButtonTouch(Ref* sender)
{
	Director::getInstance()->setDepthTest(true);	//设置深度检测
	auto home = HelloWorld::createScene();
	auto ss = TransitionProgressInOut::create(2, home);	//创建一个切换场景到特效
	Director::getInstance()->replaceScene(ss);
}

void Win::rankButtonTouch(Ref* sender)
{
	Director::getInstance()->setDepthTest(true);	//设置深度检测
	auto rank = Rank::createScene();
	auto ss = TransitionProgressInOut::create(2, rank);	//创建一个切换场景到特效
	Director::getInstance()->replaceScene(ss);
}

void Win::againButtonTouch(Ref* sender)
{
	Director::getInstance()->setDepthTest(true);	//设置深度检测
	auto again = level_1::createScene();
	auto ss = TransitionProgressInOut::create(2, again);	//创建一个切换场景到特效
	Director::getInstance()->replaceScene(ss);
}