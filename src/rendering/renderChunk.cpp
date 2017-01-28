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
	// if this somehow happens we should probably notify all the renderRecipes that will need redrawing by the renderer.
	std::vector<float> ds;

	if (m_vertecesInfo.size() == 0)
		return;

	for (unsigned int i = 0; i < m_vertecesInfo.size(); i++)
		m_vertecesInfo[i].recipe->m_chunk = nullptr; // yes i'm aware this is a bit spammed THANK YOU
}

void renderChunk::pushRecipe(renderRecipe& r)
{
	if (r.states != states) {
		p::warn("Recipe/Chunk incompatibility detected, ignoring insertion attempt");
		return;
	}

	if (r.m_chunk) {
		r.m_chunk->dropRecipeTris(r);
	}

	r.m_chunk = this;

	vertexInfo vi;
	vi.depth = r.depth;
	vi.recipe = &r;

	auto viv = { vi, vi, vi };

	if (m_verteces.size() > 0) {
		// decide where to insert.
		unsigned int j = 0;

		while (m_vertecesInfo[j].depth < r.depth) {
			j++;
		}

		for (unsigned int i = 0; i < r.triangles.size(); i++) {
			m_verteces.insert(m_verteces.begin() + j, r.triangles[i].m_verts.begin(), r.triangles[i].m_verts.end()); // wait fuck nvm fix later
			m_vertecesInfo.insert(m_vertecesInfo.begin() + j, viv.begin(), viv.end());
		}
	}
	else {
		// insert at beginning.
		for (unsigned int i = 0; i < r.triangles.size(); i++) {
			m_verteces.insert(m_verteces.begin(), r.triangles[i].m_verts.begin(), r.triangles[i].m_verts.end()); // wait fuck nvm fix later
			m_vertecesInfo.insert(m_vertecesInfo.begin(), viv.begin(), viv.end());
		}
	}
}

void renderChunk::dropRecipeTris(const renderRecipe & r)
{
	for (int i = 0; i < (int)m_vertecesInfo.size(); i++) {
		if (m_vertecesInfo[i].recipe == &r) {
			m_verteces.erase(m_verteces.begin() + i);
			m_vertecesInfo.erase(m_vertecesInfo.begin() + i);

			i--;
		}
	}
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

bool mage::renderChunk::isEmpty() const
{
	return m_vertecesInfo.size() == 0;
}

std::vector<float> renderChunk::getDepths() const
{
	std::vector<float> ds;

	if (m_vertecesInfo.size() == 0)
		return ds;

	float at = m_vertecesInfo[0].depth;

	for (unsigned int i = 0; i < m_vertecesInfo.size(); i++) {
		if (m_vertecesInfo[i].depth > at) {
			at = m_vertecesInfo[i].depth;
			ds.push_back(m_vertecesInfo[i].depth);
		}
	}

	return ds;
}

std::vector<renderChunk> renderChunk::splitAtDepth(float split) const
{
	std::vector<renderChunk> twoChunkList;
	twoChunkList.resize(2);

	twoChunkList[0].states = states;
	twoChunkList[1].states = states;

	std::set<renderRecipe*> pushSet1;
	std::set<renderRecipe*> pushSet2;

	// let's arbitrarily decide now that the depth we're splitting "on" (if it even exists in the chunk) will end up on the "top" half of the split.
	for (unsigned int i = 0; i < m_verteces.size(); i++) {
		if (m_vertecesInfo[i].depth <= split) {
			// verteces are added to the "top half"
			pushSet1.insert(m_vertecesInfo[i].recipe);
		}
		else {
			// verteces are added to the "bottom half"
			pushSet2.insert(m_vertecesInfo[i].recipe);
		}
	}

	for (auto i = pushSet1.begin(); i != pushSet1.end(); i++) {
		twoChunkList[0].pushRecipe(**i);
	}

	for (auto i = pushSet2.begin(); i != pushSet2.end(); i++) {
		twoChunkList[1].pushRecipe(**i);
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
MAGE_DeclareScriptingFunction(&renderChunk::pushRecipe, "pushTriangle");
MAGE_DeclareScriptingFunction(&renderChunk::states, "states");