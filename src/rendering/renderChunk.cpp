#include "renderChunk.h"
#include "helpers.h"
#include "renderRecipe.h"

using namespace mage;

renderChunk::renderChunk()
{
}

renderChunk::renderChunk(const renderStates& st)
{
	states = st;
}

renderChunk::~renderChunk()
{
	// if this somehow happens we should probably notify all the renderRecipes that will need redrawing.

}

void renderChunk::pushRecipe(renderRecipe& r)
{
	if (r.states != states) {
		p::warn("Recipe/Chunk incompatibility detected, ignoring insertion attempt");
		return;
	}

	r.m_chunk = this;

	// decide where to insert.
	auto insertionIterator = m_vertecesInfo.begin();
	auto insertionIterator2 = m_verteces.begin();

	while (insertionIterator->depth <= r.depth) {
		insertionIterator++;
		insertionIterator2++;
	}

	vertexInfo vi;
	vi.depth = r.depth;
	vi.recipe = &r;

	auto viv = { vi, vi, vi }; // probably don't need 3 of them...

	m_verteces.insert(insertionIterator2, r.triangles.begin(), r.triangles.end()); // wait fuck nvm fix later
	m_vertecesInfo.insert(insertionIterator, viv.begin(), viv.end());
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
	return m_verteces;
}

float renderChunk::getMinDepth() const
{
	return m_vertecesInfo.begin()->depth;
}

float renderChunk::getMaxDepth() const
{
	return m_vertecesInfo.rbegin()->depth;
}

bool renderChunk::overlaps(const renderChunk & toCheck) const
{
	// if either min depth or max depth lines are inside the other chunk it overlaps
	return (getMinDepth() > toCheck.getMinDepth() && getMinDepth() < toCheck.getMaxDepth()) || 
		(getMaxDepth() > toCheck.getMaxDepth() && getMaxDepth() < toCheck.getMaxDepth());
}

std::vector<float> renderChunk::getDepths() const
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