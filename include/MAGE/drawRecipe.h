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

namespace mage {

class resourceTexture;
class resourceShader;
class shaderSet;

class drawRecipe
{
public:
	drawRecipe();
	drawRecipe(std::vector<triangle> tris, std::shared_ptr<resourceTexture> rt = nullptr, std::shared_ptr<resourceShader> sh = nullptr);

public:
	std::vector<triangle> triangles;
	std::shared_ptr<resourceTexture> texture;
	std::shared_ptr<resourceShader> shader;
};

}