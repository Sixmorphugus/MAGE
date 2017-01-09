#include "drawRecipe.h"

#include "resourceTexture.h"
#include "resourceShader.h"

using namespace mage;

drawRecipe::drawRecipe()
{
	depth = 0;
}

drawRecipe::drawRecipe(std::vector<triangle> tris, unsigned int d, renderStates & s)
{
	triangles = tris;
	depth = d;
	states = s;
}

// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(drawRecipe);
MAGE_DeclareScriptingConstructor(drawRecipe(), "drawRecipe");
MAGE_DeclareScriptingConstructor(drawRecipe(std::vector<triangle>& tris, unsigned int, renderStates), "drawRecipe");
MAGE_DeclareScriptingFunction(&drawRecipe::states, "states");
MAGE_DeclareScriptingFunction(&drawRecipe::triangles, "triangles");
MAGE_DeclareScriptingFunction([](triangle tri) { return drawRecipe({ tri }); }, "drawRecipe");
MAGE_DeclareScriptingFunction([](std::vector<triangle> tri) { return drawRecipe(tri); }, "drawRecipe");
MAGE_DeclareScriptingFunction([](std::vector<triangle> tri, unsigned int d) { return drawRecipe(tri, d); }, "drawRecipe");
MAGE_DeclareScriptingFunction([](std::vector<triangle> tri, unsigned int d, renderStates& states) { return drawRecipe(tri, d, states); }, "drawRecipe");
