#pragma once

// mage::vertex
// -------------
// A stand-in for the SFML class "Vertex".
// It can be converted to one, too.
// -------------
// Category: Rendering
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "color.h"
#include "point.h"

namespace mage {

class vertex {
public:
	vertex();
	vertex(pointF& sc, pointF& tc = pointF(), color& t = color());

	sf::Vertex toSf();

public:
	pointF screenCoords;
	pointF textureCoords;
	sf::Color tint;
};

}