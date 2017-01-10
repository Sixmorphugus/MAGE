#pragma once

// mage::shadable
// -------------
// This is a legacy class that was used for objects that can be shaded.
// It is now part of renderStates.
// -------------
// Category: Rendering
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "platform.h"
#include "point.h"

namespace mage {

class resourceShader;

class MAGEDLL shadable {
public:
	shadable();

	void shaderRestartEffect();
	void shaderUpdate(point2F texSize = point2F()) const;
	void shaderUpdateCustom(sf::Shader* cShader, point2F texSize = point2F()) const;

public:
	std::weak_ptr<resourceShader> shader;
};

} // namespace mage