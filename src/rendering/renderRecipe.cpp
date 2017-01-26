#include "renderRecipe.h"

#include "resourceTexture.h"
#include "resourceShader.h"

using namespace mage;

renderRecipe::renderRecipe()
{
	depth = 0;
}

renderRecipe::renderRecipe(std::vector<triangle> tris, unsigned int d, renderStates & s)
{
	triangles = tris;
	depth = d;
	states = s;
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
