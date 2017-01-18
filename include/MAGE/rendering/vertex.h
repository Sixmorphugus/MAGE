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

class MAGEDLL vertex {
public:
	vertex();
	vertex(const sf::Vertex& v);
	vertex(const point2F& sc, const point2F& tc = point2F(), const colour& t = colour());

	sf::Vertex toSf();

public:
	point2F position;
	point2F texCoords;
	colour tint;
};

}