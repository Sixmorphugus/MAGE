#include "scheduleTween.h"
#include "Game.h"

using namespace mage;

scheduleTween::scheduleTween(sf::Time toTake, unsigned int phases, easingFunction ef, std::function<void(float)> callee)
{
	length = toTake;
	numFrames = phases;
	easing = ef;
	skipOnEnd = true;
	tLast = -1.f;

	onTween.registerObserver(callee);
}

scheduleTween::scheduleTween(sf::Time toTake, unsigned int phases, easingFunction ef, std::function<void(float)> callee, std::function<bool()> stopCond)
	: schedule(stopCond)
{
	length = toTake;
	numFrames = phases;
	easing = ef;
	skipOnEnd = true;
	tLast = -1.f;

	onTween.registerObserver(callee);
}

void scheduleTween::start()
{
	schedule::start();
	notifyOnTween(0.f);
}

void scheduleTween::run()
{
	// sanity
	if (!easing) {
		p::fatal("Invalid tweenSchedule easing function");
	}

	unsigned int fr = getFrame();

	if (fr >= numFrames) {
		end();
	}
	else {
		notifyOnTween(easing(fr, numFrames));
	}

	schedule::run();
}

void scheduleTween::end()
{
	if (skipOnEnd) {
		notifyOnTween(1.f);
	}

	schedule::end();
}

unsigned int scheduleTween::getFrame()
{
	// find difference between start time and now + frame length
	sf::Time dif = theGame()->getSimTime() - getStartTime();
	sf::Time frameLength = getFrameLength();

	// use the dif to find how many "frames" have elapsed
	float fr = floor(dif / frameLength);

	return (unsigned int)fr;
}

sf::Time scheduleTween::getFrameLength()
{
	return sf::seconds(length.asSeconds() / numFrames);
}

void scheduleTween::notifyOnTween(float val)
{
	if (val != tLast) {
		onTween.notify(val);
	}

	tLast = val;
}

DeclareScriptingSchedule(scheduleTween);
DeclareScriptingConstructor(scheduleTween(sf::Time, unsigned int, easingFunction, std::function<void(float)>), "scheduleTween");
DeclareScriptingConstructor(scheduleTween(sf::Time, unsigned int, easingFunction, std::function<void(float)>, std::function<bool()>), "scheduleTween");
DeclareScriptingCopyOperator(scheduleTween);
DeclareScriptingFunction(&scheduleTween::easing, "easing");
DeclareScriptingFunction(&scheduleTween::getFrame, "getFrame");
DeclareScriptingFunction(&scheduleTween::getFrameLength, "getFrameLength");
DeclareScriptingFunction(&scheduleTween::onTween, "onTween");
DeclareScriptingFunction(&scheduleTween::skipOnEnd, "skipOnEnd");