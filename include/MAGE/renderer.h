#pragma once

// mage::renderer
// -------------
// It draws things.
// TODO: better description of one of the most important engine parts
// -------------
// Category: Rendering
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "StdAfx.h"
#include "SfmlAfx.h"

namespace mage {
class renderer {
public:
	renderer(sf::RenderWindow* rw = nullptr); // Can't be constructed by script

	void render();

private:
	sf::RenderWindow* m_window;
};
}