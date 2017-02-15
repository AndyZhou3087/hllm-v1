#include "StartScene.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "cocostudio/CocoStudio.h"
#include "ThemeScene.h"
#include "CommonLayer.h"
#include "LotteryLayer.h"
#include "FourTypeGift.h"
#include "ShopLayer.h"
#include "CommonFuncs.h"
#include "SettingLayer.h"
#include "tinyxml2/tinyxml2.h"
#include "HttpUtil.h"
#include "WaitingProgress.h"
#include "ModifyNameLayer.h"
#include "HintBox.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif
bool StartScene::isToThemeScene = false;
StartScene* g_StartScene = NULL;
StartScene::StartScene()
{
	g_StartScene = NULL;
}

StartScene::~StartScene()
{
	g_StartScene = NULL;
}


Scene* StartScene::scene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	g_StartScene = StartScene::create();
	// add layer as a child to scene
	scene->addChild(g_StartScene);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	Sprite* _bg = Sprite::create("startbackground.png");
	_bg->setAnchorPoint(Vec2(0, 0));
	_bg->setPosition(Vec2(0, 0));
	addChild(_bg);

	root = CSLoader::createNode("StartLayer.csb");
	addChild(root);

	Sprite* logo = Sprite::create("jzjm_logo.png");
	logo->setPosition(Vec2(270, 820));
	addChild(logo);

	m_startBtn = (cocos2d::ui::Widget*)root->getChildByName("startBtn");
	m_startBtn->setTag(LOGIN_NONE);
	m_startBtn->addTouchEventListener(CC_CALLBACK_2(StartScene::onLogin, this));

	cocos2d::ui::Widget* m_LotteryEverydayBtn = (cocos2d::ui::Widget*)root->getChildByName("LotteryEverydayBtn");
	m_LotteryEverydayBtn->addTouchEventListener(CC_CALLBACK_2(StartScene::LotteryEveryday, this));

	m_qqloginbtn = (cocos2d::ui::Widget*)root->getChildByName("loginqqbtn");
	m_qqloginbtn->setTag(LOGIN_QQ);
	m_qqloginbtn->addTouchEventListener(CC_CALLBACK_2(StartScene::onLogin, this));

	m_wxloginbtn = (cocos2d::ui::Widget*)root->getChildByName("loginwxbtn");
	m_wxloginbtn->setTag(LOGIN_WX);
	m_wxloginbtn->addTouchEventListener(CC_CALLBACK_2(StartScene::onLogin, this));

#ifndef THIRDLOGIN
	m_qqloginbtn->setVisible(false);
	m_wxloginbtn->setVisible(false);
#endif
	SoundManager::sharedManager()->playBackMusic("sound/bg.mp3");

	int logintype = UserDefault::getInstance()->getIntegerForKey("logintype", 0);
	if (logintype == LOGIN_QQ)
	{
		SettingLayer::setChangeQQlogin = true;
		m_startBtn->setEnabled(false);
		m_qqloginbtn->setEnabled(false);
		m_wxloginbtn->setEnabled(false);
	}
	else if (logintype == LOGIN_WX)
	{
		SettingLayer::setChangeWXlogin = true;
		m_startBtn->setEnabled(false);
		m_qqloginbtn->setEnabled(false);
		m_wxloginbtn->setEnabled(false);
	}
	return true;
}

void StartScene::onExit()
{
	sendUserAnalyticsData();
	Layer::onExit();
}

void StartScene::setLoginData()
{
	int syssec = GlobalData::getSystemSecTime();
	int lastloginzerotime = GlobalData::getLastLoginRewardDaySecond();
	if (syssec - lastloginzerotime >= 24 * 60 * 60)
	{
		GlobalData::setIsGetLoginReward(false);
	}
}

void StartScene::onEnterTransitionDidFinish()
{
	if (SettingLayer::setChangeQQlogin)
	{
		SettingLayer::setChangeQQlogin = false;
		login(LOGIN_QQ);
	}
	else if (SettingLayer::setChangeWXlogin)
	{
		SettingLayer::setChangeWXlogin = false;
		login(LOGIN_WX);
	}

	GlobalData::setPlayingStage(-1);
	if (UserDefault::getInstance()->getBoolForKey("firststart", true))
	{
		UserDefault::getInstance()->setBoolForKey("firststart", false);

#ifdef ANALYTICS
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", "enterstart");
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
#endif
	}

}


