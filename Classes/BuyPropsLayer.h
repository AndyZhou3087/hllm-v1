#ifndef __BUYPROPS_LAYER_H__
#define __BUYPROPS_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
#include "GameScene.h"
USING_NS_CC;
class BuyPropsLayer : public Layer
{
public:
	BuyPropsLayer();
	~BuyPropsLayer();
	virtual bool init();
	void changeTitle();
	void ok(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(BuyPropsLayer);
	static int AnimType;
private:
	void resumeGame();
	void useProp(PropsType type);
	Sprite* _title;
	Sprite* typeIcon;
	Label* pricelbl;
	cocos2d::ui::Widget* okBtn;
	cocos2d::ui::Widget* colseBtn;

	cocos2d::ui::Widget* usetext;
	cocos2d::ui::Widget* buyusetext;
	int _type;
	Node * _rootlayer;
	void delayShowNewerGuide(float dt);
};
#endif