#include "resourceFont.h"

using namespace mage;

// font resource
resourceFont::resourceFont()
{}

resourceFont::resourceFont(std::string filepath)
	: resourceFilepath(filepath)
{}

bool resourceFont::load()
{
	auto t = resourceFilepath::load();

	data = std::make_shared<sf::Font>();

	bool success = data->loadFromFile(filepath);

	if (success) {
		return t;
	}

	return false;
}

void resourceFont::unload()
{
	data = std::shared_ptr<sf::Font>();
	resourceFilepath::unload();
}

std::shared_ptr<sf::Font> resourceFont::get()
{
	return data;
}

// sfFontResource
#include "scriptingEngine.h"

MAGE_DeclareScriptingFilepathResource(resourceFont);