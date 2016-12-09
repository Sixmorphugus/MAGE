#include "schedules.h"
#include "Game.h"

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

sf::Time schedule::getStartTime()
{
	return startTime;
}

scheduleMngr::scheduleMngr()
{
}

std::shared_ptr<schedule> scheduleMngr::add(std::shared_ptr<schedule> input)
{
	tasks.push_back(input);
	input->start();

	return input;
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

void scheduleMngr::update(sf::Time elapsed)
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

DeclareScriptingType(scheduleMngr);
DeclareScriptingConstructor(scheduleMngr(), "scheduleMngr");
DeclareScriptingFunction(&scheduleMngr::add, "add");
DeclareScriptingFunction(&scheduleMngr::endAll, "endAll");
DeclareScriptingFunction(&scheduleMngr::get, "get");
DeclareScriptingFunction(&scheduleMngr::getCount, "getCount");
DeclareScriptingFunction(&scheduleMngr::indexOf, "indexOf");
DeclareScriptingFunction(&scheduleMngr::update, "update");

DeclareScriptingType(schedule);
DeclareScriptingConstructor(schedule(), "schedule");
DeclareScriptingConstructor(schedule(std::function<bool>), "schedule");
DeclareScriptingCopyOperator(schedule);
DeclareScriptingFunction(&schedule::conditionFunction, "conditionFunction");
DeclareScriptingFunction(&schedule::end, "end");
DeclareScriptingFunction(&schedule::getStartTime, "getStartTime");
DeclareScriptingFunction(&schedule::isRunning, "isRunning");
DeclareScriptingFunction(&schedule::onEnd, "onEnd");
DeclareScriptingFunction(&schedule::onRun, "onRun");
DeclareScriptingFunction(&schedule::onStart, "onStart");
DeclareScriptingFunction(&schedule::run, "run");
DeclareScriptingFunction(&schedule::isRunning, "isRunning");
DeclareScriptingFunction(&schedule::start, "start");
DeclareScriptingFunction(&schedule::update, "update");
DeclareScriptingFunction(&schedule::onEnd, "onEnd");
DeclareScriptingFunction(&schedule::onRun, "onRun");
DeclareScriptingFunction(&schedule::onStart, "onStart");