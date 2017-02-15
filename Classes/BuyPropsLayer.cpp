#include "BuyPropsLayer.h"
#include "GlobalData.h"
#include "HintBox.h"
#include "SoundManager.h"
#include "NewerGuideLayer.h"
#include "CommonLayer.h"
int BuyPropsLayer::AnimType = A_SMALLTONORMAL;
BuyPropsLayer::BuyPropsLayer()
{
	typeIcon = NULL;
	pricelbl = NULL;
}


BuyPropsLayer::~BuyPropsLayer()
{
	AnimType = A_SMALLTONORMAL;
}
bool BuyPropsLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	_type = 0;

	_rootlayer = CSLoader::createNode("buyPropsLayer.csb");
	addChild(_rootlayer);


	okBtn = (cocos2d::ui::Widget*)_rootlayer->getChildByName("buypropbtn");
	okBtn->addTouchEventListener(CC_CALLBACK_2(BuyPropsLayer::ok, this));

	colseBtn = (cocos2d::ui::Widget*)_rootlayer->getChildByName("close");
	colseBtn->addTouchEventListener(CC_CALLBACK_2(BuyPropsLayer::close, this));

	_title = Sprite::createWithSpriteFrameName("UI/buyproptitle.png");
	_title->setPosition(262, 710);
	_rootlayer->addChild(_title);

	cocos2d::ui::Widget* light = (cocos2d::ui::Widget*)_rootlayer->getChildByName("buylight");
	light->runAction(RepeatForever::create(RotateTo::create(10,720)));

	const char* propPath[] = { "buypropprompt.png", "buyproprefresh.png", "buypropclock.png" };
	string propStr = StringUtils::format("%s", propPath[GameScene::g_whattobuy]);
	Sprite * buygoods = Sprite::createWithSpriteFrameName(propStr);
	buygoods->setPosition(Vec2(light->getPositionX(), light->getPositionY()));
	_rootlayer->addChild(buygoods);

	int count = 0;
	if (GameScene::g_whattobuy == P_PROMPT)
		count = GlobalData::getPromptProp();
	else if (GameScene::g_whattobuy == P_REFRESH)
		count = GlobalData::getRefreshProp();
	else if (GameScene::g_whattobuy == P_CLOCK)
		count = GlobalData::getAddtimeProp();
	usetext = (cocos2d::ui::Widget*)_rootlayer->getChildByName("buypropbtn")->getChildByName("usetext");
	buyusetext = (cocos2d::ui::Widget*)_rootlayer->getChildByName("buypropbtn")->getChildByName("buyusetext");
	if (count > 0)
	{
		Sprite *redpoint = Sprite::createWithSpriteFrameName("propnumbox.png");
		redpoint->setPosition(buygoods->getPositionX() + redpoint->getContentSize().width / 2 + 40, buygoods->getPositionY() + redpoint->getContentSize().height / 2 + 25);
		_rootlayer->addChild(redpoint);

		int addtimepropnum = GlobalData::getAddtimeProp();
		string propcount = StringUtils::format("%d", addtimepropnum);
		Label* countLabel = Label::createWithSystemFont(propcount, FONT_NAME, 20);
		countLabel->setPosition(redpoint->getPosition());
		countLabel->setColor(Color3B(255, 255, 255));
		if (addtimepropnum >= 100)
			countLabel->setScale(0.8f);
		_rootlayer->addChild(countLabel);
		usetext->setVisible(true);
		buyusetext->setVisible(false);
	}
	else
	{
		usetext->setVisible(false);
		buyusetext->setVisible(true);

		int price = 200;

		if (GameScene::g_whattobuy == P_PROMPT)
		{
			typeIcon = Sprite::createWithSpriteFrameName("UI/gold.png");
			typeIcon->setScale(0.4f);
		}
		else
		{
			typeIcon = Sprite::createWithSpriteFrameName("UI/diamond.png");
			typeIcon->setScale(0.4f);
			price = 20;
		}
		typeIcon->setPosition(70, 24);
		okBtn->addChild(typeIcon);

		std::string str = StringUtils::format("x%d", price);

		pricelbl = Label::createWithBMFont("fonts/buynum.fnt", str);
		pricelbl->setAnchorPoint(Vec2(0, 0.5));
		pricelbl->setPosition(80, 16);
		okBtn->addChild(pricelbl);
	}

	std::string descStr = StringUtils::format("UI/buydesc%d.png", GameScene::g_whattobuy);
	Sprite* desc = Sprite::createWithSpriteFrameName(descStr);
	desc->setPosition(270,433);
	_rootlayer->addChild(desc);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (AnimType == A_SMALLTONORMAL)
	{
		_rootlayer->setAnchorPoint(Vec2(0.5f, 0.5f));
		_rootlayer->setPosition(Vec2(270, 480));
		_rootlayer->setScale(0.1f);
		_rootlayer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5, 1)), 2));
	}
	else
	{
		_rootlayer->setPosition(0, 960);
		_rootlayer->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
	}

	this->scheduleOnce(schedule_selector(BuyPropsLayer::delayShowNewerGuide), 0.6f);
	return true;
}

