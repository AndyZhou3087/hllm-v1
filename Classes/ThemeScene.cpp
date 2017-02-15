#include "ThemeScene.h"
#include "GlobalData.h"
#include "CommonLayer.h"
#include "StartScene.h"
#include "SoundManager.h"
#include "FourTypeGift.h"
#include "ShopLayer.h"
#include "SureStageLayer.h"
#include "Stage.h"
#include "UnlockLayer.h"
#include "MyTransitionScene.h"
#include "HintBox.h"
vector<Sprite*> mapbg;
vector<Node*> mapStageLayer;
vector<Node*> themebossLayer;
vector<Node*> bossHitedLayer;
vector<Stage*> vec_stages;
bool ThemeScene::isShowingUnlock = false;

ThemeScene* g_ThemeScene = NULL;

ThemeScene::ThemeScene()
{
	isShowingUnlock = false;
	vec_stages.clear();
	themecount = 0;
}

ThemeScene::~ThemeScene()
{
	for (unsigned int i = 0; i < vec_stages.size(); i++)
	{
		Stage* s = vec_stages[i];
		delete s;
		s = NULL;
	}
	vec_stages.clear();

	for (int i = 0; i < themecount; i++)
	{
		mapStageLayer[i]->getChildByName("bossnode")->removeAllChildrenWithCleanup(true);
	}

	g_ThemeScene = NULL;
}

Scene* ThemeScene::Scene()
{
	auto scene = Scene::create();

	g_ThemeScene = ThemeScene::create();

	scene->addChild(g_ThemeScene);

	return scene;
}

bool ThemeScene::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();

	themecount = GlobalData::getMapCount();

	maxs = themecount*10;

	addBackgroundSprite();

	addScrollView();
	//addBackBtn();
	
	g_commomLayer = CommonLayer::create();
	commonLayer = g_commomLayer;
	addChild(commonLayer,1);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan=CC_CALLBACK_2(ThemeScene::onTouchBegan,this);

	listener->onTouchMoved = CC_CALLBACK_2(ThemeScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ThemeScene::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	//if (!StartScene::isToThemeScene)
	//{
		SoundManager::sharedManager()->playBackMusic("sound/bg.mp3");
	//}
	//StartScene::isToThemeScene = false;

	if (UserDefault::getInstance()->getBoolForKey("firstinstall", true))
	{
		UserDefault::getInstance()->setBoolForKey("firstinstall", false);

#ifdef ANALYTICS
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", "enterhome");
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
#endif
	}

	return true;
}

void ThemeScene::onEnterTransitionDidFinish()
{
	doBYData();

	if (GlobalData::getIsGetLoginReward())
		this->schedule(schedule_selector(ThemeScene::checkShowUnlockAnim), 1.4f);

	if (GlobalData::getIsClickNext())
	{
		GlobalData::setPlayingStage(GlobalData::getPlayingStage() + 1);
		this->scheduleOnce(schedule_selector(ThemeScene::delayShowSureStageLayer), 1.5f);
	}

	for (int i = 0; i < themecount; i++)
	{
		mapStageLayer[i]->getChildByName("bossnode")->addChild(themebossLayer[i]);
	}

	this->scheduleOnce(schedule_selector(ThemeScene::delayShowBossAmin), 0.02f);
}

void ThemeScene::delayShowBossAmin(float dt)
{
	for (int i = 0; i < themecount; i++)
	{
		string ditustr = themebossLayer[i]->getName();
		auto actionAmin = CSLoader::createTimeline(ditustr);
		themebossLayer[i]->runAction(actionAmin);
		actionAmin->gotoFrameAndPlay(0, true);
	}
}

