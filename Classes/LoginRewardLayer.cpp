#include "LoginRewardLayer.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "StartScene.h"
#include "HintBox.h"
#include "CommonLayer.h"
#include "FreeGift.h"
#include "ThemeScene.h"
Node* g_LoginRewardLayer = NULL;

int loginpropnum[] = { 200, 10, 20, 400, 3, 3, 50 };
int LoginRewardLayer::AnimType = A_SMALLTONORMAL;
LoginRewardLayer::LoginRewardLayer()
{
}


LoginRewardLayer::~LoginRewardLayer()
{
}

bool LoginRewardLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	g_LoginRewardLayer = CSLoader::createNode("loginRewardLayer.csb");
	addChild(g_LoginRewardLayer);

	bool isGet = GlobalData::getIsGetLoginReward();
	cocos2d::ui::Widget* colseBtn = (cocos2d::ui::Widget*)g_LoginRewardLayer->getChildByName("close");
	
	colseBtn->addTouchEventListener(CC_CALLBACK_2(LoginRewardLayer::close, this));

	if (AnimType == A_UPTODOWN)
		colseBtn->setVisible(false);
	else
		colseBtn->setVisible(true);

	cocos2d::ui::Button* getBtn = (cocos2d::ui::Button*)g_LoginRewardLayer->getChildByName("getbtn");
	getBtn->addTouchEventListener(CC_CALLBACK_2(LoginRewardLayer::get, this));

	if (isGet)
	{
		cocos2d::ui::Widget* isgetText = (cocos2d::ui::Widget*)getBtn->getChildByName("loginisgettext");
		isgetText->setVisible(true);
		cocos2d::ui::Widget* getText = (cocos2d::ui::Widget*)getBtn->getChildByName("settinggettext");
		getText->setVisible(false);
		getBtn->setEnabled(false);
		getBtn->setBright(false);
	}

	else
	{
		cocos2d::ui::Widget* isgetText = (cocos2d::ui::Widget*)getBtn->getChildByName("loginisgettext");
		isgetText->setVisible(false);
		cocos2d::ui::Widget* getText = (cocos2d::ui::Widget*)getBtn->getChildByName("settinggettext");
		getText->setVisible(true);
		getBtn->setEnabled(true);
	}

	for (int i = 0; i < 7; i++)
	{
		string strnum = StringUtils::format("item%d", i);
		Node* item = (cocos2d::ui::Widget*)g_LoginRewardLayer->getChildByName(strnum);
		cocos2d::ui::TextBMFont* numlbl = (cocos2d::ui::TextBMFont*)item->getChildByName("propnumlbl");
		strnum = StringUtils::format("x%d", loginpropnum[i]);
		numlbl->setString(strnum);
	}
	logindays = GlobalData::getContinueLoginDay();

	int count = logindays;

	int lastLoginRewardSecond = GlobalData::getLastLoginRewardSecond();

	int syssec = GlobalData::getSystemSecTime();

	time_t t = time(0);
	tm* tm = localtime(&t);

	int nowzero = syssec - (tm->tm_hour * 60 * 60 + tm->tm_min * 60 + tm->tm_sec);
	
	if (nowzero - lastLoginRewardSecond > 0)
	{
		if (nowzero - lastLoginRewardSecond < 24 * 60 * 60)
		{
			logindays++;
			count = logindays;
			if (count > 7)
				count = count - 7;

			/*
			if (logindays <= 2)
				GlobalData::setLotteryCount(1);
			else if (logindays >= 3 && logindays < 7)
				GlobalData::setLotteryCount(2);
			else
				GlobalData::setLotteryCount(3);
			*/
		}
		else
		{
			logindays = 1;
			count = logindays;
			GlobalData::setContinueLoginDay(1);
		}
	}
	else
	{
		if (count > 7)
			count = count - 7;
	}

	string str;
	for (int i = 0; i < count; i++)
	{
		str = StringUtils::format("item%d", i);
		Node* item = (cocos2d::ui::Widget*)g_LoginRewardLayer->getChildByName(str);
		bgitem0 = (cocos2d::ui::Widget*)item->getChildByName("loginrewarditem0");
		bgitem1 = (cocos2d::ui::Widget*)item->getChildByName("loginrewarditem1");
		bgitem2 = (cocos2d::ui::Widget*)item->getChildByName("loginrewarditem2");
		bgitem3 = (cocos2d::ui::Widget*)item->getChildByName("loginrewarditem3");
		bgitem4 = (cocos2d::ui::Widget*)item->getChildByName("loginrewarditem4");
		getIcon = (cocos2d::ui::Widget*)item->getChildByName("loginrewardget");
		if (i < count - 1)
		{
			bgitem0->setVisible(true);
			bgitem1->setVisible(false);
			bgitem2->setVisible(false);
			bgitem3->setVisible(true);
			bgitem4->setVisible(false);
			getIcon->setVisible(true);
		}
		else
		{
			if (!isGet)
			{
				//bgitem0->setVisible(false);
				bgitem1->setVisible(true);
				bgitem2->setVisible(false);
				bgitem3->setVisible(false);
				bgitem4->setVisible(true);
				getIcon->setVisible(false);
			}
			else
			{
				bgitem0->setVisible(true);
				bgitem1->setVisible(false);
				bgitem2->setVisible(false);
				bgitem3->setVisible(true);
				bgitem4->setVisible(false);
				getIcon->setVisible(true);

			}
		}
	}

	if (AnimType == A_SMALLTONORMAL)
	{
		g_LoginRewardLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
		g_LoginRewardLayer->setPosition(Vec2(270, 480));
		g_LoginRewardLayer->setScale(0.1f);
		g_LoginRewardLayer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	}
	else
	{
		g_LoginRewardLayer->setAnchorPoint(Vec2(0.0f, 0.0f));
		g_LoginRewardLayer->setPosition(0, 960);
		g_LoginRewardLayer->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
		AnimType = A_SMALLTONORMAL;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void LoginRewardLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		closeSelf(0);
	}
}

