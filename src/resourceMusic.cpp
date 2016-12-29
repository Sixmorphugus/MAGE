#include "resourceMusic.h"

using namespace mage;

// volatile music resource
resourceMusic::resourceMusic()
{}

resourceMusic::resourceMusic(std::string filepath)
	: resourceFilepath(filepath)
{}

bool resourceMusic::load()
{
	auto t = resourceFilepath::load();

	data = std::make_shared<sf::Music>();

	if (!data->openFromFile(filepath)) return false;

	data->setLoop(true);

	return t;
}

void resourceMusic::unload()
{
	data = std::shared_ptr<sf::Music>();

	resource::unload();
}

bool resourceMusic::isNeeded()
{
	// if this music track isn't referenced anywhere, unload it to save memory.
	// note that this SHOULD be safe - it's impossible to "get" an unloaded resource.
	return (numReferences() > 0);
}

std::shared_ptr<sf::Music> resourceMusic::get()
{
	return data;
}

// sfMusicResource
#include "scriptingEngine.h"

MAGE_DeclareScriptingFilepathResource(resourceMusic);
MAGE_DeclareScriptingFunction(&resourceMusic::get, "get");