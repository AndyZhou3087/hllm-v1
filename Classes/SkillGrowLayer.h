#ifndef __SKILLGROW_LAYER_H__
#define __SKILLGROW_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
class SkillGrowLayer : public Layer
{
public:
	SkillGrowLayer();
	~SkillGrowLayer();
	virtual bool init();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void growUp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void showHeroNormalAmin(float t);
	CREATE_FUNC(SkillGrowLayer);
private:
	Node * _layer;
	cocos2d::ui::Widget* _growUpBtn;
	cocos2d::ui::Button* _closeBtn;
	cocos2d::ui::Widget* _select;
	cocos2d::ui::TextBMFont* _lvnum;
	cocos2d::ui::TextBMFont* _needGoldlbl;
	int needGold;
	Sprite* _leveUpText;
	cocos2d::ui::Widget* _smallgoldicon;
	void delayShowNewerGuide(float dt);
	Node* _heroAminLayer;
	Node* _heroLvupAminLayer;
	cocostudio::timeline::ActionTimeline* actionAmin;
	cocostudio::timeline::ActionTimeline* actionAmin1;
};
extern SkillGrowLayer* g_skillGrowlayer;
#endif