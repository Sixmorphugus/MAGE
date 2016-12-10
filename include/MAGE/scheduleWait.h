#pragma once
#include "schedules.h"

namespace mage {

/*
Wait Task : call a function(using a hook<>) after a set amount of time on update.Done using wait(), which returns a task.
(The simplest type of task)
*/
class MAGEDLL scheduleWait : public schedule {
public:
	scheduleWait(sf::Time toWait, std::function<void()> callee);
	scheduleWait(sf::Time toWait, std::function<void()> callee, std::function<bool()> stopCond);

	void run(); // if it is time, call the hook and stop the task.

public:
	hook<> onWaited;
	sf::Time length;
};

} // namespace mage