void ThemeScene::addScrollView()
{
	bool isScrollAnim = false;
	Point center = Point(visibleSize.width / 2, visibleSize.height / 2);

	int maxStage = GlobalData::getHighestStage();

	int herestage = GlobalData::getPlayingStage();

	if (herestage < 0)
	{
		herestage = maxStage + 1;
	}

	int lastherestage = herestage;
	if (herestage >= maxStage + 1)
	{
		herestage = maxStage + 1;
	}
	else
	{
		if (GlobalData::getIsCurStagePass())
		{
			herestage += 1;
		}
	}
	if (herestage >= maxs)
	{
		herestage = maxs - 1;
		lastherestage = maxs - 1;
	}

	GlobalData::setIsCurStagePass(false);

	_scrolllayer = Layer::create();
	_spritesContainer = Layer::create();

	int c = 0; 
	for (c = 0; c < themecount; c++)
	{
		mapbg.at(c)->setPosition(Vec2(center) + Vec2(0, visibleSize.height * c));
		mapStageLayer.at(c)->setPosition(Vec2(0,0) + Vec2(0, visibleSize.height * c));
		_spritesContainer->addChild(mapbg.at(c), -1);
		_spritesContainer->addChild(mapStageLayer.at(c), 2);
	}
	//add coming.jpg

	Sprite* coming = Sprite::createWithSpriteFrameName("coming.jpg");
	coming->setPosition(Vec2(center) + Vec2(0, visibleSize.height * c));
	_spritesContainer->addChild(coming, 0);

	GlobalData::setUpgradeUrl("");
	if (GlobalData::getUpgradeUrl().length() > 0)
	{
		Sprite* item0 = Sprite::createWithSpriteFrameName("updatebtn.png");
		Sprite* item1 = Sprite::createWithSpriteFrameName("updatebtn.png");
		upgradebtn = MenuItemSprite::create(
			item0,
			item1,
			item0,
			CC_CALLBACK_1(ThemeScene::getUpdateCallback, this));
		upgradebtn->setPosition(Vec2(270, 480));
		item1->setScale(0.95f);
		float scale = 1.0f - item1->getScale();
		item1->setPosition(item1->getContentSize().width*scale / 2.0f, item1->getContentSize().height*scale / 2.0f);
		Menu* menu = Menu::create();
		menu->addChild(upgradebtn);
		menu->setPosition(0, 0);
		coming->addChild(menu);
	}


	int mapsize = themecount;
	for (int i = 0; i < mapsize; i++)
	{
		std::string pageStar = StringUtils::format("page%dstar", i);
		std::string starsStr = GlobalData::getChapterStageStar(pageStar);
		long long pageStageStar = atoll(starsStr.c_str());

		int chaptersize = 10;
		for (int m = 0; m < chaptersize; m++)
		{
			bool isboss = (m + 1) % BOSSSTAGEPOS == 0 ? true : false;
			int star = pageStageStar >> (m * 2) & 3;

			std::string btnstr = StringUtils::format("stagebtn%d", m);
			cocos2d::ui::Widget* stagebtn = (cocos2d::ui::Widget*)mapStageLayer[i]->getChildByName(btnstr);

			stagebtn->addTouchEventListener(CC_CALLBACK_2(ThemeScene::clickStageCallback, this));
			Stage* stage = new Stage(stagebtn->getChildByName("stagenode"));
			stage->setStar(star, isboss);
			stage->setStageNum(i * chaptersize + m + 1);
			stagebtn->setTag(i * chaptersize + m);
			stagebtn->setEnabled(i * chaptersize + m <= maxStage + 1);
			//stagebtn->setEnabled(true);
			vec_stages.push_back(stage);
			
			if (i * chaptersize + m == maxStage + 1)
			{
				stage->nomal();
			}
			if (star >= 3)
			{
				stage->hilight();
			}

			//解锁条件

			if (i > 0 && m == 0)
			{
				int lastpagestarcount = 0;
				for (int c = 0; c <= i - 1; c++)
				{
					int laststagesize = 10;
					for (int n = 0; n < laststagesize; n++)
					{
						std::string lastpagestr = StringUtils::format("page%dstar", c);
						std::string lastpagestarsStr = GlobalData::getChapterStageStar(lastpagestr);
						long long lastpageStars = atoll(lastpagestarsStr.c_str());
						int star = lastpageStars >> (n * 2) & 3;
						lastpagestarcount += star;
					}
				}

				int stagecount = 10 * i;

				if (lastpagestarcount < unlockStars[i - 1])
				{
					if (!GlobalData::getChapterUnlock(i - 1))
					{
						stage->showLock(unlockStars[i - 1]);

						if (maxStage >= stagecount)
						{
							stage->removeLock();
							GlobalData::setChapterUnlock(i - 1, true);
							GlobalData::setShowChapterUnlock(i - 1, true);
						}
					}
				}
				else
				{
					if (!GlobalData::getChapterUnlock(i - 1))
					{
						stage->showLock(unlockStars[i - 1]);

						if (maxStage >= stagecount)
						{
							stage->removeLock();
							GlobalData::setChapterUnlock(i - 1, true);
							GlobalData::setShowChapterUnlock(i - 1, true);
						}

						if (maxStage >= stagecount - 1)
						{
							GlobalData::setChapterUnlock(i - 1, true);
						}

					}
				}
			}
		}
	}

	std::string tmpstr = StringUtils::format("stagebtn%d", herestage % 10);
	int offset = 0;
	offset = mapStageLayer[herestage / 10]->getChildByName(tmpstr)->getPositionY() + herestage / 10 * 960 - 480;
	if (offset <= 0)
		offset = 0;

	_spritesContainer->setPosition(0, 0);
	scrollView = ScrollView::create();
	scrollView->setContainer(_spritesContainer);
	scrollView->setDirection(ScrollView::Direction::VERTICAL);
	scrollView->setPosition(0, 0);
	scrollView->setViewSize(visibleSize);
	scrollView->setContentSize(Size(visibleSize.width, visibleSize.height * (themecount + 1)));

	scrollView->setContentOffset(Point(0, -offset), isScrollAnim);

	scrollView->setDelegate(this);

	scrollView->setBounceable(true);
	_scrolllayer->addChild(scrollView);
	addChild(_scrolllayer, 0);


	string herestagestr = StringUtils::format("stagebtn%d", lastherestage%10);
	Node* hereItem = mapStageLayer[lastherestage/10]->getChildByName(herestagestr);
	here = Sprite::createWithSpriteFrameName("hereblank.png");

	here->setPosition(Vec2(hereItem->getPositionX(), herestage / 10 * 960 + hereItem->getPositionY() + 50));
	_spritesContainer->addChild(here, 3);

	Sprite* myhead = Sprite::createWithSpriteFrameName("here.png");
	myhead->setPosition(Vec2(here->getContentSize().width / 2, here->getContentSize().height / 2));
	here->addChild(myhead, 0, "myhead");

	hereAnim = Sprite::createWithSpriteFrameName("here_anim1.png");

	hereAnim->setVisible(false);
	hereAnim->setScale(0.5f);
	_spritesContainer->addChild(hereAnim, 1);

	if (lastherestage < herestage)
	{
		string str = StringUtils::format("stagebtn%d", herestage%10);
		Node* moveToItem = mapStageLayer[herestage / 10]->getChildByName(str);

		if (herestage / 10 > 0 && herestage % 10 == 0)
		{
			here->setPosition(Vec2(hereItem->getPositionX(), (herestage / 10 - 1) * 960 + hereItem->getPositionY() + 50));
		}

		here->runAction(MoveTo::create(1.0f, Vec2(moveToItem->getPositionX(), herestage / 10 * 960 + moveToItem->getPositionY() + 50)));
		this->scheduleOnce(schedule_selector(ThemeScene::delayShowHeadUpAndDown), 1.2f);
	}
	else
	{
		showMyHeadAnim(Vec2(hereItem->getPositionX() - 5, herestage / 10 * 960 + hereItem->getPositionY() - 5));
		here->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1, Vec2(0, 5)), MoveBy::create(1, Vec2(0, -5)), NULL)));
	}

	int logintype = GlobalData::getLoginType();
	if (logintype > LOGIN_NONE)
	{
		checkMyHead(0);
	}
}

