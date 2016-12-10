#pragma once
#include "uiBasic.h"

namespace mage {

// textEntry - place for entering text
class MAGEDLL uiTextEntry : public uiBasic
{
public:
	// constructors
	uiTextEntry(float x, float y);

	void update(sf::Time elapsed);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

public:
	bool active; // if active, text entry box can take input
	std::string contains;
	float width;
	bool mouseSensitive;
	unsigned int displayFrom;

	bool safe; // allow non-alnum characters
	bool ready; // was enter hit?

	hook<basic*> onReady;
};

} // namespace mage