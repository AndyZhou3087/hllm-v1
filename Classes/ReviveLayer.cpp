#include "ReviveLayer.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "Const.h"
#include "ThemeScene.h"
#include "HintBox.h"
#include "MyParticle.h"
#include "FourTypeGift.h"
#include "ShopLayer.h"
#include "SureStageLayer.h"
#include "BossSettlement.h"
#include "CommonLayer.h"
#include "HpBuyLayer.h"
#include "MyTransitionScene.h"
int ReviveLayer::AnimType = A_SMALLTONORMAL;
int reviveNeedGem = 20;
ReviveLayer::ReviveLayer()
{
	m_gemCunt = NULL;
	m_freeText = NULL;
	m_useText = NULL;
	m_reviveicon = NULL;
	m_totalText = NULL;
}
ReviveLayer::~ReviveLayer()
{

}

void ReviveLayer::onExit()
{
	Layer::onExit();
}

bool ReviveLayer::init()
{
	if (!Layer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();


	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	_failNode = CSLoader::createNode("FailLayer.csb");
	addChild(_failNode);

	string stageStr = StringUtils::format("%d", GlobalData::getPlayingStage() + 1);
	cocos2d::ui::TextBMFont* stagebmlbl = (cocos2d::ui::TextBMFont*)_failNode->getChildByName("failstagebmlbl");
	stagebmlbl->setString(stageStr);

	if (GlobalData::getChallengeMode() == CMBOSS)
	{
		_failNode->getChildByName("winbosstext")->setVisible(true);
		_failNode->getChildByName("faildi")->setVisible(false);

		string bossstageStr = StringUtils::format("%d", GlobalData::getDefeatedBossCount() + 1);
		stagebmlbl->setPositionX(stagebmlbl->getPositionX() - 20);
		stagebmlbl->setString(bossstageStr);
	}
	m_restart = (cocos2d::ui::Widget*)_failNode->getChildByName("restartbtn");
	m_restart->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::restart, this));
	m_restart->setPositionX(270);

	string usehpstr = StringUtils::format("x%d", GlobalData::getCurStageUseHp());
	cocos2d::ui::TextBMFont* hpbmLbl = (cocos2d::ui::TextBMFont*)m_restart->getChildByName("usehpbmlbl");
	hpbmLbl->setString(usehpstr);

	m_revive = (cocos2d::ui::Widget*)_failNode->getChildByName("failReviveBtn");
	m_revive->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::Revive, this));

	cocos2d::ui::Widget* onsalebtn = (cocos2d::ui::Widget*)_failNode->getChildByName("onsalebtn");
	onsalebtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onSale, this));
	onsalebtn->setVisible(false);

	m_close = (cocos2d::ui::Widget*)_failNode->getChildByName("failCloseBtn");
	m_close->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::FailExit, this));

	cocos2d::ui::Widget* failgiftlight = (cocos2d::ui::Widget*)_failNode->getChildByName("failgiftbg")->getChildByName("light");
	failgiftlight->runAction(RepeatForever::create(RotateTo::create(4, 720)));

	cocos2d::ui::Widget* buybtn = (cocos2d::ui::Widget*)_failNode->getChildByName("failgiftbg")->getChildByName("failgiftbtn");
	buybtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::buyPropGift, this));

	int fontsize = 18;
	int opacity = 100;
#ifdef ZFTEXTCLEAR
	fontsize = 20;
	opacity = 200;