void ThemeScene::showHereMove(int stage)
{
	if (stage < maxs)
	{
		string str = StringUtils::format("stagebtn%d", stage%10);
		Node* moveToItem = mapStageLayer[stage / 10]->getChildByName(str);
		Stage* ostage = vec_stages[stage];
		ostage->nomal();
		here->runAction(MoveTo::create(1.0f, Vec2(moveToItem->getPositionX(), stage / 10 * 960 + moveToItem->getPositionY() + 50)));
		this->scheduleOnce(schedule_selector(ThemeScene::delayShowHeadUpAndDown), 1.2f);
	}

	if (stage - 1 >= 0)
	{
		string str = StringUtils::format("stagebtn%d", (stage - 1)%10);
		Node* curItem = mapStageLayer[(stage-1) / 10]->getChildByName(str);
		Stage* ostage = vec_stages[stage - 1];
		ostage->setStar(3);
		ostage->hilight();
	}
}

void ThemeScene::delayShowHeadUpAndDown(float dt)
{
	showMyHeadAnim(Vec2(here->getPositionX() - 5, here->getPositionY() - 55));
	here->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1, Vec2(0, 5)), MoveBy::create(1, Vec2(0, -5)), NULL)));
}

void ThemeScene::clickStageCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* item = (Node*)pSender;
		int tag = item->getTag();
		GlobalData::setPlayingStage(tag);

		here->setPosition(Vec2(item->getPositionX(), tag/10 * 960 + item->getPositionY() + 50));

		showMyHeadAnim(Vec2(item->getPositionX() - 5, tag / 10 * 960 + item->getPositionY() - 5));
		delayShowSureStageLayer(0);
	}
}

