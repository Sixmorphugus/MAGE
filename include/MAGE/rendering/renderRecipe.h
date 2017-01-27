#pragma once

// mage::renderRecipe
// -------------
// A complex set of triangles and extra information that the batchRenderer will batch together with others and attempt to draw as quickly as possible.
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
#include "box.h"
#include "renderChunk.h"

namespace mage {

class resourceTexture;
class resourceShader;

class MAGEDLL renderRecipe
{
public:
	renderRecipe();
	renderRecipe(std::vector<triangle> tris, unsigned int depth = 0, renderStates& states = renderStates());

	~renderRecipe();

	bool fitsInBounds(const floatBox& renderBounds);
	void shiftTextureVerts(const point2F& shift);

	renderChunk* getChunk();

public:
	std::vector<triangle> triangles;
	renderStates states;

	unsigned int depth;

private:
	renderChunk* m_chunk;

	friend class renderChunk;
};

}