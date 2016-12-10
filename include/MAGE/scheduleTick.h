#pragma once
#include "schedules.h"

namespace mage {

/*
Tick Task: call a function (using a hook<unsigned int>) every set amount of time forever. Done using tick(), which returns a task.
(other than the int argument, just a wait task that doesn't cancel after happening once)
*/
class MAGEDLL scheduleTick : public schedule {
public:
	scheduleTick(sf::Time toWait, std::function<void(int)> callee);
	scheduleTick(sf::Time toWait, std::function<void(int)> callee, std::function<bool()> stopCond);

	void start();
	void run(); // if it is time, call the hook and reset the startTime.

	sf::Time getLastTickTime();
	sf::Time getTimeSinceLastTick();

	unsigned int getLoopCount();
public:
	hook<int> onWaited;
	sf::Time length;

private:
	sf::Time lastTickTime;
	unsigned int timesLooped;
};

}