#endif
	string paystr = CommonFuncs::gbk2utf(StringUtils::format("信息费%d元。客服电话：4009908188。", 10).c_str());
	Label* paydsc = Label::createWithSystemFont(paystr, FONT_NAME, fontsize);
	paydsc->setPosition(Vec2(230, 25));
	paydsc->setColor(Color3B(0, 0, 0));
	paydsc->setOpacity(opacity);
	_failNode->getChildByName("failgiftbg")->addChild(paydsc);

	auto herofail = CSLoader::createNode("herofail.csb");
	herofail->setPosition(Vec2(270, 580));
	_failNode->addChild(herofail);
	auto herofailAnim = CSLoader::createTimeline("herofail.csb");
	herofail->runAction(herofailAnim);
	herofailAnim->gotoFrameAndPlay(0, 14, true);

	int tipsindex[] = { 1, 2, 3, 5, 7, 9,15,16,17,18,19 };
	int rand_seed = GlobalData::getSystemSecTime();
	srand(rand_seed);
	int tipszise = sizeof(tipsindex) / sizeof(tipsindex[0]);
	int r = rand() % tipszise;
	cocos2d::ui::TextBMFont* tipsbmlbl = (cocos2d::ui::TextBMFont*)_failNode->getChildByName("tipsbmlbl");
	tipsbmlbl->setString(CommonFuncs::gbk2utf(tips[tipsindex[r]].c_str()));

	SoundManager::sharedManager()->stopBackMusic();
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_FAIL);

	if (g_gameScene != NULL)
	{
		int count = g_gameScene->getCleanCount();

		Achieve* data = GlobalData::getAchieveDataByID(1);
		if (data->finish != -1)
		{
			data->finish += count;
			GlobalData::SaveAchieveData();
		}

		data = GlobalData::getAchieveDataByID(2);
		if (data->finish != -1)
		{
			data->finish += count;
			GlobalData::SaveAchieveData();
		}

		data = GlobalData::getAchieveDataByID(3);
		if (data->finish != -1)
		{
			data->finish += count;
			GlobalData::SaveAchieveData();
		}

		data = GlobalData::getAchieveDataByID(4);
		if (data->finish != -1)
		{
			data->finish += g_gameScene->getCleanAnimalCount(15);
			GlobalData::SaveAchieveData();
		}

		data = GlobalData::getAchieveDataByID(5);
		if (data->finish != -1)
		{
			data->finish += g_gameScene->getCleanAnimalCount(16);
			GlobalData::SaveAchieveData();
		}
	}

	MyParticle::Show(_failNode, SpeciallyName[6], 270, 1000, false, 100, 2);

	/*
	int freeReviveCount = GlobalData::getMyFreeReviveCount();

	m_reviveText = (cocos2d::ui::Widget*)_failNode->getChildByName("failReviveBtn")->getChildByName("revivetext");
	m_freeText = (cocos2d::ui::Widget*)_failNode->getChildByName("failReviveBtn")->getChildByName("freetext");
	m_useText = (cocos2d::ui::Widget*)_failNode->getChildByName("failReviveBtn")->getChildByName("userevivebmlbl");
	m_reviveicon = (cocos2d::ui::Widget*)_failNode->getChildByName("failReviveBtn")->getChildByName("reviveicon");
	m_totalText = (cocos2d::ui::TextBMFont*)_failNode->getChildByName("failReviveBtn")->getChildByName("totalrevivebmlbl");
	m_gem = (cocos2d::ui::Widget*)_failNode->getChildByName("failReviveBtn")->getChildByName("smalldiamond");
	m_gemCunt = (cocos2d::ui::TextBMFont*)_failNode->getChildByName("failReviveBtn")->getChildByName("revivegembmlbl");

	std::string needGemStr = StringUtils::format("x%d", reviveNeedGem);
	m_gemCunt->setString(needGemStr);
	if (freeReviveCount > 0)
	{
		m_freeText->setVisible(true);
		m_reviveText->setPositionY(62);
		m_reviveicon->setVisible(false);
	}
	else if (GlobalData::getReviveProp() > 0)
	{
		m_useText->setVisible(true);
		m_reviveicon->setVisible(true);
		m_totalText->setVisible(true);
		string str = StringUtils::format("(%d)", GlobalData::getReviveProp());
		m_totalText->setString(str);
	}
	else
	{
		m_gem->setVisible(true);
		m_gemCunt->setVisible(true);
		m_reviveText->setPositionY(62);
		m_reviveicon->setVisible(false);
	}
	*/
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	g_gameScene->clearData();

	if (AnimType == A_SMALLTONORMAL)
	{
		_failNode->setAnchorPoint(Vec2(0.5f, 0.5f));
		_failNode->setPosition(Vec2(270, 480));
		_failNode->setScale(0.1f);
		_failNode->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	}
	else
	{
		_failNode->setAnchorPoint(Vec2(0.0f, 0.0f));
		_failNode->setPosition(0, 960);
		_failNode->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
		AnimType = A_SMALLTONORMAL;
	}

	int continuefailCount = GlobalData::getCurStageContinueFailCount();

	GlobalData::setCurStageContinueFailCount(continuefailCount + 1);
	//this->schedule(schedule_selector(ReviveLayer::updateGemLabel), 0.2f);

	return true;
}

void ReviveLayer::onEnterTransitionDidFinish()
{
	this->scheduleOnce(schedule_selector(ReviveLayer::delayShowReviveGift), 0.8f);
#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };

	if (GlobalData::getChallengeMode() == CMBOSS)
		sprintf(p_str, "bossstage%d", GlobalData::getDefeatedBossCount() + 1);
	else if (GlobalData::getChallengeMode() == CMNORMAL)
		sprintf(p_str, "stage%d", GlobalData::getPlayingStage() + 1);

	if (GlobalData::getChallengeMode() == CMBOSS || GlobalData::getChallengeMode() == CMNORMAL)
	{
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "failLevel", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
	}
#endif
	
}

void ReviveLayer::delayShowReviveGift(float dt)
{
	if (checkPcode(REVIVEGIFT))
	{
		FourTypeGift::AnimType = A_UPTODOWN;
		FourTypeGift::_type = REVIVEGIFT;
		FourTypeGift* giftLayer = FourTypeGift::create();
		Director::getInstance()->getRunningScene()->addChild(giftLayer, 0, "giftlayer");
	}
}

void ReviveLayer::onSale(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		FourTypeGift::_type = ONSALEGIFT;
		FourTypeGift::isClickPop = true;
		Layer* giftLayer = FourTypeGift::create();
		Director::getInstance()->getRunningScene()->addChild(giftLayer, 0, "giftlayer");
	}
}

