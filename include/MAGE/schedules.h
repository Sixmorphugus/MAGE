#pragma once
#include "StdAfx.h"
#include "SfmlAfx.h"

#include "hook.h"

namespace mage {

/*
The SCHEDULER is used for global animation tasks.
All tasks CAN take a function that has no arguments and returns a boolean, to verify whether they should keep running, but don't have to
Tasks take the form of different things.
*/

// you can instantiate the default task but it will go on forever and does nothing.
// if you want to do something interesting, however, you can use the HOOKS to make a custom task with its own behavior.
class MAGEDLL schedule {
public:
	schedule();
	schedule(std::function<bool()> stopCond);

	virtual void start(); // Starts the task, called when the task is given to a scheduler.
	virtual void end(); // Ends the task, called either manually to cancel the task or by update() when the task thinks it is finished.
	virtual void run(); // continues the task as it runs

	void update();

	bool isRunning(); // if this returns false to the scheduler the scheduler ususally deletes it.

	sf::Time getStartTime();

public:
	std::function<bool()> conditionFunction;

	hook<schedule*> onStart;
	hook<schedule*> onEnd;
	hook<schedule*> onRun;
private:
	bool running;
	sf::Time startTime;
};

// the manager object
class MAGEDLL scheduleMngr {
public:
	scheduleMngr();

	std::shared_ptr<schedule> add(std::shared_ptr<schedule> input);

	int indexOf(std::shared_ptr<schedule> in);

	std::shared_ptr<schedule> get(unsigned int index);
	template<class T> std::shared_ptr<T> getAs(unsigned int index);
	unsigned int getCount();

	void endAll();

	void update(sf::Time elapsed);

private:
	std::vector<std::shared_ptr<schedule>> tasks;
};

template<class T>
inline std::shared_ptr<T> scheduleMngr::getAs(unsigned int index)
{
	std::shared_ptr<schedule> s = get(index);
	return std::dynamic_pointer_cast<T>(s);
}

} // namespace mage

#define DeclareScriptingSchedule(type) \
DeclareScriptingType(type); \
DeclareScriptingBaseClass(mage::schedule, type); \
DeclareScriptingFunction(&mage::scheduleMngr::getAs<type>, "get_" STRING(type)); \
DeclareScriptingCastingFunction("to_" STRING(type), mage::schedule, type);
