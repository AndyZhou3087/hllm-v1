#ifndef __MORE_LAYER_H__
#define __MORE_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;

class MoreLayer : public Layer
{
public:

	MoreLayer();
	~MoreLayer();
	virtual bool init();
	CREATE_FUNC(MoreLayer);
	cocos2d::ui::Widget* loginRewardRedPoint;
	void checkRedPoint(float dt);
	void loginReward(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void Setting(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void more(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void message(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	static bool isHasRedPoint();
private:
	cocos2d::ui::Widget* m_loginrewardBtn;
	Node * _bgNode;

	cocos2d::ui::Widget* m_achieveBtn;
	cocos2d::ui::Widget* m_taskBtn;
	cocos2d::ui::Widget* m_taskAnimBtn;
	cocos2d::ui::Widget* m_messageBtn;
	cocos2d::ui::Widget* achieRedPoint;
	cocos2d::ui::Widget* taskRewardRedPoint;
	cocos2d::ui::Widget* messageRedPoint;

	void Achieve(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void rank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void task(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	static int checkHasFinishTask();
	static int checkHasAchie();
	static int checkHasMessage();
	void showCanAchieveAim();
};
#endif