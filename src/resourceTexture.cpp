#include "resourceTexture.h"
#include "scriptingEngine.h"

using namespace mage;

resourceTexture::resourceTexture()
{}

// texture resource
resourceTexture::resourceTexture(std::string filepath)
	: resourceFilepath(filepath)
{}

bool resourceTexture::load()
{
	auto t = resourceFilepath::load();

	data = std::make_shared<sf::Texture>();

	bool success = data->loadFromFile(filepath);

	if (success) {
		return t;
	}

	return false;
}

void resourceTexture::unload()
{
	data = std::shared_ptr<sf::Texture>();
	resourceFilepath::unload();
}

std::shared_ptr<sf::Texture> resourceTexture::get()
{
	return data;
}

// sfTextureResource
DeclareScriptingFilepathResource(resourceTexture);
DeclareScriptingFunction(&resourceTexture::get, "get");