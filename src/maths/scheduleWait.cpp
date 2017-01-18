#include "scheduleWait.h"
#include "Game.h"

using namespace mage;

scheduleWait::scheduleWait()
{
	length = 0;
}

scheduleWait::scheduleWait(interval toWait, std::function<void()> callee)
{
	length = toWait;
	onWaited.registerObserver(callee);
}

scheduleWait::scheduleWait(interval toWait, std::function<void()> callee, std::function<bool()> stopCond)
	: schedule(stopCond)
{
	length = toWait;
	onWaited.registerObserver(callee);
}

void scheduleWait::run()
{
	// find difference between start interval and now
	interval dif = theGame()->getSimTime() - getStartTime();

	if (dif >= length) {
		onWaited.notify();
		end();
	}

	schedule::run();
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingSchedule(scheduleWait);
MAGE_DeclareScriptingConstructor(scheduleWait(interval, std::function<void()>), "scheduleTick");
MAGE_DeclareScriptingConstructor(scheduleWait(interval, std::function<void()>, std::function<bool()>), "scheduleTick");
MAGE_DeclareScriptingCopyOperator(scheduleWait);
MAGE_DeclareScriptingFunction(&scheduleWait::onWaited, "onWaited");