void StartScene::LotteryEveryday(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		LotteryLayer* p = LotteryLayer::create(true, 0);
		addChild(p);
	}
}

void StartScene::login(int type)
{
	logintype = type;
	GlobalData::setLoginType(type);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/hllm", "login", "(I)V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, type);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	string headurl = "";
	string locid = "loc56701";
	if (type > 0)
	{
		headurl = "http://q.qlogo.cn/qqapp/1105233824/EF95C27003021EBA5F65B86F205948EE/40";
		if (type == LOGIN_QQ)
			locid = "EF95C27003021EBA5F65B86F205948EE";
		else
			locid = "w1234567";
	}
	getUserData(locid, "", headurl, "win32", "com.kuxx.hllm");
#endif

	m_startBtn->setEnabled(true);
	m_qqloginbtn->setEnabled(true);
	m_wxloginbtn->setEnabled(true);
}

void StartScene::onLogin(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	cocos2d::ui::Widget* nodebtn = (cocos2d::ui::Widget*)pSender;
	int tag = nodebtn->getTag();

	CommonLayer::Action(pSender, type);

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		login(tag);
	}
}

std::string StartScene::getUpgradeTime()
{
	string rfilename = StringUtils::format("%s/%s", GlobalData::getBasePath().c_str(), UPGRADETIMEFILE);
	FILE* rfile = fopen(rfilename.c_str(), "r");
	char buf[32];
	int len = 0;
	string result;
	if (rfile)
	{
		fseek(rfile, 0, SEEK_END);
		len = ftell(rfile);
		rewind(rfile);
		int rLen = fread(buf, sizeof(char), len, rfile);
		buf[rLen] = '\0';
		fclose(rfile);
		result = buf;
	}
	return result;
}

void StartScene::getUserData(string localid, string nickname, string headurl, string channelid, string pkgname)
{
	WaitingProgress* waitbox = WaitingProgress::create("数据加载中...");
	g_StartScene->addChild(waitbox, 10000, "waitbox");
	std::string nameutf8str;
	const char* retstr = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "gbkToUTF8", "(Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(nickname.c_str());
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, para1);

		retstr = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
	nameutf8str = retstr;
#else
	nameutf8str = CommonFuncs::gbk2utf(nickname.c_str());
#endif
	string url;
	url.append(HTTPURL);
	url.append("llk_login");
	url.append("&package_name=");
	url.append(pkgname);
	url.append("&playerid=");
	url.append(localid);
	url.append("&cid=");
	url.append(channelid);
	url.append("&nickname=");
	url.append(nameutf8str);
	url.append("&headportrait=");
	url.append(headurl);
	url.append("&vercode=");
	std::string vercodestr = StringUtils::format("%d", getVersionCode());
	url.append(vercodestr);
	url.append("&upgradetime=");
	url.append(getUpgradeTime());
	/*取自己数据*/
	HttpUtil::getInstance()->doData(url, httpCompleteCB, "", HTTPTYPE::GET, "", headurl);
}

