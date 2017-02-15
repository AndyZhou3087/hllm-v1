#include "AchieveLayer.h"
#include "SoundManager.h"
#include "MyParticle.h"
#include "CommonLayer.h"

AchieveLayer* g_AchieveLayer = NULL;
bool AchieveLayer::isgeting = false;
AchieveLayer::AchieveLayer()
{
	g_AchieveLayer = NULL;
	isgeting = false;
}


AchieveLayer::~AchieveLayer()
{
	g_AchieveLayer = NULL;
	isgeting = false;
}

void AchieveLayer::onExit()
{
	Layer::onExit();
}

bool AchieveLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	color->setPosition(Vec2(0, 0));
	this->addChild(color);

	Node * layer = CSLoader::createNode("AchieveLayer.csb");
	addChild(layer);

	cocos2d::ui::Widget* closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("closebtn");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(AchieveLayer::close, this));

	scrollview = (cocos2d::ui::ScrollView*)layer->getChildByName("ScrollView");
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	for (unsigned int i = 0; i < GlobalData::achieveData.size(); i++)
	{
		if (GlobalData::achieveData[i].achType != AchType::ACH_WORLD_RANK && GlobalData::achieveData[i].achType != AchType::ACH_FRIEND_RANK)
		{
			if (GlobalData::achieveData[i].finish >= GlobalData::achieveData[i].count)
				GlobalData::achieveData[i].finish = INT_MAX/2;
		}
		else
		{
			if ((GlobalData::achieveData[i].finish <= GlobalData::achieveData[i].count && GlobalData::achieveData[i].finish > 0) || GlobalData::achieveData[i].finish == INT_MAX)
				GlobalData::achieveData[i].finish = INT_MAX;
		}
	}
	sortAchieveData();

	for (unsigned int i = 0; i < GlobalData::achieveData.size(); i++)
	{
		AchieveItem* item = AchieveItem::createWithData(&GlobalData::achieveData[i]);
		item->setPosition(Vec2(270, 2470 - i * 90));
		std::string str = StringUtils::format("item%d", i);
		scrollview->addChild(item, 0, str);
	}

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void AchieveLayer::sortAchieveData()
{
	sort(GlobalData::achieveData.begin(), GlobalData::achieveData.end(), byfinish);
}

void AchieveLayer::showParticle(float dt)
{
	MyParticle::Show(color, SpeciallyName[27], 270, 480, true, 0.3f, 500);
	MyParticle::Show(color, SpeciallyName[28], 270, 480, true, 0.5f, 500);

	//p->addSpecially(SpeciallyName[28], 100 + rand() % 400, 500 + rand() % 150, true, 0.8f, 20);
}

void AchieveLayer::showRwdLayer(std::string path, int count)
{
	isgeting = true;
	color = LayerColor::create(Color4B(11, 32, 22, 150));
	color->setPosition(Vec2(0, 0));
	addChild(color, 1, 999);

	Sprite * s = Sprite::createWithSpriteFrameName(path);
	s->setPosition(Vec2(270, 480));
	color->addChild(s, 2000);

	std::string str = StringUtils::format("x%d", count);
	Label *num = Label::createWithBMFont("fonts/loginrewardnum.fnt", str);
	num->setPosition(Vec2(s->getPositionX(), 420));
	color->addChild(num, 2000);
	showParticle(0);
	schedule(schedule_selector(AchieveLayer::showParticle), 0.8f);

	scheduleOnce(schedule_selector(AchieveLayer::removeRwdLayer), 1.0f);
}

void AchieveLayer::removeRwdLayer(float dt)
{
	if (g_AchieveLayer != NULL)
	{
		unschedule(schedule_selector(AchieveLayer::showParticle));
		color->removeFromParentAndCleanup(true);
		//排序
		sortAchieveData();

		for (unsigned int i = 0; i < GlobalData::achieveData.size(); i++)
		{
			std::string str = StringUtils::format("item%d", i);
			AchieveItem * item = (AchieveItem*)scrollview->getChildByName(str);
			item->updatedata(&GlobalData::achieveData[i]);
		}
	}
	isgeting = false;
}

bool AchieveLayer::byfinish(Achieve a, Achieve b)
{
	if (a.finish > b.finish)
		return true;
	return false;
}

void AchieveLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

AchieveItem::AchieveItem()
{
	rwdPath.push_back("UI/rprop3.png");
	rwdPath.push_back("UI/rprop4.png");
	rwdPath.push_back("UI/rprop5.png");
	rwdPath.push_back("UI/rprop0.png");
	rwdPath.push_back("UI/rprop1.png");
	rwdPath.push_back("UI/rprop2.png");
}

AchieveItem::~AchieveItem()
{
}

AchieveItem* AchieveItem::createWithData(Achieve *data)
{
	AchieveItem* item = new AchieveItem;
	item->init(data);
	item->autorelease();
	return item;
}

