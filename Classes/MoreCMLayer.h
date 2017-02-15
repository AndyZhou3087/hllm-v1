#ifndef __MORECM_LAYER_H__
#define __MORECM_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;

class MoreCMLayer : public Layer
{
public:

	MoreCMLayer();
	~MoreCMLayer();
	virtual bool init();
	CREATE_FUNC(MoreCMLayer);
	void checkRedPoint(float dt);
	void morecm(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	static bool isHasRedPoint();
private:
	Node * _bgNode;
	cocos2d::ui::Widget* m_BossChallengeBtn;
	cocos2d::ui::Widget* m_ClasicBtn;
	cocos2d::ui::Widget* m_flashBtn;
	cocos2d::ui::Widget* clasicRedPoint;
	cocos2d::ui::Widget* flashRedPoint;
	void delayShowNewerGuide(float t);
	void BossChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void ClasicChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void FlashChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif