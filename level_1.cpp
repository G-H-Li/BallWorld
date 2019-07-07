#include "level_1.h"
#include "Parameter.h"
#include "Setting.h"
#include "gameOver.h"
#include "HelloWorldScene.h"
#include "Win.h"
#include <string>
#include <fstream>
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace std;
USING_NS_CC;

int level_1::score = 0;
int level_1::time = 0;

Scene* level_1::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, 0.0f));
	if (Setting::developerMode)
	{
		scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	}
	auto layer = level_1::create();
	scene->addChild(layer);
	return scene;
}

bool level_1::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//部分数据初始化
	score = 0;
	time = 0;
	isKeyPressedA = false;
	isKeyPressedD = false;
	gunNum = 1;

	//导入csb文件
	csb = CSLoader::createNode("level_1.csb");
	this->addChild(csb);

	//ui控件获取
	actorLife = static_cast<LoadingBar*>(csb->getChildByName("lifeCondiction"));
	ghostLife = static_cast<LoadingBar*>(csb->getChildByName("ghostLife"));
	timeNum = static_cast<Text*>(csb->getChildByName("timeNum"));
	scoreNum = static_cast<Text*>(csb->getChildByName("scoreNum"));
	Button* stop = static_cast<Button*>(csb->getChildByName("Button_1"));  //stop按钮的事件声明
	stop->addClickEventListener(CC_CALLBACK_1(level_1::stopButtonTouch, this));

	//各种精灵的获取
	actor = static_cast<Sprite*>(csb->getChildByName("gamingCharacter"));
	actor->setAnchorPoint(Vec2(0.5f, 0));
	ball = static_cast<Sprite*>(csb->getChildByName("ball"));
	monster = static_cast<Sprite*>(csb->getChildByName("monster"));
	levelBg1 = static_cast<Sprite*>(csb->getChildByName("levelBg1"));
	levelBg2 = static_cast<Sprite*>(csb->getChildByName("levelBg2"));
	auto bulletCache = SpriteFrameCache::getInstance();
	bulletCache->addSpriteFramesWithFile("pic/bullet.plist", "bullet.png");
	bulletFrame = bulletCache->getSpriteFrameByName("pic/bullet1.png");
	gunSprite = static_cast<Sprite*>(csb->getChildByName("gunSprite"));
	auto ghostCache = SpriteFrameCache::getInstance();
	ghostCache->addSpriteFramesWithFile("pic/ghost.plist", "ghost.png");
	ghostFrame = ghostCache->getSpriteFrameByName("pic/show.png");

	ghost = static_cast<Sprite*>(csb->getChildByName("ghost"));
	ghost->setVisible(false);

	//大怪的物理刚体
	auto ghostBody = PhysicsBody::createBox(ghost->getContentSize(), PhysicsMaterial(10.0f, 0.8f, 0.0f));
	ghostBody->setGravityEnable(false);
	ghostBody->setRotationEnable(false);
	ghostBody->setDynamic(false);
	ghostBody->setCategoryBitmask(GHOST_NUM);
	ghostBody->setCollisionBitmask(GHOST_COLLISION_NUM);
	ghostBody->setContactTestBitmask(GHOST_CONTACT_NUM);
	ghost->addComponent(ghostBody);
	ghostBody->setEnabled(false);

	//砖块的物理刚体
	for (int i = 1; i < brickNum + 1; i++)
	{
		string a = "brick_" + to_string(i);
		auto brick = static_cast<Sprite*>(csb->getChildByName(a));
		auto brickBody = PhysicsBody::createEdgeBox(brick->getContentSize(), PhysicsMaterial(1.0f, 1.1f, 0.0f));
		brickBody->setGravityEnable(false);
		brickBody->setRotationEnable(false);
		brickBody->setCategoryBitmask(BRICK_NUM);
		brickBody->setCollisionBitmask(BRICK_COLLISION_NUM);
		brickBody->setContactTestBitmask(BRICK_CONTACT_NUM);
		brick->setPhysicsBody(brickBody);
		if (i > 8)
		{
			brick->setVisible(false);
			brickBody->setEnabled(false);
		}
	}
	//构造ball的物理刚体
	auto ballBody = PhysicsBody::createCircle(300, PhysicsMaterial(0.0f,1.1f,0.0f));
	ballBody->setGravityEnable(false);
	Vect force = Vect(cocos2d::random(300,500), cocos2d::random(300,500));
	ballBody->applyImpulse(force);
	ballBody->setVelocity(Vec2(cocos2d::random(-200,300), cocos2d::random(-100,300)));
	ballBody->setVelocityLimit(400);
	ballBody->setCategoryBitmask(BALL_NUM);
	ballBody->setCollisionBitmask(BALL_COLLISION_NUM);
	ballBody->setContactTestBitmask(BALL_CONTACT_NUM);
	ball->addComponent(ballBody);

	//背景图片的物理刚体
	auto levelBg1Body = PhysicsBody::createEdgeBox(levelBg1->getContentSize(), PhysicsMaterial(0.0f, 1.1f, 0.2f));
	levelBg1Body->setCategoryBitmask(BG_NUM);
	levelBg1Body->setCollisionBitmask(BG_COLLISION_NUM);
	levelBg1Body->setContactTestBitmask(BG_CONTACT_NUM);
	levelBg1->addComponent(levelBg1Body);
	auto levelBg2Body = PhysicsBody::createEdgeBox(levelBg2->getContentSize(), PhysicsMaterial(0.0f, 1.1f, 0.2f));
	levelBg2Body->setCategoryBitmask(BG_NUM);
	levelBg2Body->setCollisionBitmask(BG_COLLISION_NUM);
	levelBg2Body->setContactTestBitmask(BG_CONTACT_NUM);
	levelBg2->addComponent(levelBg2Body);

	//游戏角色物理刚体
	auto actorBody = PhysicsBody::createBox(actor->getContentSize(), PhysicsMaterial(10.0f, 1.1f, 0.0f));
	actorBody->setCategoryBitmask(ACTOR_NUM);
	actorBody->setCollisionBitmask(ACTOR_COLLISION_NUM);
	actorBody->setContactTestBitmask(ACTOR_CONTACT_NUM);
	actorBody->setGravityEnable(false);
	actorBody->setDynamic(false);
	actorBody->setRotationEnable(false);
	actor->addComponent(actorBody);

	//怪物的物理刚体
	auto monsterBody = PhysicsBody::createBox(monster->getContentSize(), PhysicsMaterial(0.0f, 1.1f, 0.0f));
	monsterBody->setCategoryBitmask(MONSTER_NUM);
	monsterBody->setCollisionBitmask(MONSTER_COLLISION_NUM);
	monsterBody->setContactTestBitmask(MONSTER_CONTACT_NUM);
	monsterBody->setRotationEnable(false);
	monsterBody->setGravityEnable(true);
	monsterBody->applyImpulse(Vec2(100, -100));
	monsterBody->setVelocityLimit(250);
	monsterBody->setVelocity(Vec2(cocos2d::random(100, 200), cocos2d::random(100, 200)));
	monster->addComponent(monsterBody);
	
	//键盘事件建立
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(level_1::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(level_1::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, actor);
	
	//鼠标事件建立
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(level_1::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	
	//注册物理碰撞
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [](PhysicsContact& contact) { return true; };
	contactListener->onContactSeparate = CC_CALLBACK_1(level_1::onContactSeparate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//游戏刷新函数
	this->scheduleUpdate();
	this->schedule(schedule_selector(level_1::timerAndScore), 1.0f);

	return true;
}

void level_1::moveLeft()
{
	rightSpeed = 0;
	leftSpeed = initSpeed;
	if (&EventKeyboard::isStopped == false && accLeft == false)
	{
		accLeft = true;
		leftSpeed += acceleration ;
	}
	changeCharacterPosition(actor);
	changeDirection(actor);
}

void level_1::moveRight()
{
	leftSpeed = 0;
	rightSpeed = initSpeed;
	if (&EventKeyboard::isStopped == false && accRight == false)
	{
		accRight = true;
		leftSpeed += acceleration;
	}
	changeCharacterPosition(actor);
	changeDirection(actor);
}

void level_1::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		isKeyPressedA = true;
		moveLeft();
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_D)
	{
		isKeyPressedD = true;
		moveRight();
	}
}

