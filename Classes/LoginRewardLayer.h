#ifndef __LOGINREWARD_LAYER_H__
#define __LOGINREWARD_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
using namespace std;

class LoginRewardLayer : public Layer
{
public:
	LoginRewardLayer();
	~LoginRewardLayer();
	virtual bool init();
	virtual void onExit();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void get(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(LoginRewardLayer);
	static int AnimType;
private:
	void getOKAnim(const char* pic, Vec2 spoint, Vec2 dpoint);
	void closeSelf(float dt);
	cocos2d::ui::Widget*  bgitem0;
	cocos2d::ui::Widget*  bgitem1;
	cocos2d::ui::Widget*  bgitem2;
	cocos2d::ui::Widget*  bgitem3;
	cocos2d::ui::Widget*  bgitem4;
	cocos2d::ui::Widget*  getIcon;
	int logindays;
};

//extern Node* g_LoginRewardLayer;
#endif