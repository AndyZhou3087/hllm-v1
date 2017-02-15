#include "CommonLayer.h"
#include "GlobalData.h"

#include "SkillGrowLayer.h"
#include "HpBuyLayer.h"
#include "MyParticle.h"
#include "SoundManager.h"
#include "NewerGuideLayer.h"
#include "SettingLayer.h"
#include "FourTypeGift.h"
#include "LoginRewardLayer.h"
#include "HintBox.h"
#include "FourTypeGift.h"
#include "ByGift.h"
#include "CommonFuncs.h"
#include "MoreLayer.h"
#include "LotteryLayer.h"
#include "TopComNode.h"
#include "ShopLayer.h"
#include "MoreCMLayer.h"
#include "EventLayer.h"

using namespace std;

CommonLayer* g_commomLayer = NULL;

CommonLayer::CommonLayer()
{
	isnewerguiding = false;
}

CommonLayer::~CommonLayer()
{
	isnewerguiding = false;
	g_commomLayer = NULL;
}

bool CommonLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	_bgNode = CSLoader::createNode("CommonLayer.csb");
	addChild(_bgNode);

	auto action = CSLoader::createTimeline("CommonLayer.csb");
	_bgNode->runAction(action);
	action->gotoFrameAndPlay(0, 35, true);

	TopComNode* topnode = TopComNode::create();
	topnode->setPosition(Vec2(270, 910));
	_bgNode->addChild(topnode);

	m_growUpBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("growUpBtn");
	m_growUpBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::GrowUp, this));

	m_shopBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("shopBtn");
	m_shopBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::Shop, this));

	string lightstr = StringUtils::format("giftbthlight_%d", 1);
	light = (cocos2d::ui::Widget*)_bgNode->getChildByName(lightstr);
	light->runAction(RepeatForever::create(RotateTo::create(4, 720)));

	m_onsaleGiftBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("onsaleGiftBtn");
	m_onsaleGiftBtn->setTag(HOME_ONSALE);
	m_onsaleGiftBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::onGift, this));

	m_newerBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("newerbtn");
	m_newerBtn->setTag(NEWERGIFT);
	m_newerBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::onGift, this));
	m_newerBtn->setVisible(false);

	m_vipBtnNode = (cocos2d::ui::Widget*)_bgNode->getChildByName("vipbtnnode");
	m_vipBtn = (cocos2d::ui::Widget*)m_vipBtnNode->getChildByName("vipbtn");
	m_vipBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::vip, this));
	m_vipBtn->setVisible(false);

	bool showvip = ByGift::checkprice() == -1 ? false : true;
	if (showvip)
	{
		m_vipBtn->setVisible(true);
		schedule(schedule_selector(CommonLayer::vipAnim), 0.8f);
	}

	resetBtnPos();
	m_moreBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("morebtn");
	m_moreBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::more, this));

	m_morecmBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("morecmbtn");
	m_morecmBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::morecm, this));

	m_shareBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("sharebtn");
	m_shareBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::share, this));

#ifndef THIRDLOGIN
	m_shareBtn->setVisible(false);
#endif
	
	m_eventBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("enventbtn");
	m_eventBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::onevent, this));

	onsaleRedPoint = (cocos2d::ui::Widget*)m_onsaleGiftBtn->getChildByName("startredpoint");
	growRedPoint = (cocos2d::ui::Widget*)m_growUpBtn->getChildByName("startredpoint");
	moreRedPoint = (cocos2d::ui::Widget*)m_moreBtn->getChildByName("startredpoint");
	vipRedPoint = (cocos2d::ui::Widget*)m_vipBtn->getChildByName("startredpoint");
	eventRedPoint = (cocos2d::ui::Widget*)m_eventBtn->getChildByName("startredpoint");
	isShowGrowUpAnim = false;

	checkNewerGuidechapterpass();
	updateUI();

	if (GlobalData::getIsGetLoginReward())
	{
		if (GlobalData::checkGuide(21) && GlobalData::getHighestStage() <= 9 && _chapter1pass)
		{
			isnewerguiding = true;
		}
		else if (GlobalData::checkGuide(47) && GlobalData::getHighestStage() == 14)
		{
			isnewerguiding = true;
		}
		else if (GlobalData::checkGuide(48) && GlobalData::getHighestStage() == 24)
		{
			isnewerguiding = true;
		}
		else if (GlobalData::checkGuide(23) && GlobalData::getHighestStage() <= 19 && _chapter2pass)
		{
			isnewerguiding = true;
		}
		scheduleOnce(schedule_selector(CommonLayer::delayShowNewerGuide), 0.2f);
	}

	if (!isnewerguiding && !GlobalData::getIsGetLoginReward())
		scheduleOnce(schedule_selector(CommonLayer::delayShowLoginReward), 0.25f);


	if (!isnewerguiding && GlobalData::getIsGetLoginReward())
	{
		scheduleOnce(schedule_selector(CommonLayer::delayShowGift), 0.25f);
	}

	return true;
}