void ThemeScene::showMyHeadAnim(Vec2 pos)
{
	char s[32];
	Vector<SpriteFrame*> vec;
	SpriteFrame* frame;
	for (int i = 1; i <= 14; i++)
	{
		sprintf(s, "here_anim%d.png", i);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(s);
		vec.pushBack(frame);
	}
	hereAnim->stopAllActions();
	hereAnim->setPosition(pos);
	hereAnim->setVisible(true);
	Animation *animation = Animation::createWithSpriteFrames(vec, 0.1f);
	Animate *animate = Animate::create(animation);
	hereAnim->runAction(RepeatForever::create(Sequence::create(Show::create(), animate, Hide::create(), NULL)));
}


void ThemeScene::delayShowSureStageLayer(float dt)
{
	GlobalData::setIsClickNext(false);

	if (g_commomLayer != NULL)
	{
		if (g_commomLayer->isnewerguiding)
			return;
	}

	if (ThemeScene::isShowingUnlock)
		return;

	if (GlobalData::getPlayingStage() >= maxs)
		return;

	int unlockchapter = GlobalData::getPlayingStage() / 10;
	if (unlockchapter > 0 && !GlobalData::getChapterUnlock(unlockchapter - 1))
	{
		int lastpagestarcount = 0;

		for (int c = 0; c <= unlockchapter - 1; c++)
		{
			int laststagesize = 10;
			for (int n = 0; n < laststagesize; n++)
			{
				std::string lastpagestr = StringUtils::format("page%dstar", c);
				std::string lastpagestarsStr = GlobalData::getChapterStageStar(lastpagestr);
				long long lastpageStars = atoll(lastpagestarsStr.c_str());
				int star = lastpageStars >> (n * 2) & 3;
				lastpagestarcount += star;
			}
		}
		UnlockLayer* layer = UnlockLayer::create(unlockchapter - 1, unlockStars[unlockchapter - 1] - lastpagestarcount);
		Director::getInstance()->getRunningScene()->addChild(layer);
		return;
	}
	openSureStageLayer(0);
}

void ThemeScene::openSureStageLayer(float dt)
{
	GlobalData::setChallengeMode(CMNORMAL);
	SureStageLayer *pLayer = SureStageLayer::create();
	pLayer->setPosition(Vec2(0, 0));
	Director::getInstance()->getRunningScene()->addChild(pLayer);
}

