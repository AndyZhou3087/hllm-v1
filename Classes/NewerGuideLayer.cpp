
#include "NewerGuideLayer.h"
#include "GlobalData.h"
#include "GameScene.h"
#include "SkillGrowLayer.h"
#include "CommonLayer.h"
NewerGuideLayer* g_NewerLayer = NULL;
int NewerGuideLayer::_step = 0;

string descText[] = {"欢迎来到欢乐连萌的世界，让我来\n教你如何在这个世界欢乐连萌吧。", "欢迎来到欢乐连萌的世界，让我来\n教你如何在这个世界欢乐连萌吧。", "", "我们需要在这个倒计时结束前消掉下方\n所有的图形，不然你就开心不起来啦。",
"两个相同的图形如果能用不超\n过2个拐角的线相连就能消除。", "为了便于理解，让我们一起\n来看看下面这张图吧~", "拐角不超过2个就可以进行消除。", "拐角不超过2个就可以进行消除。",
"拐角不超过2个就可以进行消除。", "这需要3个拐角，就不能消除了！", "加油！偷偷告诉你，", "后面的关会越来越难，合理的\n利用道具会更简单哦，加油！", 
"","在开始消除之前，先让你\n看看我们的秘密武器。", "找不到消除的图案了吗？不要慌！\n使用提示道具试一试。", "又找不到消除的图案了吗？不要慌！\n使用重排道具试一试。", 
"这是加时道具，使用它可以\n增加15秒的闯关时间","这是作为新手的奖励，\n我来告诉你如何使用它们！", "好了，让我们抓紧时间\n继续前进吧。", "时间结束了？没关系，使用加时道具试一试。", 
"呜呜呜~~~\n挑战失败了，我可不想重新\n开始，让我们再来一次吧。", "现在你可以进入成长界面，通过升级“我”\n来给你带来更欢乐的欢乐连萌。", "点击升级按钮即可升级我，升级我\n会学习新的技能哦，快来升级我吧。", "BOSS挑战现在开放了，以你\n现在的实力应该可以轻松干掉他们，\n快去试试吧。",
"这就是游戏界面了，让我给你\n介绍一下基础游戏规则。", "看这2个闪闪发亮的图标，\n来试试滑动消除它们。", "太棒了，当局面较为复杂时，使用\n提示道具能帮你节省很多时间。", "看，所有的图标都重新排列了，这样\n我们能更容易找到可以消除的图形。",
"有时候离过关就差一点点，\n加时道具能帮你顺利过关哦。", "太棒了，这后面还有好多技能，\n我们一起来看看。", "每次消除都有几率加时，太棒了，\n但是我们需要通关第二幕才能领悟。", "后面还有更多有用的技能，\n让我们继续努力前进吧。",
"看，这里有个宝箱在闪闪发光，\n点进去看看。", "过关获得的星星累计到一定数量\n就能领取奖励，这是我们的\n第一个奖励,真是太棒了。", "后面还有更多的奖励，\n我们继续努力吧。", "这是我们通关第一幕后解锁的道具，\n来试试看吧。",
"此种石块无法进行消除，会阻挡\n你的消除路线。", "此种石块只要把宝石消除掉\n就可进行消除。","", "云会不定时散开，你要找机会\n记住被遮挡住的是什么图案。", 
"点击问号牌即可翻开查看，但是你点击\n其它图案时又会翻过去哦。","被锁住的图案是无法进行消除的，你要\n先找到对应的钥匙进行消除才能解锁哦。","", "移动要按照数字的顺序\n进行消除才能进行消除哦。", 
"被冰冻图案无法进行消除，必须\n先消除相邻的其它图案才能破碎冰块。", "被边框圈起来的图案组无法进行\n最外围的消除，只能从内部消除。", "每个BOSS都有技能，会给游戏增加难度。", "快来挑战经典模式吧！\n更多奖励等着你哦!",
"真棒，快来挑战闪电模式吧！"
};

