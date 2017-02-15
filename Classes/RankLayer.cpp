#include "RankLayer.h"
#include "CommonLayer.h"
#include "SureStageLayer.h"
#include "HttpUtil.h"
int RankLayer::rankMode = CMNORMAL;

Label * RankLayer::desclbl = NULL;
cocos2d::ui::ScrollView* RankLayer::scrollview = NULL;

const char* headimgprefix[] = {"rhead", "brhead", "crhead", "frhead"};
RankLayer::RankLayer()
{
	desclbl = NULL;
	scrollview = NULL;
}


RankLayer::~RankLayer()
{
	desclbl = NULL;
	scrollview = NULL;
	rankMode = CMNORMAL;
}

void RankLayer::onExit()
{
	Layer::onExit();
}

bool RankLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}
	pasttime = 0.0f;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	color->setPosition(Vec2(0, 0));
	this->addChild(color);

	Node * layer = CSLoader::createNode("rankLayer.csb");
	addChild(layer);

	cocos2d::ui::Widget* cmBtn = (cocos2d::ui::Widget*)layer->getChildByName("cmbtn");

	cocos2d::ui::ImageView* ranktitle = (cocos2d::ui::ImageView*)layer->getChildByName("ranktitle");
	if (rankMode == CMBOSS)
	{
		std::string str = StringUtils::format("UI/bossranktitle.png");
		ranktitle->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		ranktitle->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
	}
	else if (rankMode == CMCLASIC || rankMode == CMFLASH)
	{
		std::string titlestr;
		std::string textstr;
		if (rankMode == CMCLASIC)
		{
			titlestr = StringUtils::format("UI/clasicranktitle.png");
			textstr = StringUtils::format("UI/clasictext.png");
		}
		else
		{
			titlestr = StringUtils::format("UI/flashranktitle.png");
			textstr = StringUtils::format("UI/flashtext.png");
		}
		ranktitle->loadTexture(titlestr, cocos2d::ui::TextureResType::PLIST);
		ranktitle->setContentSize(Sprite::createWithSpriteFrameName(titlestr)->getContentSize());
		cocos2d::ui::ImageView* btntext = (cocos2d::ui::ImageView*)cmBtn->getChildByName("text");
		btntext->loadTexture(textstr, cocos2d::ui::TextureResType::PLIST);
		btntext->setContentSize(Sprite::createWithSpriteFrameName(textstr)->getContentSize());
	}
	else
	{
		cmBtn->setVisible(false);
	}
	cmBtn->setTag(rankMode);
	cmBtn->addTouchEventListener(CC_CALLBACK_2(RankLayer::cmbtn, this));

	cocos2d::ui::Widget* closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("closebtn");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(RankLayer::close, this));

	scrollview = (cocos2d::ui::ScrollView*)layer->getChildByName("ScrollView");
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	

	desclbl = Label::createWithSystemFont(CommonFuncs::gbk2utf("数据加载中，请稍等！"), FONT_NAME, 25);
	desclbl->setColor(Color3B(151, 80, 1));
	desclbl->setPosition(Vec2(270, 480));
	layer->addChild(desclbl);

	/*
	else
	{
		for (int i = 0; i < count; i++)
		{
			RankItem* item = RankItem::createWithData(GlobalData::rankData[i], i);
			item->setPosition(Vec2(270, 950 - i * 90));
			std::string str = StringUtils::format("item%d", i);
			scrollview->addChild(item, 0, str);
		}
	}*/
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	GlobalData::rankData.clear();
	this->scheduleOnce(schedule_selector(RankLayer::getRankData), 0.05f);
	return true;
}
void RankLayer::getRankData(float dt)
{
	string url;
	url.append(HTTPURL);
	url.append("llk_ranklist");
	url.append("&id=");
	url.append(GlobalData::getMyID());
	url.append("&type=");
	url.append("0");
	url.append("&boss=");
	string modestr = StringUtils::format("%d", rankMode);
	url.append(modestr);
	url.append("&cid=");
	url.append(GlobalData::getCid());
	HttpUtil::getInstance()->doData(url, httpCompleteCB);
}