void level_1::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		rightSpeed = 0;
		leftSpeed = 0;
		accLeft = false;
		isKeyPressedA = false;
		log("a released");
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_D)
	{
		leftSpeed = 0;
		rightSpeed = 0;
		isKeyPressedD = false;
		accRight = false;
		log("d released");
	}
}

void level_1::changeCharacterPosition(cocos2d::Sprite* sprite)
{
	int x = sprite->getPositionX();
	x -= (leftSpeed * 2);
	x += (rightSpeed * 2);
	//log("x is %d", x);
	//log("y is %d", sprite->getPositionY());
	if (x > 922) x = 922;
	if (x < 37) x = 37;
	sprite->setPositionX(x);
}

void level_1::changeDirection(cocos2d::Sprite* sprite)
{
	float a = sprite->getScaleX();
	if (leftSpeed == 0 && rightSpeed != 0 && a < 0)
	{
		sprite->setScaleX(-a);
	}
	else if (rightSpeed == 0 && leftSpeed != 0 && a > 0)
	{
		sprite->setScaleX(-a);
	}
}

void level_1::update(float dt)
{
	if (isKeyPressedA)
	{
		moveLeft();
	}
	else if(isKeyPressedD)
	{
		moveRight();
	}
	if (ball->getPositionY() < 30)
	{
		fileWrite(score, time);
		Director::getInstance()->setDepthTest(true);	//设置深度检测
		auto over = gameOver::createScene();
		auto ss = TransitionMoveInT::create(1, over);	//创建一个切换场景到特效
		Director::getInstance()->replaceScene(ss);
	}
	if (score > 2000)
	{
		actor->setScale(0.25f);
	}
	if (brickNum == 22 && monsterNum == 0)
	{
		levelBg1->setPositionY(-320);
		levelBg2->setPositionY(320);
		for (int i = 9; i < 31; i++)
		{
			string a = "brick_" + to_string(i);
			auto brick = static_cast<Sprite*>(csb->getChildByName(a));
			brick->setVisible(true);
			brick->getPhysicsBody()->setEnabled(true);
		}
		ghost->setVisible(true);
		ghost->getPhysicsBody()->setEnabled(true);
		ghostLife->setVisible(true);
		this->schedule(schedule_selector(level_1::changePosition), 5.0f);
		this->schedule(schedule_selector(level_1::attack), 10.0f);
	}
}