void CommonLayer::checkNewerGuidechapterpass()
{
	_chapter1pass = GlobalData::getHighestStage() >= 9 ? true : false;
	_chapter2pass = GlobalData::getHighestStage() >= 19 ? true : false;
}

void CommonLayer::delayShowNewerGuide(float t)
{
	checkNewerGuidechapterpass();
	std::vector<Node*> nodes;

	if (GlobalData::checkGuide(21))
	{
		if (GlobalData::getHighestStage() >= 9)
		{
			nodes.push_back(m_growUpBtn);
			g_NewerLayer = NewerGuideLayer::create(21);
			_bgNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			g_commomLayer->isnewerguiding = false;
			return;
		}
	}
	if (GlobalData::checkGuide(23))
	{
		if (GlobalData::getHighestStage() >= 19)
		{
			morecm(m_morecmBtn, ui::Widget::TouchEventType::ENDED);
			g_commomLayer->isnewerguiding = false;
			return;
		}
	}
	if (GlobalData::checkGuide(47))
	{
		if (GlobalData::getHighestStage() >= 14)
		{
			morecm(m_morecmBtn, ui::Widget::TouchEventType::ENDED);
			g_commomLayer->isnewerguiding = false;
			return;
		}
	}

	if (GlobalData::checkGuide(48))
	{
		if (GlobalData::getHighestStage() >= 24)
		{
			morecm(m_morecmBtn, ui::Widget::TouchEventType::ENDED);
			g_commomLayer->isnewerguiding = false;
			return;
		}
	}
	
}

void CommonLayer::vipAnim(float dt)
{
	MyParticle::Show(m_vipBtn, SpeciallyName[31], m_vipBtn->getContentSize().width/2 - 5, 60, true, 0.1f, 10);

	MyParticle::Show(m_vipBtn, SpeciallyName[32], m_vipBtn->getContentSize().width / 2 - 5, 60, true, 0.1f, 10);
	MyParticle::Show(m_vipBtn, SpeciallyName[33], m_vipBtn->getContentSize().width / 2 - 5, 60, true, 0.1f, 10);
	MyParticle::Show(m_vipBtn, SpeciallyName[34], m_vipBtn->getContentSize().width / 2 - 5, 60, true, 0.1f, 10);
}
void CommonLayer::updateRedPoint(float dt)
{
	bool isGrowup = checkifGrowUp() && _chapter1pass;
	growRedPoint->setVisible(isGrowup);

	if (isGrowup)
	{
		showCanGrowUpAnim();
	}

	resetBtnPos();
	moreRedPoint->setVisible(!GlobalData::getIsGetLoginReward() || MoreLayer::isHasRedPoint());

	eventRedPoint->setVisible(hasNewEvent());
}

void CommonLayer::showCanGrowUpAnim()
{
	MyParticle::Show(m_growUpBtn, SpeciallyName[31], 45, 60, true, 0.1f, 10);
	MyParticle::Show(m_growUpBtn, SpeciallyName[32], 45, 60, true, 0.1f, 10);
	MyParticle::Show(m_growUpBtn, SpeciallyName[33], 45, 60, true, 0.1f, 10);
}

void CommonLayer::updateUI()
{
	isShowGrowUpAnim = false;
	schedule(schedule_selector(CommonLayer::updateRedPoint), 0.3f);
}

void CommonLayer::setMoreCMbtnVisible(bool val)
{
	m_morecmBtn->setVisible(val);
}
void CommonLayer::setMorebtnVisible(bool val)
{
	m_moreBtn->setVisible(val);
}

void CommonLayer::Shop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ShopLayer *pShop = ShopLayer::create();
		addChild(pShop);
	}
}

void CommonLayer::GrowUp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (_chapter1pass)
		{
			if (GlobalData::checkGuide(21))
			{
				isnewerguiding = false;
				if (g_NewerLayer != NULL)
					g_NewerLayer->removeSelf();
			}
			SkillGrowLayer *layer = SkillGrowLayer::create();
			g_skillGrowlayer = layer;
			addChild(layer);
		}
		else
		{
			HintBox* hint = HintBox::create("通关第10关解锁", 0);
			addChild(hint, 1000);
		}
	}
}


void CommonLayer::vip(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ByGift::isClickPop = true;
		Layer* layer = ByGift::create();
		this->addChild(layer);
	}

}

void CommonLayer::more(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		setMorebtnVisible(false);
		MoreLayer* layer = MoreLayer::create();
		this->addChild(layer);
	}
}