void StartScene::httpCompleteCB(std::string retdata, int code, std::string tag)
{
	//log("retdata = %s", retdata.c_str());
	rapidjson::Document doc;

	bool isdataok = true;

	if (code == 0)
	{
		doc.Parse<0>(retdata.c_str());
		if (!doc.HasParseError())  //打印解析错误
		{
			if (doc.IsObject() && doc.HasMember("data"))
			{
				rapidjson::Value& objectData = doc["data"];

				int mytotallv = 0;
				std::string skill = "";
				int coin = 0;
				int gem = 0;
				int hpcount = 25;
				int hpmax = 25;
				int prompt = 0;
				int refresh = 0;
				int addtime = 0;
				std::string stage = "";
				int bestscore = 0;
				int bossbestscore = 0;
				int bossbestcount = 0;
				int cotinuelogindays = 0;
				int maxstage = -1;
				int lasttakeawardtime = 0;
				std::string achivestr = "";

				if (objectData.HasMember("id"))
				{
					rapidjson::Value& data = objectData["id"];
					string idstr = StringUtils::format("%d", data.GetInt());
					GlobalData::setMyID(idstr);
					GlobalData::init();
					//std::string mynamestr = objectData["nickname"].GetString();
					//if (GlobalData::getMyName().length() <= 0)
					//	GlobalData::setMyName(mynamestr);
					GlobalData::setHeadImg(tag);
				}

				if (objectData.HasMember("level"))
				{
					rapidjson::Value& data = objectData["level"];
					mytotallv = data.GetInt();
					if (GlobalData::getMyTotalSkillLevel()  <= 0)
						GlobalData::setMyTotalSkillLvel(mytotallv);
				}

				if (objectData.HasMember("skill_level"))
				{
					rapidjson::Value& data = objectData["skill_level"];
					skill = data.GetString();
				}
				if (skill.length() <= 0)
				{
					skill = "0,0,0,0,0,0";
				}
				vector<string> tmp1;
				CommonFuncs::split(skill, tmp1, ",");
				for (unsigned int i = 0; i < tmp1.size(); i++)
				{
					if (GlobalData::getMySkillLevel(i) <= 0)
						GlobalData::setMySkilLevel(i, atoi(tmp1[i].c_str()));
				}
				if (objectData.HasMember("coin"))
				{
					rapidjson::Value& data = objectData["coin"];
					coin = data.GetInt();
					if (GlobalData::getGoldCount() <= 0)
						GlobalData::setGoldCount(coin);
				}
				if (objectData.HasMember("diamond"))
				{
					rapidjson::Value& data = objectData["diamond"];
					gem = data.GetInt();
					if (GlobalData::getGemCount() <= 0)
						GlobalData::setGemCount(gem);
				}
				if (objectData.HasMember("life"))
				{
					rapidjson::Value& data = objectData["life"];
					hpcount = data.GetInt();
				}

				if (objectData.HasMember("lifeupperlimit"))
				{
					rapidjson::Value& data = objectData["lifeupperlimit"];
					hpmax = data.GetInt();
				}
				if (hpmax == 0)
				{
					hpmax = 25;
					hpcount = 25;
					GlobalData::setHpCount(hpcount);
					GlobalData::setMyMaxHp(hpmax);
				}
				if (hpmax > GlobalData::getMyMaxHp())
				{
					GlobalData::setMyMaxHp(hpmax);
				}

				if (objectData.HasMember("find"))
				{
					rapidjson::Value& data = objectData["find"];
					prompt = data.GetInt();
					if (GlobalData::getPromptProp() <= 0)
						GlobalData::setPromptProp(prompt);
				}
				if (objectData.HasMember("reset"))
				{
					rapidjson::Value& data = objectData["reset"];
					refresh = data.GetInt();
					if (GlobalData::getRefreshProp() <= 0)
						GlobalData::setRefreshProp(refresh);
				}
				if (objectData.HasMember("addtime"))
				{
					rapidjson::Value& data = objectData["addtime"];
					addtime = data.GetInt();
					if (GlobalData::getAddtimeProp() <= 0)
						GlobalData::setAddtimeProp(addtime);
				}
				if (objectData.HasMember("finished_pass"))
				{
					rapidjson::Value& data = objectData["finished_pass"];
					stage = data.GetString();
				}

				vector<string> stagestrvec;

				for (int i = 0; i < GlobalData::getMapCount(); i++)
				{
					if (stage.length() < 50)
						stagestrvec.push_back("000000");
					else
						stagestrvec.push_back(stage.substr(i * 5, 5));
					std::string chapter = StringUtils::format("page%dstar", i);
					long pageStageStar = strtol(stagestrvec[i].c_str(), NULL, 16);

					std::string stars = StringUtils::format("%ld", pageStageStar);

					std::string starsStr = GlobalData::getChapterStageStar(chapter.c_str());
					long long curPageStageStar = atoll(starsStr.c_str());
					if (curPageStageStar <= 0)
						GlobalData::setChapterStageStar(chapter.c_str(), stars.c_str());
				}
				if (objectData.HasMember("bestscore"))
				{
					rapidjson::Value& data = objectData["bestscore"];
					bestscore = data.GetInt();
					if (GlobalData::getBestCMScore(CMNORMAL) <= 0)
						GlobalData::setBestCMScore(CMNORMAL, bestscore);
				}
				if (objectData.HasMember("bossstatus"))
				{
					rapidjson::Value& data = objectData["bossstatus"];
					bossbestcount = data.GetInt();
					if (GlobalData::getBestDefeatedBossCount() <= 0)
						GlobalData::setBestDefeatedBossCount(bossbestcount);
				}
				if (objectData.HasMember("bossbestscore"))
				{
					rapidjson::Value& data = objectData["bossbestscore"];
					bossbestscore = data.GetInt();
					if (GlobalData::getBestCMScore(CMBOSS))
						GlobalData::setBestCMScore(CMBOSS, bossbestscore);
				}

				if (objectData.HasMember("higheststage"))
				{
					rapidjson::Value& data = objectData["higheststage"];
					maxstage = data.GetInt();
					if (GlobalData::getHighestStage() <= -1)
						GlobalData::setHighestStage(maxstage);
				}
				if (objectData.HasMember("achievement"))
				{
					rapidjson::Value& data = objectData["achievement"];
					achivestr = data.GetString();
				}

				if (objectData.HasMember("buy"))
				{
					rapidjson::Value& data = objectData["buy"];
				}
				if (objectData.HasMember("url"))
				{
					rapidjson::Value& data = objectData["url"];
					GlobalData::setShareUrl(data.GetString());
				}
				if (objectData.HasMember("classicscore"))
				{
					rapidjson::Value& data = objectData["classicscore"];
					if (GlobalData::getBestCMScore(CMCLASIC) <= 0)
						GlobalData::setBestCMScore(CMCLASIC, data.GetInt());
				}
				if (objectData.HasMember("flashscore"))
				{
					rapidjson::Value& data = objectData["flashscore"];
					if (GlobalData::getBestCMScore(CMFLASH) <= 0)
						GlobalData::setBestCMScore(CMFLASH, data.GetInt());
				}
				if (objectData.HasMember("upgradeurl"))
				{
					rapidjson::Value& data = objectData["upgradeurl"];
					GlobalData::setUpgradeUrl(data.GetString());
				}
				/*
				if (objectData.HasMember("login_days"))
				{
					rapidjson::Value& data = objectData["login_days"];
					cotinuelogindays = data.GetInt();
				}
				if (objectData.HasMember("lasttakeawardtime"))
				{
					rapidjson::Value& data = objectData["lasttakeawardtime"];
					lasttakeawardtime = data.GetInt();
				}
				GlobalData::setContinueLoginDay(cotinuelogindays);
				GlobalData::setLastLoginRewardSecond(lasttakeawardtime);
				*/
				string hexstr;
				int achieveSize = GlobalData::achieveData.size();
				int strsize = achivestr.size();
				if (strsize < achieveSize * 3)
				{
					achivestr.clear();

					for (int i = 0; i < achieveSize; i++)
					{
						achivestr.append("000");
					}
				}

				for (int i = 0; i < achieveSize; i++)
				{
					hexstr = achivestr.substr(i * 3, 3);

					if (strcmp(hexstr.c_str(), "fff") == 0)
						GlobalData::achieveData[i].finish = -1;
					else
						GlobalData::achieveData[i].finish = (int)strtol(hexstr.c_str(), NULL, 16);
				}
			}
		}
		else
		{
			//log("GetParseError %s\n", doc.GetParseError());
			isdataok = false;
		}
	}
	else
	{
		if (GlobalData::getMyID().length() <= 0)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			JniMethodInfo methodInfo;
			const char* str = "";
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "UUID", "()Ljava/lang/String;"))
			{
				jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
				str = methodInfo.env->GetStringUTFChars(jstr, 0);
			}
			GlobalData::setMyID(str);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

			if (GlobalData::getLoginType() == LOGIN_NONE)
			{
				GlobalData::setMyID("loc123");
			}
			else if (GlobalData::getLoginType() == LOGIN_QQ)
			{
				GlobalData::setMyID("q123");
			}
			else if (GlobalData::getLoginType() == LOGIN_WX)
			{
				GlobalData::setMyID("w123");
			}
