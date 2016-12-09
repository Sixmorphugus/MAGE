#include "resourceGroup.h"

using namespace mage;

resourceGroup::resourceGroup()
{}

resourceGroup::resourceGroup(std::string filepath)
	: resourceFilepath(filepath)
{}

bool resourceGroup::load()
{
	rc = std::make_shared<Group>();

	if (!rc->loadFromFile(filepath)) return false;
	return resourceFilepath::load();
}

void resourceGroup::unload()
{
	rc = nullptr;
	resourceFilepath::unload();
}

std::shared_ptr<Group> resourceGroup::get()
{
	return rc;
}

bool resourceGroup::isNeeded()
{
	// if this music track isn't referenced anywhere, unload it to save memory.
	// note that this SHOULD be safe - it's impossible to "get" an unloaded resource.
	return (numReferences() > 0);
}

#include "scriptingEngine.h"

DeclareScriptingFilepathResource(resourceGroup);
DeclareScriptingFunction(&resourceGroup::get, "get");