void RankLayer::httpCompleteCB(std::string retdata, int code, std::string tag)
{
	rapidjson::Document doc;

	if (code == 0)
	{
		doc.Parse<0>(retdata.c_str());
		if (doc.HasParseError())  //打印解析错误
		{
			//log("GetParseError %s\n", doc.GetParseError());
		}
		else
		{
			int myrank = 0;
			RankData myrankdata;
			if(doc.IsObject() && doc.HasMember("ret"))
			{
				rapidjson::Value& objectData = doc["ret"];
				myrank = objectData.GetInt();
				myrankdata.rank = myrank;
			}
			if (doc.IsObject() && doc.HasMember("data"))
			{
				rapidjson::Value& objectData = doc["data"];
				if (objectData.IsObject() && objectData.HasMember("list"))
				{
					rapidjson::Value& jsonArray = objectData["list"];
					if (jsonArray.IsArray())
					{
						int size = jsonArray.Size();
						for (int i = 0; i < size; i++)
						{
							rapidjson::Value& arraydoc = jsonArray[i];
							if (i == size - 1)
							{
								strcpy(myrankdata.name, GlobalData::getMyName().c_str());
								strcpy(myrankdata.headimg, GlobalData::getHeadImg().c_str());

								rapidjson::Value& datascore = arraydoc["score"];
								myrankdata.score = datascore.GetInt();
							}
							else
							{
								RankData rankdata;
								rankdata.rank = i + 1;
								rapidjson::Value& jsondata = arraydoc["nickname"];
								strcpy(rankdata.name, jsondata.GetString());
								jsondata = arraydoc["headportrait"];
								strcpy(rankdata.headimg, jsondata.GetString());

								jsondata = arraydoc["score"];
								rankdata.score = jsondata.GetInt();
								GlobalData::rankData.push_back(rankdata);
							}
						}
						if (size > 0)
						{
							GlobalData::rankData.insert(GlobalData::rankData.begin(), myrankdata);
						}
					}
				}
			}
		}
	}
	int ranksize = GlobalData::rankData.size();
	if (ranksize > 1)
	{
		for (int i = 1; i < ranksize; i++)
		{
			string headimgname = StringUtils::format("%s%02d.png", headimgprefix[RankLayer::rankMode], i);
			std::string fullpath = FileUtils::getInstance()->getWritablePath() + headimgname;

			bool isHasfile = FileUtils::getInstance()->isFileExist(fullpath);
			if (isHasfile)
				FileUtils::getInstance()->removeFile(fullpath);
		}

		string headname = StringUtils::format("%s%02d.png", headimgprefix[RankLayer::rankMode], 1);
		HttpUtil::getInstance()->doData(GlobalData::rankData[1].headimg, httpDownImgCB, headname, HTTPTYPE::GET, "", "1");
	}
	else
	{
		if (desclbl != NULL)
			desclbl->setString("");
	}
}

void RankLayer::httpDownImgCB(std::string retdata, int code, std::string tag)
{
	int next = atoi(tag.c_str()) + 1;
	int size = GlobalData::rankData.size();
	if (size - 1 >= next)
	{
		string url = GlobalData::rankData[next].headimg;
		string headfilename = StringUtils::format("%s%02d.png", headimgprefix[RankLayer::rankMode], next);
		string tag = StringUtils::format("%d", next);
		HttpUtil::getInstance()->doData(GlobalData::rankData[next].headimg, httpDownImgCB, headfilename, HTTPTYPE::GET, "", tag);
	}
	else
	{
		showRankData();
	}
}

void RankLayer::showRankData()
{
	int count = GlobalData::rankData.size();
	if (count > 0)
	{
		int itemheight = 90;
		int scrollinnerheight = count * itemheight;

		if (scrollview == NULL)
			return;

		int scorellheight = scrollview->getContentSize().height;
		if (scrollinnerheight <= scorellheight)
			scrollinnerheight = scorellheight;
		scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, scrollinnerheight));
		for (int i = 0; i < count; i++)
		{
			RankItem* item = RankItem::createWithData(GlobalData::rankData[i], i);
			item->setPosition(Vec2(270, scrollinnerheight - itemheight / 2 - i * 90));
			std::string str = StringUtils::format("item%d", i);
			if (scrollview != NULL)
				scrollview->addChild(item, 0, str);
		}
		if (desclbl != NULL)
			desclbl->setString("");
	}
	else
	{
		if (desclbl != NULL)
			desclbl->setString(CommonFuncs::gbk2utf("获取数据失败，请检查网络"));
	}
}

void RankLayer::cmbtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btn = (Node*)pSender;
		GlobalData::setChallengeMode((ChallengeMode)btn->getTag());
		Layer* layer = SureStageLayer::create();
		this->addChild(layer);
	}
}

void RankLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

RankItem::RankItem()
{
	head = NULL;
}

RankItem::~RankItem()
{
	head = NULL;
}