#endif
		}
		else
		{
			GlobalData::setMyID(GlobalData::getMyID());
		}

		isdataok = false;
	}


	if (!isdataok)
	{
		GlobalData::setLoginType(LOGIN_NONE);

		HintBox* hint = HintBox::create("获取数据失败，转为游客登录！", 0);
		Director::getInstance()->getRunningScene()->addChild(hint, 1000);
	}
	/*取自己头像*/
	int logintype = GlobalData::getLoginType();
	if (logintype > LOGIN_QQ)
	{
		string headimgname = "qhead00.png";
		if (logintype == LOGIN_WX)
			headimgname = "wxhead00.png";

		HttpUtil::getInstance()->doData(GlobalData::getHeadImg(), NULL, headimgname);
	}
	//g_StartScene->scheduleOnce(schedule_selector(StartScene::loadUserData), 0.05f);
	g_StartScene->scheduleOnce(schedule_selector(StartScene::getMessageData), 0.05f);
}

void StartScene::loadUserData(float dt)
{
	getAchieveData();
	getTaskData();
	GlobalData::init();
	setLoginData();
	GlobalData::setTotalLoginDay(GlobalData::getTotalLoginDay() + 1);
	GlobalData::setIsHomeAutoPopGift(true);
	isToThemeScene = true;
	m_startBtn->setEnabled(false);
	m_qqloginbtn->setEnabled(false);
	m_wxloginbtn->setEnabled(false);

	//兼容前几个版本，最大关卡：10001 start
	int maxstage = GlobalData::getHighestStage();
	int chapter = maxstage / 10000;
	int stage = maxstage % 10000;
	//兼容前几个版本，最大关卡：10001 end
	GlobalData::setHighestStage(chapter * 10 + stage);

	int level = GlobalData::getMyTotalSkillLevel();
#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "setPlayerLevel", "(I)V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, level);
	}
