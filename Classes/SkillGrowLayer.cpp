#include "SkillGrowLayer.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "GameScene.h"
#include "StartScene.h"
#include "HintBox.h"
#include "NewerGuideLayer.h"
#include "CommonLayer.h"

SkillGrowLayer* g_skillGrowlayer = NULL;
SkillGrowLayer::SkillGrowLayer()
{

}

SkillGrowLayer::~SkillGrowLayer()
{
	g_skillGrowlayer = NULL;
}
bool SkillGrowLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	_layer = CSLoader::createNode("skillGrowLayer.csb");
	addChild(_layer);
	_heroAminLayer = CSLoader::createNode("hero.csb");
	_heroAminLayer->setPosition(270, 660);
	_heroAminLayer->setRotationSkewY(0);

	_layer->addChild(_heroAminLayer, 1);
	actionAmin = CSLoader::createTimeline("hero.csb");
	_heroAminLayer->runAction(actionAmin);
	actionAmin->gotoFrameAndPlay(0, 30, true);

	_heroLvupAminLayer = CSLoader::createNode("herolvup.csb");
	_heroLvupAminLayer->setPosition(270, 660);
	_heroLvupAminLayer->setRotationSkewY(0);

	_layer->addChild(_heroLvupAminLayer, 1);
	actionAmin1 = CSLoader::createTimeline("herolvup.csb");
	_heroLvupAminLayer->runAction(actionAmin1);
	_heroLvupAminLayer->setVisible(false);

	_closeBtn = (cocos2d::ui::Button*)_layer->getChildByName("growclosebtn");
	_closeBtn->addTouchEventListener(CC_CALLBACK_2(SkillGrowLayer::close, this));

	_growUpBtn = (cocos2d::ui::Button*)_layer->getChildByName("growupbtn");
	_growUpBtn->addTouchEventListener(CC_CALLBACK_2(SkillGrowLayer::growUp, this));

	_smallgoldicon = (cocos2d::ui::Button*)_layer->getChildByName("growupbtn")->getChildByName("smallgold");
	_needGoldlbl = (cocos2d::ui::TextBMFont*)_layer->getChildByName("growupbtn")->getChildByName("growgoldbmlbl");
	_select = (cocos2d::ui::Widget*)_layer->getChildByName("select");

	_leveUpText = Sprite::createWithSpriteFrameName("UI/leveluptext.png");
	_leveUpText->setVisible(false);
	_leveUpText->setPosition(Vec2(270, 453));
	addChild(_leveUpText, 1);

	int myTotalSkillLevel = GlobalData::getMyTotalSkillLevel();

	int index = myTotalSkillLevel % 6;

	std::string str = StringUtils::format("skillbox_%d", index + 1);
	cocos2d::ui::Widget* skillbox = (cocos2d::ui::Widget*)_layer->getChildByName(str);
	_select->setPosition(Vec2(skillbox->getPositionX() - 17, skillbox->getPositionY()));
	_select->setVisible(true);

	_lvnum = (cocos2d::ui::TextBMFont*)_layer->getChildByName("slv");
	str = StringUtils::format("%d", myTotalSkillLevel);
	_lvnum->setString(str);

	for (int i = 0; i < 6; i++)
	{
		str = StringUtils::format("skillbox_%d", i + 1);
		for (int m = 0; m < GlobalData::getMySkillLevel(i); m++)
		{
			std::string starstr = StringUtils::format("star_%d", m + 1);
			_layer->getChildByName(str)->getChildByName(starstr)->setVisible(true);
		}
		cocos2d::ui::TextBMFont* valuelbl = (cocos2d::ui::TextBMFont*)_layer->getChildByName(str)->getChildByName("value");
		int level = GlobalData::getMySkillLevel(i);
		string valuestr;
		if (level > 0)
		{
			if (i == S_ADDCOMBOTIME)
				valuestr = StringUtils::format("%.1f", GlobalData::mapSkillData[i].data[level - 1]);
			else
				valuestr = StringUtils::format("%.0f", GlobalData::mapSkillData[i].data[level - 1]);
		}
		else
		{
			valuestr = StringUtils::format("%d", 0);
		}
		valuelbl->setString(valuestr);
	}
	needGold = 100 + 10 * GlobalData::getMyTotalSkillLevel();
	str = StringUtils::format("x%d", needGold);
	_needGoldlbl->setString(str);

	if (GlobalData::getMyTotalSkillLevel() >= 30)
	{
		cocos2d::ui::Widget* lvuptext = (cocos2d::ui::Widget*)_layer->getChildByName("growupbtn")->getChildByName("lvuptext");
		lvuptext->setVisible(false);
		_smallgoldicon->setVisible(false);
		_needGoldlbl->setVisible(false);
		cocos2d::ui::Widget* lvmaxtext = (cocos2d::ui::Widget*)_layer->getChildByName("growupbtn")->getChildByName("lvmaxtext");
		lvmaxtext->setVisible(true);
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_layer->setAnchorPoint(Vec2(0.5f, 0.5f));
	_layer->setPosition(Vec2(270, 480));
	_layer->setScale(0.1f);
	_layer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	this->scheduleOnce(schedule_selector(SkillGrowLayer::delayShowNewerGuide), 0.5f);
	return true;
}

void SkillGrowLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}


void SkillGrowLayer::growUp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if (GlobalData::checkGuide(22))
		{
			if (g_NewerLayer != NULL)
				g_NewerLayer->removeSelf();
		}

		int nextlevel = GlobalData::getMyTotalSkillLevel() + 1;

		if (nextlevel > 30)
			return;

		if (needGold <= GlobalData::getGoldCount())
		{
			Achieve* data = GlobalData::getAchieveDataByID(19);
			if (data->finish != -1)
			{
				data->finish += needGold;
				GlobalData::SaveAchieveData();
			}

			data = GlobalData::getTaskDataByID(4);
			if (data->finish != -1)
			{
				data->finish += needGold;
				GlobalData::SaveTaskData();
			}

#ifdef ANALYTICS
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", "coin");
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, needGold);
			}

			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "setPlayerLevel", "(I)V"))
			{
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, nextlevel);
			}
#endif

			GlobalData::setMyTotalSkillLvel(nextlevel);
			std::string str = StringUtils::format("%d", nextlevel);
			_lvnum->setString(str);
			GlobalData::setGoldCount(GlobalData::getGoldCount() - needGold);

			needGold = 100 + 10 * nextlevel;

			std::string goldstr = StringUtils::format("x%d", needGold);
			_needGoldlbl->setString(goldstr);
			int myskillindex = nextlevel % 6 == 0 ? 5 : (nextlevel % 6) - 1;
			GlobalData::setMySkilLevel(myskillindex, GlobalData::getMySkillLevel(myskillindex) + 1);

			int mycurSkillLevel = GlobalData::getMySkillLevel(myskillindex);

			str = StringUtils::format("skillbox_%d", myskillindex + 1);
			string starstr = StringUtils::format("star_%d", mycurSkillLevel);
			cocos2d::ui::Widget* skillbox = (cocos2d::ui::Widget*)_layer->getChildByName(str);
			skillbox->getChildByName(starstr)->setVisible(true);
			
			cocos2d::ui::TextBMFont* valuelbl = (cocos2d::ui::TextBMFont*)_layer->getChildByName(str)->getChildByName("value");
			string valuestr;
			if (myskillindex == S_ADDCOMBOTIME)
				valuestr = StringUtils::format("%.1f", GlobalData::mapSkillData[myskillindex].data[mycurSkillLevel - 1]);
			else
				valuestr = StringUtils::format("%.0f", GlobalData::mapSkillData[myskillindex].data[mycurSkillLevel - 1]);

			valuelbl->setString(valuestr);

			int index = nextlevel % 6;
			str = StringUtils::format("skillbox_%d", index + 1);
			skillbox = (cocos2d::ui::Widget*)_layer->getChildByName(str);
			_select->setPosition(Vec2(skillbox->getPositionX() - 17, skillbox->getPositionY()));
			_select->setVisible(true);

			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_SKILLSUCC);

			HintBox* hint = HintBox::create("升级成功", 0);
			addChild(hint, 1000);

			if (GlobalData::getMyTotalSkillLevel() >= 30)
			{
				cocos2d::ui::Widget* lvuptext = (cocos2d::ui::Widget*)_layer->getChildByName("growupbtn")->getChildByName("lvuptext");
				lvuptext->setVisible(false);
				_smallgoldicon->setVisible(false);
				_needGoldlbl->setVisible(false);
				cocos2d::ui::Widget* lvmaxtext = (cocos2d::ui::Widget*)_layer->getChildByName("growupbtn")->getChildByName("lvmaxtext");
				lvmaxtext->setVisible(true);
			}
			_heroAminLayer->setVisible(false);
			_heroLvupAminLayer->setVisible(true);
			actionAmin1->gotoFrameAndPlay(0, 46, false);
			this->scheduleOnce(schedule_selector(SkillGrowLayer::showHeroNormalAmin), 1.5f);

			data = GlobalData::getAchieveDataByID(20);
			if (data->finish != -1)
			{
				data->finish = GlobalData::getMyTotalSkillLevel();
				GlobalData::SaveAchieveData();
			}

			data = GlobalData::getAchieveDataByID(21);
			if (data->finish != -1)
			{
				data->finish = GlobalData::getMyTotalSkillLevel();
				GlobalData::SaveAchieveData();
			}

			data = GlobalData::getAchieveDataByID(22);
			if (data->finish != -1)
			{
				data->finish = GlobalData::getMyTotalSkillLevel();
				GlobalData::SaveAchieveData();
			}
		}
		else
		{
			HintBox* hint = HintBox::create("金币不足", 2);
			addChild(hint, 1000);
		}

	}
}
void SkillGrowLayer::showHeroNormalAmin(float t)
{
	_heroAminLayer->setVisible(true);
	_heroLvupAminLayer->setVisible(false);
}

void SkillGrowLayer::delayShowNewerGuide(float dt)
{
	if (GlobalData::checkGuide(22) && GlobalData::getHighestStage() <= 9)
	{
		std::vector<Node*> nodes;
		nodes.push_back(_growUpBtn);
		g_NewerLayer = NewerGuideLayer::create(22);
		_layer->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
	}
}