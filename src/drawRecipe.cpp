#include "drawRecipe.h"

#include "resourceTexture.h"
#include "resourceShader.h"

using namespace mage;

drawRecipe::drawRecipe()
{
	texture = nullptr;
	shader = nullptr;
}

drawRecipe::drawRecipe(std::vector<triangle> tris, std::shared_ptr<resourceTexture> rt, std::shared_ptr<resourceShader> sh)
{
	triangles = tris;
	texture = rt;
	shader = sh;
}

// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(drawRecipe);
MAGE_DeclareScriptingConstructor(drawRecipe(), "drawRecipe");
MAGE_DeclareScriptingConstructor(drawRecipe(std::vector<triangle>& tris, std::shared_ptr<resourceTexture> rt, std::shared_ptr<resourceShader> sh), "drawRecipe");
MAGE_DeclareScriptingFunction(&drawRecipe::shader, "shader");
MAGE_DeclareScriptingFunction(&drawRecipe::texture, "texture");
MAGE_DeclareScriptingFunction(&drawRecipe::triangles, "triangles");

// extra constructors
MAGE_DeclareScriptingFunction([](triangle tri) { return drawRecipe({ tri }); }, "drawRecipe");
MAGE_DeclareScriptingFunction([](std::vector<triangle> tri) { return drawRecipe(tri); }, "drawRecipe");
MAGE_DeclareScriptingFunction([](std::vector<triangle> tri, std::shared_ptr<resourceTexture> rt) { return drawRecipe(tri, rt); }, "drawRecipe");