#include "scheduleTween.h"
#include "Game.h"

using namespace mage;

scheduleTween::scheduleTween(interval toTake, unsigned int phases, easingFunction ef, std::function<void(float)> callee)
{
	length = toTake;
	numFrames = phases;
	easing = ef;
	skipOnEnd = true;
	tLast = -1.f;

	onTween.registerObserver(callee);
}

scheduleTween::scheduleTween(interval toTake, unsigned int phases, easingFunction ef, std::function<void(float)> callee, std::function<bool()> stopCond)
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
	// find difference between start interval and now + frame length
	interval dif = theGame()->getSimTime() - getStartTime();
	interval frameLength = getFrameLength();

	// use the dif to find how many "frames" have elapsed
	float fr = floor((dif / frameLength).get());

	return (unsigned int)fr;
}

interval scheduleTween::getFrameLength()
{
	return (length / numFrames);
}

void scheduleTween::notifyOnTween(float val)
{
	if (val != tLast) {
		onTween.notify(val);
	}

	tLast = val;
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingSchedule(scheduleTween);
MAGE_DeclareScriptingConstructor(scheduleTween(interval, unsigned int, easingFunction, std::function<void(float)>), "scheduleTween");
MAGE_DeclareScriptingConstructor(scheduleTween(interval, unsigned int, easingFunction, std::function<void(float)>, std::function<bool()>), "scheduleTween");
MAGE_DeclareScriptingCopyOperator(scheduleTween);
MAGE_DeclareScriptingFunction(&scheduleTween::easing, "easing");
MAGE_DeclareScriptingFunction(&scheduleTween::getFrame, "getFrame");
MAGE_DeclareScriptingFunction(&scheduleTween::getFrameLength, "getFrameLength");
MAGE_DeclareScriptingFunction(&scheduleTween::onTween, "onTween");
MAGE_DeclareScriptingFunction(&scheduleTween::skipOnEnd, "skipOnEnd");