#pragma once

// mage::renderChunk
// -------------
// A chunk that always garentees the batchRenderer can draw it with a single draw call.
// Generally these are only used by the batchRenderer - you can however create your own chunk and make the batchRenderer draw it.
// These are different to drawRecipes because:
// - The verteces are stored in SF format and do not need conversion.
// - renderChunks group things by "depth" allowing multiple draw depths to be acheived in one chunk.
// renderChunks are the basis of the batchRenderer's batch drawing capability - the batchRenderer creates chunks that use its "page" (combined texture) as theirs.
// -------------
// Category: Rendering
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "renderStates.h"
#include "triangle.h"

namespace mage {
class MAGEDLL renderChunk {
public:
	renderChunk();
	renderChunk(renderStates& st);

	void pushTriangle(triangle& tri, float depth = 0.f);
	void clearVerts();

	std::vector<vertex> getVertexList(); // don't use this in normal code, it's for debugging
	std::vector<sf::Vertex> getSfVertexList();

public:
	renderStates states;

private:
	std::map< float, std::vector<sf::Vertex> > m_verteces; // we store the verteces as SF so we don't have to convert during render time
};
}
