#include "MoreLayer.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "SettingLayer.h"
#include "LoginRewardLayer.h"
#include "CommonLayer.h"
#include "RankLayer.h"
#include "AchieveLayer.h"
#include "TaskLayer.h"
#include "MyParticle.h"
#include "MessageLayer.h"
using namespace std;

MoreLayer::MoreLayer()
{
}

MoreLayer::~MoreLayer()
{
}

bool MoreLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 220));
	this->addChild(color);

	_bgNode = CSLoader::createNode("moreLayer.csb");
	addChild(_bgNode);

	auto action = CSLoader::createTimeline("moreLayer.csb");
	_bgNode->runAction(action);
	action->gotoFrameAndPlay(0, 35, true);

	cocos2d::ui::Widget* m_moreBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("morebtn");
	m_moreBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::more, this));

	m_loginrewardBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("loginrewardbtn");
	m_loginrewardBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::loginReward, this));

	cocos2d::ui::Widget* m_settingBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("settingBtn");
	m_settingBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::Setting, this));

	m_achieveBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("achievebtn");
	m_achieveBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::Achieve, this));

	m_taskBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("taskbtn");
	m_taskBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::task, this));

	m_taskAnimBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("taskanimbtn");
	m_taskAnimBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::task, this));


	cocos2d::ui::Widget* m_rankBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("rankbtn");
	m_rankBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::rank, this));

	cocos2d::ui::Widget* m_messageBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("messagebtn");
	m_messageBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::message, this));

	loginRewardRedPoint = (cocos2d::ui::Widget*)m_loginrewardBtn->getChildByName("startredpoint");
	loginRewardRedPoint->setVisible(!GlobalData::getIsGetLoginReward());

	taskRewardRedPoint = (cocos2d::ui::Widget*)m_taskBtn->getChildByName("startredpoint");
	achieRedPoint = (cocos2d::ui::Widget*)m_achieveBtn->getChildByName("startredpoint");
	messageRedPoint = (cocos2d::ui::Widget*)m_messageBtn->getChildByName("startredpoint");

	this->schedule(schedule_selector(MoreLayer::checkRedPoint), 0.2f);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		more(m_moreBtn, ui::Widget::TouchEventType::ENDED);
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool MoreLayer::isHasRedPoint()
{
	bool ishasAchie = checkHasAchie() > 0 ? true : false;
	bool ishasTask = checkHasFinishTask() > 0 ? true : false;
	bool ishasmessage = checkHasMessage() > 0 ? true : false;
	return ishasAchie || ishasTask || ishasmessage;
}

void MoreLayer::checkRedPoint(float dt)
{
	loginRewardRedPoint->setVisible(!GlobalData::getIsGetLoginReward());

	bool ishasAchie = checkHasAchie() > 0 ? true : false;
	achieRedPoint->setVisible(ishasAchie);
	if (ishasAchie)
	{
		showCanAchieveAim();
	}

	bool ishasTask = checkHasFinishTask() > 0 ? true : false;

	m_taskBtn->setVisible(!ishasTask);
	m_taskAnimBtn->setVisible(ishasTask);

	bool ishasmessage = checkHasMessage() > 0 ? true : false;
	messageRedPoint->setVisible(ishasmessage);
}

int MoreLayer::checkHasAchie()
{
	int count = 0;
	for (unsigned int i = 0; i < GlobalData::achieveData.size(); i++)
	{
		if (GlobalData::achieveData[i].achType != AchType::ACH_WORLD_RANK && GlobalData::achieveData[i].achType != AchType::ACH_FRIEND_RANK)
		{
			if (GlobalData::achieveData[i].finish >= GlobalData::achieveData[i].count)
				count++;
		}
		else
		{
			if ((GlobalData::achieveData[i].finish <= GlobalData::achieveData[i].count && GlobalData::achieveData[i].finish > 0) || GlobalData::achieveData[i].finish == INT_MAX)
				count++;
		}
	}
	return count;
}

int MoreLayer::checkHasFinishTask()
{
	int count = 0;
	for (unsigned int i = 0; i < GlobalData::taskData.size(); i++)
	{
		if (GlobalData::taskData[i].finish >= GlobalData::taskData[i].count)
			count++;
	}
	return count;
}

void MoreLayer::showCanAchieveAim()
{
	MyParticle::Show(m_achieveBtn, SpeciallyName[31], 45, 60, true, 0.1f, 10);
	MyParticle::Show(m_achieveBtn, SpeciallyName[32], 45, 60, true, 0.1f, 10);
	MyParticle::Show(m_achieveBtn, SpeciallyName[33], 45, 60, true, 0.1f, 10);
}

int MoreLayer::checkHasMessage()
{
	int count = 0;
	for (unsigned int i = 0; i < GlobalData::mapMessageData.size(); i++)
	{
		for (unsigned int j = 0; j < GlobalData::mapMessageData[i].size(); j++)
		{
			if (GlobalData::mapMessageData[i][j].status <= 0)
				count++;
		}
	}
	return count;
}

void MoreLayer::Setting(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Layer* layer = SettingLayer::create();
		this->addChild(layer);
	}
}

void MoreLayer::loginReward(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		LoginRewardLayer::AnimType = A_SMALLTONORMAL;
		LoginRewardLayer* layer = LoginRewardLayer::create();
		this->addChild(layer, 100);
	}
}

void MoreLayer::more(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (g_commomLayer != NULL)
			g_commomLayer->setMorebtnVisible(true);
		this->removeFromParentAndCleanup(true);
	}
}

void MoreLayer::Achieve(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		g_AchieveLayer = AchieveLayer::create();
		this->addChild(g_AchieveLayer);
	}
}

void MoreLayer::task(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		g_TaskLayer = TaskLayer::create();
		this->addChild(g_TaskLayer);
	}
}


void MoreLayer::rank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Layer* layer = RankLayer::create();
		this->addChild(layer);
	}
}

void MoreLayer::message(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		MessageLayer* layer = MessageLayer::create();
		g_messageLayer = layer;
		this->addChild(layer);
	}
}


