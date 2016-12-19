#include "objBasic.h"

// IT'S JUST A BASIC WITH THE SPRITE FUNCTIONALITY REMOVED LOL!!!

namespace mage {

class MAGEDLL objSpriteless : public objBasic {
public:
	objSpriteless(float x, float y);

	unsigned int addSprite(textureData texture) = delete;
	void setCurrentSprite(int spr) = delete;
	void replaceCurrentSprite(textureData texture) = delete;
	void sfSync() = delete;

	virtual sf::Vector2f getSize();
};

}