void LoginRewardLayer::get(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btn = (cocos2d::ui::Button*)pSender;
		btn->setEnabled(false);
		btn->setBright(false);

		time_t t = time(0);
		tm* tm = localtime(&t);
		
		int syssec = GlobalData::getSystemSecTime();
		int nowzero = syssec - (tm->tm_hour * 60 * 60 + tm->tm_min * 60 + tm->tm_sec);

		GlobalData::setLastLoginRewardSecond(syssec);
		GlobalData::setLastLoginRewardDaySecond(nowzero);

		GlobalData::setContinueLoginDay(logindays);

		GlobalData::setIsGetLoginReward(true);

		int index = logindays - 1;
		if (logindays > 7)
			index = logindays - 7 - 1;

		string str = StringUtils::format("item%d", index);
		Node* item = (cocos2d::ui::Widget*)g_LoginRewardLayer->getChildByName(str);
		bgitem0 = (cocos2d::ui::Widget*)item->getChildByName("loginrewarditem0");
		bgitem1 = (cocos2d::ui::Widget*)item->getChildByName("loginrewarditem1");
		bgitem2 = (cocos2d::ui::Widget*)item->getChildByName("loginrewarditem2");
		bgitem3 = (cocos2d::ui::Widget*)item->getChildByName("loginrewarditem3");
		getIcon = (cocos2d::ui::Widget*)item->getChildByName("loginrewardget");
		bgitem0->setVisible(true);
		bgitem1->setVisible(false);
		bgitem2->setVisible(false);
		bgitem3->setVisible(true);
		bgitem4->setVisible(false);
		getIcon->setVisible(true);
		
		cocos2d::ui::Widget* isgetText = (cocos2d::ui::Widget*)btn->getChildByName("loginisgettext");
		isgetText->setVisible(true);
		cocos2d::ui::Widget* getText = (cocos2d::ui::Widget*)btn->getChildByName("settinggettext");
		getText->setVisible(false);

		int pcount = loginpropnum[index];
		if (index == 0)
		{
			GlobalData::setGoldCount(GlobalData::getGoldCount() + pcount);
			getOKAnim("UI/gold.png", Vec2(item->getPositionX(), item->getPositionY()), Vec2(388, 925));
		}
		else if (index == 1)
		{
			GlobalData::setHpCount(GlobalData::getHpCount() + pcount);
		}
		else if (index == 2)
		{
			GlobalData::setGemCount(GlobalData::getGemCount() + pcount);
			getOKAnim("UI/diamond.png", Vec2(item->getPositionX(), item->getPositionY()), Vec2(204, 925));
		}
		else if (index == 3)
		{
			GlobalData::setGoldCount(GlobalData::getGoldCount() + pcount);
			getOKAnim("UI/gold.png", Vec2(item->getPositionX(), item->getPositionY()), Vec2(388, 925));
		}
		else if (index == 4)
		{
			GlobalData::setRefreshProp(GlobalData::getRefreshProp() + pcount);
		}
		else if (index == 5)
		{
			GlobalData::setPromptProp(GlobalData::getPromptProp() + pcount);
		}
		else if (index == 6)
		{
			GlobalData::setGemCount(GlobalData::getGemCount() + pcount);
			getOKAnim("UI/diamond.png", Vec2(item->getPositionX(), item->getPositionY()), Vec2(204, 925));
		}
		HintBox* hintbox = HintBox::create("领取成功", 0);
		Director::getInstance()->getRunningScene()->addChild(hintbox, 200);
		this->scheduleOnce(schedule_selector(LoginRewardLayer::closeSelf), 1.5f);

		if (g_commomLayer != NULL)
		{
			g_commomLayer->scheduleOnce(schedule_selector(CommonLayer::delayShowNewerGuide), 1.55f);
			if (!g_commomLayer->isnewerguiding)
				g_commomLayer->scheduleOnce(schedule_selector(CommonLayer::delayShowGift), 1.6f);

		}
		if (g_ThemeScene != NULL)
		{
			g_ThemeScene->schedule(schedule_selector(ThemeScene::checkShowUnlockAnim), 0.1f);
		}

#ifdef ANALYTICS
		const char* popstr[] = { "coin", "hp", "gem", "coin", "refresh", "prompt", "gem" };
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", popstr[index]);
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "bonus", "(Ljava/lang/String;II)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, pcount, BONUSE_LOGIN);
		}
		sprintf(p_str, "%s", "getloginreward");
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
#endif
	}
}

void LoginRewardLayer::getOKAnim(const char* pic, Vec2 spoint, Vec2 dpoint)
{
	for (int i = 0; i<6; i++)
	{
		Sprite* s = Sprite::createWithSpriteFrameName(pic);
		s->setOpacity(0);
		s->setPosition(spoint);
		this->addChild(s, 100);
		s->runAction(Sequence::create(DelayTime::create(i*0.06f), Spawn::create(FadeIn::create(0.2f), EaseSineIn::create(MoveTo::create(0.3f, dpoint)), NULL), FadeOut::create(0.05f), NULL));
	}
}

void LoginRewardLayer::closeSelf(float dt)
{
	this->removeFromParentAndCleanup(true);
}

void LoginRewardLayer::onExit()
{
	Layer::onExit();
}
