#pragma once

// mage::drawRecipe
// -------------
// A complex set of triangles and extra information that the renderer will batch together with others and attempt to draw as quickly as possible.
// Consists of:
// - Triangles.
// - A shaderSet (optional)
// - A texture (optional)
// -------------
// Category: Rendering
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "StdAfx.h"

#include "triangle.h"
#include "renderStates.h"

namespace mage {

class resourceTexture;
class resourceShader;

class MAGEDLL drawRecipe
{
public:
	drawRecipe();
	drawRecipe(std::vector<triangle> tris, unsigned int depth = 0, renderStates& states = renderStates());

public:
	std::vector<triangle> triangles;
	renderStates states;

	unsigned int depth;
};

}