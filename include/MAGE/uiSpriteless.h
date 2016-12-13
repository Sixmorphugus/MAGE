#include "uiBasic.h"

namespace mage {

	class uiSpriteless : public uiBasic {
	public:
		uiSpriteless(float x, float y);

		unsigned int addSprite(textureData texture) = delete;
		int getCurrentSpriteIndex() const = delete;
		const spriteData* getCurrentSprite() const = delete;
		spriteData* getCurrentSprite() = delete;
		void setCurrentSprite(int spr) = delete;
		void replaceCurrentSprite(textureData texture) = delete;
		void sfSync() = delete;
	};

}