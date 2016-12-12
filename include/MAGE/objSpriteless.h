#include "objBasic.h"

// IT'S JUST A BASIC WITH THE SPRITE FUNCTIONALITY REMOVED LOL!!!

namespace mage {

class objSpriteless : public objBasic {
public:
	objSpriteless(float x, float y);

	unsigned int addSprite(textureData texture) = delete;
	int getCurrentSpriteIndex() const = delete;
	const spriteData* getCurrentSprite() const = delete;
	spriteData* getCurrentSprite() = delete;
	void setCurrentSprite(int spr) = delete;
	void replaceCurrentSprite(textureData texture) = delete;
	void sfSync() = delete;
};

}