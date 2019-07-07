#pragma once
#include "cocos2d.h"
#include "ui/UILoadingBar.h"
#include "ui/UIText.h"

class level_1 :public cocos2d::Layer
{
public:
	level_1() {}
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(level_1);
	//按钮的回调函数
	void stopButtonTouch(Ref* sender);
	//键盘事件回调函数
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	//鼠标事件回调函数
	virtual void onMouseDown(cocos2d::Event* event);
	//改变人物位置以及图片方向
	void changeCharacterPosition(cocos2d::Sprite* sprite);
	void changeDirection(cocos2d::Sprite* sprite);
	void moveRight();//控制人物右移
	void moveLeft();//控制人物左移
	//控制子弹的发射
	void bulletOut(cocos2d::Sprite* sprite);
	//处理碰撞函数
	bool onContactSeparate(cocos2d::PhysicsContact& contact);
	//游戏刷新函数
	virtual void update(float dt);//各种帧刷新处理
	void timerAndScore(float dt); //处理计时
	void changePosition(float dt); //改变幽灵位置(debug
	void attack(float dt); //幽灵的全屏攻击

	void fileWrite(int score, int time); //将时间和分数写入

	static int time;  //游戏时间（单位：秒
	static int score;  //所得分数
private:
	//精灵声明
	cocos2d::Node* csb;
	cocos2d::Sprite* actor;
	cocos2d::Sprite* monster;
	cocos2d::Sprite* ball;
	cocos2d::Sprite* levelBg1;
	cocos2d::Sprite* levelBg2;
	cocos2d::Sprite* gunSprite;
	cocos2d::Sprite* ghost;

	//精灵帧声明
	cocos2d::SpriteFrame* bulletFrame;
	cocos2d::SpriteFrame* ghostFrame;

	//ui控件获取
	cocos2d::ui::LoadingBar* actorLife;
	cocos2d::ui::LoadingBar* ghostLife;
	cocos2d::ui::Text* timeNum;
	cocos2d::ui::Text* scoreNum;

	//人物属性
	bool accLeft = false;  //加速度方向开关
	bool accRight = false;
	int lifeNum = 100;  //生命值
	int leftSpeed = 0;  //速度
	int rightSpeed = 0;
	const int initSpeed = 5;  //初始速度
	const int acceleration = 3;  //初始加速度
	bool isKeyPressedA, isKeyPressedD; //键盘按下
	//游戏画面属性
	int brickNum = 30;  //砖块数量
	int monsterNum = 1; //小怪数量
	int gunNum = 1;  //枪的序号
	int ghostLifeNum = 100; //幽灵生命值
};