void ThemeScene::checkMyHead(float dt)
{
	bool isneedupdate = true;
	string headimgname;
	if (GlobalData::getLoginType() == LOGIN_QQ)
		headimgname = "qhead00.png";
	else
		headimgname = "wxhead00.png";
	std::string fullpath = FileUtils::getInstance()->getWritablePath() + headimgname;

	bool isHasfile = FileUtils::getInstance()->isFileExist(fullpath);
	if (isHasfile)
	{
		Data filedata = FileUtils::getInstance()->getDataFromFile(fullpath.c_str());
		if (!filedata.isNull())
		{
			Image* img = new Image;
			img->initWithImageData(filedata.getBytes(), filedata.getSize());
			Texture2D* texture = new cocos2d::Texture2D();
			bool isImg = texture->initWithImage(img);
			if (isImg)
			{
				isneedupdate = false;
				unschedule(schedule_selector(ThemeScene::checkMyHead));
				here->removeChildByName("myhead");
				Sprite* head = Sprite::createWithTexture(texture);
				head->setPosition(Vec2(here->getContentSize().width / 2, here->getContentSize().height / 2 + 5));
				int w = img->getWidth();
				int h = img->getHeight();
				head->setTextureRect(Rect(0, 0, w, h));
				head->setScaleX(48.0f / w);
				head->setScaleY(48.0f / h);
				here->addChild(head);
				texture->release();
			}
			img->release();
		}
		Sprite* headbox = Sprite::createWithSpriteFrameName("herebox.png");
		headbox->setPosition(Vec2(here->getContentSize().width / 2, here->getContentSize().height / 2));
		here->addChild(headbox);
	}

	if (isneedupdate)
	{
		this->schedule(schedule_selector(ThemeScene::checkMyHead), 0.15f);
	}
}

void ThemeScene::checkShowUnlockAnim(float dt)
{
	int size = themecount;
	int maxstage = GlobalData::getHighestStage();
	for (int i = 0; i < size - 1; i++)
	{
		int paststage = 0;
		if (GlobalData::getChapterUnlock(i) && !GlobalData::getShowChapterUnlock(i))
		{
			this->unschedule(schedule_selector(ThemeScene::checkShowUnlockAnim));
			GlobalData::setShowChapterUnlock(i, true);

			paststage = (i + 1)* 10;
			Stage* ostage = vec_stages[paststage];
			ostage->removeLock();
			ShowUnlockAnim();
			break;
		}
	}
}

void ThemeScene::removeUnlockAnim(float dt)
{
	this->removeChildByName("unlocklayer");
	isShowingUnlock = false;
}

void ThemeScene::ShowUnlockAnim()
{
	isShowingUnlock = true;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	color->setPosition(Vec2(0, 0));
	this->addChild(color, 10, "unlocklayer");

	Sprite* light = Sprite::createWithSpriteFrameName("UI/action1.png");
	light->setPosition(color->getContentSize().width / 2, color->getContentSize().height / 2 + 150);
	color->addChild(light);
	light->runAction(RepeatForever::create(RotateTo::create(4, 720)));

	Sprite* unlocktext = Sprite::createWithSpriteFrameName("unlocktext.png");
	unlocktext->setPosition(light->getPosition());
	color->addChild(unlocktext);

	this->schedule(schedule_selector(ThemeScene::delayShowParticle), 0.8f, 1, 0.3f);

	this->scheduleOnce(schedule_selector(ThemeScene::removeUnlockAnim), 2.5f);
}

void ThemeScene::delayShowParticle(float dt)
{
	MyParticle::Show(getChildByName("unlocklayer"), SpeciallyName[7], 270, 700, true, 0.5, 300);
	MyParticle::Show(getChildByName("unlocklayer"), SpeciallyName[8], 270, 700, true, 0.3f, 40);
	MyParticle::Show(getChildByName("unlocklayer"), SpeciallyName[9], 270, 700, true, 0.5, 40);
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_FIREWORKS);
}

bool ThemeScene::onTouchBegan(Touch * touch, Event *unused_event)
{
	return true;

}

void ThemeScene::onTouchMoved(Touch*touch, Event*unused_event)
{

}

void ThemeScene::onTouchEnded(Touch*touch, Event *unused_event)
{

}


void ThemeScene::scrollViewDidScroll(ScrollView* view)
{

}


