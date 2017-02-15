#include "EventLayer.h"
#include "SoundManager.h"
#include "CommonLayer.h"
#include "MyMenu.h"
#include "WaitingProgress.h"
#include "HttpUtil.h"
#include "StartScene.h"
#include "SureStageLayer.h"
#include "HintBox.h"
#include "ShopLayer.h"
EventLayer* g_EventLayer = NULL;

static int giftpropnum[][5] = { { 500, 2000, 2, 2, 2 }, { 288, 2888, 0, 0, 0 }, { 100, 1000, 3, 3, 3 }, { 100, 0, 1, 0, 1 }, { 1000, 0, 1, 1, 0 }, { 0, 0, 3, 3, 3 }, { 300, 0, 0, 0, 0 }, { 0, 0, 2, 2, 2 }, { 88, 0, 1, 1, 1 } };
const char* giftnames[] = { "优惠大礼包", "财富礼包", "闯关礼包", "钻石礼包", "金币礼包", "道具礼包", "新手礼包", "复活礼包", "新手礼包" };
EventLayer::EventLayer()
{
	scrollview = NULL;
	finishtext = NULL;
}


EventLayer::~EventLayer()
{
	g_EventLayer = NULL;
	scrollview = NULL;
	finishtext = NULL;
}

void EventLayer::onExit()
{
	Layer::onExit();
}

bool EventLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	color->setPosition(Vec2(0, 0));
	this->addChild(color);

	Node * layer = CSLoader::createNode("enventLayer.csb");
	addChild(layer);

	cocos2d::ui::Widget* closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("closebtn");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(EventLayer::close, this));

	scrollview = (cocos2d::ui::ScrollView*)layer->getChildByName("scrollview");

	contentnode = (cocos2d::ui::Widget*)layer->getChildByName("contentnode");
	contentnode->setVisible(false);

	descnode = (cocos2d::ui::Widget*)contentnode->getChildByName("descnode");

	rewardnode = (cocos2d::ui::Widget*)contentnode->getChildByName("rewardnode");

	rewardnode1 = (cocos2d::ui::Widget*)contentnode->getChildByName("rewardnode1");
	rewardnode1->setVisible(false);

	eventTextTitle = (cocos2d::ui::Widget*)contentnode->getChildByName("title");

	imageTitle = (cocos2d::ui::ImageView*)rewardnode1->getChildByName("imagetitle");


	finishbg = (cocos2d::ui::Widget*)rewardnode1->getChildByName("eventfinishbg");
	finishbar = (cocos2d::ui::LoadingBar*)finishbg->getChildByName("eventfinishbar");

	finishtext = (cocos2d::ui::Text*)finishbg->getChildByName("finishtext");


	cocos2d::ui::Widget* joinBtn = (cocos2d::ui::Widget*)contentnode->getChildByName("enventbtn");
	joinBtn->addTouchEventListener(CC_CALLBACK_2(EventLayer::onevent, this));

	WaitingProgress* waitbox = WaitingProgress::create("数据加载中...");
	addChild(waitbox, 10000, "waitbox");

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleOnce(schedule_selector(EventLayer::getEventsData), 0.1f);
	return true;
}

void EventLayer::getEventsData(float dt)
{
	std::string url;
	url.append(HTTPURL);
	url.append("llk_EventList");
	url.append("&vercode=");
	std::string vercodestr = StringUtils::format("%d",StartScene::getVersionCode());
	url.append(vercodestr);
	url.append("&cid=");
	url.append(GlobalData::getCid());
	HttpUtil::getInstance()->doData(url, httpEventDataCompleteCB);
}

void EventLayer::httpEventDataCompleteCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		StartScene::parseEventJson(retdata);

		if (GlobalData::EventsData.size() > 0)
			g_EventLayer->addEvents();
		else
		{
			Label* desclbl = Label::createWithSystemFont(CommonFuncs::gbk2utf("暂无活动~"), FONT_NAME, 25);
			desclbl->setColor(Color3B(255, 255, 255));
			desclbl->setPosition(Vec2(270, 480));
			g_EventLayer->addChild(desclbl, 2);
		}
	}
	//else
	//{
	//	Label* desclbl = Label::createWithSystemFont(CommonFuncs::gbk2utf("获取数据失败，请检查网络连接！"), FONT_NAME, 25);
	//	desclbl->setColor(Color3B(255, 255, 255));
	//	desclbl->setPosition(Vec2(270, 480));
	//	g_messageLayer->addChild(desclbl, 2);
	//}
	g_EventLayer->removeChildByName("waitbox");
}