void BuyPropsLayer::ok(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GameScene::g_whattobuy == P_PROMPT)
		{
			if (GlobalData::getPromptProp() > 0)
			{
				GlobalData::setPromptProp(GlobalData::getPromptProp() - 1);
				useProp(P_PROMPT);
				resumeGame();

				Achieve* data = GlobalData::getAchieveDataByID(9);
				if (data->finish != -1)
				{
					data->finish++;
					GlobalData::SaveAchieveData();
				}

			}
			else if (GlobalData::getGoldCount() >= 200)
			{
				GlobalData::setGoldCount(GlobalData::getGoldCount() - 200);
				useProp(P_PROMPT);
				resumeGame();

				Achieve* data = GlobalData::getAchieveDataByID(19);
				if (data->finish != -1)
				{
					data->finish += 200;
					GlobalData::SaveAchieveData();
				}

				data = GlobalData::getAchieveDataByID(9);
				if (data->finish != -1)
				{
					data->finish ++;
					GlobalData::SaveAchieveData();
				}

				data = GlobalData::getTaskDataByID(4);
				if (data->finish != -1)
				{
					data->finish += 200;
					GlobalData::SaveTaskData();
				}
			}
			else
			{
				HintBox* hint = HintBox::create("金币不足", 2);
				addChild(hint, 1000);
			}
		}
		else if (GameScene::g_whattobuy == P_REFRESH)
		{
			if (GlobalData::getRefreshProp() > 0)
			{
				GlobalData::setRefreshProp(GlobalData::getRefreshProp() - 1);
				useProp(P_REFRESH);
				resumeGame();

				Achieve* data = GlobalData::getAchieveDataByID(8);
				if (data->finish != -1)
				{
					data->finish ++;
					GlobalData::SaveAchieveData();
				}
			}
			else if (GlobalData::getGemCount() >= 20)
			{
				GlobalData::setGemCount(GlobalData::getGemCount() - 20);
				useProp(P_REFRESH);
				resumeGame();
				Achieve* data = GlobalData::getAchieveDataByID(9);
				if (data->finish != -1)
				{
					data->finish++;
					GlobalData::SaveAchieveData();
				}
			}
			else
			{
				HintBox* hint = HintBox::create("钻石不足", 1);
				addChild(hint, 1000);
			}
		}

		else if (GameScene::g_whattobuy == P_CLOCK)
		{
			if (_type == 1 && GlobalData::checkGuide(19))
			{
				if (g_NewerLayer != NULL)
					g_NewerLayer->removeSelf();
				useProp(P_CLOCK);
				resumeGame();
				return;
			}
			if (GlobalData::getAddtimeProp() > 0)
			{
				GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() - 1);
				useProp(P_CLOCK);
				resumeGame();
				Achieve* data = GlobalData::getAchieveDataByID(7);
				if (data->finish != -1)
				{
					data->finish++;
					GlobalData::SaveAchieveData();
				}
			}
			else if (GlobalData::getGemCount() >= 20)
			{
				GlobalData::setGemCount(GlobalData::getGemCount() - 20);
				useProp(P_CLOCK);
				resumeGame();
				Achieve* data = GlobalData::getAchieveDataByID(7);
				if (data->finish != -1)
				{
					data->finish++;
					GlobalData::SaveAchieveData();
				}
			}
			else
			{
				HintBox* hint = HintBox::create("钻石不足", 1);
				addChild(hint, 1000);
			}
		}
	}
}

void BuyPropsLayer::useProp(PropsType type)
{
	if (type == P_PROMPT)
	{
		g_gameScene->usePrompt();
	}
	else if (type == P_REFRESH)
	{
		g_gameScene->useRefresh();
	}
	else if (type == P_CLOCK)
	{
		g_gameScene->useClock();
	}
}

void BuyPropsLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		resumeGame();
	}
}

void BuyPropsLayer::resumeGame()
{
	this->removeFromParentAndCleanup(true);
	g_gameScene->gameResume();
}

void BuyPropsLayer::changeTitle()
{
	_title->setSpriteFrame("UI/continuegametext.png");
	_type = 1;
}

void BuyPropsLayer::delayShowNewerGuide(float dt)
{
	if (_type == 1)
	{
		if (GlobalData::checkGuide(19) && GlobalData::getAddtimeProp() > 0)
		{
			usetext->setVisible(true);
			buyusetext->setVisible(false);
			if (typeIcon != NULL)
				typeIcon->setVisible(false);
			if (pricelbl != NULL)
				pricelbl->setVisible(false);
			vector<Node*> nodes;
			nodes.push_back(okBtn);
			g_NewerLayer = NewerGuideLayer::create(19);
			_rootlayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
		}
	}
}