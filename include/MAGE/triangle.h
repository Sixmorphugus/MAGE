#pragma once

// mage::triangle
// -------------
// It's the only thing that the renderer can actually draw.
// A triangle is an object containing three verteces (3x mage::vertex.)
// It doesn't really have much else going for it besides constructors that can be used to make "triangles" render as lines or dots by putting all the points in the same place.
// -------------
// Category: Rendering
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "vertex.h"

namespace mage {
class triangle {
public:
	triangle(vertex& v1); // dot
	triangle(vertex& v1, vertex& v2); // line
	triangle(vertex& v1, vertex& v2, vertex& v3); // triangle

public:
	vertex vA, vB, vC;
};
}