#endif


	string myname = GlobalData::getMyName();
	if (myname.compare("") == 0)
	{
		myname = StringUtils::format("游客%d", GlobalData::getSystemSecTime());
		GlobalData::setMyName(CommonFuncs::gbk2utf(myname.c_str()));
	}
	Director::getInstance()->replaceScene(ThemeScene::Scene());

}

void StartScene::getAchieveData()
{
	std::string achieveStr = GlobalData::getAchieveData();
	if (achieveStr.length() > 0)//读保存的成就数据格式：1,2,3,;4,5,6;7,8,9;10,11,12;
	{
		vector<string> tmp;
		CommonFuncs::split(achieveStr, tmp, ";");

		for (unsigned int i = 0; i < tmp.size(); i++)
		{
			vector<string> data;
			CommonFuncs::split(tmp[i], data, ",");

			int id = atoi(data[0].c_str());

			for (unsigned int m = 0; m < GlobalData::achieveData.size(); m++)
			{
				if (id == GlobalData::achieveData[m].id)
				{
					GlobalData::achieveData[m].finish = atoi(data[7].c_str());
					GlobalData::achieveData[m].datetime = atoi(data[8].c_str());
					break;
				}
			}
		}
	}
}

void StartScene::getTaskData()
{
	parseTaskConfig(GlobalData::getXMLDataPath("data/task.xml"));

	int dayofyear = GlobalData::getDayOfYear();
	if (GlobalData::getLastLoginDayOfYear() != dayofyear)
	{
		GlobalData::clearTaskData();
		GlobalData::setLastLoginDayOfYear(dayofyear);
	}

	std::string taskStr = GlobalData::getTaskData();
	if (taskStr.length() > 0)//读保存的成就数据格式：1,2,3,;4,5,6;7,8,9;10,11,12;
	{
		vector<string> tmp;
		CommonFuncs::split(taskStr, tmp, ";");

		for (unsigned int i = 0; i < tmp.size(); i++)
		{
			vector<string> data;
			CommonFuncs::split(tmp[i], data, ",");

			int id = atoi(data[0].c_str());

			for (unsigned int m = 0; m < GlobalData::taskData.size(); m++)
			{
				if (id == GlobalData::taskData[m].id)
				{
					GlobalData::taskData[m].finish = atoi(data[7].c_str());
					GlobalData::taskData[m].datetime = atoi(data[8].c_str());
					break;
				}
			}
		}
	}
}

void StartScene::getMessageData(float dt)
{
	string url;
	url.append(HTTPURL);
	url.append("llk_MessageList");
	url.append("&id=");
	url.append(GlobalData::getMyID());
	url.append("&vercode=");
	std::string vercodestr = StringUtils::format("%d", StartScene::getVersionCode());
	url.append(vercodestr);
	url.append("&cid=");
	url.append(GlobalData::getCid());
	HttpUtil::getInstance()->doData(url, httpMessageDataCompleteCB);
}

void StartScene::httpMessageDataCompleteCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		parseMessageJson(retdata);
	}
	g_StartScene->scheduleOnce(schedule_selector(StartScene::getEventData), 0.02f);
}

void StartScene::getEventData(float dt)
{
	std::string url;
	url.append(HTTPURL);
	url.append("llk_EventList");
	url.append("&vercode=");
	std::string vercodestr = StringUtils::format("%d", StartScene::getVersionCode());
	url.append(vercodestr);
	url.append("&cid=");
	url.append(GlobalData::getCid());
	HttpUtil::getInstance()->doData(url, httpEventDataCompleteCB);
}

