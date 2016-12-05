#pragma once
#include "objBasic.h"

namespace mage {

// casts light
class MAGEDLL objLight : public objBasic
{
public:
	objLight(float posX, float posY, textureData sprite, textureData lightSprite);

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	virtual void registerProperties();

	virtual void update(sf::Time elapsed);

	CLONEABLE(objLight);
public:
	sf::Color lightColor;
	sf::Vector2f lightOffset;

	animator lightAnimations;

	sf::Sprite lSpr;
};

}