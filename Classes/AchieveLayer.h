#ifndef __ACHIEVE_LAYER_H__
#define __ACHIEVE_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
#include "GlobalData.h"
USING_NS_CC;
class AchieveLayer : public Layer
{
public:
	AchieveLayer();
	~AchieveLayer();
	virtual bool init();
	virtual void onExit();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	static void sortAchieveData();
	static bool byfinish(Achieve a, Achieve b);
	void removeRwdLayer(float dt);
	CREATE_FUNC(AchieveLayer);
	void showRwdLayer(std::string path, int count);
	static bool isgeting;
private:
	cocos2d::ui::ScrollView* scrollview;
	LayerColor* color;
	EventListenerTouchOneByOne* listener;
	void showParticle(float dt);
};
extern AchieveLayer* g_AchieveLayer;
class AchieveItem : public Sprite
{
public:
	AchieveItem();
	~AchieveItem();
	bool init(Achieve *data);
	void updatedata(Achieve *data);
	void get(Ref* sender);
	static AchieveItem* createWithData(Achieve *data);

private:
	Label* mname;
	Label* mcount;
	Label* mdesc;
	Label* progresslbl;
	Sprite* rwdSprite;
	Sprite* text0;
	Sprite* text1;
	MenuItemSprite *getbtn;
	std::vector<std::string> rwdPath;
	Achieve *mdata;
};
#endif