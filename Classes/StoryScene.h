#ifndef __STORYSCENE_H__
#define __STORYSCENE_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
class StoryScene : public Layer
{
public:
	StoryScene();
	~StoryScene();
	static cocos2d::Scene* scene();
	virtual bool init();
	CREATE_FUNC(StoryScene);
private:
	void showDescLbl(float dt);
	cocos2d::ui::Widget* _desclbl;
	bool _isClick;
};
#endif