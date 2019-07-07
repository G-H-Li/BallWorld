#include "Tutarial.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "HelloWorldScene.h"

using namespace cocos2d::ui;
USING_NS_CC;

Scene* Tutarial::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Tutarial::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Tutarial::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto csb = CSLoader::createNode("tutarial.csb");
	this->addChild(csb);

	auto home = static_cast<Button*>(csb->getChildByName("homeButton"));
	home->addClickEventListener(CC_CALLBACK_1(Tutarial::homeButtonTouch, this));
	return true;
}

void Tutarial::homeButtonTouch(Ref* sender)
{
	Director::getInstance()->setDepthTest(true);	//设置深度检测
	auto home = HelloWorld::createScene();
	auto ss = TransitionProgressInOut::create(1, home);	//创建一个切换场景到特效
	Director::getInstance()->replaceScene(ss);
}