RankItem* RankItem::createWithData(RankData data, int index)
{
	RankItem* item = new RankItem;
	item->init(data, index);
	item->autorelease();
	return item;
}

bool RankItem::init(RankData data, int index)
{
	bool ret = false;
	if (index == 0)
	{
		ret = Sprite::initWithSpriteFrameName("UI/myrankitem.png");
		//data.score = GlobalData::getBestCMScore((ChallengeMode)RankLayer::rankMode);
	}
	else
	{
		ret = Sprite::initWithSpriteFrameName("UI/shopgemitem.png");
	}

	if (ret)
	{
		Label * mname = Label::createWithSystemFont(data.name, FONT_NAME, 25);
		mname->setColor(Color3B(151, 80, 1));
		mname->setAnchorPoint(Vec2(0, 0.5));
		mname->setPosition(Vec2(160, 62));
		addChild(mname);

		Label * scoretext = Label::createWithSystemFont(CommonFuncs::gbk2utf("分数"), FONT_NAME, 22);
		scoretext->setColor(Color3B(151, 80, 1));
		scoretext->setAnchorPoint(Vec2(0, 0.5));
		scoretext->setPosition(Vec2(160, 30));
		addChild(scoretext);

		std::string str = StringUtils::format("%d", data.score);
		for (int i = (int)str.length() - 3; i > 0; i -= 3)
			str.insert(i, ",");
		Label * score = Label::createWithBMFont("fonts/amountnum.fnt", str);
		score->setScale(0.8f);
		score->setAnchorPoint(Vec2(0, 0.5));
		score->setPosition(Vec2(scoretext->getPositionX() + scoretext->getContentSize().width + 5, 25));
		addChild(score);

		Sprite* medal;
		std::string medalstr;
		if (index <= 3 && index > 0)
		{
			medalstr = StringUtils::format("UI/medal%d.png", index);
			medal = Sprite::createWithSpriteFrameName(medalstr);
			medal->setPosition(Vec2(medal->getContentSize().width / 2 + 20, this->getContentSize().height / 2));
			this->addChild(medal);
		}
		if (index > 0)
		{
			if (data.score == GlobalData::rankData[index - 1].score)
				data.rank = GlobalData::rankData[index - 1].rank;
		}
		Label* ranklbl;
		std::string rankstr = StringUtils::format("%d", data.rank);
		if (index == 0 && data.rank >= 10000)
		{
			ranklbl = Label::createWithSystemFont(rankstr, FONT_NAME, 22);
			ranklbl->setColor(Color3B(151, 80, 1));
			ranklbl->setPosition(45, this->getContentSize().height / 2);
		}
		else
		{
			ranklbl = Label::createWithBMFont("fonts/loginrewardpropnum.fnt", rankstr);
			ranklbl->setPosition(45, this->getContentSize().height / 2 - 4);
		}
		addChild(ranklbl);
		upataheadimg(index);
	}

	return ret;
}

void RankItem::upataheadimg(int index)
{
	if (head == NULL)
	{
		head = Sprite::createWithSpriteFrameName("UI/settinghead.png");
		head->setScale(0.8f);
		head->setPosition(Vec2(120, this->getContentSize().height / 2 + 4));
		this->addChild(head);
	}

	int logintype = GlobalData::getLoginType();
	std::string headimgname;

	if (index == 0)
	{
		if (logintype > LOGIN_NONE)
		{
			headimgname = StringUtils::format("qhead%02d.png", index);
			if (logintype == LOGIN_WX)
				headimgname = StringUtils::format("wxhead%02d.png", index);
		}
	}
	else
	{
		headimgname = StringUtils::format("%s%02d.png", headimgprefix[RankLayer::rankMode], index);
	}

	std::string path = FileUtils::getInstance()->getWritablePath() + headimgname;

	bool isHasfile = FileUtils::getInstance()->isFileExist(path);
	if (isHasfile)
	{
		Data filedata = FileUtils::getInstance()->getDataFromFile(path.c_str());
		if (!filedata.isNull())
		{
			Image* img = new Image;
			img->initWithImageData(filedata.getBytes(), filedata.getSize());
			Texture2D* texture = new cocos2d::Texture2D();
			bool isImg = texture->initWithImage(img);

			if (isImg)
			{
				head->setTexture(texture);
				int w = img->getWidth();
				int h = img->getHeight();
				head->setTextureRect(Rect(0, 0, w, h));
				head->setScaleX(60.0f / w);
				head->setScaleY(60.0f / h);
				texture->release();
			}
			img->release();
		}

	}

}