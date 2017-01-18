#include "gameState.h"

#include "resourceScene.h"
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

gameState::gameState(std::shared_ptr<resourceScene> gr)
{
	setVarDefaults();

	if (!gr)
		return;

	if (!gr->isLoaded()) {
		p::warn("Unable to create gameState from unloaded mageGroupResource");
	}
	else {
		scene::set(gr.get());
	}
}

std::string gameState::serialize()
{
	std::string serializedGroup = scene::serialize();

	// generate attributes
	std::stringstream attribStream;

	attribStream << "BGCOL " << (int)bgCol.r << "," << (int)bgCol.g << "," << (int)bgCol.b << "@\n";
	attribStream << "FOG " << (int)lightingAmb.r << "," << (int)lightingAmb.g << "," << (int)lightingAmb.b << "," << (int)lightingAmb.a << "@\n";

	attribStream << "BOUNDS " << mapBounds.position.x << "," << mapBounds.position.y << "," << mapBounds.size.x << "," << mapBounds.size.y << "@\n";

	std::string attribString = attribStream.str();

	// add attributes onto the start of the string
	serializedGroup = attribString + serializedGroup;

	return serializedGroup;
}

bool gameState::deserialize(std::string saveString)
{
	auto things = splitString(saveString, '@', '#');

	for (unsigned int line = 0; line < things.size(); line++)
	{
		std::string sLine = things[line];

		// there is only one space in a save file line.
		// this defines why the line is there.
		auto spaceSplit = splitStringAtFirst(sLine);

		if (spaceSplit.size() < 2) {
			// skip this line
			p::info("Ignoring wrong space format/blank section at line " + std::to_string(line));
			continue;
		}

		std::string type = spaceSplit[0];
		std::string sData = spaceSplit[1];

		if (type == "BGCOL") {
			// format:
			// BGCOL r,g,b

			auto commaSp = splitString(sData, ',', '\0', 3);

			bgCol.r = atoi(commaSp[0].c_str());
			bgCol.g = atoi(commaSp[1].c_str());
			bgCol.b = atoi(commaSp[2].c_str());
		}
		else if (type == "FOG") {
			// format:
			// FOG r,g,b

			auto commaSp = splitString(sData, ',', '\0', 4);

			lightingAmb.r = atoi(commaSp[0].c_str());
			lightingAmb.g = atoi(commaSp[1].c_str());
			lightingAmb.b = atoi(commaSp[2].c_str());
			lightingAmb.a = atoi(commaSp[3].c_str());
		}
		else if (type == "BOUNDS") {
			// format:
			// BOUNDS left,top,width,height
			auto commaSp = splitString(sData, ',', '\0', 4);

			mapBounds.position.x = atoi(commaSp[0].c_str());
			mapBounds.position.y = atoi(commaSp[1].c_str());
			mapBounds.size.x = atoi(commaSp[2].c_str());
			mapBounds.size.y = atoi(commaSp[3].c_str());

			// bounds is always last so don't use any more cpu interval looking for other gameState lines
			break;
		}
	}

	return scene::deserialize(saveString);
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
	mapBounds = sf::FloatRect(0, 0, 800, 800); // don't ask
	lightingAmb = sf::Color::Transparent;
}

#include "scriptingEngine.h"

// SEBind for gameState
MAGE_DeclareScriptingType(gameState);
MAGE_DeclareScriptingBaseClass(scene, gameState);
MAGE_DeclareScriptingBaseClass(taggable, gameState);
MAGE_DeclareScriptingConstructor(gameState(), "gameState");
MAGE_DeclareScriptingConstructor(gameState(const gameState&), "gameState");
MAGE_DeclareScriptingConstructor(gameState(std::shared_ptr<resourceScene>), "gameState");
MAGE_DeclareScriptingConstructor(gameState(std::shared_ptr<resourceScene>), "gameState");
MAGE_DeclareScriptingFunction(&gameState::bgCol, "bgCol");
MAGE_DeclareScriptingFunction(&gameState::getStateUseTime, "getStateUseTime");
MAGE_DeclareScriptingFunction(&gameState::lightingAmb, "lightingAmb");
MAGE_DeclareScriptingFunction(&gameState::mapBounds, "mapBounds");

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