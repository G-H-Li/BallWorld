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
	Director::getInstance()->setDepthTest(true);	//������ȼ��
	auto home = HelloWorld::createScene();
	auto ss = TransitionProgressInOut::create(1, home);	//����һ���л���������Ч
	Director::getInstance()->replaceScene(ss);
}