void CommonLayer::morecm(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		setMoreCMbtnVisible(false);
		MoreCMLayer* layer = MoreCMLayer::create();
		this->addChild(layer);
	}
}

void CommonLayer::share(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo methodInfo;
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Share", "showShare", "()V"))
		{
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		}
#endif
	}
}

void CommonLayer::onevent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		GlobalData::saveEventsIds();
		EventLayer* layer = EventLayer::create();
		g_EventLayer = layer;
		this->addChild(layer);
	}
}

void CommonLayer::Action(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN || type == ui::Widget::TouchEventType::MOVED)
	{
		((cocos2d::ui::Widget*)pSender)->runAction(Button_ACTION);
	}
	else if (type == ui::Widget::TouchEventType::CANCELED)
	{
		((cocos2d::ui::Widget*)pSender)->runAction(Button_ACTION1);
	}
	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		((cocos2d::ui::Widget*)pSender)->runAction(Button_ACTION1);
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);
	}
}

bool CommonLayer::hasNewEvent()
{
	std::string locids = GlobalData::getEventsIds();
	std::string ids;
	for (unsigned int i = 0; i < GlobalData::EventsData.size(); i++)
	{
		std::string tmp = StringUtils::format("%d", GlobalData::EventsData[i].id);
		ids.append(tmp);
		ids.append("-");
	}
	if (ids.length() <= 0 || locids.compare(ids) == 0)
		return false;
	return true;

}

bool CommonLayer::checkifGrowUp()
{
	if (GlobalData::getGoldCount() >= 100 + 10 * (GlobalData::getMyTotalSkillLevel() + 1) && GlobalData::getMyTotalSkillLevel() < 30)
		return true;
	return false;
}

void CommonLayer::onGift(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btn = (Node*)pSender;
		FourTypeGift::_type = btn->getTag();
		FourTypeGift::isClickPop = true;
		Layer* giftLayer = FourTypeGift::create();
		Director::getInstance()->getRunningScene()->addChild(giftLayer, 0, "giftlayer");
	}
}

void CommonLayer::delayShowLoginReward(float dt)
{
	LoginRewardLayer::AnimType = A_UPTODOWN;
	LoginRewardLayer* layer = LoginRewardLayer::create();
	Director::getInstance()->getRunningScene()->addChild(layer, 100);
}

void CommonLayer::delayShowGift(float dt)
{
	if (GlobalData::getIsHomeAutoPopGift())
	{
		showGift(0);
	}
	GlobalData::setIsHomeAutoPopGift(false);
}

void CommonLayer::showGift(int pos)
{
#ifdef AUTOPOPGIFT
	const char* str = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/OGThranPay", "checkPCode", "(I)Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, pos);
		str = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
#endif
	int index = -1;
	int pcodecount = sizeof(PayCode) / sizeof(PayCode[0]);
	for (int i = 0; i < pcodecount; i++)
	{
		if (PayCode[i].compare(str) == 0)
		{
			index = i;
			break;
		}
	}
	int gindex = index - sizeof(gemNum) / sizeof(gemNum[0]);
	if (gindex >= 0)
	{
		if (gindex < GJGIFT)
		{
			FourTypeGift::AnimType = A_UPTODOWN;
			FourTypeGift::_type = gindex;
			Layer* giftLayer = FourTypeGift::create();
			Director::getInstance()->getRunningScene()->addChild(giftLayer, 1000, "giftlayer");

		}
		else if (gindex >= GJGIFT && gindex < ENDGIFT)
		{
			Layer* giftLayer = ByGift::create();
			Director::getInstance()->getRunningScene()->addChild(giftLayer, 1000, "giftlayer");
		}
	}
#endif
}

void CommonLayer::resetBtnPos()
{
	bool ret = false;
	bool ret1 = true;
	int gemcount = sizeof(gemNum) / sizeof(gemNum[0]);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	char p_str[64] = { 0 };
	sprintf(p_str, "%s", PayCode[gemcount + HOME_ONSALE].c_str());
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/OGThranPay", "checkPCodeExist", "(Ljava/lang/String;)Z"))
	{
		jstring str1 = methodInfo.env->NewStringUTF(p_str);
		ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, str1);
	}

	sprintf(p_str, "%s", PayCode[gemcount + NEWERGIFT].c_str());
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/OGThranPay", "checkPCodeExist", "(Ljava/lang/String;)Z"))
	{
		jstring str1 = methodInfo.env->NewStringUTF(p_str);
		ret1 = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, str1);
	}

#endif
	m_onsaleGiftBtn->setVisible(ret);
	light->setVisible(ret);

	if (ret1)
	{
		m_newerBtn->setVisible(true);
		if (!ret)
		{
			m_newerBtn->setPositionY(m_onsaleGiftBtn->getPositionY());
			light->setPositionY(m_onsaleGiftBtn->getPositionY());
		}
	}
}