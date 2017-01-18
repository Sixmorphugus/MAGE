#pragma once
#include "scene.h"
#include "taggable.h"
#include "colour.h"
#include "box.h"
#include "timer.h"

namespace mage {

class resourceScene;
class resourceMusic;
class interval;

// A state can be loaded into the game in its entirety, which means it erases the old one.
// It can also be COMBINED with the current state held in the stateMngr.

// A state can ALSO only have objects of a certain type loaded on top of or combined with objects of the same type.
// So you can have a state that represents a level and no ui and just load its world objects over the current ones without
//  erasing the current UI and replacing it with nothing.

// this new gameState definition means that states are defined via scripting, NOT via their resource file.
// a resource file can now only hold an object group - a state must be created around it for it to work.

class MAGEDLL gameState : public scene, public taggable
{
public:
	gameState(); // creates empty state
	gameState(const gameState& gr); // creates state with a group as the main objects (copying them out immediately)
	gameState(std::shared_ptr<resourceScene> gr);

	virtual std::string serialize();
	virtual bool deserialize(std::string serialized);

	virtual void preUpdateObjects(interval elapsed);
	virtual void updateObjects(interval elapsed);

	virtual void clearObjects();

	template<class Filter = gmo> void set(gameState* gs);

	interval getStateUseTime(); // interval since state was last "set"

private:
	void setVarDefaults();

public:
	colour bgCol;
	floatBox mapBounds;
	colour lightingAmb;

private:
	timer stateClock;
};

// The statemanager will track not only the current master state but the current COMBINATION of states that are being used.
// startUsing and stopUsing are always called at times that MAKE SENSE.
// This REALLY justifies adding yet another subsystem manager to the engine.

// The current state can be accessed with "state" in the SE, shorthand for "states.getCurrent()". "states", however, leads you here

class MAGEDLL gameStateMngr {
public:
	gameStateMngr();

	void add(std::string name, std::shared_ptr<gameState> input);
	bool exists(std::string name);

	std::string nameOf(std::shared_ptr<gameState> input);

	void setCurrent(std::string name);
	template<class Filter = gmo> void setCurrent(std::string name);

	void combineCurrent(std::string name);
	template<class Filter = gmo> void combineCurrent(std::string name);

	std::shared_ptr<gameState> get(std::string name);
	std::shared_ptr<gameState> getByIndex(unsigned int index);

	unsigned int getCount();

	std::vector<std::shared_ptr<gameState>> list(std::string tag = "");

public:
	std::shared_ptr<gameState> current;

private:
	// all of these children are autodeleted
	// allocated ones use shared_ptr, current is just an object
	// by the way - all of these shared_ptrs are here so we can take stuff directly from the scripting engine and not have the game crash
	// because the scripting engine still thinks it can deallocate said "stuff"
	std::map< std::string, std::shared_ptr<gameState> > stateCache;
};

// gameStateMngr templates
template<class Filter>
inline void gameStateMngr::setCurrent(std::string name) {
	current->set<Filter>(get(name).get());
}

template<class Filter>
inline void gameStateMngr::combineCurrent(std::string name) {
	current->combine<Filter>(get(name).get());
}

template<class Filter>
inline void gameState::set(gameState * gs)
{
	if (ignoreIncorporation)
		return;

	if (gs == nullptr)
		return;

	stateClock.restart();

	bgCol = gs->bgCol;
	lightingAmb = gs->lightingAmb;
	mapBounds = gs->mapBounds;

	scene::set<Filter>(gs);
}

} // namespace mage