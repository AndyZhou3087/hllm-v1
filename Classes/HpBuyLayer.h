#ifndef __HPBUY_LAYER_H__
#define __HPBUY_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
class HpBuyLayer : public Layer
{
public:
	HpBuyLayer();
	~HpBuyLayer();
	virtual bool init();
	void hpUp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void hpMax(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(HpBuyLayer);
	void showBuyOKAnim(const char* pic, Vec2 spoint, Vec2 dpoint);
	static int AnimType;
private:
	int _powerSec;
	int _powertime;//ʱ��
	int _powercount;//����
	bool _isAutoPop;
	cocos2d::ui::TextBMFont* _recoveryTime;
	cocos2d::ui::Widget* _hpRecoveryText;
	cocos2d::ui::Widget* _hpMaxTextlbl;
	cocos2d::ui::Widget* _hpRecoveryTimeBox;
	cocos2d::ui::Widget* _closeBtn;

	int maxHpCount;
	void initTime();//��ʼ��ʱ��
	void SaveTime();//����
	void updateTime(float t);//����ʱ��
	void removeSelf(float dt);
};
#endif