void level_1::onMouseDown(cocos2d::Event* event)
{
	auto bullet = Sprite::createWithSpriteFrame(bulletFrame);
	bullet->setTag(40);
	bullet->setVisible(false);
	this->addChild(bullet);
	auto bulletBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(0.0f, 1.1f, 0.0f));
	bulletBody->setCategoryBitmask(BULLET_NUM);
	bulletBody->setCollisionBitmask(BULLET_COLLISION_NUM);
	bulletBody->setContactTestBitmask(BULLET_CONTACT_NUM);
	bulletBody->setRotationEnable(false);
	bulletBody->setGravityEnable(false);
	bullet->addComponent(bulletBody);
	EventMouse* mouse = (EventMouse*)event;
	auto type = mouse->getMouseButton();
	if (type == MOUSE_BUTTON_LEFT)
	{
		bulletOut(bullet);
	}
	else if (type == MOUSE_BUTTON_RIGHT)
	{
		switch (gunNum)
		{
		case 1:
			gunNum = 2;
			bulletFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pic/bullet2.png");
			bullet->setSpriteFrame(bulletFrame);
			gunSprite->setSpriteFrame(bulletFrame);
			break;
		case 2:
			gunNum = 3;
			bulletFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pic/bullet3.png");
			bullet->setSpriteFrame(bulletFrame);
			gunSprite->setSpriteFrame(bulletFrame);
			break;
		case 3:
			gunNum = 1;
			bulletFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pic/bullet1.png");
			bullet->setSpriteFrame(bulletFrame);
			gunSprite->setSpriteFrame(bulletFrame);
			break;
		}
	}
}

void level_1::bulletOut(cocos2d::Sprite* sprite)
{
	sprite->setVisible(true);
	sprite->getPhysicsBody()->setVelocity(Vec2(0, 0));
	sprite->setPosition(Vec2(actor->getPositionX(), actor->getPositionY() + 50));
	sprite->getPhysicsBody()->applyImpulse(Vec2(0.0f, 1000.0f));
	sprite->getPhysicsBody()->setVelocity(Vec2(0.0f, 500.0f));
	sprite->setRotation(-90.0f);
}

