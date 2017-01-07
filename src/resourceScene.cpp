#include "resourceScene.h"
#include "scene.h"

using namespace mage;

resourceScene::resourceScene()
{}

resourceScene::resourceScene(std::string filepath)
	: resourceFilepath(filepath)
{}

bool resourceScene::load()
{
	rc = std::make_shared<scene>();

	if (!rc->loadFromFile(filepath)) return false;
	return resourceFilepath::load();
}

void resourceScene::unload()
{
	rc = nullptr;
	resourceFilepath::unload();
}

std::shared_ptr<scene> resourceScene::get()
{
	return rc;
}

bool resourceScene::isNeeded()
{
	// if this music track isn't referenced anywhere, unload it to save memory.
	// note that this SHOULD be safe - it's impossible to "get" an unloaded resource.
	return (numReferences() > 0);
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingFilepathResource(resourceScene);
MAGE_DeclareScriptingFunction(&resourceScene::get, "get");