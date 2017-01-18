#include "scheduleTick.h"
#include "Game.h"

using namespace mage;

scheduleTick::scheduleTick(interval toWait, std::function<void(int)> callee)
{
	length = toWait;
	onWaited.registerObserver(callee);
	lastTickTime = theGame()->getSimTime();
}

scheduleTick::scheduleTick(interval toWait, std::function<void(int)> callee, std::function<bool()> stopCond)
	: schedule(stopCond)
{
	length = toWait;
	onWaited.registerObserver(callee);
	lastTickTime = theGame()->getSimTime();
}

void scheduleTick::start()
{
	schedule::start();
	lastTickTime = getStartTime();
}

void scheduleTick::run()
{
	// find difference between start interval and now
	interval dif = getTimeSinceLastTick();

	if (dif >= length) {
		timesLooped++;
		lastTickTime = theGame()->getSimTime();

		onWaited.notify(timesLooped);
	}

	schedule::run();
}

interval scheduleTick::getLastTickTime()
{
	return lastTickTime;
}

interval scheduleTick::getTimeSinceLastTick()
{
	return theGame()->getSimTime() - lastTickTime;
}

unsigned int scheduleTick::getLoopCount()
{
	return timesLooped;
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingSchedule(scheduleTick);
MAGE_DeclareScriptingConstructor(scheduleTick(interval, std::function<void(int)>), "scheduleTick");
MAGE_DeclareScriptingConstructor(scheduleTick(interval, std::function<void(int)>, std::function<bool()>), "scheduleTick");
MAGE_DeclareScriptingCopyOperator(scheduleTick);
MAGE_DeclareScriptingFunction(&scheduleTick::getLastTickTime, "getLastTickTime");
MAGE_DeclareScriptingFunction(&scheduleTick::getTimeSinceLastTick, "getTimeSinceLastTick");
MAGE_DeclareScriptingFunction(&scheduleTick::getLoopCount, "getLoopCount");
MAGE_DeclareScriptingFunction(&scheduleTick::length, "length");
MAGE_DeclareScriptingFunction(&scheduleTick::onWaited, "onWaited");