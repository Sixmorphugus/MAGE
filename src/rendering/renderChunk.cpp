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

float renderChunk::getMinDepth() const
{
	if(m_verteces.size() == 0)
		return 0.f;

	return m_verteces.begin()->first;
}

float renderChunk::getMaxDepth() const
{
	if (m_verteces.size() == 0)
		return 0.f;

	return m_verteces.rbegin()->first;
}

bool renderChunk::overlaps(const renderChunk & toCheck)
{
	// if either min depth or max depth lines are inside the other chunk it overlaps
	return (getMinDepth() > toCheck.getMinDepth() && getMinDepth() < toCheck.getMaxDepth()) || 
		(getMaxDepth() > toCheck.getMaxDepth() && getMaxDepth() < toCheck.getMaxDepth());
}

std::vector<float> mage::renderChunk::getDepths() const
{
	std::vector<float> ds;

	for (auto i = m_verteces.begin(); i != m_verteces.end(); i++) {
		ds.push_back(i->first);
	}

	return ds;
}

std::vector<renderChunk> renderChunk::splitAtDepth(float split) const
{
	std::vector<renderChunk> twoChunkList;
	twoChunkList.resize(2);

	twoChunkList[0].states = states;
	twoChunkList[1].states = states;

	// let's arbitrarily decide now that the depth we're splitting "on" (if it even exists in the chunk) will end up on the "top" half of the split.
	for (auto i = m_verteces.begin(); i != m_verteces.end(); i++) {
		if (i->first <= split) {
			// verteces are added to the "top half"
			twoChunkList[0].m_verteces[i->first] = i->second;
		}
		else {
			// verteces are added to the "bottom half"
			twoChunkList[1].m_verteces[i->first] = i->second;
		}
	}

	return twoChunkList;
}

std::vector<renderChunk> renderChunk::splice(const renderChunk& toSplice)
{
	// don't overuse the splice function; it's a very complex last resort.
	std::vector<renderChunk> chunkList;

	auto spliceDepths = getDepths();

	for (unsigned int i = 0; i < spliceDepths.size(); i++) {
		auto s1 = splitAtDepth(spliceDepths[i]);
		auto s2 = toSplice.splitAtDepth(spliceDepths[i]);

		chunkList.push_back(s1[0]);
		chunkList.push_back(s2[0]);
		chunkList.push_back(s1[1]);
		chunkList.push_back(s2[1]);
	}

	// remove empty chunks
	for (int i = 0; i < (int)chunkList.size(); i++) {
		if (chunkList[i].m_verteces.size() == 0) {
			chunkList.erase(chunkList.begin() + i);
			i--;
		}
	}

	return chunkList;
}

#include "scriptingEngine.h"
MAGE_DeclareScriptingType(renderChunk);
MAGE_DeclareScriptingConstructor(renderChunk(), "renderChunk");
MAGE_DeclareScriptingConstructor(renderChunk(renderStates&), "renderChunk");
MAGE_DeclareScriptingFunction(&renderChunk::clearVerts, "clearVerts");
MAGE_DeclareScriptingFunction(&renderChunk::getVertexList, "getVertexList");
MAGE_DeclareScriptingFunction(&renderChunk::pushTriangle, "pushTriangle");
MAGE_DeclareScriptingFunction(&renderChunk::states, "states");