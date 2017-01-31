#include "gameState.h"

#include "Game.h"
#include "stringHelpers.h"
#include "timer.h"

using namespace mage;

// GAME STATE MANAGER IMPLEMENTATION
gameStateMngr::gameStateMngr()
{
	current = std::make_shared<gameState>();

	// cool default state stuff?
	// naaaaah
}

void gameStateMngr::add(std::string name, std::shared_ptr<gameState> input)
{
	if (exists(name)) {
		p::warn("State already exists with name \"" + name + "\", replacing");
	}

	stateCache[name] = input;
}

bool gameStateMngr::exists(std::string name)
{
	return stateCache.count(name) > 0;
}

std::string gameStateMngr::nameOf(std::shared_ptr<gameState> input)
{
	// iteration
	for (auto i = stateCache.begin(); i != stateCache.end(); i++) {
		if (i->second == input) {
			return i->first;
		}
	}

	return "";
}

void gameStateMngr::setCurrent(std::string name)
{
	setCurrent<>(name);
}

void gameStateMngr::combineCurrent(std::string name)
{
	combineCurrent<>(name);
}

std::shared_ptr<gameState> gameStateMngr::get(std::string name)
{
	if (exists(name))
		return stateCache[name];
	else
		return nullptr;
}

std::shared_ptr<gameState> gameStateMngr::getByIndex(unsigned int index)
{
	if (index >= stateCache.size())
		return nullptr;

	auto curState = stateCache.begin();

	for (unsigned int i = 0; i < index; i++) {
		curState++;
	}

	return curState->second;
}

unsigned int gameStateMngr::getCount()
{
	return stateCache.size();
}

std::vector<std::shared_ptr<gameState>> gameStateMngr::list(std::string tag)
{
	std::vector<std::shared_ptr<gameState>> results;

	// now we must iterate
	for (auto i = stateCache.begin(); i != stateCache.end(); i++) {
		if (i->second->hasTag(tag)) {
			results.push_back(i->second);
		}
	}

	return results;
}

// GAME STATE IMPLEMENTATION
gameState::gameState()
{
	setVarDefaults();
}

gameState::gameState(const gameState & gr)
{
	setVarDefaults();
	scene::set(&gr);
}

void gameState::preUpdateObjects(interval elapsed)
{
	onPreUpdate.notify(this);

	scene::preUpdateObjects(elapsed);
}

void gameState::updateObjects(interval elapsed)
{
	onUpdate.notify(this);

	scene::updateObjects(elapsed);
}

void gameState::clearObjects()
{
	scene::clearObjects();
}

interval gameState::getStateUseTime()
{
	return stateClock.getElapsedTime();
}

void gameState::setVarDefaults()
{
	bgCol = sf::Color::Black; // default state screen color is black
	mapBounds = floatBox(pointF(0, 0, 0), pointF(800, 800, 800)); // don't ask
	lightingAmb = sf::Color::Transparent;
}

#include "scriptingEngine.h"

// SEBind for gameState
MAGE_DeclareScriptingType(gameState);
MAGE_DeclareScriptingBaseClass(scene, gameState);
MAGE_DeclareScriptingBaseClass(taggable, gameState);
MAGE_DeclareScriptingSerializable(gameState, cereal::JSONInputArchive);
MAGE_DeclareScriptingConstructor(gameState(), "gameState");
MAGE_DeclareScriptingConstructor(gameState(const gameState&), "gameState");
MAGE_DeclareScriptingFunction(&gameState::bgCol, "bgCol");
MAGE_DeclareScriptingFunction(&gameState::getStateUseTime, "getStateUseTime");
MAGE_DeclareScriptingFunction(&gameState::lightingAmb, "lightingAmb");
MAGE_DeclareScriptingFunction(&gameState::mapBounds, "mapBounds");

MAGE_DeclareSerializationBase(gameState);

// for gameStateMngr
MAGE_DeclareScriptingType(gameStateMngr);
MAGE_DeclareScriptingFunction(&gameStateMngr::add, "add");
MAGE_DeclareScriptingFunction(&gameStateMngr::combineCurrent<>, "combineCurrent");
MAGE_DeclareScriptingFunction(&gameStateMngr::current, "current");
MAGE_DeclareScriptingFunction(&gameStateMngr::exists, "exists");
MAGE_DeclareScriptingFunction(&gameStateMngr::get, "get");
MAGE_DeclareScriptingFunction(&gameStateMngr::getByIndex, "getByIndex");
MAGE_DeclareScriptingFunction(&gameStateMngr::getCount, "getCount");
MAGE_DeclareScriptingFunction(&gameStateMngr::list, "list");
MAGE_DeclareScriptingFunction(&gameStateMngr::nameOf, "nameOf");
MAGE_DeclareScriptingFunction(&gameStateMngr::setCurrent<>, "setCurrent");