NewerGuideLayer::NewerGuideLayer()
{

}
NewerGuideLayer::~NewerGuideLayer()
{
	iscanclick = false;
}
NewerGuideLayer* NewerGuideLayer::create(int type)
{
	NewerGuideLayer *pRet = new NewerGuideLayer();
	if (pRet && pRet->init(type))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool NewerGuideLayer::init(int step)
{
	_step = step;
	_sj = NULL;
	isCanClick = true;
    if (!Layer::init()) 
	{
		return false;
    }
	color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);
	/*
	_clipNode = ClippingNode::create();

	_clipNode->setInverted(true);
	_clipNode->setAlphaThreshold(0);
	_clipNode->addChild(color);

	this->addChild(_clipNode);

	*/

	guidetextbox = Sprite::createWithSpriteFrameName("guidetextbox.png");
	guidetextbox->setPosition(Vec2(320, 200));
	addChild(guidetextbox);

	heroGuider = Sprite::createWithSpriteFrameName("guidehero.png");
	heroGuider->setPosition(Vec2(guidetextbox->getPositionX() - guidetextbox->getContentSize().width / 2, guidetextbox->getPositionY() + guidetextbox->getContentSize().height / 2 + 20));
	addChild(heroGuider);

	std::string desc = CommonFuncs::gbk2utf(StringUtils::format("%s", descText[_step].c_str()).c_str());
	_guideText = Label::createWithBMFont("fonts/guidetext.fnt", desc);
	_guideText->setAnchorPoint(Vec2(0, 0.5f));
	_guideText->setAlignment(TextHAlignment::LEFT);
	_guideText->setColor(Color3B(97, 49, 8));
	_guideText->setPosition(Vec2(35, guidetextbox->getContentSize().height/2));

	if (_step == 0 || _step == 3 || _step == 11 || _step == 23 || _step == 26 || _step == 31 || _step == 32 )
	{
		heroGuider->setPositionY(480);
		guidetextbox->setPositionY(480);
	}
	else if (_step == 47 || _step == 48)
	{
		heroGuider->setPositionY(300);
		guidetextbox->setPositionY(300);
	}
	else if (_step == 46)
	{
		heroGuider->setPositionY(650);
		guidetextbox->setPositionY(650);
	}
	else if (_step == 4 || _step == 25)
	{
		heroGuider->setPositionY(750);
		guidetextbox->setPositionY(750);
		if (_step == 25)
			GlobalData::setIsShowNewerGuidePromp(true);
	}
	else if (_step == 17 || _step == 19 || _step == 20 || _step == 22 || _step == 29 || _step == 30 || _step == 33)
	{
		heroGuider->setPositionY(90);
		guidetextbox->setPositionY(90);
	}
	else if (_step == 2 || _step == 12 || _step == 38 || _step == 42)
	{
		heroGuider->setVisible(false);
		guidetextbox->setVisible(false);
	}
	else if (_step >= 36)
	{
		heroGuider->setPositionY(780);
		guidetextbox->setPositionY(780);
	}

	heroGuider->setPosition(Vec2(guidetextbox->getPositionX() - guidetextbox->getContentSize().width / 2 + 33, guidetextbox->getPositionY() + guidetextbox->getContentSize().height / 2 + 30));

	GlobalData::setNewerGuideStep(_step);

	guidetextbox->addChild(_guideText);

	iscanclick = false;

	this->scheduleOnce(schedule_selector(NewerGuideLayer::canClick), 1.0f);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		if (!iscanclick)
			return true;

		if ( _step == 4 || (_step == 26 || _step == 27 || _step == 28) || _step == 24)
		{
			removeSelf();
			if (g_gameScene != NULL)
			{
				if (_step == 24)
				{
					_step = 3;
					g_gameScene->NewerGuideClickCB(_step);
				}
				else if (_step == 26)
				{
					g_gameScene->NewerGuideClickCB(15);
				}
				else if (_step == 27)
				{
					g_gameScene->NewerGuideClickCB(16);
				}
				else if (_step == 28)
				{
					g_gameScene->NewerGuideClickCB(18);
				}

				else
				{
					g_gameScene->NewerGuideClickCB(_step + 1);
				}
			}
		}
		else if (_step == 1)
		{
			removeSelf();
			if (g_gameScene != NULL)
				g_gameScene->NewerGuideClickCB(3);
			//showGamingGuide();
		}
		else if (_step == 3)
		{
			showGamingGuide();
		}
		else if (_step == 8 || _step == 9)
		{
			showNextSetp(0);
		}
		else if (_step == 10 || _step == 18)//游戏中介绍完毕
		{
			removeSelf();
			if (g_gameScene != NULL)
				//g_gameScene->show321Anim();
				g_gameScene->showReadyAnim();
		}
		else if (_step == 30 || _step == 34 || _step == 23 || _step == 47 || _step == 48)
		{
			removeSelf();
		}

		else if (_step >= 36 && _step <= 46)
		{
			removeSelf();
			if (_step == 37 || _step == 41)
			{
				if (g_gameScene != NULL)
				{
					g_gameScene->showNextBlocksGuide(_step);
				}
			}
			else
				GlobalData::g_gamestatus = GAMESTART;
		}

		if (_step == 25)
		{
			Point location = Director::getInstance()->convertToGL(touch->getLocationInView());
			if (_hightNodes.size() >= 2)
			{
				if ((location.x >= _hightNodes[0]->getPositionX() - _hightNodes[0]->getContentSize().width / 2 && location.x <= _hightNodes[0]->getPositionX() + _hightNodes[0]->getContentSize().width / 2
					&& location.y >= _hightNodes[0]->getPositionY() - _hightNodes[0]->getContentSize().height / 2 && location.y <= _hightNodes[0]->getPositionY() + _hightNodes[0]->getContentSize().height / 2)
					|| (location.x >= _hightNodes[1]->getPositionX() - _hightNodes[1]->getContentSize().width / 2 && location.x <= _hightNodes[1]->getPositionX() + _hightNodes[1]->getContentSize().width / 2
					&& location.y >= _hightNodes[1]->getPositionY() - _hightNodes[1]->getContentSize().height / 2 && location.y <= _hightNodes[1]->getPositionY() + _hightNodes[1]->getContentSize().height / 2))
				{
					for (unsigned int i = 0; i < _hightNodes.size(); i++)
					{
						_hightNodes[i]->setLocalZOrder(NEWERLAYERZOER + 1 + i);
					}
					g_gameScene->onTouchBegan(touch, event);
				}

			}
		}
		return true;
	};
	listener->onTouchMoved = [=](Touch* touch, Event* event)
	{
		if (!iscanclick)
			return;
		if (_step == 25)
		{
			Point location = Director::getInstance()->convertToGL(touch->getLocationInView());
			if (_hightNodes.size() >= 2)
			{
				if ((location.x >= _hightNodes[0]->getPositionX() - _hightNodes[0]->getContentSize().width / 2 && location.x <= _hightNodes[0]->getPositionX() + _hightNodes[0]->getContentSize().width / 2
					&& location.y >= _hightNodes[0]->getPositionY() - _hightNodes[0]->getContentSize().height / 2 && location.y <= _hightNodes[0]->getPositionY() + _hightNodes[0]->getContentSize().height / 2)
					|| (location.x >= _hightNodes[1]->getPositionX() - _hightNodes[1]->getContentSize().width / 2 && location.x <= _hightNodes[1]->getPositionX() + _hightNodes[1]->getContentSize().width / 2
					&& location.y >= _hightNodes[1]->getPositionY() - _hightNodes[1]->getContentSize().height / 2 && location.y <= _hightNodes[1]->getPositionY() + _hightNodes[1]->getContentSize().height / 2))
				{
					for (unsigned int i = 0; i < _hightNodes.size(); i++)
					{
						_hightNodes[i]->setLocalZOrder(NEWERLAYERZOER + 1 + i);
					}
					g_gameScene->onTouchMoved(touch, event);
				}
			}
		}
	};
	
	listener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		if (!iscanclick)
			return;
		if (_step == 25)
		{
			Point location = Director::getInstance()->convertToGL(touch->getLocationInView());
			if (_hightNodes.size() >= 2)
			{
				if ((location.x >= _hightNodes[0]->getPositionX() - _hightNodes[0]->getContentSize().width / 2 && location.x <= _hightNodes[0]->getPositionX() + _hightNodes[0]->getContentSize().width / 2
					&& location.y >= _hightNodes[0]->getPositionY() - _hightNodes[0]->getContentSize().height / 2 && location.y <= _hightNodes[0]->getPositionY() + _hightNodes[0]->getContentSize().height / 2)
					|| (location.x >= _hightNodes[1]->getPositionX() - _hightNodes[1]->getContentSize().width / 2 && location.x <= _hightNodes[1]->getPositionX() + _hightNodes[1]->getContentSize().width / 2
					&& location.y >= _hightNodes[1]->getPositionY() - _hightNodes[1]->getContentSize().height / 2 && location.y <= _hightNodes[1]->getPositionY() + _hightNodes[1]->getContentSize().height / 2))
				{
					for (unsigned int i = 0; i < _hightNodes.size(); i++)
					{
						_hightNodes[i]->setLocalZOrder(NEWERLAYERZOER + 1 + i);
					}
					g_gameScene->onTouchEnded(touch, event);
				}
			}
		}
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
    return true;
}

