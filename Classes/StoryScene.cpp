#include "StoryScene.h"
#include "LoadingScene.h"

StoryScene::StoryScene()
{
}


StoryScene::~StoryScene()
{

}

Scene* StoryScene::scene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	StoryScene *layer = StoryScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool StoryScene::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	Node * layer = CSLoader::createNode("storyLayer.csb");
	addChild(layer);

	auto storyanim = CSLoader::createTimeline("storyLayer.csb");
	layer->runAction(storyanim);
	storyanim->gotoFrameAndPlay(0, 1210, false);

	_isClick = false;
	_desclbl = (cocos2d::ui::Widget*)layer->getChildByName("storydesc");

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		if (_isClick)
			return true;


		if (_desclbl->isVisible())
		{
			_isClick = true;
			Director::getInstance()->replaceScene(LoadingScene::scene());
		}
		else
		{
			storyanim->gotoFrameAndPlay(1210);
			showDescLbl(0.0);
		}
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleOnce(schedule_selector(StoryScene::showDescLbl), 24.5f);
	return true;
}

void StoryScene::showDescLbl(float dt)
{
	_desclbl->setVisible(true);
	_desclbl->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.7f),FadeIn::create(0.7f), NULL)));
	//Director::getInstance()->replaceScene(LoadingScene::scene());
}