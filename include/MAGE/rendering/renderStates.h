#pragma once

// mage::renderStates
// -------------
// Stand-In for sf::RenderStates.
// If texture is null you'll draw a solid color.
// If shader is null there's no shader used.
// -------------
// Category: Rendering
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "point.h"
#include "shaders.h"

namespace mage {
	extern const blendMode blendDefault;

	class resourceTexture;
	class resourceShader;

	class MAGEDLL renderStates : public shadable {
	public:
		renderStates();
		renderStates(sf::RenderStates& states); // WARNING: slow function - please don't do the conversion this way around in releases
		renderStates(std::shared_ptr<resourceTexture> tex, std::shared_ptr<resourceShader> sha, blendMode b = blendDefault);

		sf::RenderStates toSf();

		bool operator==(renderStates& rh);
		bool operator!=(renderStates& rh);
		
	public:
		// renderStates in MAGE don't have a transform but do have everything else.
		blendMode blend;

		bool usePage;

		std::weak_ptr<resourceTexture> texture;
	};
}