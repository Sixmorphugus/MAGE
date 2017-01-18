#pragma once
#include "schedules.h"
#include "interval.h"

namespace mage {

/*
Tick Task: call a function (using a hook<unsigned int>) every set amount of interval forever. Done using tick(), which returns a task.
(other than the int argument, just a wait task that doesn't cancel after happening once)
*/
class MAGEDLL scheduleTick : public schedule {
public:
	scheduleTick(interval toWait, std::function<void(int)> callee);
	scheduleTick(interval toWait, std::function<void(int)> callee, std::function<bool()> stopCond);

	void start();
	void run(); // if it is interval, call the hook and reset the startTime.

	interval getLastTickTime();
	interval getTimeSinceLastTick();

	unsigned int getLoopCount();
public:
	hook<int> onWaited;
	interval length;

private:
	interval lastTickTime;
	unsigned int timesLooped;
};

}