void NewerGuideLayer::canClick(float dt)
{
	iscanclick = true;
}

void NewerGuideLayer::Step5CB()
{
	GlobalData::setGuide(5, false);
	_sj->setVisible(false);
	scheduleOnce(schedule_selector(NewerGuideLayer::showNextSetp), 0.7f);
}

void NewerGuideLayer::showGamingGuide()
{
	_step = 5;
	isCanClick = false;
	guidetextbox->setPosition(Vec2(320, 200));
	heroGuider->setPosition(Vec2(guidetextbox->getPositionX() - guidetextbox->getContentSize().width / 2, guidetextbox->getPositionY() + guidetextbox->getContentSize().height / 2 + 20));
	_blockcontent = Sprite::createWithSpriteFrameName("guidegameblocks.png");
	_blockcontent->setPosition(Vec2(270, 1080));
	addChild(_blockcontent);
	std::string desc = CommonFuncs::gbk2utf(StringUtils::format("%s", descText[_step].c_str()).c_str());
	_guideText->setString(desc);
	_blockcontent->runAction(Sequence::create(DelayTime::create(0.5), EaseSineIn::create(MoveTo::create(0.5f, Vec2(270, 520))), CallFunc::create(CC_CALLBACK_0(NewerGuideLayer::Step5CB, this)), NULL));

}
void NewerGuideLayer::showNextSetp(float dt)
{
	if (_step == 5)
	{
		LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 100), _blockcontent->getContentSize().width, _blockcontent->getContentSize().height);
		if (_blockcontent != NULL)
			_blockcontent->addChild(color);
		
		Sprite* block1 = Sprite::createWithSpriteFrameName("16.png");
		block1->setPosition(Vec2(110, 610));
		addChild(block1, 0, "1");

		Sprite* block2 = Sprite::createWithSpriteFrameName("16.png");
		block2->setPosition(Vec2(165, 610));
		addChild(block2, 0, "2");

		Sprite* line1 = Sprite::createWithSpriteFrameName("guideline1.png");
		line1->setPosition(Vec2(400, 572));
		addChild(line1, 0, "3");

		Sprite* block3 = Sprite::createWithSpriteFrameName("1.png");
		block3->setPosition(Vec2(375, 605));
		addChild(block3, 0, "4");

		Sprite* block4 = Sprite::createWithSpriteFrameName("1.png");
		block4->setPosition(Vec2(430, 550));
		addChild(block4, 0, "5");

		Sprite* line2 = Sprite::createWithSpriteFrameName("guideline2.png");
		line2->setPosition(Vec2(70, 498));
		addChild(line2, 0, "6");

		Sprite* block5 = Sprite::createWithSpriteFrameName("14.png");
		block5->setPosition(Vec2(110, 550));
		addChild(block5, 0, "7");

		Sprite* block6 = Sprite::createWithSpriteFrameName("14.png");
		block6->setPosition(Vec2(110, 435));
		addChild(block6, 0, "8");
	}
	else if (_step == 8)
	{
		std::string str;
		for (int i = 0; i <= 8; i++)
		{
			str = StringUtils::format("%d", i);
			this->removeChildByName(str);
		}
		Sprite* line = Sprite::createWithSpriteFrameName("guideline3.png");
		line->setPosition(Vec2(375, 593));
		addChild(line, 0, "11");

		Sprite* block1 = Sprite::createWithSpriteFrameName("12.png");
		block1->setPosition(Vec2(217, 610));
		addChild(block1, 0 , "9");
		Sprite* block2 = Sprite::createWithSpriteFrameName("12.png");
		block2->setPosition(Vec2(432, 490));
		addChild(block2, 0, "10");
	}
	else if (_step == 9)
	{
		if (_blockcontent != NULL)
		{
			_blockcontent->setVisible(false);
			this->removeChildByName("9");
			this->removeChildByName("10");
			this->removeChildByName("11");
		}
	}

	_sj->setVisible(true);
	_sj->setPosition(Vec2(270, 300));

	if (_step == 5)
		_step = 8;
	else if (_step == 8 || _step == 9)
		_step++;
	std::string desc = CommonFuncs::gbk2utf(StringUtils::format("%s", descText[_step].c_str()).c_str());
	_guideText->setString(desc); //guidetext.fnt

	if (_step == 10)
	{
		std::string desc1 = CommonFuncs::gbk2utf("                             滑动手指连接两个\n相同图形也可以进行消除哦。");
		Label* guideText1 = Label::createWithBMFont("fonts/guidetext.fnt", desc1);
		guideText1->setAnchorPoint(Vec2(0, 0.5f));
		guideText1->setAlignment(TextHAlignment::LEFT);
		guideText1->setColor(Color3B(255, 0, 0));
		guideText1->setPosition(Vec2(_guideText->getPositionX(), guidetextbox->getContentSize().height / 2));
		guidetextbox->addChild(guideText1);
		_guideText->setPositionY(_guideText->getPositionY() + 13);
	}
}

