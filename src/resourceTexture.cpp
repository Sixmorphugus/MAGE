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

	m_data = sf::Texture();

	bool success = m_data.loadFromFile(filepath);

	if (success) {
		return t;
	}

	return false;
}

void resourceTexture::unload()
{
	m_data = sf::Texture();
	resourceFilepath::unload();
}

sf::Texture* resourceTexture::get()
{
	return &m_data;
}

point2U resourceTexture::getSize()
{
	return point2U(m_data.getSize());
}

bool resourceTexture::getIsRepeated()
{
	return m_data.isRepeated();
}

bool resourceTexture::getIsSmooth()
{
	return m_data.isSmooth();
}

void resourceTexture::setRepeated(bool repeated)
{
	m_data.setRepeated(repeated);
}

void resourceTexture::setSmooth(bool smooth)
{
	m_data.setRepeated(smooth);
}

// sfTextureResource
MAGE_DeclareScriptingFilepathResource(resourceTexture);
MAGE_DeclareScriptingFunction(&resourceTexture::getSize, "getSize");
MAGE_DeclareScriptingFunction(&resourceTexture::getIsRepeated, "getIsRepeated");
MAGE_DeclareScriptingFunction(&resourceTexture::getIsSmooth, "getIsSmooth");
MAGE_DeclareScriptingFunction(&resourceTexture::setRepeated, "setRepeated");
MAGE_DeclareScriptingFunction(&resourceTexture::setSmooth, "setSmooth");