#pragma once

// mage SFML Includes
// -------------
// Everything MAGE needs from the SFML library.
// MAGE's backbone is SFML, though many overt uses of SFML types are hidden to decrease the required dependencies for modules.
// The idea is that MAGE wraps/replaces most of SFML so you don't have to use it.
// -------------
// Category: Core
// Has Hooks: N/A
// Script-Accessable: NO
// -------------

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

// some renames
namespace mage {
	typedef sf::BlendMode blendMode;
}