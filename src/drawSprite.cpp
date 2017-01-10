#include "drawSprite.h"
#include "resourceTexture.h"

using namespace mage;

drawSprite::drawSprite()
{
}

drawSprite::drawSprite(std::shared_ptr<resourceTexture> tex)
{
	states.texture = tex;

	sizeToTexture(tex);
	mapTextureCoordsToFullTexture(tex);
}

drawSprite::drawSprite(pointF & position, pointF& size, std::shared_ptr<resourceTexture> tex)
	: drawBox(position, size)
{
	states.texture = tex;

	sizeToTexture(tex);
	mapTextureCoordsToFullTexture(tex);
}

void drawSprite::setTexture(std::shared_ptr<resourceTexture>& tex)
{
	states.texture = tex;

	sizeToTexture(tex);
	mapTextureCoordsToFullTexture(tex);
}

std::shared_ptr<resourceTexture> drawSprite::getTexture() const
{
	return states.texture.lock();
}

floatBox drawSprite::getSpriteTextureCoords()
{
	return floatBox(pointF(texCoord1.x, texCoord1.y), pointF(texCoord4.x, texCoord4.y), mage::CORNERS);
}

void drawSprite::setSpriteTextureCoords(floatBox& box)
{
	texCoord1 = point2F(box.position.x, box.position.y);
	texCoord2 = point2F(box.position.x + box.size.x, box.position.y);
	texCoord3 = point2F(box.position.x, box.position.y + box.size.y);
	texCoord4 = point2F(box.position.x + box.size.x, box.position.y + box.size.y);
}

void drawSprite::sizeToTexture(std::shared_ptr<resourceTexture> tex)
{
	if (!tex)
		return;

	setScale(tex->getSize().convertAxis<float>().to3());
}

void drawSprite::mapTextureCoordsToFullTexture(std::shared_ptr<resourceTexture> tex)
{
	if (!tex)
		return;

	setSpriteTextureCoords(floatBox(pointF(0.f, 0.f), tex->getSize().convertAxis<float>().to3()));
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingType(drawSprite);
MAGE_DeclareScriptingBaseClass(drawBox, drawSprite);
MAGE_DeclareScriptingBaseClass(transformableBox, drawSprite);
MAGE_DeclareScriptingBaseClass(transformable, drawSprite);
MAGE_DeclareScriptingConstructor(drawSprite(), "drawSprite");
MAGE_DeclareScriptingConstructor(drawSprite(pointF& position, pointF& size, std::shared_ptr<resourceTexture> tex), "drawSprite");
MAGE_DeclareScriptingConstructor(drawSprite(std::shared_ptr<resourceTexture> tex), "drawSprite");
MAGE_DeclareScriptingCopyOperator(drawSprite);
MAGE_DeclareScriptingFunction(&drawSprite::getSpriteTextureCoords, "getSpriteTextureCoords");
MAGE_DeclareScriptingFunction(&drawSprite::setSpriteTextureCoords, "setSpriteTextureCoords");
MAGE_DeclareScriptingFunction(&drawSprite::sizeToTexture, "sizeToTexture");
MAGE_DeclareScriptingFunction(&drawSprite::mapTextureCoordsToFullTexture, "mapTextureCoordsToFullTexture");