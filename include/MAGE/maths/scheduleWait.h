#pragma once
#include "schedules.h"
#include "interval.h"

namespace mage {

/*
Wait Task : call a function(using a hook<>) after a set amount of interval on update.Done using wait(), which returns a task.
(The simplest type of task)
*/
class MAGEDLL scheduleWait : public schedule {
public:
	scheduleWait(interval toWait, std::function<void()> callee);
	scheduleWait(interval toWait, std::function<void()> callee, std::function<bool()> stopCond);

	void run(); // if it is interval, call the hook and stop the task.

public:
	hook<> onWaited;
	interval length;
};

} // namespace mage