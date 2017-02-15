#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class StartScene : public Layer
{
public:

	StartScene();
	~StartScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void onExit();
	virtual void onEnterTransitionDidFinish();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    // implement the "static node()" method manually
	static cocos2d::Scene* scene();
	CREATE_FUNC(StartScene);
	void setLoginData();
	static bool isToThemeScene;
	static void getUserData(std::string localid, std::string nickname, std::string headurl, std::string channelid, std::string pkgname);
	void getMessageData(float dt);
	static void parseMessageJson(std::string retdata);

	void getEventData(float dt);
	static void parseEventJson(std::string retdata);

	static int getVersionCode();
	void sendUserAnalyticsData();
private:
	cocos2d::ui::Widget* m_startBtn;
	void LotteryEveryday(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onLogin(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void login(int type);
	void parseTaskConfig(std::string filepath);
	void getAchieveData();
	void getTaskData();
	
	void loadUserData(float dt);
	static std::string getUpgradeTime();
	static void httpCompleteCB(std::string retdata, int code, std::string tag);
	static void httpMessageDataCompleteCB(std::string retdata, int code, std::string tag);
	static void httpEventDataCompleteCB(std::string retdata, int code, std::string tag);
	Node *root;
	cocos2d::ui::Widget* m_bg;
	cocostudio::timeline::ActionTimeline* m_actionAmin;
	Node *m_AminLayer;
	int logintype;
	cocos2d::ui::Widget* m_qqloginbtn;
	cocos2d::ui::Widget* m_wxloginbtn;
};
#endif // __START_SCENE_H__
