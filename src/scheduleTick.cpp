#include "scheduleTick.h"
#include "Game.h"

using namespace mage;

scheduleTick::scheduleTick(sf::Time toWait, std::function<void(int)> callee)
{
	length = toWait;
	onWaited.registerObserver(callee);
	lastTickTime = theGame()->getSimTime();
}

scheduleTick::scheduleTick(sf::Time toWait, std::function<void(int)> callee, std::function<bool()> stopCond)
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
	// find difference between start time and now
	sf::Time dif = getTimeSinceLastTick();

	if (dif >= length) {
		timesLooped++;
		lastTickTime = theGame()->getSimTime();

		onWaited.notify(timesLooped);
	}

	schedule::run();
}

sf::Time scheduleTick::getLastTickTime()
{
	return lastTickTime;
}

sf::Time scheduleTick::getTimeSinceLastTick()
{
	return theGame()->getSimTime() - lastTickTime;
}

unsigned int scheduleTick::getLoopCount()
{
	return timesLooped;
}

DeclareScriptingSchedule(scheduleTick);
DeclareScriptingConstructor(scheduleTick(sf::Time, std::function<void(int)>), "scheduleTick");
DeclareScriptingConstructor(scheduleTick(sf::Time, std::function<void(int)>, std::function<bool()>), "scheduleTick");
DeclareScriptingCopyOperator(scheduleTick);
DeclareScriptingFunction(&scheduleTick::getLastTickTime, "getLastTickTime");
DeclareScriptingFunction(&scheduleTick::getTimeSinceLastTick, "getTimeSinceLastTick");
DeclareScriptingFunction(&scheduleTick::getLoopCount, "getLoopCount");
DeclareScriptingFunction(&scheduleTick::length, "length");
DeclareScriptingFunction(&scheduleTick::onWaited, "onWaited");