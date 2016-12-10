#include "resourceSoundBuffer.h"

using namespace mage;

// sound resource
resourceSoundBuffer::resourceSoundBuffer()
{}

resourceSoundBuffer::resourceSoundBuffer(std::string filepath)
	: resourceFilepath(filepath)
{}

bool resourceSoundBuffer::load()
{
	auto t = resourceFilepath::load();

	data = std::make_shared<sf::SoundBuffer>();

	bool success = data->loadFromFile(filepath);

	if (success) {
		return t;
	}

	return false;
}

void resourceSoundBuffer::unload()
{
	data = std::shared_ptr<sf::SoundBuffer>();
	resourceFilepath::unload();
}

std::shared_ptr<sf::SoundBuffer> resourceSoundBuffer::get()
{
	return data;
}

// SE
#include "scriptingEngine.h"

DeclareScriptingFilepathResource(resourceSoundBuffer);
DeclareScriptingFunction(&resourceSoundBuffer::get, "get");