void StartScene::httpEventDataCompleteCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		parseEventJson(retdata);
	}

	g_StartScene->scheduleOnce(schedule_selector(StartScene::loadUserData), 0.05f);
}

void StartScene::parseEventJson(std::string retdata)
{
	rapidjson::Document doc;
	doc.Parse<0>(retdata.c_str());
	if (!doc.HasParseError())  //打印解析错误
	{
		if (doc.IsObject() && doc.HasMember("data"))
		{
			rapidjson::Value& objectData = doc["data"];
			if (objectData.HasMember("events"))
			{
				rapidjson::Value& Data = objectData["events"];
				if (Data.IsArray())
				{
					for (unsigned int i = 0; i < GlobalData::EventsData.size(); i++)
					{
						GlobalData::EventsData.clear();
					}

					MEvent mdata;
					for (unsigned int i = 0; i < Data.Size(); i++)
					{
						rapidjson::Value& mevent = Data[i];
						rapidjson::Value& valuedata = mevent["type"];
						int type = valuedata.GetInt();
						mdata.type = type;

						valuedata = mevent["id"];
						mdata.id = valuedata.GetInt();

						valuedata = mevent["title"];
						strcpy(mdata.title, valuedata.GetString());

						valuedata = mevent["rule"];
						strcpy(mdata.rulestr, valuedata.GetString());

						valuedata = mevent["perioddesc"];
						strcpy(mdata.timestr, valuedata.GetString());

						if (mevent.HasMember("data1"))
						{
							valuedata = mevent["data1"];
							strcpy(mdata.data, valuedata.GetString());
						}
						if (mevent.HasMember("starttime"))
						{
							valuedata = mevent["starttime"];
							mdata.startime = valuedata.GetInt();
						}

						if (mevent.HasMember("endtime"))
						{
							valuedata = mevent["endtime"];
							mdata.endtime = valuedata.GetInt();
						}

						valuedata = mevent["awardrule"];
						string propstr = valuedata.GetString();
						vector<string> tmp1;
						CommonFuncs::split(propstr, tmp1, ";");

						int count = tmp1.size() > 3 ? 3 : tmp1.size();
						
						for (int m = 0; m < count; m++)
						{
							vector<string> tmp2;
							CommonFuncs::split(tmp1[m], tmp2, "-");
							if (tmp2.size() >= 2)
							{
								mdata.propid[m] = atoi(tmp2[0].c_str());
								mdata.propnum[m] = atoi(tmp2[1].c_str());
							}
						}

						if (type <= MAXEVENTTYPE)
							GlobalData::EventsData.push_back(mdata);
					}
				}
			}
		}
	}
}

void StartScene::parseMessageJson(std::string retdata)
{
	rapidjson::Document doc;
	doc.Parse<0>(retdata.c_str());
	if (!doc.HasParseError())  //打印解析错误
	{
		if (doc.IsObject() && doc.HasMember("data"))
		{
			rapidjson::Value& objectData = doc["data"];
			if (objectData.HasMember("msgs"))
			{
				rapidjson::Value& Data = objectData["msgs"];
				if (Data.IsArray())
				{
					for (unsigned int i = 0; i < GlobalData::mapMessageData.size(); i++)
					{
						GlobalData::mapMessageData[i].clear();
					}

					Message msgdata;
					for (unsigned int i = 0; i < Data.Size(); i++)
					{
						rapidjson::Value& message = Data[i];
						rapidjson::Value& valuedata = message["type"];
						int type = valuedata.GetInt();
						msgdata.type = type;

						valuedata = message["mid"];
						msgdata.id = valuedata.GetInt();

						valuedata = message["title"];
						strcpy(msgdata.title, valuedata.GetString());

						valuedata = message["content"];
						strcpy(msgdata.content, valuedata.GetString());

						valuedata = message["prize"];
						strcpy(msgdata.prize, valuedata.GetString());

						valuedata = message["status"];
						msgdata.status = valuedata.GetInt();

						valuedata = message["time"];
						msgdata.datetime = valuedata.GetInt();

						if (type == 0)
							GlobalData::mapMessageData[type].push_back(msgdata);
						else
							GlobalData::mapMessageData[type - 1].push_back(msgdata);
					}
				}
			}
		}
	}
}