void ThemeScene::addBackgroundSprite()
{
	yun = Sprite::createWithSpriteFrameName("yun.png");
	yun->setAnchorPoint(Vec2(0, 0));

	yun->setPosition(0, 0);
	this->addChild(yun, 1);
}

void ThemeScene::getUpdateCallback(Ref* pSender)
{
	if (GlobalData::getBasePath().length() <= 0)
	{

	}
	else
	{
		upgradeLayer = UpgradeLayer::create();
		Director::getInstance()->getRunningScene()->addChild(upgradeLayer, 100);
		upgradeManager = new UpgradeManager(GlobalData::getUpgradeUrl().c_str(), GlobalData::getBasePath().c_str());
		upgradeManager->setDelegate(this);
		upgradeManager->update();
	}
}

void ThemeScene::onError(UpgradeManager::ErrorCode errorCode)
{
	upgradeLayer->removeFromParentAndCleanup(true);
	upgradeLayer = NULL;
}

/**
* @brief 下载进度（0 - 100）
* @param percent 进度百分比
*/
void ThemeScene::onProgress(double downedbytesize, double totalbytesize)
{
	upgradeLayer->updataUI(downedbytesize, totalbytesize);
}

/**
* @brief 下载下载完成并且解压完成
*/
void ThemeScene::onSuccess() 
{
	string wfile = StringUtils::format("%s/%s", GlobalData::getBasePath().c_str(), UPGRADETIMEFILE);
	FILE* file = fopen(wfile.c_str(), "wb");
	string systime = StringUtils::format("%d", GlobalData::getSystemSecTime());
	if (file)
	{
		fwrite(systime.c_str(), sizeof(unsigned char), systime.length(), file);
		fclose(file);
	}
	upgradebtn->setVisible(false);
	upgradeLayer->removeFromParentAndCleanup(true);
	upgradeLayer = NULL;

	MyTransitionScene::isloadingUpgrade = true;
	Director::getInstance()->replaceScene(MyTransitionScene::scene());
}

void ThemeScene::doBYData()
{

	int mmount = GlobalData::getBYMonth();
	int mday = GlobalData::getBYDay();

	char para[30] = { 0 };

	int pastday = 0;

	int buybytime = GlobalData::getBYTime();

	int systime = GlobalData::getSystemSecTime();

	pastday = (systime - buybytime) / (3600 * 24);

	bool isump = GlobalData::getIsUmp();

	if (buybytime == 0 && isump)
	{
		pastday = 1;
	}

	int  month = GlobalData::getMonthOfYear();
	int  day = GlobalData::getDayOfMonth();

	int bprice = GlobalData::getBYPrice();

	bool isGetBYGem = false;
	if (pastday > 0 && mday != day)
	{
		if (month != mmount)
		{
			if (isump)
			{
				isGetBYGem = true;
				GlobalData::setBYTime(GlobalData::getSystemSecTime());
				GlobalData::setBYMonth(month);
				GlobalData::setBYDay(day);
			}
			else
			{
				if (pastday <= 31)
				{
					isGetBYGem = true;
					GlobalData::setBYDay(day);

				}
				else
				{
					GlobalData::setBYTime(0);
				}
			}
		}
		else
		{
			isGetBYGem = true;
			GlobalData::setBYDay(day);
		}

	}
	if (isGetBYGem && bprice > 0)
	{
		string giftname;
		int gemnum = 0;
		if (bprice == 8)
		{
			gemnum = 6;
			giftname = "高级";
		}
		else if (bprice == 10)
		{
			gemnum = 8;
			giftname = "尊享";
		}
		else if (bprice == 15)
		{
			gemnum = 15;
			giftname = "特权";
		}
		else if (bprice == 20)
		{
			gemnum = 25;
			giftname = "至尊";
		}
		else if (bprice == 30)
		{
			gemnum = 35;
			giftname = "豪华";
		}
		GlobalData::setGemCount(GlobalData::getGemCount() + gemnum);
		string desc = StringUtils::format("已成功领取今日%s礼包中的%d个金币", giftname.c_str(), gemnum);
		HintBox* hint = HintBox::create("通关第25关解锁", 0);
		addChild(hint, 1000);
	}
}