void level_1::stopButtonTouch(Ref* sender)
{
	Director::getInstance()->setDepthTest(true);	//设置深度检测
	auto mainScene = HelloWorld::createScene();
	auto ss = TransitionZoomFlipAngular::create(1, mainScene);	//创建一个切换场景到特效
	Director::getInstance()->replaceScene(ss);
}

bool level_1::onContactSeparate(cocos2d::PhysicsContact& contact)
{
	auto bodyA = (Sprite*)(contact.getShapeA()->getBody()->getNode());
	auto bodyB = (Sprite*)(contact.getShapeB()->getBody()->getNode());
	if (!bodyA || !bodyB) return false;
	int tagA = bodyA->getTag();
	int tagB = bodyB->getTag();
	if (tagA == 40)
	{
		if (gunNum == 1)
		{
			auto bullet1Effect = ParticleFire::create();
			bullet1Effect->setStartSize(0.5f);
			bullet1Effect->setEndSize(0.01f);
			bullet1Effect->setDuration(2.0f);
			bullet1Effect->setPosition(bodyA->getPosition());
			this->addChild(bullet1Effect);
		}
		if (gunNum == 3)
		{
			auto bullet3Effect = ParticleGalaxy::create();
			bullet3Effect->setStartSize(0.5f);
			bullet3Effect->setEndSize(0.01f);
			bullet3Effect->setDuration(2.0f);
			bullet3Effect->setPosition(bodyA->getPosition());
			this->addChild(bullet3Effect);
		}
		bodyA->removeFromParent();
	}
	if (tagB == 40)
	{
		if (gunNum == 1)
		{
			auto bullet1Effect = ParticleFire::create();
			bullet1Effect->setStartSize(0.5f);
			bullet1Effect->setEndSize(0.01f);
			bullet1Effect->setDuration(2.0f);
			bullet1Effect->setPosition(bodyB->getPosition());
			this->addChild(bullet1Effect);
		}
		if (gunNum == 3)
		{
			auto bullet3Effect = ParticleGalaxy::create();
			bullet3Effect->setStartSize(0.5f);
			bullet3Effect->setEndSize(0.01f);
			bullet3Effect->setDuration(2.0f);
			bullet3Effect->setPosition(bodyB->getPosition());
			this->addChild(bullet3Effect);
		}
		bodyB->removeFromParent();
	}
	if (tagA == 5 && tagB != 40 && tagB != 25)
	{
		brickNum -= 1;
		auto brickBoomEffect = ParticleExplosion::create();
		brickBoomEffect->setEndSize(0.01f);
		brickBoomEffect->setStartSize(0.01f);
		brickBoomEffect->setSpeed(1500);
		this->addChild(brickBoomEffect);
		brickBoomEffect->setPosition(bodyA->getPosition());
		int addition = (COMPLETE_LEVEL1_TIME - time > 0) ? (COMPLETE_LEVEL1_TIME - time) : 0;
		score += (100 + addition);
		bodyA->removeFromParentAndCleanup(true);
	}
	if (tagB == 5 && tagA != 40 && tagA != 25)
	{
		brickNum -= 1;
		auto brickBoomEffect = ParticleExplosion::create();
		brickBoomEffect->setEndSize(0.01f);
		brickBoomEffect->setStartSize(0.01f);
		brickBoomEffect->setSpeed(1500);
		this->addChild(brickBoomEffect);
		brickBoomEffect->setPosition(bodyB->getPosition());
		brickBoomEffect->setVisible(true);
		int addition = (COMPLETE_LEVEL1_TIME - time > 0) ? (COMPLETE_LEVEL1_TIME - time) : 0;
		score += (100 + addition);
		bodyB->removeFromParentAndCleanup(true);
	}
	if (tagA == 23 && tagB != 3)
	{
		monsterNum -= 1;
		int addition = (COMPLETE_LEVEL1_TIME - time > 0) ? (COMPLETE_LEVEL1_TIME - time) : 0;
		score += (500 + addition);
		bodyA->removeFromParentAndCleanup(true);
	}
	if (tagB == 23 && tagA != 3)
	{
		monsterNum -= 1;
		int addition = (COMPLETE_LEVEL1_TIME - time > 0) ? (COMPLETE_LEVEL1_TIME - time) : 0;
		score += (500 + addition);
		bodyB->removeFromParentAndCleanup(true);
	}
	if (tagA == 3 &&(tagB == 23 || tagB == 25))
	{
		lifeNum -= 25;
		actorLife->setPercent(lifeNum);
		if (lifeNum == 0)
		{
			fileWrite(score, time);
			Director::getInstance()->setDepthTest(true);	//设置深度检测
			auto over = gameOver::createScene();
			auto ss = TransitionMoveInT::create(1, over);	//创建一个切换场景到特效
			Director::getInstance()->replaceScene(ss);
		}
	}
	if (tagB == 3 && (tagA == 23 || tagA == 25))
	{
		lifeNum -= 25;
		actorLife->setPercent(lifeNum);
		if (lifeNum == 0)
		{
			fileWrite(score, time);
			Director::getInstance()->setDepthTest(true);	//设置深度检测
			auto over = gameOver::createScene();
			auto ss = TransitionMoveInT::create(1, over);	//创建一个切换场景到特效
			Director::getInstance()->replaceScene(ss);
		}
	}
	if (tagA == 25) //清除敌方子弹
	{
		bodyA->removeFromParentAndCleanup(true);
	}
	if (tagB == 25) //清除敌方子弹
	{
		bodyB->removeFromParentAndCleanup(true);
	}
	if (tagA == 4 && tagB == 51)
	{
		ghostLifeNum -= 5;
		ghostLife->setPercent(ghostLifeNum);
		if (ghostLifeNum == 0 && brickNum == 0)
		{
			fileWrite(score, time);
			Director::getInstance()->setDepthTest(true);	//设置深度检测
			auto win = Win::createScene();
			auto ss = TransitionMoveInT::create(1, win);	//创建一个切换场景到特效
			Director::getInstance()->replaceScene(ss);
		}
	}
	if (tagB == 4 && tagA == 51)
	{
		ghostLifeNum -= 5;
		ghostLife->setPercent(ghostLifeNum);
		if (ghostLifeNum == 0 && brickNum == 0)
		{
			fileWrite(score, time);
			Director::getInstance()->setDepthTest(true);	//设置深度检测
			auto win = Win::createScene();
			auto ss = TransitionMoveInT::create(1, win);	//创建一个切换场景到特效
			Director::getInstance()->replaceScene(ss);
		}
	}

	return true;
}

