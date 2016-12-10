#include "resourceGameState.h"

#include "gameState.h"

using namespace mage;

resourceGameState::resourceGameState()
{}

resourceGameState::resourceGameState(std::string filepath)
	: resourceFilepath(filepath)
{}

bool resourceGameState::load()
{
	rc = std::make_shared<gameState>();

	if (!rc->loadFromFile(filepath)) return false;
	return resourceFilepath::load();
}

void resourceGameState::unload()
{
	rc = nullptr;
	resourceFilepath::unload();
}

std::shared_ptr<gameState> resourceGameState::get()
{
	return rc;
}

bool resourceGameState::isNeeded()
{
	return (numReferences() > 0);
}

#include "scriptingEngine.h"

DeclareScriptingFilepathResource(resourceGameState);
DeclareScriptingFunction(&resourceGameState::get, "get");