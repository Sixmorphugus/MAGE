#include "renderChunk.h"
#include "helpers.h"

using namespace mage;

renderChunk::renderChunk()
{
}

void renderChunk::pushTriangle(triangle & tri, unsigned int depth)
{
	if (m_verteces.size() < depth) {
		m_verteces.resize(depth);
	}

	// do insertion
	m_verteces[depth].push_back(tri.vA.toSf());
	m_verteces[depth].push_back(tri.vB.toSf());
	m_verteces[depth].push_back(tri.vC.toSf());
}

void renderChunk::clearVerts()
{
	m_verteces.clear();
}

std::vector<vertex> renderChunk::getVertexList()
{
	auto cvv = getSfVertexList();
	std::vector<vertex> vList;

	for (unsigned int i = 0; i < cvv.size(); i++) {
		vList.push_back(cvv[i]);
	}

	return vList;
}

std::vector<sf::Vertex> renderChunk::getSfVertexList()
{
	return collapseVectorVector<sf::Vertex>(m_verteces);
}
