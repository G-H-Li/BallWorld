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
	//��ť�Ļص�����
	void stopButtonTouch(Ref* sender);
	//�����¼��ص�����
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	//����¼��ص�����
	virtual void onMouseDown(cocos2d::Event* event);
	//�ı�����λ���Լ�ͼƬ����
	void changeCharacterPosition(cocos2d::Sprite* sprite);
	void changeDirection(cocos2d::Sprite* sprite);
	void moveRight();//������������
	void moveLeft();//������������
	//�����ӵ��ķ���
	void bulletOut(cocos2d::Sprite* sprite);
	//������ײ����
	bool onContactSeparate(cocos2d::PhysicsContact& contact);
	//��Ϸˢ�º���
	virtual void update(float dt);//����֡ˢ�´���
	void timerAndScore(float dt); //�����ʱ
	void changePosition(float dt); //�ı�����λ��(debug
	void attack(float dt); //�����ȫ������

	void fileWrite(int score, int time); //��ʱ��ͷ���д��

	static int time;  //��Ϸʱ�䣨��λ����
	static int score;  //���÷���
private:
	//��������
	cocos2d::Node* csb;
	cocos2d::Sprite* actor;
	cocos2d::Sprite* monster;
	cocos2d::Sprite* ball;
	cocos2d::Sprite* levelBg1;
	cocos2d::Sprite* levelBg2;
	cocos2d::Sprite* gunSprite;
	cocos2d::Sprite* ghost;

	//����֡����
	cocos2d::SpriteFrame* bulletFrame;
	cocos2d::SpriteFrame* ghostFrame;

	//ui�ؼ���ȡ
	cocos2d::ui::LoadingBar* actorLife;
	cocos2d::ui::LoadingBar* ghostLife;
	cocos2d::ui::Text* timeNum;
	cocos2d::ui::Text* scoreNum;

	//��������
	bool accLeft = false;  //���ٶȷ��򿪹�
	bool accRight = false;
	int lifeNum = 100;  //����ֵ
	int leftSpeed = 0;  //�ٶ�
	int rightSpeed = 0;
	const int initSpeed = 5;  //��ʼ�ٶ�
	const int acceleration = 3;  //��ʼ���ٶ�
	bool isKeyPressedA, isKeyPressedD; //���̰���
	//��Ϸ��������
	int brickNum = 30;  //ש������
	int monsterNum = 1; //С������
	int gunNum = 1;  //ǹ�����
	int ghostLifeNum = 100; //��������ֵ
};

