#include "renderRecipe.h"

#include "resourceTexture.h"
#include "resourceShader.h"
#include "renderChunk.h"

using namespace mage;

renderRecipe::renderRecipe()
{
	depth = 0;
	m_chunk = nullptr;
}

renderRecipe::renderRecipe(std::vector<triangle> tris, unsigned int d, renderStates & s)
{
	triangles = tris;
	states = s;
	m_chunk = nullptr;
	depth = d;
}

renderRecipe::~renderRecipe()
{
	if(m_chunk)
		m_chunk->dropRecipeTris(*this);
}

bool renderRecipe::fitsInBounds(const floatBox& renderBounds)
{
	for (unsigned int i = 0; i < triangles.size(); i++) {
		for (unsigned int j = 0; j < triangles[i].m_verts.size(); j++) {
			if (renderBounds.contains(triangles[i].m_verts[j].position))
				return false;
		}
	}

	return true;
}

void renderRecipe::shiftTextureVerts(const point2F& shift)
{
	for (unsigned int t = 0; t < triangles.size(); t++) {
		for (unsigned int i = 0; i < triangles[t].m_verts.size(); i++) {
			triangles[t].m_verts[i].texCoords += shift.toSf2();
		}
	}
}

renderChunk* mage::renderRecipe::getChunk()
{
	return m_chunk;
}

// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(renderRecipe);
MAGE_DeclareScriptingConstructor(renderRecipe(), "renderRecipe");
MAGE_DeclareScriptingConstructor(renderRecipe(std::vector<triangle>& tris, unsigned int, renderStates), "renderRecipe");
MAGE_DeclareScriptingFunction(&renderRecipe::states, "states");
MAGE_DeclareScriptingFunction(&renderRecipe::triangles, "triangles");
MAGE_DeclareScriptingFunction([](triangle tri) { return renderRecipe({ tri }); }, "renderRecipe");
MAGE_DeclareScriptingFunction([](std::vector<triangle> tri) { return renderRecipe(tri); }, "renderRecipe");
MAGE_DeclareScriptingFunction([](std::vector<triangle> tri, unsigned int d) { return renderRecipe(tri, d); }, "renderRecipe");
MAGE_DeclareScriptingFunction([](std::vector<triangle> tri, unsigned int d, renderStates& states) { return renderRecipe(tri, d, states); }, "renderRecipe");