void EventLayer::addEvents()
{
	int eventsize = GlobalData::EventsData.size();
	int itemwidth = 120;
	int scrollinnerwidth = eventsize * itemwidth;

	if (scrollview == NULL)
		return;

	int scorellwidth = scrollview->getContentSize().width;
	if (scrollinnerwidth <= scorellwidth)
		scrollinnerwidth = scorellwidth;
	scrollview->setInnerContainerSize(Size(scrollinnerwidth , scrollview->getContentSize().height));
	for (int i = 0; i < eventsize; i++)
	{
		int type = GlobalData::EventsData[i].type;
		std::string str0 = StringUtils::format("UI/enventbtn%d_0.png", type);
		std::string str1 = StringUtils::format("UI/enventbtn%d_1.png", type);
		Sprite* item0 = Sprite::createWithSpriteFrameName(str0);
		Sprite* item1 = Sprite::createWithSpriteFrameName(str1);
		pItem[i] = MenuItemSprite::create(
			item0,
			item1,
			item0,
			CC_CALLBACK_1(EventLayer::clickEventCallback, this));
		pItem[i]->setTag(i);
		pItem[i]->setPosition(Vec2(0, 0));
		item1->setScale(0.95f);
		float scale = 1.0f - item1->getScale();
		item1->setPosition(item1->getContentSize().width*scale / 2.0f, item1->getContentSize().height*scale / 2.0f);
		MyMenu* menu = MyMenu::create();
		menu->addChild(pItem[i], 1, "item");
		menu->setPosition(itemwidth / 2 + i * 120, scrollview->getContentSize().height/2);
		std::string str = StringUtils::format("itemmenu%d", i);
		scrollview->addChild(menu, 0, str);
	}

	clickEventCallback(pItem[0]);
	contentnode->setVisible(true);
	GlobalData::saveEventsIds();
	schedule(schedule_selector(EventLayer::updateFinishProgress), 0.5f);
}

void EventLayer::clickEventCallback(Ref* pSender)
{

	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);

	MenuItemSprite* item = (MenuItemSprite*)pSender;
	tag = item->getTag();
	curEventData = GlobalData::EventsData[tag];

	cocos2d::ui::ImageView* envent = (cocos2d::ui::ImageView*)contentnode->getChildByName("envent");

	std::string eventstr = StringUtils::format("UI/envent%d.png", curEventData.type);
	envent->loadTexture(eventstr, cocos2d::ui::TextureResType::PLIST);
	envent->setContentSize(Sprite::createWithSpriteFrameName(eventstr)->getContentSize());

	for (unsigned int i = 0; i < GlobalData::EventsData.size(); i++)
	{
		if (tag == i)
			pItem[i]->selected();
		else
			pItem[i]->unselected();
	}
	
	cocos2d::ui::Text* title = (cocos2d::ui::Text*)contentnode->getChildByName("title");
	title->setString(curEventData.title);
	cocos2d::ui::Text* timetext = (cocos2d::ui::Text*)descnode->getChildByName("timetext");
	timetext->setString(curEventData.timestr);

	cocos2d::ui::Text* desctext = (cocos2d::ui::Text*)descnode->getChildByName("desctext");
	desctext->setString(curEventData.rulestr);

	desctext->ignoreContentAdaptWithSize(false);
	desctext->setContentSize(Size(400, 100));

	cocos2d::ui::Text* timetitle = (cocos2d::ui::Text*)descnode->getChildByName("timetitle");
	cocos2d::ui::Text* ruletitle = (cocos2d::ui::Text*)descnode->getChildByName("ruletitle");

	timetitle->setTextColor(Color4B(78, 10, 10, 255));
	timetext->setTextColor(Color4B(78, 10, 10, 255));
	ruletitle->setTextColor(Color4B(78, 10, 10, 255));
	desctext->setTextColor(Color4B(78, 10, 10, 255));

	finishbg->setVisible(false);
	if (curEventData.type <= 1)
	{
		rewardnode->setVisible(true);
		rewardnode1->setVisible(false);
		descnode->setPositionY(180);
		eventTextTitle->setVisible(true);
		imageTitle->setVisible(false);
		for (int i = 0; i < 3; i++)
		{
			std::string str = StringUtils::format("propimg%d", i);
			cocos2d::ui::ImageView* propimg = (cocos2d::ui::ImageView*)rewardnode->getChildByName(str);
			str = StringUtils::format("UI/rprop%d.png", curEventData.propid[i]);
			propimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
			propimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

			str = StringUtils::format("lbl%d", i);
			cocos2d::ui::TextBMFont* num = (cocos2d::ui::TextBMFont*)rewardnode->getChildByName(str);
			str = StringUtils::format("x%d", curEventData.propnum[i]);
			num->setString(str);

			str = StringUtils::format("giftname%d", i);
			cocos2d::ui::TextBMFont* giftnamelbl = (cocos2d::ui::TextBMFont*)rewardnode->getChildByName(str);
			str = StringUtils::format("giftdesc%d", i);
			cocos2d::ui::TextBMFont* giftdesc = (cocos2d::ui::TextBMFont*)rewardnode->getChildByName(str);
			if (curEventData.propid[i] < 100)
			{
				giftnamelbl->setVisible(false);
				giftdesc->setVisible(false);
			}
			else
			{
				int gindex = curEventData.propid[i] - 100;
				str = StringUtils::format("价值%d元", pgiftprice[gindex]);
				giftdesc->setString(CommonFuncs::gbk2utf(str.c_str()));
				num->setVisible(false);
				giftnamelbl->setString(CommonFuncs::gbk2utf(giftnames[gindex]));
			}
		}
	}
	else if (curEventData.type <= 3)
	{
		rewardnode->setVisible(false);
		rewardnode1->setVisible(true);
		descnode->setPositionY(160);
		eventTextTitle->setVisible(false);
		imageTitle->setVisible(true);

		timetitle->setTextColor(Color4B::WHITE);
		timetext->setTextColor(Color4B::WHITE);
		ruletitle->setTextColor(Color4B::WHITE);
		desctext->setTextColor(Color4B::WHITE);

		std::string str = StringUtils::format("UI/eventtitle%d.png", curEventData.type);
		imageTitle->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		imageTitle->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		if (curEventData.type == 3)
		{
			int finishcount = GlobalData::getEventstatus(curEventData.type);
			int total = atoi(curEventData.data);
			int percent = finishcount * 100 / total;

			finishbg->setVisible(true);
			finishbar->setPercent(percent);
			string finishstr = StringUtils::format("%d/%d", finishcount, total);

			finishtext->setString(finishstr);
		}
	}
}
void EventLayer::doEventNotice(int type)
{
	std::string url;
	url.append(HTTPURL);
	url.append("llk_notice");
	url.append("&id=");
	url.append(GlobalData::getMyID());
	url.append("&type=");
	std::string typestr = StringUtils::format("%d", type);
	url.append("&type=");
	url.append(typestr);
	HttpUtil::getInstance()->doData(url, httpEventNoticeCB);
}

