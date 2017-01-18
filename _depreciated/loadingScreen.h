#pragma once
#include "platform.h"
#include "SfmlAfx.h"

namespace mage {

// for drawing loading screens in the ui.
// multipurpose and updateless
class MAGEDLL loadingScreen :
	public sf::Drawable
{
public:
	loadingScreen();

	virtual bool canDraw() const;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

public:
	std::string loading;
	unsigned int count, countMax;
};

} // namespace mage