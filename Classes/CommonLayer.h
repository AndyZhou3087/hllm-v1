#ifndef __COMMON_LAYER_H__
#define __COMMON_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
#include "CommonFuncs.h"
USING_NS_CC;

class CommonLayer : public Layer
{
public:
	enum DISPLAYNUMTYPE
	{
		HPTYPE,
		GOLDTYPE,
		DIAMONDTYPE
	};
	CommonLayer();
	~CommonLayer();
	virtual bool init();

	void Shop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void GrowUp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void more(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void morecm(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void share(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onGift(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onevent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void vip(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updateRedPoint(float dt);
	static void Action(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void vipAnim(float dt);
	void delayShowNewerGuide(float t);
	CREATE_FUNC(CommonLayer);
	void updateUI();

	bool isnewerguiding;
	void setMorebtnVisible(bool val);
	void setMoreCMbtnVisible(bool val);
	void delayShowGift(float dt);
	static void showGift(int pos);
	bool hasNewEvent();
private:
	cocos2d::ui::Widget* light;
	cocos2d::ui::Widget* m_growUpBtn;
	cocos2d::ui::Widget* m_onsaleGiftBtn;
	cocos2d::ui::Widget* m_vipBtnNode;
	cocos2d::ui::Widget* m_vipBtn;
	cocos2d::ui::Widget* m_moreBtn;
	cocos2d::ui::Widget* m_morecmBtn;
	cocos2d::ui::Widget* m_shareBtn;
	cocos2d::ui::Widget* m_eventBtn;
	cocos2d::ui::Widget* m_shopBtn;
	cocos2d::ui::Widget* m_newerBtn;
	cocos2d::ui::Widget* growRedPoint;
	cocos2d::ui::Widget* onsaleRedPoint;
	cocos2d::ui::Widget* moreRedPoint;
	cocos2d::ui::Widget* eventRedPoint;
	cocos2d::ui::Widget* vipRedPoint;
	bool checkifGrowUp();

	Node * _bgNode;

	bool isShowGrowUpAnim;

	bool _chapter1pass;
	bool _chapter2pass;
	void delayShowLoginReward(float dt);
	void checkNewerGuidechapterpass();
	void showCanGrowUpAnim();

	void resetBtnPos();
};
extern CommonLayer* g_commomLayer;
#endif