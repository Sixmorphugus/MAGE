#include "uiBasic.h"

namespace mage {

	class MAGEDLL uiSpriteless : public uiBasic {
	public:
		uiSpriteless(float x, float y);

		unsigned int addSprite(textureData texture) = delete;
		void setCurrentSprite(int spr) = delete;
		void replaceCurrentSprite(textureData texture) = delete;
		void sfSync() = delete;

		virtual sf::Vector2f getSize();
	};

}