void NewerGuideLayer::setData(std::vector<Node*> hiligt)
{
	_sj = Sprite::createWithSpriteFrameName("guidesj.png");
	if (hiligt.size() > 0)
	{
		for (unsigned int i = 0; i < hiligt.size(); i++)
		{

			_hightNodes.push_back(hiligt[i]);
			_Ozoder.push_back(hiligt[i]->getLocalZOrder());
			//_clipNode->setStencil(hiligt[i]);
			hiligt[i]->setLocalZOrder(NEWERLAYERZOER + 1 + i);
			_sj->setPosition(Vec2(hiligt[0]->getPositionX() + hiligt[0]->getContentSize().width / 2, hiligt[0]->getPositionY() - hiligt[0]->getContentSize().height / 2));

		}

	}
	else
	{
		if (_step == 0 || _step == 3 || _step == 11 || _step == 26 || _step == 31)
			_sj->setPosition(270, 300);
		else
			_sj->setPosition(270, 480);


	}
	if (_step == 32)
	{
		_sj->setPosition(200, 280);
	}
	if (_step != 25)
		_sj->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.8f, 20), RotateTo::create(0.8f, -20), NULL)));
	else
	{
		_sj->setPosition(_hightNodes[1]->getPosition());
		_sj->runAction(RepeatForever::create(Sequence::create(MoveTo::create(1.5f, _hightNodes[0]->getPosition()), MoveTo::create(1.5f, _hightNodes[1]->getPosition()), NULL)));
	}
	if (_step == 1 || _step == 46)
	{
		_sj->setVisible(false);
	}
	this->getParent()->addChild(_sj, NEWERLAYERZOER + 100, "newersj");
}

void NewerGuideLayer::removeSelf()
{
	iscanclick = false;
	if (_Ozoder.size() > 0)
	{
		for (unsigned int i = 0; i < _hightNodes.size(); i++)
		{
			_hightNodes[i]->setLocalZOrder(_Ozoder[i]);
		}
	}

	GlobalData::setGuide(_step, false);

	if (_step == 25)
		GlobalData::setIsShowNewerGuidePromp(false);

	this->getParent()->removeChildByName("newersj");
	this->removeFromParentAndCleanup(true);
	g_NewerLayer = NULL;
}

int NewerGuideLayer::getType()
{
	return _step;
}