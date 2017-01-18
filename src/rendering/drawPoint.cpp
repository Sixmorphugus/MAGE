#include "drawPoint.h"

using namespace mage;

mage::drawPoint::drawPoint()
{
}

drawPoint::drawPoint(pointF & point)
	: transformable(point)
{
}

renderRecipe drawPoint::generateDrawRecipe()
{
	states.shaderUpdate();

	auto drawPos = getTransformedPoints()[0];

	return renderRecipe({triangle(vertex(point2F(drawPos.x, drawPos.y), point2F(), tint))}, getRealPosition().z, states);
}

#include "scriptingEngine.h"
MAGE_DeclareScriptingType(drawPoint);
MAGE_DeclareScriptingBaseClass(transformable, drawPoint);
MAGE_DeclareScriptingBaseClass(renderable, drawPoint);
MAGE_DeclareScriptingConstructor(drawPoint(), "drawPoint");
MAGE_DeclareScriptingConstructor(drawPoint(pointF&), "drawPoint");
MAGE_DeclareScriptingFunction(&drawPoint::tint, "tint");
MAGE_DeclareScriptingFunction(&drawPoint::states, "states");