#include "gameOver.h"
#include "level_1.h"
#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "ui/UIText.h"
#include <string>

using namespace std;
USING_NS_CC;

Scene* gameOver::createScene()
{
	auto scene = Scene::create();
	auto layer = gameOver::create();
	scene->addChild(layer);
	return scene;
}

bool gameOver::init()
{
	if (!Layer::init()) return false;

	auto csb = CSLoader::createNode("gameOver.csb");
	addChild(csb);

	auto score = static_cast<Text*>(csb->getChildByName("scoreNum"));
	auto time = static_cast<Text*>(csb->getChildByName("timeNum"));
	auto home = static_cast<Button*>(csb->getChildByName("home"));
	auto again = static_cast<Button*>(csb->getChildByName("again"));

	home->addClickEventListener(CC_CALLBACK_1(gameOver::homeButtonTouch, this));
	again->addClickEventListener(CC_CALLBACK_1(gameOver::againButtonTouch, this));

	scoreNum = level_1::score;
	timeNum = level_1::time;

	showNum(score, scoreNum);
	showNum(time, timeNum);
}

void gameOver::homeButtonTouch(Ref* sender)
{
	Director::getInstance()->setDepthTest(true);	//设置深度检测
	auto mainScene = HelloWorld::createScene();
	auto ss = TransitionCrossFade::create(1, mainScene);	//创建一个切换场景到特效
	Director::getInstance()->replaceScene(ss);
}

void gameOver::againButtonTouch(Ref* sender)
{
	Director::getInstance()->setDepthTest(true);	//设置深度检测
	auto level1 = level_1::createScene();
	auto ss = TransitionMoveInT::create(1, level1);	//创建一个切换场景到特效
	Director::getInstance()->replaceScene(ss);
}

void gameOver::showNum(cocos2d::ui::Text* text,const int num)
{
	int count = 0;
	while (count != num)
	{
		count++;
		text->setString(to_string(count));
	}
}