#include "drawPoint.h"

using namespace mage;

drawPoint::drawPoint()
{
}

drawPoint::drawPoint(const pointF& point)
	: transformable(point)
{
}

void drawPoint::doTransformUpdate()
{
	makeDirty();
	transformable::doTransformUpdate();
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