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

	data = sf::SoundBuffer();

	bool success = data.loadFromFile(filepath);

	if (success) {
		return t;
	}

	return false;
}

void resourceSoundBuffer::unload()
{
	data = sf::SoundBuffer();
	resourceFilepath::unload();
}

sf::SoundBuffer* resourceSoundBuffer::get()
{
	return &data;
}

unsigned int resourceSoundBuffer::getChannelCount()
{
	return data.getChannelCount();
}

time mage::resourceSoundBuffer::getDuration()
{
	return data.getDuration();
}

unsigned int resourceSoundBuffer::getSampleRate()
{
	return data.getSampleRate();
}

// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingFilepathResource(resourceSoundBuffer);
MAGE_DeclareScriptingFunction(&resourceSoundBuffer::getChannelCount, "getChannelCount");
MAGE_DeclareScriptingFunction(&resourceSoundBuffer::getDuration, "getDuration");
MAGE_DeclareScriptingFunction(&resourceSoundBuffer::getSampleRate, "getSampleRate");