bool AchieveItem::init(Achieve *data)
{
	if (Sprite::initWithSpriteFrameName("UI/shopgemitem.png"))
	{
		Sprite* propbox = Sprite::createWithSpriteFrameName("UI/achievepropbox.png");
		propbox->setPosition(Vec2(40, 47));
		addChild(propbox);

		mname = Label::createWithSystemFont(data->name, FONT_NAME, 25);
		mname->setColor(Color3B(151, 80, 1));
		mname->setAnchorPoint(Vec2(0, 0.5));
		mname->setPosition(Vec2(90, 62));
		addChild(mname);

		std::string countstr = StringUtils::format("x%d", data->rcount);
		//mcount = Label::createWithSystemFont(countstr, FONT_NAME, 18);
		//mcount->setColor(Color3B(255, 255, 255));
		mcount = Label::createWithBMFont("fonts/shopgiftnum.fnt", countstr);
		mcount->setScale(0.9f);
		mcount->setPosition(Vec2(propbox->getPositionX(), 18));
		addChild(mcount);
		
		mdesc = Label::createWithSystemFont(data->desc, FONT_NAME, 20);
		mdesc->setAnchorPoint(Vec2(0, 0.5));
		mdesc->setColor(Color3B(79, 58, 57));
		mdesc->setPosition(Vec2(90, 25));
		addChild(mdesc);

		progresslbl = Label::createWithSystemFont("", FONT_NAME, 20);

		progresslbl->setAnchorPoint(Vec2(1, 0.5));
		progresslbl->setColor(Color3B(0, 0, 0));
		progresslbl->setPosition(Vec2(420, this->getContentSize().height/2));
		addChild(progresslbl);

		rwdSprite = Sprite::createWithSpriteFrameName(rwdPath[data->rwdtype - 4]);
		rwdSprite->setPosition(Vec2(propbox->getContentSize().width / 2, propbox->getContentSize().height/2 - 3));
		rwdSprite->setScale(0.8f);
		propbox->addChild(rwdSprite);

		getbtn = MenuItemSprite::create(Sprite::createWithSpriteFrameName("UI/greenbtnbig.png"),
			Sprite::createWithSpriteFrameName("UI/greenbtnbig.png"),
			Sprite::createWithSpriteFrameName("UI/rewardbtn1.png"),
			CC_CALLBACK_1(AchieveItem::get, this));
		getbtn->setScale(0.5f);
		getbtn->setPosition(Vec2(390, this->getContentSize().height / 2));
		Menu* getbtnMenu = Menu::create(getbtn, NULL);
		getbtnMenu->setPosition(Point::ZERO);
		addChild(getbtnMenu);

		text0 = Sprite::createWithSpriteFrameName("UI/settinggettext.png");
		text0->setPosition(Vec2(getbtn->getContentSize().width / 2, getbtn->getContentSize().height/2));
		getbtn->addChild(text0);

		text1 = Sprite::createWithSpriteFrameName("UI/yilqu.png");
		text1->setPosition(Vec2(getbtn->getContentSize().width / 2, getbtn->getContentSize().height / 2));
		getbtn->addChild(text1);
		updatedata(data);
		return true;
	}
	return false;
}

void AchieveItem::updatedata(Achieve *data)
{
	mdata = data;
	mname->setString(data->name);
	mdesc->setString(data->desc);

	std::string countstr = StringUtils::format("x%d", data->rcount);
	mcount->setString(countstr);
	int status = -1;//-1:已领取；0可以领取；1进行中

	int finishcount = mdata->finish;

	if (data->achType != AchType::ACH_WORLD_RANK && data->achType != AchType::ACH_FRIEND_RANK)
	{
		if (finishcount == -1)
			status = -1;
		else if (finishcount >= data->count)
		{
			status = 0;
			mdata->finish = INT_MAX/2;
		}
		else
			status = 1;

	}
	else
	{
		if (finishcount == -1)
			status = -1;
		else if ((finishcount <= data->count && finishcount > 0) || finishcount == INT_MAX)
		{
			status = 0;
			mdata->finish = INT_MAX;
		}
		else
			status = 1;
	}

	if (status == -1)
	{
		getbtn->setEnabled(false);
		getbtn->setVisible(true);
		text0->setVisible(false);
		text1->setVisible(true);
		mdata->finish = -1;
	}
	else if (status == 0)
	{
		getbtn->setEnabled(true);
		getbtn->setVisible(true);
		text0->setVisible(true);
		text1->setVisible(false);
	}
	else
	{
		getbtn->setVisible(false);
		getbtn->setEnabled(true);
		text0->setVisible(false);
		text1->setVisible(false);
		std::string str = StringUtils::format("%d/%d", finishcount, data->count);
		progresslbl->setString(str);
	}

	SpriteFrame* newSprite = SpriteFrameCache::getInstance()->getSpriteFrameByName(rwdPath[data->rwdtype - 4]);
	rwdSprite->setSpriteFrame(newSprite);
}

void AchieveItem::get(Ref* sender)
{
	if (AchieveLayer::isgeting)
		return;

	getbtn->setEnabled(false);
	mdata->finish = -1;
	//保存数据
	GlobalData::SaveAchieveData();
	if (g_AchieveLayer != NULL)
	{

		int index = mdata->rwdtype - 4;
		if (index == 0)
		{
			GlobalData::setPromptProp(GlobalData::getPromptProp() + mdata->rcount);
		}
		else if (index == 1)
		{
			GlobalData::setRefreshProp(GlobalData::getRefreshProp() + mdata->rcount);
		}
		else if (index == 2)
		{
			GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + mdata->rcount);
		}
		else if (index == 3)
		{
			GlobalData::setGemCount(GlobalData::getGemCount() + mdata->rcount);
		}
		else if (index == 4)
		{
			GlobalData::setGoldCount(GlobalData::getGoldCount() + mdata->rcount);
		}
		else if (index == 5)
		{
			GlobalData::setHpCount(GlobalData::getHpCount() + mdata->rcount);
		}

		g_AchieveLayer->showRwdLayer(rwdPath[index], mdata->rcount);

#ifdef ANALYTICS
		const char* popstr[] = {"prompt", "refresh", "addtime", "gem", "coin", "hp"};
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", popstr[index]);
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "bonus", "(Ljava/lang/String;II)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, mdata->rcount, BONUSE_ACHI);
		}
#endif
	}

}