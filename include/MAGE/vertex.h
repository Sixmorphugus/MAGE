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

#include "colour.h"
#include "point.h"

namespace mage {

class vertex {
public:
	vertex();
	vertex(pointF& sc, pointF& tc = pointF(), colour& t = colour());

	sf::Vertex toSf();

public:
	pointF position;
	pointF texCoords;
	colour tint;
};

}