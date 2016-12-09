#include "scheduleWait.h"
#include "Game.h"

using namespace mage;

scheduleWait::scheduleWait(sf::Time toWait, std::function<void()> callee)
{
	length = toWait;
	onWaited.registerObserver(callee);
}

scheduleWait::scheduleWait(sf::Time toWait, std::function<void()> callee, std::function<bool()> stopCond)
	: schedule(stopCond)
{
	length = toWait;
	onWaited.registerObserver(callee);
}

void scheduleWait::run()
{
	// find difference between start time and now
	sf::Time dif = theGame()->getSimTime() - getStartTime();

	if (dif >= length) {
		onWaited.notify();
		end();
	}

	schedule::run();
}

DeclareScriptingSchedule(scheduleWait);
DeclareScriptingConstructor(scheduleWait(sf::Time, std::function<void()>), "scheduleTick");
DeclareScriptingConstructor(scheduleWait(sf::Time, std::function<void()>, std::function<bool()>), "scheduleTick");
DeclareScriptingCopyOperator(scheduleWait);
DeclareScriptingFunction(&scheduleWait::onWaited, "onWaited");