void StartScene::parseTaskConfig(std::string filepath)
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	unsigned char pBuffer[5000];
	memset(pBuffer, 0x00, sizeof(pBuffer));
	strcpy((char *)pBuffer, FileUtils::getInstance()->getStringFromFile(filepath).c_str());
	int err = pDoc->Parse((const char*)pBuffer);
	if (err != 0)
	{
		delete pDoc;
		return;
	}
	tinyxml2::XMLElement *rootEle = pDoc->RootElement();

	tinyxml2::XMLElement *element = rootEle->FirstChildElement();
	while (element != NULL)
	{
		if (strcmp(element->Name(), "s") == 0)
		{
			Achieve taskData;
			taskData.id = element->IntAttribute("id");
			strcpy(taskData.name, element->Attribute("name"));
			strcpy(taskData.desc, element->Attribute("des"));
			taskData.count = element->IntAttribute("count");
			taskData.achType = element->IntAttribute("ex");
			taskData.rwdtype = element->IntAttribute("rtype");
			taskData.rcount = element->IntAttribute("rcount");
			taskData.finish = 0;
			taskData.datetime = 0;
			GlobalData::taskData.push_back(taskData);
		}
		element = element->NextSiblingElement();
	}
	delete pDoc;
}

int StartScene::getVersionCode()
{
	int vercode = 110;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "getVersionCode", "()I"))
	{
		vercode = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
	}
#endif
	return vercode;
}

void StartScene::sendUserAnalyticsData()
{
	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
	string idstr = StringUtils::format("%s", GlobalData::getMyID().c_str());
	writedoc.AddMember("uid", idstr.c_str(), allocator);

	const char* macstr = "123456";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "MACID", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		macstr = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
#endif
	writedoc.AddMember("mac", macstr, allocator);

	string logintypestr = StringUtils::format("%d", logintype);
	writedoc.AddMember("logintype", logintypestr.c_str(), allocator);
	
	int logindays = GlobalData::getContinueLoginDay();

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

	string logindaystr = StringUtils::format("%d", count);
	writedoc.AddMember("logindays", logindaystr.c_str(), allocator);

	string hpstr = StringUtils::format("%d", GlobalData::getHpCount());
	writedoc.AddMember("hp", hpstr.c_str(), allocator);

	string coinstr = StringUtils::format("%d", GlobalData::getGoldCount());
	writedoc.AddMember("coin", coinstr.c_str(), allocator);

	string gemstr = StringUtils::format("%d", GlobalData::getGemCount());
	writedoc.AddMember("gem", gemstr.c_str(), allocator);

	string bombstr = StringUtils::format("%d", 0);
	writedoc.AddMember("bomb", bombstr.c_str(), allocator);

	string changestr = StringUtils::format("%d", 0);
	writedoc.AddMember("change", changestr.c_str(), allocator);

	string icestr = StringUtils::format("%d", 0);
	writedoc.AddMember("ice", icestr.c_str(), allocator);

	string promptstr = StringUtils::format("%d", GlobalData::getPromptProp());
	writedoc.AddMember("find", promptstr.c_str(), allocator);

	string refreshstr = StringUtils::format("%d", GlobalData::getRefreshProp());
	writedoc.AddMember("refresh", refreshstr.c_str(), allocator);

	string addtimestr = StringUtils::format("%d", GlobalData::getAddtimeProp());
	writedoc.AddMember("addtime", addtimestr.c_str(), allocator);

	string histagestr = StringUtils::format("%d", GlobalData::getHighestStage() + 1);
	writedoc.AddMember("histage", histagestr.c_str(), allocator);


	int totalstars = 0;
	for (int i = 0; i < GlobalData::getMapCount(); i++)
	{
		std::string chapter = StringUtils::format("page%dstar", i);
		std::string starsStr = GlobalData::getChapterStageStar(chapter.c_str());

		long long cruPageStageStar = atoll(starsStr.c_str());

		for (int m = 0; m < 10; m++)
		{
			totalstars += cruPageStageStar >> (m * 2) & 3;
		}
	}

	string totalstarstr = StringUtils::format("%d", totalstars);
	writedoc.AddMember("totalstars", totalstarstr.c_str(), allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	writedoc.Accept(writer);
	string url;
	url.append(ANALYTICS_HTTPURL);
	url.append("llk_insertplayerdata");
	url.append("&param=");
	url.append(buffer.GetString());
	HttpUtil::getInstance()->doData(url);
}