void EventLayer::httpEventNoticeCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		rapidjson::Document doc;
		doc.Parse<0>(retdata.c_str());
		if (!doc.HasParseError())  //打印解析错误
		{
			if (doc.IsObject() && doc.HasMember("data"))
			{
				rapidjson::Value& objectData = doc["data"];

				rapidjson::Value& valuedata = objectData["type"];
				int type = valuedata.GetInt();
				MEvent* mevent = GlobalData::getEventByType(type);
				if (type == 2)
				{
					GlobalData::setEventstatus(type, 1);
					mevent = GlobalData::getEventByType(3);
					if (mevent != NULL)
					{
						int lastcount = GlobalData::getEventstatus(3);
						if (lastcount < atoi(mevent->data))
						{
							int finishcount = lastcount + ShopLayer::buyprice;
							if (finishcount >= atoi(mevent->data))
							{
								//notice
								int loctime = GlobalData::getSystemSecTime();
								if (loctime >= mevent->startime && loctime <= mevent->endtime)
								{
									EventLayer::doEventNotice(3);
								}
							}
							else
							{
								GlobalData::setEventstatus(3, finishcount);
							}
						}
						ShopLayer::buyprice = 0;
					}
				}
				else if (type == 3)
					GlobalData::setEventstatus(type, atoi(mevent->data));
			}
		}
	}
}

void EventLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void EventLayer::onevent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		MEvent mevent = GlobalData::EventsData[tag];
		if (mevent.type == 0)
		{
			if (GlobalData::getHighestStage() >= 14)
			{
				GlobalData::setChallengeMode(CMCLASIC);
				Layer* layer = SureStageLayer::create();
				Director::getInstance()->getRunningScene()->addChild(layer, 100);
				this->removeFromParentAndCleanup(true);
			}
			else
			{
				HintBox* hint = HintBox::create("请先解锁经典模式", 0);
				addChild(hint, 1000);
			}

		}
		else if (mevent.type == 1)
		{
			if (GlobalData::getHighestStage() >= 24)
			{
				GlobalData::setChallengeMode(CMFLASH);
				Layer* layer = SureStageLayer::create();
				Director::getInstance()->getRunningScene()->addChild(layer, 100);
				this->removeFromParentAndCleanup(true);
			}
			else
			{
				HintBox* hint = HintBox::create("请先解锁闪电模式", 0);
				addChild(hint, 1000);
			}
		}
		else if (mevent.type == 2 || mevent.type == 3)
		{
			ShopLayer::AnimType = A_UPTODOWN;
			Layer* layer = ShopLayer::create();
			addChild(layer, 1000);
		}

	}
}

void EventLayer::updateFinishProgress(float dt)
{
	if (finishtext != NULL && curEventData.type == 3)
	{
		int finishcount = GlobalData::getEventstatus(curEventData.type);
		int total = atoi(curEventData.data);
		int percent = finishcount * 100 / total;

		finishbg->setVisible(true);
		finishbar->setPercent(percent);
		string finishstr = StringUtils::format("%d/%d", finishcount, total);

		finishtext->setString(finishstr);
	}
}