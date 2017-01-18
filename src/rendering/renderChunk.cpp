#include "renderChunk.h"
#include "helpers.h"

using namespace mage;

renderChunk::renderChunk()
{
}

renderChunk::renderChunk(renderStates& st)
{
	states = st;
}

void renderChunk::pushTriangle(triangle & tri, float depth)
{
	if (!m_verteces.count(depth)) {
		m_verteces[depth] = std::vector<sf::Vertex>();
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
	return collapseVectorMap<float, sf::Vertex>(m_verteces);
}

#include "scriptingEngine.h"
MAGE_DeclareScriptingType(renderChunk);
MAGE_DeclareScriptingConstructor(renderChunk(), "renderChunk");
MAGE_DeclareScriptingConstructor(renderChunk(renderStates&), "renderChunk");
MAGE_DeclareScriptingFunction(&renderChunk::clearVerts, "clearVerts");
MAGE_DeclareScriptingFunction(&renderChunk::getVertexList, "getVertexList");
MAGE_DeclareScriptingFunction(&renderChunk::pushTriangle, "pushTriangle");
MAGE_DeclareScriptingFunction(&renderChunk::states, "states");