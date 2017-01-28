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
class renderRecipe;

class MAGEDLL renderChunk {
private:
	struct vertexInfo {
		float depth;
		renderRecipe* recipe;
	};

public:
	renderChunk();
	renderChunk(const renderStates& st);

	~renderChunk();

	void pushRecipe(renderRecipe& r);
	void dropRecipeTris(const renderRecipe& r);
	void clearVerts();

	std::vector<vertex> getVertexList(); // don't use this in normal code, it's for debugging
	std::vector<sf::Vertex> getSfVertexList();

	float getMinDepth() const;
	float getMaxDepth() const;

	bool overlaps(const renderChunk& toCheck) const;
	bool isEmpty() const;

	std::vector<float> getDepths() const;

	std::vector<renderChunk> splitAtDepth(float split) const; // splits the render chunk in half at a specific depth point. Produces two renderChunks.
	std::vector<renderChunk> splice(const renderChunk& toSplice); // will produce an ordered list with the least possible amount of chunks required to draw in the right order

public:
	renderStates states;

private:
	std::vector<vertexInfo> m_vertecesInfo; 
	std::vector<sf::Vertex> m_verteces; // we store the verteces as SF so we don't have to convert during render time
};
}
