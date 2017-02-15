#ifndef __REVIVELAYER_H_
#define __REVIVELAYER_H_
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
USING_NS_CC;

class ReviveLayer : public Layer
{
public:
	ReviveLayer();
	~ReviveLayer();
	virtual bool init();
	virtual void onExit();
	virtual void onEnterTransitionDidFinish();
	CREATE_FUNC(ReviveLayer);
	static int AnimType;
private:
	cocos2d::ui::Widget* m_restart;
	cocos2d::ui::Widget* m_revive;
	cocos2d::ui::Widget* m_close;
	cocos2d::ui::Widget* m_reviveText;
	cocos2d::ui::TextBMFont* m_gemCunt;
	cocos2d::ui::Widget* m_freeText;
	cocos2d::ui::Widget* m_useText;
	cocos2d::ui::Widget* m_reviveicon;
	cocos2d::ui::TextBMFont* m_totalText;
	cocos2d::ui::Widget* m_gem;
	Node * _failNode;
	void FailExit(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void restart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void Revive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void buyPropGift(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSale(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updateGemLabel(float dt);
	void delayShowBossSettlement();
	void delayPlayWinSound(float dt);
	void enterBossSettlementLayer(float dt);
	void delayShowReviveGift(float dt);
	bool checkPcode(int pcodeindex);
};
#endif
