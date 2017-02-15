#include "MoreCMLayer.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "CommonLayer.h"
#include "NewerGuideLayer.h"
#include "HintBox.h"
#include "RankLayer.h"

using namespace std;

MoreCMLayer::MoreCMLayer()
{
}

MoreCMLayer::~MoreCMLayer()
{
}

bool MoreCMLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 220));
	this->addChild(color);

	_bgNode = CSLoader::createNode("CMSelectLayer.csb");
	addChild(_bgNode);

	auto action = CSLoader::createTimeline("CMSelectLayer.csb");
	_bgNode->runAction(action);
	action->gotoFrameAndPlay(0, 35, true);

	cocos2d::ui::Widget* morecmBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("morecmbtn");
	morecmBtn->addTouchEventListener(CC_CALLBACK_2(MoreCMLayer::morecm, this));

	m_BossChallengeBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("bossBtn");
	m_BossChallengeBtn->addTouchEventListener(CC_CALLBACK_2(MoreCMLayer::BossChallenge, this));

	m_ClasicBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("clasicbtn");
	m_ClasicBtn->addTouchEventListener(CC_CALLBACK_2(MoreCMLayer::ClasicChallenge, this));

	m_flashBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("flashbtn");
	m_flashBtn->addTouchEventListener(CC_CALLBACK_2(MoreCMLayer::FlashChallenge, this));

	clasicRedPoint = (cocos2d::ui::Widget*)m_ClasicBtn->getChildByName("startredpoint");
	clasicRedPoint->setVisible(false);

	flashRedPoint = (cocos2d::ui::Widget*)m_flashBtn->getChildByName("startredpoint");
	flashRedPoint->setVisible(false);

	int curyearofday = GlobalData::getDayOfYear();
	if (GlobalData::getCModeYearOfDay(CMCLASIC) != curyearofday)
	{
		GlobalData::setCModeCount(CMCLASIC, 1);
	}
	if (GlobalData::getCModeYearOfDay(CMFLASH) != curyearofday)
	{
		GlobalData::setCModeCount(CMFLASH, 1);
	}

	this->schedule(schedule_selector(MoreCMLayer::checkRedPoint), 0.2f);
	scheduleOnce(schedule_selector(MoreCMLayer::delayShowNewerGuide), 0.1f);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		morecm(morecmBtn, ui::Widget::TouchEventType::ENDED);
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void MoreCMLayer::morecm(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (g_commomLayer != NULL)
			g_commomLayer->setMoreCMbtnVisible(true);
		this->removeFromParentAndCleanup(true);
	}
}

bool MoreCMLayer::isHasRedPoint()
{
	int classmodecount = GlobalData::getCModeCount(CMCLASIC);
	int flashmodecount = GlobalData::getCModeCount(CMFLASH);
	return (classmodecount + flashmodecount) > 0 ? true : false;
}

void MoreCMLayer::checkRedPoint(float dt)
{
	int cmcount = GlobalData::getCModeCount(CMCLASIC);

	if (cmcount > 0)
	{
		std::string countstr = StringUtils::format("%d", cmcount);
		cocos2d::ui::Text* countlbl = (cocos2d::ui::Text*)clasicRedPoint->getChildByName("countlbl");
		countlbl->setString(countstr);
		clasicRedPoint->setVisible(true);
	}
	else
	{
		clasicRedPoint->setVisible(false);
	}

	cmcount = GlobalData::getCModeCount(CMFLASH);
	if (cmcount > 0)
	{
		std::string countstr = StringUtils::format("%d", cmcount);
		cocos2d::ui::Text* countlbl = (cocos2d::ui::Text*)flashRedPoint->getChildByName("countlbl");
		countlbl->setString(countstr);
		flashRedPoint->setVisible(true);
	}
	else
	{
		flashRedPoint->setVisible(false);
	}
}

void MoreCMLayer::BossChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getHighestStage() >= 19)
		{
			if (GlobalData::checkGuide(23))
			{
				g_commomLayer->isnewerguiding = false;
				if (g_NewerLayer != NULL)
					g_NewerLayer->removeSelf();
			}
			RankLayer::rankMode = CMBOSS;
			Layer* layer = RankLayer::create();
			this->addChild(layer);
		}
		else
		{
			HintBox* hint = HintBox::create("通关第20关解锁", 0);
			addChild(hint, 1000);
		}
	}
}

void MoreCMLayer::ClasicChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if (GlobalData::getHighestStage() >= 14)
		{
			if (GlobalData::checkGuide(47))
			{
				g_commomLayer->isnewerguiding = false;
				if (g_NewerLayer != NULL)
					g_NewerLayer->removeSelf();
			}
			RankLayer::rankMode = CMCLASIC;
			Layer* layer = RankLayer::create();
			this->addChild(layer);
		}
		else
		{
			HintBox* hint = HintBox::create("通关第15关解锁", 0);
			addChild(hint, 1000);
		}
	}
}

void MoreCMLayer::FlashChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getHighestStage() >= 24)
		{
			if (GlobalData::checkGuide(48))
			{
				g_commomLayer->isnewerguiding = false;
				if (g_NewerLayer != NULL)
					g_NewerLayer->removeSelf();
			}
			RankLayer::rankMode = CMFLASH;
			Layer* layer = RankLayer::create();
			this->addChild(layer);
		}
		else
		{
			HintBox* hint = HintBox::create("通关第25关解锁", 0);
			addChild(hint, 1000);
		}
	}
}

void MoreCMLayer::delayShowNewerGuide(float t)
{
	std::vector<Node*> nodes;

	if (GlobalData::checkGuide(23))
	{
		if (GlobalData::getHighestStage() >= 19)
		{
			nodes.push_back(m_BossChallengeBtn);
			g_NewerLayer = NewerGuideLayer::create(23);
			_bgNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			g_commomLayer->isnewerguiding = false;
			return;
		}
	}
	if (GlobalData::checkGuide(47))
	{
		if (GlobalData::getHighestStage() >= 14)
		{
			nodes.push_back(m_ClasicBtn);
			g_NewerLayer = NewerGuideLayer::create(47);
			_bgNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			g_commomLayer->isnewerguiding = false;
			return;
		}
	}

	if (GlobalData::checkGuide(48))
	{
		if (GlobalData::getHighestStage() >= 24)
		{
			nodes.push_back(m_flashBtn);
			g_NewerLayer = NewerGuideLayer::create(48);
			_bgNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			g_commomLayer->isnewerguiding = false;
			return;
		}
	}

}