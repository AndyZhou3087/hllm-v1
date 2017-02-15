#include "MyParticle.h"


MyParticle::MyParticle()
{

}

MyParticle::~MyParticle()
{

}

MyParticle * MyParticle::create(const std::string& plistFile, bool isbleand, float duration, float emissionRate)
{
	MyParticle *ret = new (std::nothrow) MyParticle();
	if (ret && ret->initwithdata(plistFile, isbleand, duration, emissionRate))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return ret;
}

bool MyParticle::initwithdata(const std::string& plistFile, bool isbleand, float duration, float emissionRate)
{
	if (!initWithFile(plistFile))
		return false;
	setBlendAdditive(isbleand);
	setDuration(duration);
	setEmissionRate(emissionRate);
	setAutoRemoveOnFinish(true);
	return true;
}

MyParticle* MyParticle::Show(Node* parent, const std::string& plistFile, int x, int y, bool isbleand, float duration, float emissionRate)
{
	MyParticle* particle = create(plistFile + ".plist", isbleand, duration, emissionRate);
	particle->setPosition(Vec2(x, y));
	parent->addChild(particle, 1000);
	return particle;
}