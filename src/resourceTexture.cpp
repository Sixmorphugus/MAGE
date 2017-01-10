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

	m_data = std::make_shared<sf::Texture>();

	bool success = m_data->loadFromFile(filepath);

	if (success) {
		return t;
	}

	return false;
}

void resourceTexture::unload()
{
	m_data = std::shared_ptr<sf::Texture>();
	resourceFilepath::unload();
}

std::shared_ptr<sf::Texture> resourceTexture::get()
{
	return m_data;
}

point2U resourceTexture::getSize()
{
	return point2U(m_data->getSize());
}

// sfTextureResource
MAGE_DeclareScriptingFilepathResource(resourceTexture);