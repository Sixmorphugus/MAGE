#pragma once

// mage::drawSprite
// -------------
// A version of drawBox that's geared more towards drawing textures.
// Is a renderable.
// -------------
// Category: Rendering
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "drawBox.h"

namespace mage {

	class MAGEDLL drawSprite : public drawBox {
	public:
		drawSprite();
		drawSprite(std::shared_ptr<resourceTexture> tex);
		drawSprite(pointF& position, pointF& size, std::shared_ptr<resourceTexture> tex = nullptr);

		void setTexture(std::shared_ptr<resourceTexture>& tex);
		std::shared_ptr<resourceTexture> getTexture() const;

		floatBox getSpriteTextureCoords();
		void setSpriteTextureCoords(floatBox& box);

		void sizeToTexture(std::shared_ptr<resourceTexture> tex);
		void mapTextureCoordsToFullTexture(std::shared_ptr<resourceTexture> tex);
	};

}