void ReviveLayer::updateGemLabel(float dt)
{
	if (GlobalData::getMyFreeReviveCount() > 0)
	{
		m_reviveText->setPositionY(62);
	}
	else if (GlobalData::getReviveProp() > 0)
	{

		m_gem->setVisible(false);
		m_gemCunt->setVisible(false);
		m_reviveText->setPositionY(62);
		m_useText->setVisible(true);
		m_reviveicon->setVisible(true);
		m_totalText->setVisible(true);
	}
	else
	{
		m_gem->setVisible(true);
		m_gemCunt->setVisible(true);
		m_reviveText->setPositionY(62);
		m_reviveicon->setVisible(false);
	}

}

void ReviveLayer::buyPropGift(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ShopLayer::beginPayment(PROPGIFT + sizeof(gemNum) / sizeof(gemNum[0]));
	}
}

void ReviveLayer::FailExit(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getChallengeMode() == CMBOSS)
		{
			this->removeFromParentAndCleanup(true);
			delayShowBossSettlement();
		}
		else
		{
			if (g_gameScene != NULL)
			{
				Director::getInstance()->replaceScene(MyTransitionScene::scene());
			}
			else
				this->removeFromParentAndCleanup(true);
		}
	}
}

void ReviveLayer::restart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if (GlobalData::getHpCount() >= GlobalData::getCurStageUseHp())
		{
			this->removeFromParentAndCleanup(true);
			GlobalData::setHpCount(GlobalData::getHpCount() - GlobalData::getCurStageUseHp());
			if (GlobalData::getChallengeMode() == CMNORMAL)
				g_gameScene->restart();
			else if (GlobalData::getChallengeMode() == CMBOSS)
				g_gameScene->resetBossStage();

			if (GlobalData::getCurStageUseHp() > 0)
			{
				Achieve *data = GlobalData::getTaskDataByID(7);
				if (data->finish != -1)
				{
					data->finish += GlobalData::getCurStageUseHp();
					GlobalData::SaveTaskData();
				}
			}
#ifdef ANALYTICS
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", "hp");
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, GlobalData::getCurStageUseHp());
			}
#endif
		}
		else
		{
			HpBuyLayer::AnimType = A_UPTODOWN;
			HpBuyLayer* layer = HpBuyLayer::create();
			this->addChild(layer, 100);
		}
	}
}

void ReviveLayer::delayShowBossSettlement()
{
	if (g_gameScene != NULL)
		g_gameScene->scheduleOnce(schedule_selector(ReviveLayer::enterBossSettlementLayer), 0.8f);

}

void ReviveLayer::enterBossSettlementLayer(float dt)
{
	BossSettlement* pLayer = BossSettlement::create();
	Director::getInstance()->getRunningScene()->addChild(pLayer, 1000);
}

void ReviveLayer::delayPlayWinSound(float dt)
{
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_WIN);
}

void ReviveLayer::Revive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		bool isCanRevive = false;

		if (GlobalData::getMyFreeReviveCount() > 0)
		{
			GlobalData::setMyFreeReviveCount(GlobalData::getMyFreeReviveCount() - 1);
			isCanRevive = true;

#ifdef ANALYTICS
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", "revive");
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1);
			}
#endif
		}
		else if (GlobalData::getReviveProp() > 0)
		{
			GlobalData::setReviveProp((GlobalData::getReviveProp() - 1));
			isCanRevive = true;
		}
		else
		{
			if (GlobalData::getGemCount() >= reviveNeedGem)
			{
				GlobalData::setGemCount(GlobalData::getGemCount() - reviveNeedGem);
				isCanRevive = true;
#ifdef ANALYTICS
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };
				sprintf(p_str, "%s", "gem");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 20);
				}
#endif
			}
			else
			{
				//HintBox* hint = HintBox::create("钻石不足", 1);
				//addChild(hint, 1000);
				FourTypeGift::AnimType = A_UPTODOWN;
				FourTypeGift::_type = REVIVEGIFT;
				FourTypeGift* giftLayer = FourTypeGift::create();
				Director::getInstance()->getRunningScene()->addChild(giftLayer, 0, "giftlayer");
			}
		}
		if (isCanRevive)
		{
			if (g_gameScene != NULL)
			{
				g_gameScene->gameRevive();
			}
			this->removeFromParentAndCleanup(true);
		}
	}
}

bool ReviveLayer::checkPcode(int pcodeindex)
{
	bool ret = true;
	int gemcount = sizeof(gemNum) / sizeof(gemNum[0]);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	char p_str[64] = { 0 };
	sprintf(p_str, "%s", PayCode[gemcount + pcodeindex].c_str());
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/OGThranPay", "checkPCodeExist", "(Ljava/lang/String;)Z"))
	{
		jstring str1 = methodInfo.env->NewStringUTF(p_str);
		ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, str1);
	}
#endif
	return ret;
}