#include "FourTypeGift.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "ShopLayer.h"
#include "GameScene.h"
#include "CommonLayer.h"
int FourTypeGift::AnimType = A_SMALLTONORMAL;
int FourTypeGift::_type = COINGIFT;
bool FourTypeGift::isshowinggift = false;
bool FourTypeGift::isClickPop = false;
FourTypeGift::FourTypeGift()
{
	isshowinggift = false;
}

FourTypeGift::~FourTypeGift()
{
	isshowinggift = false;
	isClickPop = false;
}

bool FourTypeGift::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 220));
	this->addChild(color);

	std::string giftName[] = {"onsaleGiftLayer.csb", "richGiftLayer.csb", "CGGiftLayer.csb", "gemGiftLayer.csb", "coinGiftLayer.csb", "propGiftLayer.csb", "newerGiftLayer.csb", "reviveGiftLayer.csb",
							"firstGiftLayer.csb", "onsaleGiftLayer.csb", "richGiftLayer.csb", "CGGiftLayer.csb", "gemGiftLayer.csb", "coinGiftLayer.csb", "propGiftLayer.csb", "onsaleGiftLayer.csb", 
							"richGiftLayer.csb", "CGGiftLayer.csb", "gemGiftLayer.csb", "coinGiftLayer.csb", "propGiftLayer.csb", "onsaleGiftLayer.csb", "gemGiftLayer.csb", "coinGiftLayer.csb", "propGiftLayer.csb" };
	_layer = CSLoader::createNode(giftName[_type]);
	addChild(_layer);

	auto layerAnim = CSLoader::createTimeline(giftName[_type]);
	_layer->runAction(layerAnim);
	layerAnim->gotoFrameAndPlay(0, 30, true);

	cocos2d::ui::Widget* zkbox = (cocos2d::ui::Widget*)_layer->getChildByName("zkbox");

	std::string pricestr = StringUtils::format("%d.00", pgiftyuanprice[_type]);
	cocos2d::ui::TextBMFont* yuanpricelbl = (cocos2d::ui::TextBMFont*)zkbox->getChildByName("yuanprice");
	yuanpricelbl->setString(pricestr);

	pricestr = StringUtils::format("%d.00", pgiftprice[_type]);
	cocos2d::ui::TextBMFont* pricelbl = (cocos2d::ui::TextBMFont*)zkbox->getChildByName("price");
	pricelbl->setString(pricestr);

	cocos2d::ui::Button* colseBtn = (cocos2d::ui::Button*)_layer->getChildByName("closebtn");
	colseBtn->addTouchEventListener(CC_CALLBACK_2(FourTypeGift::close, this));
#ifdef ZFTEXTCLEAR
	colseBtn->setVisible(false);
	cocos2d::ui::Widget* colseBtnclear = (cocos2d::ui::Widget*)_layer->getChildByName("closebtnclear");
	colseBtnclear->addTouchEventListener(CC_CALLBACK_2(FourTypeGift::close, this));
	colseBtnclear->setVisible(true);
#endif

	getBtn = (cocos2d::ui::Widget*)_layer->getChildByName("giftbtn");
	getBtn->addTouchEventListener(CC_CALLBACK_2(FourTypeGift::get, this));
	getBtn->setTag(_type + sizeof(gemNum) / sizeof(gemNum[0]));

	getBtn->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 0.95f), ScaleTo::create(0.5f, 1.0f), NULL)));

	cocos2d::ui::Widget* buytext0 = (cocos2d::ui::Widget*)getBtn->getChildByName("buytext0");
	cocos2d::ui::Widget* buytext1 = (cocos2d::ui::Widget*)getBtn->getChildByName("buytext1");

	if (GlobalData::getBuyTextType() == 0)
	{
		buytext0->setVisible(false);
		buytext1->setVisible(true);
	}
	else
	{
		buytext0->setVisible(true);
		buytext1->setVisible(false);
	}
	int fontsize = 18;
	int opacity = 100;
#ifdef ZFTEXTCLEAR
	fontsize = 22;
	opacity = 200;
#endif

	string paystr = CommonFuncs::gbk2utf(StringUtils::format("您将通过短信购买此礼包，信息费%d元。\n如有问题请联系客服电话：4009908188。", pgiftprice[_type]).c_str());
	Label* paydsc = Label::createWithSystemFont(paystr, FONT_NAME, fontsize);
	paydsc->setPosition(Vec2(270, 55));
	paydsc->setOpacity(opacity);
	_layer->addChild(paydsc);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	if (AnimType == A_SMALLTONORMAL)
	{
		_layer->setAnchorPoint(Vec2(0.5f, 0.5f));
		_layer->setPosition(Vec2(270, 480));
		_layer->setScale(0.1f);
		_layer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	}
	else
	{
		_layer->setPosition(0, 960);
		_layer->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
		AnimType = A_SMALLTONORMAL;
	}

	isshowinggift = true;

#ifdef ANALYTICS
	if (isClickPop)
	{
		string enventstr = StringUtils::format("clickpop%s", packagename[_type]);
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", enventstr.c_str());
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
	}
	else
	{
		string enventstr = StringUtils::format("autopop%s", packagename[_type]);
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", enventstr.c_str());
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
	}
#endif
	this->schedule(schedule_selector(FourTypeGift::showAnim), 1.5f);
	return true;
}

void FourTypeGift::showAnim(float dt)
{
	MyParticle::Show(getBtn, SpeciallyName[35], getBtn->getContentSize().width / 2, getBtn->getContentSize().height / 2, true, 0.3f, 10);
}

void FourTypeGift::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (_type != REVIVEGIFT)
		{
			if (g_gameScene != NULL && GlobalData::g_gamestatus != GAMEOVER)
				g_gameScene->gameResume();
		}
#ifdef ANALYTICS
		if (isClickPop)
		{
			string enventstr = StringUtils::format("clickpopX%s", packagename[_type]);

			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", enventstr.c_str());
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
		}
		else
		{
			string enventstr = StringUtils::format("autopopX%s", packagename[_type]);
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", enventstr.c_str());
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
		}
#endif
		this->removeFromParentAndCleanup(true);
	}
}

void FourTypeGift::get(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
#ifdef ANALYTICS
		if (isClickPop)
		{
			string enventstr = StringUtils::format("clickpopOK%s", packagename[_type]);

			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", enventstr.c_str());
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
		}
		else
		{
			string enventstr = StringUtils::format("autopopOK%s", packagename[_type]);
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", enventstr.c_str());
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
		}
#endif
		int tag = getBtn->getTag();
		ShopLayer::beginPayment(tag);
	}
}