#include "schedules.h"
#include "Game.h"
#include "interval.h"

using namespace mage;

schedule::schedule()
{
	running = false;
	startTime = sf::seconds(0.f);

	conditionFunction = nullptr;
}

schedule::schedule(std::function<bool()> stopCond)
{
	running = false;
	startTime = sf::seconds(0.f);

	conditionFunction = stopCond;
}

void schedule::start()
{
	if (!running) {
		running = true;
		startTime = theGame()->getSimTime();

		onStart.notify(this);
	}
}

void schedule::end()
{
	if (running) {
		running = false;
		onEnd.notify(this);
	}
}

void schedule::run()
{
	if (conditionFunction) {
		if (conditionFunction()) {
			end();
		}
	}

	onRun.notify(this);
}

void schedule::update()
{
	if (running) {
		run();
	}
}

bool schedule::isRunning()
{
	return running;
}

interval schedule::getStartTime()
{
	return startTime;
}

scheduleMngr::scheduleMngr()
{
}

unsigned int scheduleMngr::add(std::shared_ptr<schedule> input)
{
	auto tsk = tasks.size();

	tasks.push_back(input);
	input->start();

	return tsk;
}

int scheduleMngr::indexOf(std::shared_ptr<schedule> in)
{
	for (unsigned int i = 0; i < tasks.size(); i++) {
		if (tasks[i] == in) {
			return i;
		}
	}

	return -1;
}

std::shared_ptr<schedule> scheduleMngr::get(unsigned int index)
{
	if (index >= tasks.size()) {
		return nullptr;
	}

	return tasks[index];
}

unsigned int scheduleMngr::getCount()
{
	return tasks.size();
}

void scheduleMngr::endAll()
{
	for (unsigned int i = 0; i < tasks.size(); i++) {
		tasks[i]->end();
	}
}

void scheduleMngr::update(interval elapsed)
{
	// update tasks that are running, forget about tasks that finished
	for (int i = 0; i < (int)tasks.size(); i++) {
		if (tasks[i]->isRunning()) {
			tasks[i]->update();
		}
		else {
			tasks.erase(tasks.begin() + i);
			i--;
		}
	}
}

// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(scheduleMngr);
MAGE_DeclareScriptingConstructor(scheduleMngr(), "scheduleMngr");
MAGE_DeclareScriptingFunction(&scheduleMngr::add, "add");
MAGE_DeclareScriptingFunction(&scheduleMngr::endAll, "endAll");
MAGE_DeclareScriptingFunction(&scheduleMngr::get, "get");
MAGE_DeclareScriptingFunction(&scheduleMngr::getCount, "getCount");
MAGE_DeclareScriptingFunction(&scheduleMngr::indexOf, "indexOf");
MAGE_DeclareScriptingFunction(&scheduleMngr::update, "update");

MAGE_DeclareScriptingType(schedule);
MAGE_DeclareScriptingConstructor(schedule(), "schedule");
MAGE_DeclareScriptingConstructor(schedule(std::function<bool()>), "schedule");
MAGE_DeclareScriptingCopyOperator(schedule);
MAGE_DeclareScriptingFunction(&schedule::conditionFunction, "conditionFunction");
MAGE_DeclareScriptingFunction(&schedule::end, "end");
MAGE_DeclareScriptingFunction(&schedule::getStartTime, "getStartTime");
MAGE_DeclareScriptingFunction(&schedule::isRunning, "isRunning");
MAGE_DeclareScriptingFunction(&schedule::onEnd, "onEnd");
MAGE_DeclareScriptingFunction(&schedule::onRun, "onRun");
MAGE_DeclareScriptingFunction(&schedule::onStart, "onStart");
MAGE_DeclareScriptingFunction(&schedule::run, "run");
MAGE_DeclareScriptingFunction(&schedule::isRunning, "isRunning");
MAGE_DeclareScriptingFunction(&schedule::start, "start");
MAGE_DeclareScriptingFunction(&schedule::update, "update");
MAGE_DeclareScriptingFunction(&schedule::onEnd, "onEnd");
MAGE_DeclareScriptingFunction(&schedule::onRun, "onRun");
MAGE_DeclareScriptingFunction(&schedule::onStart, "onStart");