void level_1::timerAndScore(float dt)
{
	time += 1;
	timeNum->setString(to_string(time));
	scoreNum->setString(to_string(score));
}

void level_1::changePosition(float dt)
{
	Action* action = MoveTo::create(2, Vec2(cocos2d::random(200, 700), cocos2d::random(300, 500)));
	ghost->runAction(action);
	ghostFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pic/run.png");
	ghost->setSpriteFrame(ghostFrame);
}

void level_1::fileWrite(int score, int time)
{
	fstream rank;
	rank.open("rank.txt", ios::out || ios::app);
	if (rank.is_open())
	{
		rank << time <<' '<< score<<endl;
		log("read yes");
		rank.close();
	}
	else log("false");
}

void level_1::attack(float dt)
{
	ghostFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pic/attack.png");
	ghost->setSpriteFrame(ghostFrame);
	for (int i = 0; i < 41; i++)
	{
		int x = 25 + (20 * i);
		auto army = Sprite::create("pic/army.png");
		army->setTag(25);
		army->setPosition(Vec2(x, 610));
		auto armyBody = PhysicsBody::createBox(army->getContentSize(), PhysicsMaterial(0.0f, 0.5f, 0.0f));
		armyBody->setRotationEnable(false);
		armyBody->setGravityEnable(false);
		armyBody->setCategoryBitmask(ARMY_NUM);
		armyBody->setCollisionBitmask(ARMY_COLLISION_NUM);
		armyBody->setContactTestBitmask(ARMY_CONTACT_NUM);
		armyBody->setVelocity(Vec2(0.0f, -200.0f));
		armyBody->applyImpulse(Vec2(0.0f, -50.0f));
		army->addComponent(armyBody);
		this->addChild(army);
	}
}